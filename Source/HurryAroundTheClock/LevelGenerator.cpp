// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"


FLevelPart ULevelGenerator::createFirstLevelPart(int32 width, int32 height) {

	FLevelPart levelPart = FLevelPart(width, height);

	FBlockArray *blockLayer = &(levelPart.blockMatrix.GetData()[height - 1]);
	FDirectionalBlock *block = blockLayer->elements.GetData();
	for (int32 i = 0; i < width; i++) {
		*block = FDirectionalBlock(EBlockType::WALL, EDirection::UP);
		block++;
	}

	int32 step = height;

	blockLayer = levelPart.blockMatrix.GetData();
	for (int32 i = 0; i < height - 1; i++) {
		for (int32 j = 0; j < width; j += step) {
			block = &(blockLayer->elements.GetData()[j]);
			*block = FDirectionalBlock(EBlockType::WALL, EDirection::UP);
		}
		blockLayer++;
	}

	blockLayer = levelPart.blockMatrix.GetData();
	block = blockLayer->elements.GetData();
	for (int32 i = 0; i < width; i++) {
		if (block->blockType != EBlockType::WALL) {
			*block = FDirectionalBlock(EBlockType::SPIKE, EDirection::DOWN);
		}
		block++;
	}

	blockLayer = &(levelPart.blockMatrix.GetData()[height - 2]);
	block = blockLayer->elements.GetData();
	for (int32 i = 0; i < width; i++) {
		if (block->blockType != EBlockType::WALL) {
			*block = FDirectionalBlock(EBlockType::SPIKE, EDirection::UP);
		}
		block++;
	}

	return levelPart;
}


FLevelPart ULevelGenerator::createNextLevelPart(FLevelPart lastLevelPart) {

	int32 width = lastLevelPart.width, height = lastLevelPart.height;
	FLevelPart nextLevelPart = FLevelPart(width, height);

	FBlockArray *blockLayerLLP, *blockLayerNLP;
	FDirectionalBlock *blockLLP, *blockNLP;

	blockLayerLLP = lastLevelPart.blockMatrix.GetData();
	blockLayerNLP = nextLevelPart.blockMatrix.GetData();

	for (int32 i = 0; i < height; i++) {
		
		blockLLP = &(blockLayerLLP->elements.GetData()[width - 1]);
		blockNLP = blockLayerNLP->elements.GetData();
		*blockNLP = *blockLLP;

		blockLLP = blockLayerLLP->elements.GetData();
		blockNLP++;

		for (int32 j = 0; j < width - 1; j++) {
			*blockNLP = *blockLLP;
			blockNLP++;
			blockLLP++;
		}

		blockLayerLLP++;
		blockLayerNLP++;
	}

	return nextLevelPart;
}


