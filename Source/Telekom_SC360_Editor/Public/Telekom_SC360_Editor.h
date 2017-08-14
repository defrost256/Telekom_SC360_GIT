// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleInterface.h"
#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(TrafficProbeLog, All, All);

class FTelekom_SC360_EditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};