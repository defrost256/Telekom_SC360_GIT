// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineInterface_FL.h"

TSharedPtr<FMessageLog> UEngineInterface_FL::lastLog;
TSharedPtr<FTokenizedMessage> UEngineInterface_FL::currentMessage;
FName UEngineInterface_FL::lastLogName;

void UEngineInterface_FL::DrawDebugArc(UObject * WorldContextObject, FVector Center, float Radius, float FillRatio, int32 NumSegments, FLinearColor LineColor, float LifeTime, float Thickness, FRotator Rotation, bool bDrawAxis)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		DrawDebugArc_Internal(World, Center, Radius, FillRatio, NumSegments, LineColor, LifeTime, Thickness, Rotation, bDrawAxis);
	}
}

void UEngineInterface_FL::DrawDebugArc_Internal(UWorld * InWorld, FVector Center, float Radius, float FillRatio, int32 NumSegments, FLinearColor LineColor, float LifeTime, float Thickness, FRotator Rotation, bool bDrawAxis)
{
	FColor color = LineColor.ToFColor(true);
	FillRatio = FMath::Clamp(FillRatio, -1.f, 1.f);
	FMatrix TM;
	TM.SetOrigin(Center);
	FQuat rot = Rotation.Quaternion();
	TM.SetAxis(0, rot.GetAxisX());
	TM.SetAxis(1, rot.GetAxisY());
	TM.SetAxis(2, rot.GetAxisZ());

	// no debug line drawing on dedicated server
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		ULineBatchComponent* const LineBatcher = GetDebugLineBatcher(InWorld, false, LifeTime, false);
		if (LineBatcher)
		{
			const float LineLifeTime = (LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime;
			const FVector AxisY = rot.GetAxisY();
			const FVector AxisZ = rot.GetAxisZ();

			// Need at least 4 segments
			NumSegments = FMath::Max(NumSegments, 4);
			const float AngleStep = 2.f * PI * FillRatio / float(NumSegments);

			TArray<FBatchedLine> Lines;
			Lines.Empty(NumSegments + 2);

			float Angle = 0.f;
			bool bFirst = true;
			while (NumSegments--)
			{
				const FVector Vertex1 = Center + Radius * (AxisY * FMath::Cos(Angle) + AxisZ * FMath::Sin(Angle));
				if (bFirst)
				{
					Lines.Add(FBatchedLine(Center, Vertex1, color, LineLifeTime, Thickness, SDPG_World));
					bFirst = false;
				}
				Angle += AngleStep;
				const FVector Vertex2 = Center + Radius * (AxisY * FMath::Cos(Angle) + AxisZ * FMath::Sin(Angle));
				Lines.Add(FBatchedLine(Vertex1, Vertex2, color, LineLifeTime, Thickness, SDPG_World));
				if (NumSegments == 0)
				{
					Lines.Add(FBatchedLine(Vertex2, Center, color, LineLifeTime, Thickness, SDPG_World));
				}
			}
			if (bDrawAxis)
			{
				Lines.Add(FBatchedLine(Center - Radius * AxisY, Center + Radius * AxisY, color, LineLifeTime, Thickness, SDPG_World));
				Lines.Add(FBatchedLine(Center - Radius * AxisZ, Center + Radius * AxisZ, color, LineLifeTime, Thickness, SDPG_World));
			}
			LineBatcher->DrawLines(Lines);
		}
	}
}

void UEngineInterface_FL::CreateMessageLog(const FName & name)
{
	if (lastLogName.Compare(name) != 0)
	{
		lastLog = TSharedPtr<FMessageLog>(new FMessageLog(name));
		lastLogName = name;
	}
	lastLog->NewPage(FText::AsCultureInvariant(name.ToString()));
}

void UEngineInterface_FL::ClearMessageLog()
{
}

void UEngineInterface_FL::StartComposing(EMessageLogSeverity Severity, const FText & Message)
{
	if (lastLog.IsValid())
	{
		currentMessage = FTokenizedMessage::Create(EMessageSeverity::Type(Severity), Message);
	}
}

void UEngineInterface_FL::FinishComposing()
{
	if (lastLog.IsValid() && currentMessage.IsValid())
	{
		lastLog->AddMessage(currentMessage.ToSharedRef());
	}
}

void UEngineInterface_FL::ShowLog(EMessageLogSeverity Severity, bool bShowIfEmpty)
{
	if (lastLog.IsValid())
	{
		lastLog->Open(EMessageSeverity::Type(Severity), bShowIfEmpty);
	}
}

void UEngineInterface_FL::AddUObjectToken(UObject * WorldContextObject, UObject * object, const FText & label)
{
	if (currentMessage.IsValid())
	{
		currentMessage->AddToken(FUObjectToken::Create(object, label));
	}
}

void UEngineInterface_FL::AddTextToken(UObject * WorldContextObject, const FText & text)
{
	if (currentMessage.IsValid())
	{
		currentMessage->AddToken(FTextToken::Create(text));
	}
}
