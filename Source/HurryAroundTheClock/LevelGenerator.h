// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelGenerator.generated.h"


UENUM(BlueprintType)
enum class EDirection: uint8 {
	UP			UMETA(DisplayName = "Up"),
	RIGHT		UMETA(DisplayName = "Right"),
	DOWN		UMETA(DisplayName = "Down"),
	LEFT		UMETA(DisplayName = "Left")
};


UENUM(BlueprintType)
enum class EBlockType : uint8 {
	EMPTY		UMETA(DisplayName = "Empty"),
	WALL		UMETA(DisplayName = "Wall"),
	SPIKE		UMETA(DisplayName = "Spike")
};


USTRUCT(BlueprintType)
struct FDirectionalBlock {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelGenerator")
	EBlockType blockType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelGenerator")
	EDirection direction;

	FDirectionalBlock() {
		blockType = EBlockType::EMPTY;
		direction = EDirection::UP;
	}

	FDirectionalBlock(EBlockType blockType, EDirection direction) {
		this->blockType = blockType;
		this->direction = direction;
	}
};


USTRUCT(BlueprintType)
struct FBlockArray {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelGenerator")
	TArray<FDirectionalBlock> elements;

	FBlockArray() {
		elements.Init(FDirectionalBlock(), 0);
	}

	FBlockArray(int32 size) {
		elements.Init(FDirectionalBlock(), size);
	}
};


USTRUCT(BlueprintType)
struct FLevelPart {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelGenerator")
	TArray<FBlockArray> blockMatrix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelGenerator")
	int32 width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelGenerator")
	int32 height;

	FLevelPart() {
		width = 0;
		height = 0;
		blockMatrix.Init(FBlockArray(width), height);
	}

	FLevelPart(int32 width, int32 height) {
		this->width = width;
		this->height = height;
		blockMatrix.Init(FBlockArray(width), height);
	}
};


UCLASS()
class HURRYAROUNDTHECLOCK_API ULevelGenerator : public UBlueprintFunctionLibrary {

	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "LevelGenerator")
	static FLevelPart createFirstLevelPart(int32 width, int32 height);

	UFUNCTION(BlueprintCallable, Category = "LevelGenerator")
	static FLevelPart createNextLevelPart(FLevelPart lastLevelPart);
};
