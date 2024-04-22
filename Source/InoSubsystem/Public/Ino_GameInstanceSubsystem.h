/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "InoSubsystem/Activation.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Ino_GameInstanceSubsystem.generated.h"

/**
 * Ino_GameInstanceSubsystem
 * Provides a GameInstanceSubsystem base class from which SubBlueprint classes can be derived
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class INOSUBSYSTEM_API UIno_GameInstanceSubsystem : public UGameInstanceSubsystem, public FActivation
{
	GENERATED_BODY()

public:
	UIno_GameInstanceSubsystem()
		: FActivation(true),
		  bInitialized(false),
		  bActiveDefault(true)
	{
	}

	virtual UWorld* GetWorld() const override final
	{
		return GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
	}

protected:
	uint8 bInitialized:1;

private:
	// Middle function Begin
	void PostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);
	void StartGameInstance(UGameInstance* GameInstance);
	// Middle function End
public:
	// UGameInstanceSubsystem implementation Begin
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// UGameInstanceSubsystem implementation End

	virtual void WorldBeginPlay() { ReceiveWorldBeginPlay(); }
	virtual void WorldBeginTearingDown(UWorld* World) { ReceiveWorldBeginTearingDown(); }

protected:
	/**The event called after this subsystem is created.*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Initialize")
	void ReceiveInitialize();

	/**The event called before this subsystem is destroyed.*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Deinitialize")
	void ReceiveDeinitialize();

	/**The event called after world begin play.*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName="WorldBeginPlay")
	void ReceiveWorldBeginPlay();

	/**The event called before world tear down.*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName="WorldBeginTearDown")
	void ReceiveWorldBeginTearingDown();

	/**
	 * @param Outer - The outer of UIno_LocalPlayerSubsystem should be a UGameInstance
	 * @return - The return value determines whether to create the Subsystem
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName="ShouldCreateSubsystem")
	bool ReceiveShouldCreateSubsystem(UObject* Outer) const;

public:
	/**Return Outer of this Subsystem*/
	UFUNCTION(BlueprintPure, Category="Subsystem|GameInstanceSubsystem", meta=(KeyWords="Get"))
	UGameInstance* GetGameInstance() const { return Super::GetGameInstance(); }
public:
	/**
	 * Activates the object.
	 * @param bReset - Whether the activation should happen even if IsActive returns true.
	 */
	UFUNCTION(BlueprintCallable, Category="Activation")
	virtual void Activate(bool bReset = false) override { FActivation::Activate(bReset); }

	/**
	 * Deactivates the object.
	 */
	UFUNCTION(BlueprintCallable, Category="Activation")
	virtual void Deactivate() override { FActivation::Deactivate(); };

	/**
	 * Sets whether the object is active or not
	 * @param bNewActive - The new active state of the object
	 * @param bReset - Whether the activation should happen even if IsActive returns true.
	 */
	UFUNCTION(BlueprintCallable, Category="Activation")
	virtual void SetActive(bool bNewActive, bool bReset = false) override final
	{
		FActivation::SetActive(bNewActive, bReset);
	}

	/**
	 * Toggles the active state of the object
	 */
	UFUNCTION(BlueprintCallable, Category="Activation")
	virtual void ToggleActive() override final { FActivation::ToggleActive(); }

	/**
	 * Returns whether the object is active or not
	 * @return - The active state of the object.
	 */
	UFUNCTION(BlueprintCallable, Category="Activation", meta=(KeyWords="Get"))
	bool IsActive() const { return FActivation::IsActive(); }

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FActivatedSignature, UIno_GameInstanceSubsystem, OnActivated,
	                                                    UObject*, Object, bool, bReset);

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FDeactivateSignature, UIno_GameInstanceSubsystem,
	                                                   OnDeactivated, UObject*, Object);

	/** Called when the component has been activated, with parameter indicating if it was from a reset */
	UPROPERTY(BlueprintAssignable, Category="Activation")
	FActivatedSignature OnActivated;
	/** Called when the component has been deactivated */
	UPROPERTY(BlueprintAssignable, Category="Activation")
	FDeactivateSignature OnDeactivated;

protected:
	UPROPERTY(EditAnywhere, Category="Activation")
	uint8 bActiveDefault:1;

	virtual void ReceiveActivate(bool bReset) override { if (bInitialized) OnActivated.Broadcast(this, bReset); }
	virtual void ReceiveDeactivate() override { if (bInitialized) OnDeactivated.Broadcast(this); }
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
