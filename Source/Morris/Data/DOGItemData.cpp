// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGItemData.h"

void UDOGItemData::PostInitProperties()
{
	Super::PostInitProperties();
	ShopPrice.Min = 50.f;
	ShopPrice.Max = 100.f;
}
