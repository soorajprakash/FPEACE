// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Gameplay/Interfaces/FPCDamageReceiver.h"
#include "FPCCharacter.generated.h"

struct FOnAttributeChangeData;
enum class ELivingCharacterHealthState : uint8;
class UMetaSoundSource;
class UFPCHealthAttributeSet;
class UFPCOperatorData;
class UFPCAbilitySystemComponent;
class AFPCGameplayPlayerController;
class UFPCSkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthAttributeChanged, const float, CurrentHealth, const ELivingCharacterHealthState, HealthState);

UCLASS(Abstract)
class FPEACE_API AFPCCharacter : public ACharacter, public IAbilitySystemInterface, public IFPCDamageReceiver
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintAssignable)
	FOnHealthAttributeChanged OnHealthChangedEvent;

	virtual void OnReceivedDamage(TWeakObjectPtr<AFPCCharacter> From, FName HitBone) override;

	UFUNCTION(BlueprintCallable)
	virtual void PlayVocalSound(USoundBase* Sound, float CrossfadeDuration = 1);

	//	--------------------- GETTER FUNCTIONS ---------------------

	TWeakObjectPtr<UFPCSkeletalMeshComponent> GetTPSBodyMeshComp() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TArray<TSubclassOf<class UFPCOperatorAbilityBase>> BasicAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FPEACE")
	ELivingCharacterHealthState HealthState;

	/*
	 * Name of the content ID used to fetch default attribute values
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	FName ContentID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<UMetaSoundSource> VocalCrossfader;

	//	--------------------- ATTRIBUTES ---------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<UFPCHealthAttributeSet> HealthAttributeSet;

	//	--------------------- COMPONENTS ---------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> MainBodyMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UAudioComponent> VocalAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FPEACE|Components")
	TObjectPtr<UFPCAbilitySystemComponent> FPCAbilitySystemComponent;

	//	--------------------- FUNCTIONS ---------------------

	UFUNCTION(BlueprintNativeEvent)
	void OnHitDamageTaken(UAnimMontage* HitReactionMontage);

	UFUNCTION(BlueprintNativeEvent)
	void OnDeath();
	
	void OnHealthChanged(const FOnAttributeChangeData& ChangeData);

	virtual void OnDeath_Implementation();

	//	--------------------- OVERRIDES ---------------------

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
};
