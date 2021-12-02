// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MAssetsLib.generated.h"

UCLASS()
class MASSETS_API UMAssetsLib final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Log a message if debugging is enabled */
	static void Trace(FString Message, bool Debug);

public:
	/**
	 * Load assets that extends a C++ PrimaryDataAsset type.
	 * This function will NOT work if you extend the type with a blueprint class.
	 * The operation is synchronous and returns UObject base objects for the loaded values.
	 */
	UFUNCTION(BlueprintCallable, Category = "MAssets|Methods")
	static TArray<UObject*> LoadAssets(FName AssetTypeName, FString FromPath, UClass* AssetType, bool Debug);
};
