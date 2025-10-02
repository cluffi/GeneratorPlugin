#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CoreObject.generated.h"

USTRUCT(BlueprintType)
struct FDeleteQueueItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 LayerNumber = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 BlockIndex = 0;
};

USTRUCT(BlueprintType)
struct FGenerateQueueItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double BlockPositionX = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double BlockPositionY = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 LayerNumber = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 BlockIndex = 0;
};

USTRUCT(BlueprintType)
struct FSnapshotItemLayer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<int32> DependedBlocks;
};

USTRUCT(BlueprintType)
struct FSnapshotItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FGuid ActorGUID = FGuid();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FTransform Transform = FTransform();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<FSnapshotItemLayer> SnaphotLayers;
};

USTRUCT(BlueprintType)
struct FSnapshotLayer
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 MaxActorsPerBlock = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 Delta = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double StartOfTraceHeight = 500;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bUseCustomTraceHeightForEachBlock = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural", meta = (EditCondition = "bUseCustomTraceHeightForEachBlock"))
    double MinStartOfTraceHeight = 500;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural", meta = (EditCondition = "bUseCustomTraceHeightForEachBlock"))
    double MaxStartOfTraceHeight = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double TraceDistance = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FGuid Map;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<FGuid> Assets;
};

USTRUCT(BlueprintType)
struct FSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<FSnapshotItem> SnaphotItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<FSnapshotLayer> SnaphotLayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 Seed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double BlockSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bEnableTerrainCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FVector Location = FVector::Zero();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bRandomRotation = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bRandomScale = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bFixedScale = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double MinFixedScale = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double MaxFixedScale = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FVector MinScale = FVector::One();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FVector MaxScale = FVector::One();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FVector MinRotate = FVector::Zero();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FVector MaxRotate = FVector::Zero();
};

USTRUCT(BlueprintType)
struct FBlockStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bDirty = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double StartOfTraceHeight = 500;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double Density = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    FRandomStream Stream = FRandomStream();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<AActor*> Actors;
};

USTRUCT(BlueprintType)
struct FLayerStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 MaxActorsPerBlock = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    int32 Delta = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural", meta = (EditCondition = "!bUseCustomTraceHeightForEachBlock"))
    double StartOfTraceHeight = 500;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bUseCustomTraceHeightForEachBlock = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Procedural")
    bool bGenerated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural", meta = (EditCondition = "bUseCustomTraceHeightForEachBlock"))
    double MinStartOfTraceHeight = 500;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural", meta = (EditCondition = "bUseCustomTraceHeightForEachBlock"))
    double MaxStartOfTraceHeight = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    double TraceDistance = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    UTexture2D* Map;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<UStaticMesh*> Assets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural")
    TArray<FBlockStruct> Blocks;
};

UCLASS(HideCategories = (Rendering, Replication, Collision, Input, Actor, LOD, Cooking, Physics, HLOD, LevelInstance, WorldPartition, DataLayers, Networking, StaticMeshActor, Actor, Navigation, Mobility))
class PROCEDURALGENERATOR_API ACoreObject : public AStaticMeshActor
{
    GENERATED_BODY()

private:
    double TimeFromLastUpdate;

public:
    ACoreObject();

    virtual void Tick(float DeltaTime) override;

    virtual bool ShouldTickIfViewportsOnly() const override;

    UPROPERTY(BlueprintReadWrite)
    int32 NumberOfCoreObject;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
    int32 Seed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
    double BlockSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
    bool bEnableTerrainCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update")
    bool bAutoUpdate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update")
    double UpdateDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
    bool bRandomRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
    bool bRandomScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (EditCondition = "bRandomScale"))
    bool bFixedScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "0", EditCondition = "bRandomScale && bFixedScale"))
    double MinFixedScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "0", EditCondition = "bRandomScale && bFixedScale"))
    double MaxFixedScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "0", EditCondition = "bRandomScale && !bFixedScale"))
    FVector MinScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "0", EditCondition = "bRandomScale && !bFixedScale"))
    FVector MaxScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "-180", ClampMax = "180", EditCondition = "bRandomRotation"))
    FVector MinRotate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "-180", ClampMax = "180", EditCondition = "bRandomRotation"))
    FVector MaxRotate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Show Generation")
    int32 ShowLayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Show Generation")
    double ShowDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Show Generation")
    double GridHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layers")
    TArray<FLayerStruct> GeneratorLayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors Control")
    int32 IdexOfDeletingLayer;

    UPROPERTY(BlueprintReadWrite, Category = "Force Generation")
    TArray<FGenerateQueueItem> GenerateQueue;

    UPROPERTY(BlueprintReadWrite, Category = "Force Generation")
    TArray<FDeleteQueueItem> DeleteQueue;

    UPROPERTY(BlueprintReadWrite, Category = "Hidden")
    FSnapshot LastSnapshot;

    UPROPERTY(BlueprintReadWrite, Category = "Hidden")
    FSnapshot CurrentSnapshot;

    UFUNCTION(CallInEditor, DisplayName = "Generate", Category = "Generation")
    void CallGenerate();

    UFUNCTION(CallInEditor, DisplayName = "Update", Category = "Generation")
    void CallUpdate();

    UFUNCTION(CallInEditor, DisplayName = "Clear All Layers", Category = "Actors Control")
    void ClearAllLayers();

    UFUNCTION(CallInEditor, DisplayName = "Delete All Layer Actors By Index", Category = "Actors Control")
    void CallDeleteAllLayerActorsByIndex();

    UFUNCTION(CallInEditor, DisplayName = "Delete All Actors", Category = "Actors Control")
    void CallDeleteAllActors();

    UFUNCTION(CallInEditor, DisplayName = "Show Generation Grid", Category = "Show Generation")
    void CallShowGenerationGrid();

    UFUNCTION(CallInEditor, DisplayName = "Show Start Of Trace Height", Category = "Show Generation")
    void CallShowStartOfTraceHeight();

    UFUNCTION(CallInEditor, DisplayName = "Force Generate", Category = "Force Generation")
    void ForceGenerate();
};