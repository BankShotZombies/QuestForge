#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_QuestAsset : public FAssetTypeActions_Base
{
public:
	// Returns the display name shown for this asset type in the editor.
	virtual FText GetName() const override;

	// Returns the color used for this asset type in the Content Browser.
	virtual FColor GetTypeColor() const override;

	// Returns the asset class this type action is associated with.
	virtual UClass* GetSupportedClass() const override;

	// Returns the editor category this asset type appears under.
	virtual uint32 GetCategories() override;

	// Opens the custom asset editor when the asset is activated.
	virtual void OpenAssetEditor(
		const TArray<UObject*>& InObjects,
		TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
};