// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAssetsLib.h"
#include "UObject/Object.h"
#include "MAssetTemplate.generated.h"

const FPrimaryAssetType GMAssetTemplateAssetType = FPrimaryAssetType("MAsset.Template");

UCLASS(BlueprintType)
class MASSETS_API UMAssetTemplate : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Id;

	static void Load()
	{
		constexpr auto UseDebug = true;
		constexpr auto AssetPath = TEXT("/Assets");

		const auto Assets = UMAssetsLib::LoadAssets(GMAssetTemplateAssetType, AssetPath, UMAssetTemplate::StaticClass(), UseDebug);
		for (const auto Asset : Assets)
		{
			const auto TypedAsset = Cast<UMAssetTemplate>(Asset);
			UE_LOG(LogTemp, Display, TEXT("UMAssetTemplate: %s"), *TypedAsset->Id.ToString());
		}
	}

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(GMAssetTemplateAssetType, Id);
	}

	virtual bool IsEditorOnly() const override
	{
		return false;
	}
};
