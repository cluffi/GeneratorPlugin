#include "ProceduralGenerator.h"
#include "CoreObject.h"
#include "Editor.h"
#include "Engine/World.h"
#include "ToolMenus.h"
#include "UObject/ObjectSaveContext.h"
#include "CoreObject.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "FProceduralGeneratorModule"

void FProceduralGeneratorModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("ProceduralGenerator: StartupModule called"));

    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FProceduralGeneratorModule::RegisterMenuExtensions));
}

void FProceduralGeneratorModule::ShutdownModule()
{
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
}

void FProceduralGeneratorModule::RegisterMenuExtensions()
{
    UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
    FToolMenuSection& ToolsSection = ToolsMenu->AddSection(
        "ProceduralGenerator",
        INVTEXT("Procedural Generator")
    );

    ToolsSection.AddEntry(FToolMenuEntry::InitMenuEntry(
        "ProceduralGenerator",
        INVTEXT("Create core object"),
        INVTEXT("Creates a root object that will be used to configure generation."),
        FSlateIcon(),
        FExecuteAction::CreateLambda([this]()
            {
                bool bSpawned = false;

                UWorld* World = GEditor->GetEditorWorldContext().World();

                if (!World)
                    return;

                UClass* BPClass = LoadClass<AActor>(nullptr, TEXT("/ProceduralGenerator/CoreObject.CoreObject_C"));

                int32 MaxActorNumber = -1;
                
                for (TActorIterator<ACoreObject> It(World, ACoreObject::StaticClass(), EActorIteratorFlags::SkipPendingKill | EActorIteratorFlags::OnlyActiveLevels); It; ++It)
                {
                    ACoreObject* Actor = *It;

                    if (!Actor->IsTemplate() && Actor->NumberOfCoreObject > MaxActorNumber)
                            MaxActorNumber = Actor->NumberOfCoreObject;
                }

                MaxActorNumber++;
                
                FActorSpawnParameters SpawnParams;
                AActor* SpawnedActor = World->SpawnActor<AActor>(BPClass, FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams);

                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Log, TEXT("ACustomObject successfuly was spawned."));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("ACustomObject can not be spawed."));
                    return;
                }

                if (MaxActorNumber)
                {
                    SpawnedActor->SetFolderPath(FName(FString::Printf(TEXT("ProceduralGenerator%d"), MaxActorNumber)));
                    SpawnedActor->SetActorLabel(FString::Printf(TEXT("CoreObject%d"), MaxActorNumber));
                }
                else
                {
                    SpawnedActor->SetFolderPath(FName(TEXT("ProceduralGenerator")));
                    SpawnedActor->SetActorLabel(TEXT("CoreObject"));
                }

                ACoreObject* CoreObj = Cast<ACoreObject>(SpawnedActor);
                CoreObj->NumberOfCoreObject = MaxActorNumber;
            })
    ));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProceduralGeneratorModule, ProceduralGenerator)

