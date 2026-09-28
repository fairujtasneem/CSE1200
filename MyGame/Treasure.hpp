#ifndef TREASURE_HPP
#define TREASURE_HPP

#include "iGraphics.h"
#include "GameState.hpp"
#include "Score.hpp"
#include "Level.hpp"
#include <stdio.h>

// ======================================================
// SCREEN SIZE
// ======================================================

#define TREASURE_SCREEN_WIDTH 1500
#define TREASURE_SCREEN_HEIGHT 1000

// ======================================================
// TREASURE TEXTURE
// ======================================================

GLuint treasureTexture = 0;

int treasureImageWidth = 0;
int treasureImageHeight = 0;

// ======================================================
// TREASURE POSITION (Placed safely inside 4167 boundary)
// ======================================================

float treasureX = 3700; // Fixed: Changed from 4400 to 3700
float treasureY = 180;

// ======================================================
// TREASURE SIZE
// ======================================================

int treasureWidth = 125;
int treasureHeight = 125;

// ======================================================
// TREASURE STATUS
// ======================================================

bool treasureVisible = true;
bool treasureCollected = false;

// ======================================================
// LOAD TREASURE PNG
// ======================================================

GLuint loadTreasurePNG(
	const char *filename,
	int *width,
	int *height
	)
{
	int channels;

	unsigned char *imageData = stbi_load(
		filename,
		width,
		height,
		&channels,
		4
		);

	if (imageData == NULL)
	{
		printf("ERROR: Cannot load treasure image: %s\n", filename);
		return 0;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		*width,
		*height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		imageData
		);

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	printf("Treasure image loaded: %s\n", filename);
	return texture;
}

// ======================================================
// RESET TREASURE
// ======================================================

void resetTreasure()
{
	treasureVisible = true;
	treasureCollected = false;

	// Reset position inside the island
	treasureX = 3700;
	treasureY = 180;
}

// ======================================================
// INITIALIZE TREASURE
// ======================================================

void initializeTreasure()
{
	treasureTexture = loadTreasurePNG(
		"treasure.png",
		&treasureImageWidth,
		&treasureImageHeight
		);

	resetTreasure();
}

// ======================================================
// DRAW TREASURE
// ======================================================

void drawTreasure(float cameraX)
{
	if (!treasureVisible || treasureCollected || treasureTexture == 0)
	{
		return;
	}

	// Calculate treasure screen position relative to camera
	float screenX = treasureX - cameraX;

	// Don't draw if it is completely off screen
	if (screenX + treasureWidth < 0 || screenX > TREASURE_SCREEN_WIDTH)
	{
		return;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, treasureTexture);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);

	// Bottom Left
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(screenX, treasureY);

	// Bottom Right
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(screenX + treasureWidth, treasureY);

	// Top Right
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(screenX + treasureWidth, treasureY + treasureHeight);

	// Top Left
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(screenX, treasureY + treasureHeight);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

// ======================================================
// COLLECT TREASURE
// ======================================================

void collectTreasure(int playerLife)
{
	if (treasureCollected)
	{
		return;
	}

	treasureCollected = true;
	treasureVisible = false;

	// 1. Calculate and add level score
	addLevelScore(currentLevel, playerLife);

	// 2. Unlock the next level
	if (currentLevel == 1)
	{
		completeLevel1();
	}
	else if (currentLevel == 2)
	{
		completeLevel2();
	}
	else if (currentLevel == 3)
	{
		completeLevel3();
	}

	// 3. Trigger LEVEL_COMPLETE state
	levelCompleted = true;
	currentState = LEVEL_COMPLETE;

	printf("\n====================================\n");
	printf("TREASURE COLLECTED! LEVEL COMPLETE!\n");
	printf("Treasure Points : +%d\n", levelTreasurePoints);
	printf("Health Bonus    : +%d\n", healthBonus);
	printf("Level Score     : +%d\n", levelScore);
	printf("Total Score     : %d\n", totalScore);
	printf("====================================\n\n");
}

// ======================================================
// TREASURE COLLISION
// ======================================================

bool treasureCollision(
	float playerX,
	float playerY,
	int playerWidth,
	int playerHeight,
	float cameraX = 0.0f
	)
{
	if (!treasureVisible || treasureCollected)
	{
		return false;
	}

	// Player bounding box
	float playerLeft = playerX;
	float playerRight = playerX + playerWidth;
	float playerBottom = playerY;
	float playerTop = playerY + playerHeight;

	// Treasure bounding box
	float treasureLeft = treasureX;
	float treasureRight = treasureX + treasureWidth;
	float treasureBottom = treasureY;
	float treasureTop = treasureY + treasureHeight;

	// Bounding box collision check
	if (playerRight > treasureLeft &&
		playerLeft < treasureRight &&
		playerTop > treasureBottom &&
		playerBottom < treasureTop)
	{
		return true;
	}

	return false;
}

// ======================================================
// CHECK TREASURE COLLECTION
// ======================================================

void checkTreasureCollection(
	float playerX,
	float playerY,
	int playerWidth,
	int playerHeight,
	int playerLife,
	float cameraX = 0.0f
	)
{
	// Level completes if player touches the treasure box OR reaches the end of the island (X >= 3600)
	if (treasureCollision(playerX, playerY, playerWidth, playerHeight, cameraX) || playerX >= 3600)
	{
		collectTreasure(playerLife);
	}
}

#endif // TREASURE_HPP