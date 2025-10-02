#include "CoreObject.h"
#include "ProceduralGenerator.h"
#include "ProceduralGeneratorBPLibrary.h"

ACoreObject::ACoreObject()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void ACoreObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeFromLastUpdate += DeltaTime;

    if (TimeFromLastUpdate >= UpdateDelay)
    {
        if (GetWorld() && GetWorld()->WorldType == EWorldType::Editor)
        {
            if (!this->IsTemplate())
            {
                UFunction* Function = FindFunction(TEXT("AutoUpdate"));
                if (Function)
                {
                    ProcessEvent(Function, nullptr);
                    TimeFromLastUpdate = 0.0;
                }
            }
        }
    }
}

void ACoreObject::ForceGenerate()
{
    FScopedSlowTask SlowTask1(1, NSLOCTEXT("Procedural Generator", "Generating", "Generation..."), true);
    SlowTask1.MakeDialog(false, false);

    UFunction* Function1 = FindFunction(TEXT("Generate"));

    if (Function1)
    {
        ProcessEvent(Function1, nullptr);
        SlowTask1.EnterProgressFrame(1.0f);
    }

    int32 AmountOfBlocks = GenerateQueue.Num();

    UE_LOG(LogTemp, Log, TEXT("%d"), AmountOfBlocks);

    FScopedSlowTask SlowTask2(AmountOfBlocks, NSLOCTEXT("Procedural Generator", "Generating", "Generation..."), true);
    SlowTask2.MakeDialog(false, false);

    for (int32 i = 0; i < AmountOfBlocks; i++)
    {
        SlowTask2.EnterProgressFrame(1.0f);

        struct FParams
        {
            bool Generated;
        }Params(false);

        UFunction* Function2 = FindFunction(TEXT("GenerateBlockFromQueue"));

        if (Function2)
        {
            ProcessEvent(Function2, &Params);
        }
    }
}

bool ACoreObject::ShouldTickIfViewportsOnly() const
{
    return true;
}

void ACoreObject::CallGenerate()
{
    FString FunctionName = TEXT("Generate");

    this->CallFunctionByNameWithArguments(*FunctionName, *GLog, nullptr, true);
}

void ACoreObject::CallUpdate()
{
    FString FunctionName = TEXT("Update");

    this->CallFunctionByNameWithArguments(*FunctionName, *GLog, nullptr, true);
}

void ACoreObject::ClearAllLayers()
{
    for (FLayerStruct& Layer : GeneratorLayers)
    {
        for (FBlockStruct& Block : Layer.Blocks)
        {
            Block.Actors.RemoveAll(
                [](AActor* Actor)
                {
                    return !IsValid(Actor);
                });
        }
    }
}

void ACoreObject::CallDeleteAllLayerActorsByIndex()
{
    FString FunctionName = TEXT("DeleteAllLayerActorsByIndex");
    FString Args = FString::FromInt(IdexOfDeletingLayer) + TEXT(" true");
    FString Call = FunctionName + TEXT(" ") + Args;

    this->CallFunctionByNameWithArguments(*Call, *GLog, nullptr, true);
}

void ACoreObject::CallDeleteAllActors()
{
    FString FunctionName = TEXT("DeleteAllActors");

    this->CallFunctionByNameWithArguments(*FunctionName, *GLog, nullptr, true);
}

void ACoreObject::CallShowGenerationGrid()
{
    FString FunctionName = TEXT("ShowGenerationGrid");

    this->CallFunctionByNameWithArguments(*FunctionName, *GLog, nullptr, true);
}

void ACoreObject::CallShowStartOfTraceHeight()
{
    FString FunctionName = TEXT("ShowStartOfTraceHeight");

    this->CallFunctionByNameWithArguments(*FunctionName, *GLog, nullptr, true);
}
