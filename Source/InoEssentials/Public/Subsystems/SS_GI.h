/* Copyright (c) 2021-2023 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SS_GI.generated.h"

class UGameInstanceSubsystem;
struct FWorldContext;

DECLARE_LOG_CATEGORY_EXTERN(InoSSGI, Log, All);

UCLASS(Abstract, Within = GameInstance, Blueprintable, BlueprintType, DisplayName="Ino GI SS", Category="Ino|SS|GI")
class INOESSENTIALS_API UInoGISS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	struct FWorldContext* WorldContext = nullptr;
	
	//~ Begin UObject Interface
	virtual class UWorld* GetWorld() const final;
	struct FWorldContext* GetWorldContext() const { return WorldContext; }
	
	void OnLevelChanged(ULevel* Level, UWorld* World);

	void OnPostWorldInit(UWorld* World, const UWorld::InitializationValues Values);
	
public:
	void ForceInit();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ino|SS|GI")
		bool AutomaticWorldContext = true;

	UFUNCTION(BlueprintImplementableEvent, Category="Ino|SS|GI")
		void Init();

	UFUNCTION(BlueprintImplementableEvent, Category="Ino|SS|GI")
		void DeInit();
	
	UFUNCTION(BlueprintCallable, Category="Ino|SS|GI")
		void SetWorldContext(const UObject* NewWorldContext);

	UFUNCTION(BlueprintCallable, Category="Ino SS GI")
		UObject* GetWorldContextObject();

	UFUNCTION(BlueprintImplementableEvent, Category="Ino|SS|GI")
		void WorldInit();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Ino|SS|GI")
		void WorldUpdated();
};

