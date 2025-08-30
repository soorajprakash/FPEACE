#include "ObjectPoolSubsystem.h"
#include "ObjectPooledActorInterface.h"
#include "ObjectPoolSettings.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_CLASS(LogObjectPool, Display, All);

UObjectPool::UObjectPool()
{
}

bool UObjectPool::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	switch (WorldType)
	{
	case EWorldType::None:
		return false;
	case EWorldType::Game:
		return true;
	case EWorldType::Editor:
		return false;
	case EWorldType::PIE:
		return true;
	case EWorldType::EditorPreview:
		return false;
	case EWorldType::GamePreview:
		return true;
	case EWorldType::GameRPC:
		return true;
	case EWorldType::Inactive:
		return false;
	}

	return Super::DoesSupportWorldType(WorldType);
}

void UObjectPool::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!bCreateContainerActors) return;
	if (PoolRootActor.IsValid()) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Name = same as object pool name (unique across PIE sessions)
	Params.Name = MakeUniqueObjectName(&InWorld, AActor::StaticClass(), *FString::Printf(TEXT("%s"), *GetName()));

	AActor* Root = InWorld.SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity, Params);
	if (!Root) return;

	// Keep it lightweight
	Root->SetCanBeDamaged(false);
	Root->SetReplicates(false);
	Root->SetActorHiddenInGame(true); // invisible container (no components anyway)

#if WITH_EDITOR
	// Editor niceties: label + folder (folders are editor/dev only)
	Root->SetActorLabel(GetName()); // dev/editor only. :contentReference[oaicite:1]{index=1}
	Root->SetFolderPath(*(RootFolderPath.ToString())); // dev/editor only. :contentReference[oaicite:2]{index=2}
#endif

	PoolRootActor = Root;
}

void UObjectPool::Deinitialize()
{
	const UObjectPoolSettings* Settings = GetDefault<UObjectPoolSettings>();

	if (!Settings || !Settings->bDestroyOnEndPlay)
	{
		return;
	}

	for (TTuple<TSubclassOf<AActor>, TTuple<TArray<AActor*>, TArray<AActor*>>> Element : Pool)
	{
		for (AActor* ActivePoolActor : Element.Value.Key)
		{
			if (IsValid(ActivePoolActor))
			{
				ActivePoolActor->Destroy();
			}
		}

		for (AActor* InactivePoolActor : Element.Value.Value)
		{
			if (IsValid(InactivePoolActor))
			{
				InactivePoolActor->Destroy();
			}
		}
	}

	Super::Deinitialize();
}

bool UObjectPool::Push(AActor* Actor)
{
	if (IsValid(Actor))
	{
		if (Actor->GetClass()->ImplementsInterface(UObjectPooledActor::StaticClass()))
		{
			if (!Pool.Contains(Actor->GetClass()))
			{
				FPooledActorSettings NewPooledActorSettings;
				if (!AddActorType(Actor->GetClass(), NewPooledActorSettings))
				{
					UE_LOG(LogObjectPool, Error, TEXT("Pushed actor of type \"%s\" did not already exist in pool and could not be added either"),
					       *Actor->GetClass()->GetName())
					return false;
				}

				if (!Pool.Contains(Actor->GetClass()))
				{
					return false;
				}
			}

			TTuple<TArray<AActor*>, TArray<AActor*>>* Subpools = Pool.Find(Actor->GetClass());
			Subpools->Key.Remove(Actor);
			Subpools->Value.AddUnique(Actor);
			IObjectPooledActor::Execute_OnPushedToPool(Actor);
			return true;
		}
	}

	return false;
}

bool UObjectPool::Pull(const TSubclassOf<AActor> Class, AActor*& Actor_Out)
{
	TTuple<TArray<AActor*>, TArray<AActor*>>* Subpool = Pool.Find(Class);
	if (!Subpool)
	{
		FPooledActorSettings NewPooledActorSettings;
		if (!AddActorType(Class, NewPooledActorSettings))
		{
			UE_LOG(LogObjectPool, Error, TEXT("Pulled actor type \"%s\" did not already exist in pool and could not be added either"), *Class->GetName())
			return false;
		}

		Subpool = Pool.Find(Class);
		if (!Subpool)
		{
			UE_LOG(LogObjectPool, Error, TEXT("Could not find subpool even after adding actor type"))
			return false;
		}
	}

	AActor* PulledActor = nullptr;

	// inactive pool
	for (AActor* Element : Subpool->Value)
	{
		if (IsValid(Element))
		{
			PulledActor = Element;
			Actor_Out = PulledActor;
			Subpool->Value.Remove(PulledActor);
			break;
		}
	}

	if (IsValid(PulledActor))
	{
		Subpool->Key.AddUnique(PulledActor);
		IObjectPooledActor::Execute_OnPulledFromPool(PulledActor);
		Actor_Out = PulledActor;
		return true;
	}
	else
	{
		FPooledActorSettings* Settings = ActivePoolSettings.Find(Class);
		if (Settings && Settings->bCanExpand)
		{
			PulledActor = SpawnNewActor(Class);
			if (!IsValid(PulledActor))
			{
				return false;
			}
			Subpool->Key.AddUnique(PulledActor);
			IObjectPooledActor::Execute_OnPulledFromPool(PulledActor);
			Actor_Out = PulledActor;
			return true;
		}

		return false;
	}
}

AActor* UObjectPool::SpawnNewActor(const TSubclassOf<AActor>& Class)
{
	UWorld* World = GetWorld();
	if (!World || !*Class) return nullptr;

	FActorSpawnParameters P;
	P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* Instance = World->SpawnActor<AActor>(Class, FTransform::Identity, P);
	if (!Instance) return nullptr;

	// Attach to the class container so the Outliner stays organized
	if (AActor* TypeRoot = TypeRootActors.FindRef(Class).Get())
	{
		Instance->AttachToActor(TypeRoot, FAttachmentTransformRules::KeepWorldTransform); // keep exact world location.

#if WITH_EDITOR
		// Optional: set a neat label or folder (both dev/editor only)
		Instance->SetActorLabel(FString::Printf(TEXT("%s_%d"), *Class->GetName(), ++NumSpawnedForThisClass)); // editor only.
		Instance->SetFolderPath(TypeRoot->GetFolderPath()); // editor only.
#endif
	}

	// Your existing "push into pool" path (hide, disable, OnPushedToPool, etc.)
	return Instance;
}

bool UObjectPool::AddActorType(TSubclassOf<AActor> Class, FPooledActorSettings ActorSettings)
{
	if (IsValid(Class))
	{
		if (!UKismetSystemLibrary::DoesClassImplementInterface(Class, UObjectPooledActor::StaticClass()))
		{
			UE_LOG(LogObjectPool, Error, TEXT("The actor class \"%s\" does not implement the \"%s\" interface"), *Class->GetName(),
			       *UObjectPooledActor::StaticClass()->GetName())
			return false;
		}

		if (!Pool.Contains(Class))
		{
			if (bCreateContainerActors && PoolRootActor.IsValid())
			{
				if (!TypeRootActors.FindRef(Class).IsValid())
				{
					UWorld* World = GetWorld();
					if (World)
					{
						FActorSpawnParameters P;
						P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						// Give the type container a nice unique name: PoolName_ClassName
						const FString Label = FString::Printf(TEXT("%s_%s"), *GetName(), *GetNameSafe(Class));
						P.Name = MakeUniqueObjectName(World, AActor::StaticClass(), *Label);

						AActor* TypeRoot = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity, P);
						if (TypeRoot)
						{
							TypeRoot->SetReplicates(false);
							TypeRoot->SetActorHiddenInGame(true);

							// Attach under the pool root without changing world transform.
							TypeRoot->AttachToActor(PoolRootActor.Get(), FAttachmentTransformRules::KeepWorldTransform);

#if WITH_EDITOR
							TypeRoot->SetActorLabel(Label); // editor/dev only. :contentReference[oaicite:5]{index=5}
							// Optional subfolder: "Pools/PoolName"
							const FString SubFolder = FString::Printf(TEXT("%s/%s"), *RootFolderPath.ToString(), *GetName());
							TypeRoot->SetFolderPath(*SubFolder);
#endif
							TypeRootActors.Add(Class, TypeRoot);
						}
					}
				}
			}

			TArray<AActor*> InactivePool;

			ActivePoolSettings.Add(Class, ActorSettings);

			for (int i = 0; i < ActorSettings.InitialSpawnCount; ++i)
			{
				AActor* NewActor = SpawnNewActor(Class);
				if (!IsValid(NewActor))
				{
					UE_LOG(LogObjectPool, Error, TEXT("AddActorType() failed to spawn new actor"))
					continue;
				}
				InactivePool.AddUnique(NewActor);
				IObjectPooledActor::Execute_OnPushedToPool(NewActor);
			}

			Pool.Add(Class, TTuple<TArray<AActor*>, TArray<AActor*>>(TArray<AActor*>(), InactivePool));
			return true;
		}
	}

	return false;
}
