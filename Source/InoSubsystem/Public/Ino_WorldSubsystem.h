/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "InoSubsystem/Activation.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/World.h"
#include "Ino_WorldSubsystem.generated.h"

/**
 * UIno_WorldSubsystem
 * Provides a WorldSubsystem base class from which SubBlueprint classes can be derived
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class INOSUBSYSTEM_API UIno_WorldSubsystem : public UWorldSubsystem, public FActivation
{
	GENERATED_BODY()

public:
	UIno_WorldSubsystem()
		: FActivation(true),
		  bInitialized(false),
		  bActiveDefault(true)
	{
	}

protected:
	uint8 bInitialized:1;

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void PostInitialize() override { ReceivePostInitialize(); }
	virtual void OnWorldBeginPlay(UWorld& InWorld) override { ReceiveWorldBeginPlay(); }
	virtual void OnWorldComponentsUpdated(UWorld& World) override { ReceiveWorldComponentsUpdated(); }
	virtual void UpdateStreamingState() override { ReceiveUpdateStreamingState(); }

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
	}
	
	/**This event called after this subsystem is created.*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Initialize")
	void ReceiveInitialize();

	/**This event called before this subsystem is destroyed.*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Deinitialize")
	void ReceiveDeinitialize();

	/**
	 * @param Outer - The outer of UIno_LocalPlayerSubsystem should be a UWorld
	 * @return - The return value determines whether to create the Subsystem
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName="ShouldCreateSubsystem")
	bool ReceiveShouldCreateSubsystem(UObject* Outer) const;

	/**
	 * Called once all UWorldSubsystems have been initialized
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName="PostInitialze")
	void ReceivePostInitialize();

	/**
	 * Called when world is ready to start gameplay before the game mode transitions to the correct state and call BeginPlay on all actors
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName="WorldBeginPlay")
	void ReceiveWorldBeginPlay();

	/**
	 * Called after world components (e.g. line batcher and all level components) have been updated
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName="WorldComponentsUpdated")
	void ReceiveWorldComponentsUpdated();
	
	/**
	 * Updates sub-system required streaming levels (called by world's UpdateStreamingState function)
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName="UpdateStreamingState")
	void ReceiveUpdateStreamingState();

	/**
	 * Get world subsystem dependent level
	 */
	UFUNCTION(BlueprintPure,Category="Subsystem|WorldSubsystem")
	TSoftObjectPtr<UWorld> GetWorldSoftPtr() const { return Super::GetWorld(); }

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

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FActivatedSignature, UIno_WorldSubsystem, OnActivated,
	                                                    UObject*, Object, bool, bReset);

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FDeactivateSignature, UIno_WorldSubsystem,
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
