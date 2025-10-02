#pragma once

#include "Engine/LODActor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralGenerator.h"
#include "ProceduralGeneratorBPLibrary.generated.h"

UCLASS()
class UProceduralGeneratorBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Layer Map", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static UTexture2D* GetLayerMap(ACoreObject* Object, int32 Layer);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Layer Map", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void SetLayerMap(ACoreObject* Object, int32 Layer, UTexture2D* Map);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Object", Keywords = "ProceduralGenerator", WorldContext = "WorldContextObject"), Category = "ProceduralGenerator")
	static ACoreObject* CreateObject(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Map", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static TArray<int32> ReadMap(ACoreObject* Object, int32 Layer);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Actor Folder", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void SetActorFolder(AActor* Actor, FName FolderPath);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get All Actors In Editor", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static TArray<AActor*> GetAllActorsInEditor();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Snap To Floor", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void SnapToFloor(AActor* Actor);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Force GC Call", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void ForceGC();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Sort By GUID", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void SortByGUID(UPARAM(ref) TArray<FSnapshotItem>& Array);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clear Array From Not Valid Actors", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void ClearArrayFromNotValidActors(UPARAM(ref) FBlockStruct& Block);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Random Float in Range", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static float RandFloatInRange(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex, float Start, float End);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Random Bool", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static bool RandBool(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Random Integer in Range", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static int32 RandIntegerInRange(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex, int32 Start, int32 End);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Current Seed", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static int32 GetCurrentSeed(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mark Blocks", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void MarkBlocks(UPARAM(ref) TArray<FSnapshotItem>& LastSnapshot, UPARAM(ref) TArray<FSnapshotItem>& CurrentSnapshot, ACoreObject* CoreObject);

private:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mark Helper", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static void MarkHelper(FSnapshotItemLayer Layer, TArray<FSnapshotItemLayer>& Dirty);

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Asset GUID", Keywords = "ProceduralGenerator"), Category = "ProceduralGenerator")
	static FGuid GetAssetGUID(UObject* Asset);
};
