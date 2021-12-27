// Fill out your copyright notice in the Description page of Project Settings.


#include "MAssetsLib.h"

#include "Engine/AssetManager.h"
#include "Engine/PrimaryAssetLabel.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"

void UMAssetsLib::Trace(FString Message, bool Debug)
{
	if (!Debug) { return; }
	UE_LOG(LogTemp, Display, TEXT("MAssets: %s"), *Message);
}

TArray<UObject*> UMAssetsLib::LoadAssets(FName AssetTypeName, FString FromPath, UClass* AssetType, bool Debug)
{
	TArray<UObject*> Assets;
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (!Manager)
	{
		Trace("No active message manager", Debug);
		return Assets;
	}

	// Look for assets in the given path
	// BluePrintClasses -> false: Only accept DataAssets that are instances of a C++ PrimaryDataAsset
	// EditorOnly -> false: Always assume asset loading is runtime.
	// Synchronous -> true: Do it now.
	const auto ScanResult = Manager->ScanPathForPrimaryAssets(AssetTypeName, FromPath, AssetType, false, false, true);
	Trace(FString::Printf(TEXT("Scan for %s assets: Found: %d"), *FromPath, ScanResult), Debug);

	// Load the list of asset ids
	TArray<FPrimaryAssetId> AssetIds;
	Manager->GetPrimaryAssetIdList(AssetTypeName, AssetIds);

	// Load all assets of the given type
	const TSharedPtr<FStreamableHandle> Handle = Manager->LoadPrimaryAssetsWithType(AssetTypeName);
	if (Handle.IsValid())
	{
		Trace(FString::Printf(TEXT("Loading %s assets"), *AssetTypeName.ToString()), Debug);
		Handle->WaitUntilComplete();
	}
	else
	{
		Trace(TEXT("Failed to load any assets"), Debug);
		return Assets;
	}

	// Load asset data
	TArray<FAssetData> AssetDataRecords;
	Manager->GetPrimaryAssetDataList(AssetTypeName, AssetDataRecords);
	for (FAssetData AssetData : AssetDataRecords)
	{
		auto AssetInstance = Manager->GetPrimaryAssetObject(AssetData.GetPrimaryAssetId());
		Assets.Add(AssetInstance);
	}

	Trace(FString::Printf(TEXT("Fetched %s x %d assets"), *AssetTypeName.ToString(), Assets.Num()), Debug);
	return Assets;
}

FString UMAssetsLib::GetPluginAssetPath(const FString PluginName)
{
	const auto PluginRef = IPluginManager::Get().FindPlugin(PluginName);
	if (!PluginRef)
	{
		return "Invalid plugin or not loaded";
	}

	return PluginRef->GetMountedAssetPath();
}
