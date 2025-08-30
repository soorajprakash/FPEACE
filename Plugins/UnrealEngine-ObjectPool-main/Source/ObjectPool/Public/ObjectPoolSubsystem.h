#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolSettings.h"
#include "Runtime/Engine/Public/Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"


UCLASS()
class OBJECTPOOL_API UObjectPool : public UWorldSubsystem
{
	GENERATED_BODY()

	UObjectPool();

protected:
	// virtual void BeginPlay() override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

public:
	// --- New: runtime-only container actors (not saved, not replicated) ---
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> PoolRootActor;

	// One child container per Actor class registered via AddActorType()
	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, TWeakObjectPtr<AActor>> TypeRootActors;

	// (Optional) editor-only folder path grouping:
	UPROPERTY(EditAnywhere, Category="Outliner")
	FName RootFolderPath = FName("Pools");

	// (Optional) toggle:
	UPROPERTY(EditAnywhere, Category="Outliner")
	bool bCreateContainerActors = true;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<AActor>, FPooledActorSettings> ActivePoolSettings;

	UFUNCTION(BlueprintInternalUseOnly)
	AActor* SpawnNewActor(const TSubclassOf<AActor>& Class);

	UFUNCTION(BlueprintCallable, Category="ObjectPool", meta=( Description="Moves an actor into the object pool and sends a OnPushed event to the object pool interface" ))
	bool Push(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="ObjectPool", meta=( ToolTip= "Pulls an actor from the pool or spawns a new one if needed and allowed"))
	bool Pull(TSubclassOf<AActor> Class, AActor* & Actor_Out);

	// Adds sub-pool of the chosen actor class to the object pool
	UFUNCTION(BlueprintCallable, Category="ObjectPool")
	bool AddActorType(TSubclassOf<AActor> Class, FPooledActorSettings ActorSettings);

	// Tuple key = Active pool, Tuple Value = Inactive Pool
	TMap<TSubclassOf<AActor>, TTuple<TArray<AActor*>, TArray<AActor*>>> Pool;

private:
	int32 NumSpawnedForThisClass;
};
