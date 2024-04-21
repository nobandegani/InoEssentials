﻿/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "InoSubsystem/InoSubsystemTickable.h"
#include "Ino_GameInstanceSubsystem.h"
#include "Ino_GameInstanceSubsystemTickable.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class INOSUBSYSTEM_API UIno_GameInstanceSubsystemTickable : public UIno_GameInstanceSubsystem,
                                                                         public FTickableSubsystem
{
	GENERATED_BODY()

public:
	UIno_GameInstanceSubsystemTickable()
		: UIno_GameInstanceSubsystem(),
		  bIsTickEnabled(true),
		  bIsTickableWhenPaused(false)
	{
	}

	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// USubsystem implementation End

private:
	//Defaults Begin

	/**Whether to enable Tick*/
	UPROPERTY(EditAnywhere, Category="Tick")
	uint8 bIsTickEnabled:1;

	/**Whether to enable Tick when paused*/
	UPROPERTY(EditAnywhere, Category="Tick")
	uint8 bIsTickableWhenPaused:1;

	//Defaults End

public:
	//FTickableSubsystem implementation Begin
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override final { return Super::GetStatID(); }
	virtual UWorld* GetTickableGameObjectWorld() const override final { return GetWorld(); }
	virtual ETickableTickType GetTickableTickType() const override final;
	//FTickableSubsystem implementation End

protected:
	//Blueprint function Begin

	/**Event called every frame*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Tick")
	void ReceiveTick(float DeltaSeconds);

	/** 
	 * Set this Subsystem's tick functions to be enabled or disabled.
	 * This only modifies the tick function on this Subsystem
	 * @param bEnabled - Whether it should be enabled or not
	 */
	UFUNCTION(BlueprintCallable, Category="Subsystem|Tick")
	virtual void SetTickEnabled(bool const bEnabled) override final { FTickableSubsystem::SetTickEnabled(bEnabled); }

	/**
	 * Returns whether this Subsystem has tick enabled or not
	 */
	UFUNCTION(BlueprintCallable, Category="Subsystem|Tick", meta=(KeyWords="Get"))
	virtual bool IsTickable() const override final;

	/**
	 * Sets whether this actor can tick when paused.
	 * @param bTickableWhenPaused - Whether it should be Tickable or not when paused
	 */
	UFUNCTION(BlueprintCallable, Category="Subsystem|Tick")
	virtual void SetTickableWhenPaused(bool const bTickableWhenPaused) override final
	{
		FTickableSubsystem::SetTickableWhenPaused(bTickableWhenPaused);
	}

	/**
	 * Returns whether this Subsystem has tick enabled or not when paused
	 */
	UFUNCTION(BlueprintCallable, Category="Subsystem|Tick", meta=(KeyWords="Get"))
	virtual bool IsTickableWhenPaused() const override final { return FTickableSubsystem::IsTickableWhenPaused(); }

	//Blueprint function End
};
