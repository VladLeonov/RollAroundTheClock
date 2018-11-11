// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"


inline bool randomBool(float probability) {
	return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < probability;
}


inline void fillArrayOfMarks(TArray<bool> &array, float probability) {
	for (int32 i = 0; i < array.Num(); i++) {
		array.GetData()[i] = randomBool(probability);
	}
}

inline void fillArrayOfMarks(TArray<bool> &array, float probability, TArray<bool> &limiters) {
	for (int32 i = 0; i < array.Num(); i++) {
		array.GetData()[i] = limiters.GetData()[i] && randomBool(probability);
	}
}

FLevelPart ULevelGenerator::createFirstLevelPart(int32 width, int32 height, int32 widthPart, FGenerationParameters generationParameters) {

	FLevelPart stubLevelPart = FLevelPart(width, height, widthPart);
	fillArrayOfMarks(stubLevelPart.ceilingWallMarks, generationParameters.horizontalWallProbability);
	return createNextLevelPart(stubLevelPart, generationParameters);
}


FLevelPart ULevelGenerator::createNextLevelPart(FLevelPart lastLevelPart, FGenerationParameters generationParameters) {

	int32 width = lastLevelPart.width;
	int32 height = lastLevelPart.height;
	int32 widthPart = lastLevelPart.widthPart;
	FLevelPart nextLevelPart = FLevelPart(width, height, widthPart);
	int32 numHorizontalParts = width / widthPart;

	TArray<bool> &ceilingMarks = nextLevelPart.ceilingWallMarks;
	TArray<bool> floorMarks = lastLevelPart.ceilingWallMarks;
	TArray<bool> wallMarks;
	TArray<bool> ceilingSpikesMarks;
	TArray<bool> floorSpikesMarks;
	wallMarks.SetNum(numHorizontalParts);
	ceilingSpikesMarks.SetNum(numHorizontalParts);
	floorSpikesMarks.SetNum(numHorizontalParts);

	fillArrayOfMarks(ceilingMarks, generationParameters.horizontalWallProbability);
	fillArrayOfMarks(wallMarks, generationParameters.verticalWallProbability);
	fillArrayOfMarks(ceilingSpikesMarks, generationParameters.spikesProbability, ceilingMarks);
	fillArrayOfMarks(floorSpikesMarks, generationParameters.spikesProbability, floorMarks);

	//floor
	FBlockArray *blockLayer = &(nextLevelPart.blockMatrix.GetData()[height - 1]);
	FDirectionalBlock *block = blockLayer->elements.GetData();
	for (int32 i = 0; i < numHorizontalParts; i++) {
		if (floorMarks.GetData()[i] 
			|| floorMarks.GetData()[(i - 1 + numHorizontalParts) % numHorizontalParts] 
			|| wallMarks.GetData()[i]) {
			*block = FDirectionalBlock(EBlockType::WALL, EDirection::UP);
		}
		if (floorMarks.GetData()[i]) {
			for (int32 j = 0; j < widthPart - 1; j++) {
				block++;
				*block = FDirectionalBlock(EBlockType::WALL, EDirection::UP);
			}
			block++;
		} else {
			block += widthPart;
		}
	}

	//seiling spikes
	blockLayer = nextLevelPart.blockMatrix.GetData();
	block = blockLayer->elements.GetData();
	for (int32 i = 0; i < numHorizontalParts; i++) {
		if (ceilingSpikesMarks.GetData()[i]) {
			if (ceilingSpikesMarks.GetData()[(i - 1 + numHorizontalParts) % numHorizontalParts]) {
				*block = FDirectionalBlock(EBlockType::SPIKE, EDirection::DOWN);
			}
			for (int32 j = 0; j < widthPart - 1; j++) {
				block++;
				*block = FDirectionalBlock(EBlockType::SPIKE, EDirection::DOWN);
			}
			block++;
		} else {
			block += widthPart;
		}
	}

	//floor spikes
	blockLayer = &(nextLevelPart.blockMatrix.GetData()[height - 2]);
	block = blockLayer->elements.GetData();
	for (int32 i = 0; i < numHorizontalParts; i++) {
		if (floorSpikesMarks.GetData()[i]) {
			if (floorSpikesMarks.GetData()[(i - 1 + numHorizontalParts) % numHorizontalParts]) {
				*block = FDirectionalBlock(EBlockType::SPIKE, EDirection::UP);
			}
			for (int32 j = 0; j < widthPart - 1; j++) {
				block++;
				*block = FDirectionalBlock(EBlockType::SPIKE, EDirection::UP);
			}
			block++;
		}
		else {
			block += widthPart;
		}
	}

	//walls
	blockLayer = nextLevelPart.blockMatrix.GetData();
	for (int32 i = 0; i < height - 1; i++) {
		for (int32 j = 0; j < numHorizontalParts; j++) {
			if (wallMarks.GetData()[j]) {
				blockLayer->elements.GetData()[j * widthPart]
					= FDirectionalBlock(EBlockType::WALL, EDirection::UP);
			}
		}
		blockLayer++;
	}

	return nextLevelPart;
}


