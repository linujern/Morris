// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBtnIIcon.h"

#include "Morris/DOGGameInstance.h"

void UDOGBtnIIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDOGBtnIIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!Player)
	{
		AActor* A = UGameplayStatics::GetActorOfClass(GetWorld(), ADOGPlayer::StaticClass());
		Player = Cast<ADOGPlayer>(A);
	}
	else
	{
		//Change icon
		if(InputData)
		{
			switch (Cast<UDOGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->InputType)
			{
			case PC:
				InputIcon->SetBrushFromTexture(InputData->PCIcon, false);
				break;
			case XBOX:
				InputIcon->SetBrushFromTexture(InputData->XboxIcon, false);
				break;
			case PLAYSTATION:
				InputIcon->SetBrushFromTexture(InputData->PlaystationIcon, false);
				break;
			}
		}
	}
}
