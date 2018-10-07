// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestCClass.generated.h"

/**
 * 
 */
UCLASS()
class HURRYAROUNDTHECLOCK_API UTestCClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:
		int get5();
	
};
