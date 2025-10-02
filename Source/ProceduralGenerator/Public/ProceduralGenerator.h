#pragma once

#include "Modules/ModuleManager.h"


class FProceduralGeneratorModule : public IModuleInterface
{
	void RegisterMenuExtensions();

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
