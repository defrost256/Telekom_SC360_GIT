// Fill out your copyright notice in the Description page of Project Settings.

#include "Telekom_SC360_Editor.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FTelekom_SC360_EditorModule, Telekom_SC360_Git);

DEFINE_LOG_CATEGORY(TrafficProbeLog);

void FTelekom_SC360_EditorModule::StartupModule()
{
	UE_LOG(TrafficProbeLog, Warning, TEXT("Telekom_SC360_Editor Module starting up"));
}

void FTelekom_SC360_EditorModule::ShutdownModule()
{
	UE_LOG(TrafficProbeLog, Warning, TEXT("Telekom_SC360_Editor Module shutting down"));
}
