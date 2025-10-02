#include "ProceduralGeneratorBPLibrary.h"
#include "CoreObject.h"
#include "Editor/EditorEngine.h"
#include "Editor/Transactor.h"
#include "ProceduralGenerator.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "UObject/ReferenceChainSearch.h"
#include "RenderingThread.h"
#include "WorldPartition/WorldPartition.h"

UProceduralGeneratorBPLibrary::UProceduralGeneratorBPLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

UTexture2D* UProceduralGeneratorBPLibrary::GetLayerMap(ACoreObject* Object, int32 Layer)
{
    if (Object->GeneratorLayers.IsValidIndex(Layer))
        return Object->GeneratorLayers[Layer].Map;
    return nullptr;
}

void UProceduralGeneratorBPLibrary::SetLayerMap(ACoreObject* Object, int32 Layer, UTexture2D* Map)
{
    if (Object->GeneratorLayers.IsValidIndex(Layer))
        Object->GeneratorLayers[Layer].Map = Map;
}

ACoreObject* UProceduralGeneratorBPLibrary::CreateObject(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateObject: WorldContextObject == nullptr!"));
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateObject: UWorld not found!"));
        return nullptr;
    }

    FActorSpawnParameters SpawnInfo;

    ACoreObject* SpawnedActor = World->SpawnActor<ACoreObject>(SpawnInfo);

    if (SpawnedActor)
    {
        UE_LOG(LogTemp, Log, TEXT("ACustomObject successfuly was spawned."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACustomObject can not be spawed."));
    }

    return SpawnedActor;
}

TArray<int32> UProceduralGeneratorBPLibrary::ReadMap(ACoreObject* Object, int32 Layer)
{
    UTexture2D* Texture = Object->GeneratorLayers[Layer].Map;

    if (!Texture || !Texture->GetPlatformData() || Texture->GetPlatformData()->Mips.Num() == 0)
        return TArray<int32> {};

    FTextureSource& Source = Texture->Source;

    if (Source.IsValid())
    {
        uint8* PixelData = Source.LockMip(0);

        if (PixelData)
        {
            TArray<int32> DensityMap;

            int32 TextureWidth = Source.GetSizeX();
            int32 TextureHeight = Source.GetSizeY();

            for (int32 i = 0; i < TextureWidth * TextureHeight * 4; i += 4)
            {
                uint8 B = PixelData[i];
                uint8 G = PixelData[i + 1];
                uint8 R = PixelData[i + 2];
                uint8 A = PixelData[i + 3];
                DensityMap.Add(B);
                DensityMap.Add(G);
                DensityMap.Add(R);
                DensityMap.Add(A);
            }

            Source.UnlockMip(0);
            Texture->UpdateResource();
            FlushRenderingCommands();

            return DensityMap;
        }
    }
    return TArray<int32> {};
}

void UProceduralGeneratorBPLibrary::SetActorFolder(AActor* Actor, FName FolderPath)
{
    Actor->SetFolderPath(FolderPath);
}

TArray<AActor*> UProceduralGeneratorBPLibrary::GetAllActorsInEditor()
{
    return GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->GetAllLevelActors();
}

void UProceduralGeneratorBPLibrary::SnapToFloor(AActor* Actor)
{
    FVector Origin;
    FVector BoxExtent;
    FVector PivotOffset;
    Actor->GetActorBounds(false, Origin, BoxExtent, false);
    PivotOffset = Origin - Actor->GetActorLocation();

    FHitResult HitResult;
    FVector Start = Origin + FVector(0, 0, 1000);
    FVector End = Origin - FVector(0, 0, 100000);
    FCollisionQueryParams Params(SCENE_QUERY_STAT(SnapTrace), true, Actor);

    if (Actor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        int HeightLog = int(HitResult.Location.Z);
        //UE_LOG(LogTemp, Log, TEXT("HitResult.Z: %d %d"), HeightLog, int(BoxExtent.Z));
        Actor->SetActorLocation(HitResult.Location + BoxExtent.Z - PivotOffset);
    }
}

void UProceduralGeneratorBPLibrary::ForceGC()
{
    CollectGarbage(RF_NoFlags);
}

void UProceduralGeneratorBPLibrary::SortByGUID(UPARAM(ref) TArray<FSnapshotItem>& Array)
{
    Array.Sort([](const FSnapshotItem& A, const FSnapshotItem& B) {
        return A.ActorGUID < B.ActorGUID;
        });
}

void UProceduralGeneratorBPLibrary::ClearArrayFromNotValidActors(UPARAM(ref) FBlockStruct& Block)
{
    Block.Actors.RemoveAll(
        [](AActor* Actor)
        {
            return true;
        });
}

float UProceduralGeneratorBPLibrary::RandFloatInRange(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex, float Start, float End)
{
    if (!CoreObject)
        return 0.0f;
    if (!CoreObject->GeneratorLayers.IsValidIndex(LayerNumber))
        return 0.0f;
    if (!CoreObject->GeneratorLayers[LayerNumber].Blocks.IsValidIndex(BlockIndex))
        return 0.0f;

    float Rand = CoreObject->GeneratorLayers[LayerNumber].Blocks[BlockIndex].Stream.FRand();

    return FMath::Lerp(Start, End, Rand);
}

int32 UProceduralGeneratorBPLibrary::GetCurrentSeed(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex)
{
    if (!CoreObject)
        return 0;
    if (!CoreObject->GeneratorLayers.IsValidIndex(LayerNumber))
        return 0;
    if (!CoreObject->GeneratorLayers[LayerNumber].Blocks.IsValidIndex(BlockIndex))
        return 0;

    FRandomStream Stream = CoreObject->GeneratorLayers[LayerNumber].Blocks[BlockIndex].Stream;

    return Stream.GetCurrentSeed();
}

bool UProceduralGeneratorBPLibrary::RandBool(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex)
{
    if (!CoreObject)
        return 0;
    if (!CoreObject->GeneratorLayers.IsValidIndex(LayerNumber))
        return 0;
    if (!CoreObject->GeneratorLayers[LayerNumber].Blocks.IsValidIndex(BlockIndex))
        return 0;

    FRandomStream Stream = CoreObject->GeneratorLayers[LayerNumber].Blocks[BlockIndex].Stream;

    return Stream.GetUnsignedInt() >> 31;
}

int32 UProceduralGeneratorBPLibrary::RandIntegerInRange(ACoreObject* CoreObject, int32 LayerNumber, int32 BlockIndex, int32 Start, int32 End)
{
    if (!CoreObject)
        return 0;
    if (!CoreObject->GeneratorLayers.IsValidIndex(LayerNumber))
        return 0;
    if (!CoreObject->GeneratorLayers[LayerNumber].Blocks.IsValidIndex(BlockIndex))
        return 0;

    FRandomStream Stream = CoreObject->GeneratorLayers[LayerNumber].Blocks[BlockIndex].Stream;

    return Stream.RandRange(Start, End);
}

void UProceduralGeneratorBPLibrary::MarkBlocks(UPARAM(ref) TArray<FSnapshotItem>& LastSnapshot, UPARAM(ref) TArray<FSnapshotItem>& CurrentSnapshot, ACoreObject* CoreObject)
{
    int32 i = 0, j = 0, k = 0;

    int32 LastSnapshotLength    = LastSnapshot.Num();
    int32 CurrentSnapshotLength = CurrentSnapshot.Num();

    int32 LastSnapshotLayers    = LastSnapshot.Num() ? LastSnapshot[0].SnaphotLayers.Num() : 0;
    int32 CurrentSnapshotLayers = CurrentSnapshot.Num() ? CurrentSnapshot[0].SnaphotLayers.Num() : 0;

    if (!LastSnapshotLength && !CurrentSnapshotLength)
        return;

    TArray<FSnapshotItemLayer> Dirty;
    Dirty.SetNum(FMath::Min(LastSnapshotLayers, CurrentSnapshotLayers));

    while (i < LastSnapshotLength || j < CurrentSnapshotLength)
    {
        if (i >= LastSnapshotLength)
        {
            for (FSnapshotItemLayer Layer : CurrentSnapshot[j].SnaphotLayers)
            {
                MarkHelper(Layer, Dirty);
            }
            j++;
        }
        else if (j >= CurrentSnapshotLength || LastSnapshot[i].ActorGUID < CurrentSnapshot[j].ActorGUID)
        {
            for (FSnapshotItemLayer Layer : LastSnapshot[i].SnaphotLayers)
            {
                MarkHelper(Layer, Dirty);
            }
            i++;
        }
        else if (CurrentSnapshot[j].ActorGUID < LastSnapshot[i].ActorGUID)
        {
            for (FSnapshotItemLayer Layer : CurrentSnapshot[j].SnaphotLayers)
            {
                MarkHelper(Layer, Dirty);
            }
            j++;
        }
        else //LastSnapshot[i].ActorGUID == CurrentSnapshot[j].ActorGUID
        {
            if (LastSnapshot[i].Transform.GetRotation() != CurrentSnapshot[j].Transform.GetRotation() || LastSnapshot[i].Transform.GetTranslation() != CurrentSnapshot[j].Transform.GetTranslation() || LastSnapshot[i].Transform.GetScale3D() != CurrentSnapshot[j].Transform.GetScale3D())
            {
                for (FSnapshotItemLayer Layer : LastSnapshot[i].SnaphotLayers)
                {
                    MarkHelper(Layer, Dirty);
                }

                for (FSnapshotItemLayer Layer : CurrentSnapshot[j].SnaphotLayers)
                {
                    MarkHelper(Layer, Dirty);
                }
            }
            i++, j++;
        }
    }

    Dirty.SetNum(CurrentSnapshotLayers);

    for (FSnapshotItemLayer Layer : Dirty)
    {
        for (int32 Block : Layer.DependedBlocks)
        {
            if (CoreObject->GeneratorLayers.IsValidIndex(k))
            {
                if (CoreObject->GeneratorLayers[k].Blocks.IsValidIndex(Block))
                {
                    CoreObject->GeneratorLayers[k].Blocks[Block].bDirty = true;
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("Block = %d is not walid"), Block);
                }
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("k = %d is not walid"), k);
            }
        }
        k++;
    }
}

void UProceduralGeneratorBPLibrary::MarkHelper(FSnapshotItemLayer Layer, TArray<FSnapshotItemLayer>& Dirty)
{
    for (int32 Block : Layer.DependedBlocks)
    {
        for (int32 i = 0; i < Dirty.Num(); i++)
        {
            Dirty[i].DependedBlocks.AddUnique(Block);
        }
    }
}

FGuid UProceduralGeneratorBPLibrary::GetAssetGUID(UObject* Asset)
{
    if (!Asset)
        return FGuid();

    return Asset->GetPackage()->GetGuid();
}
