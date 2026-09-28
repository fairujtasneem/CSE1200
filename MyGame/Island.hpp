#ifndef ISLAND_HPP
#define ISLAND_HPP

#define _CRT_SECURE_NO_WARNINGS

#include "iGraphics.h"
#include "GameState.hpp"
#include "Player.hpp"
#include "Treasure.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

// ======================================================
// SCREEN SIZE
// ======================================================

#define ISLAND_WIDTH 1500
#define ISLAND_HEIGHT 1000
#define ISLAND_WORLD_WIDTH 4167

// ======================================================
// CAMERA
// ======================================================

float islandCameraX = 0;

// ======================================================
// PROCEDURAL SOUND FX GENERATORS
// ======================================================

void generateShootSound()
{
	FILE *check = NULL;
	if (fopen_s(&check, "shoot.wav", "r") == 0 && check != NULL)
	{
		fclose(check);
		return;
	}

	int sampleRate = 22050;
	float duration = 0.25f;
	int numSamples = (int)(sampleRate * duration);

	FILE *f = NULL;
	if (fopen_s(&f, "shoot.wav", "wb") != 0 || f == NULL) return;

	int dataSize = numSamples * 2;
	int fileSize = 36 + dataSize;
	short channels = 1, bitsPerSample = 16;
	int byteRate = sampleRate * channels * (bitsPerSample / 8);
	short blockAlign = channels * (bitsPerSample / 8);

	fwrite("RIFF", 1, 4, f); fwrite(&fileSize, 4, 1, f);
	fwrite("WAVEfmt ", 1, 8, f);
	int subchunk1Size = 16; short audioFormat = 1;
	fwrite(&subchunk1Size, 4, 1, f); fwrite(&audioFormat, 2, 1, f);
	fwrite(&channels, 2, 1, f); fwrite(&sampleRate, 4, 1, f);
	fwrite(&byteRate, 4, 1, f); fwrite(&blockAlign, 2, 1, f);
	fwrite(&bitsPerSample, 2, 1, f);
	fwrite("data", 1, 4, f); fwrite(&dataSize, 4, 1, f);

	for (int i = 0; i < numSamples; i++)
	{
		float t = (float)i / (float)sampleRate;
		float freq = 800.0f - (t * 2200.0f);
		float sample = sin(2.0f * 3.14159f * freq * t) * (1.0f - (t / duration));
		short s = (short)(sample * 24000.0f);
		fwrite(&s, 2, 1, f);
	}
	fclose(f);
}

void generateKillSound()
{
	FILE *check = NULL;
	if (fopen_s(&check, "kill.wav", "r") == 0 && check != NULL)
	{
		fclose(check);
		return;
	}

	int sampleRate = 22050;
	float duration = 0.45f;
	int numSamples = (int)(sampleRate * duration);

	FILE *f = NULL;
	if (fopen_s(&f, "kill.wav", "wb") != 0 || f == NULL) return;

	int dataSize = numSamples * 2;
	int fileSize = 36 + dataSize;
	short channels = 1, bitsPerSample = 16;
	int byteRate = sampleRate * channels * (bitsPerSample / 8);
	short blockAlign = channels * (bitsPerSample / 8);

	fwrite("RIFF", 1, 4, f); fwrite(&fileSize, 4, 1, f);
	fwrite("WAVEfmt ", 1, 8, f);
	int subchunk1Size = 16; short audioFormat = 1;
	fwrite(&subchunk1Size, 4, 1, f); fwrite(&audioFormat, 2, 1, f);
	fwrite(&channels, 2, 1, f); fwrite(&sampleRate, 4, 1, f);
	fwrite(&byteRate, 4, 1, f); fwrite(&blockAlign, 2, 1, f);
	fwrite(&bitsPerSample, 2, 1, f);
	fwrite("data", 1, 4, f); fwrite(&dataSize, 4, 1, f);

	for (int i = 0; i < numSamples; i++)
	{
		float t = (float)i / (float)sampleRate;
		float noise = ((rand() % 2000) - 1000) / 1000.0f;
		float lowBoom = sin(2.0f * 3.14159f * 90.0f * t);
		float sample = (0.7f * lowBoom + 0.3f * noise) * exp(-6.0f * t);
		short s = (short)(sample * 28000.0f);
		fwrite(&s, 2, 1, f);
	}
	fclose(f);
}

void playShootSound()
{
	PlaySound(TEXT("shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

void playKillSound()
{
	PlaySound(TEXT("kill.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

// ======================================================
// ISLAND PLAYER
// ======================================================

float islandPlayerX = 150;
float islandPlayerY = 180;
int islandPlayerWidth = 150;
int islandPlayerHeight = 180;
float islandPlayerSpeed = 25;

GLuint islandPlayerTexture[3] = { 0, 0, 0 };
int islandPlayerImageWidth[3] = { 0, 0, 0 };
int islandPlayerImageHeight[3] = { 0, 0, 0 };
int islandPlayerFrame = 0;
int islandPlayerAnimationCounter = 0;
bool islandPlayerMoving = false;

// ======================================================
// ISLAND BACKGROUND
// ======================================================

GLuint islandBackgroundTexture = 0;
int islandBackgroundWidth = 0;
int islandBackgroundHeight = 0;

// ======================================================
// LEVEL 3 ONLY: GREEN HILL & REALISTIC ROUNDED VIPER BOSS
// ======================================================

GLuint hillTexture = 0;
int hillImageWidth = 0;
int hillImageHeight = 0;
float hillStartX = 3100;
float hillRenderWidth = 950;
float hillRenderHeight = 450;

GLuint snakeTexture = 0;
int snakeImageWidth = 0;
int snakeImageHeight = 0;
float snakeBossX = 3450;
float snakeBossY = 220;
int snakeBossWidth = 260;
int snakeBossHeight = 300;

int snakeBossHealth = 6;
int snakeBossMaxHealth = 6;
bool snakeBossAlive = true;
int snakeHurtTimer = 0;
float snakeTongueTimer = 0;

// ======================================================
// ARROW SHOOTING SYSTEM
// ======================================================

#define MAX_ARROWS 10

struct IslandArrow
{
	float x;
	float y;
	bool active;
	float speed;
};

IslandArrow playerArrows[MAX_ARROWS];
int arrowShootCooldown = 0;

// ======================================================
// INDIGENOUS ENEMIES
// ======================================================

float islandEnemy1X = 2200;
float islandEnemy1Y = 180;
int islandEnemy1Health = 3;
bool islandEnemy1Alive = false;

float islandEnemy2X = 2550;
float islandEnemy2Y = 180;
int islandEnemy2Health = 3;
bool islandEnemy2Alive = false;

int islandEnemyWidth = 160;
int islandEnemyHeight = 190;
float islandEnemySpeed = 3.5f;

int level3KilledCount = 0;
int level3TotalEnemies = 5;

GLuint islandEnemyTexture[3] = { 0, 0, 0 };
int islandEnemyImageWidth[3] = { 0, 0, 0 };
int islandEnemyImageHeight[3] = { 0, 0, 0 };
int islandEnemyFrame = 0;
int islandEnemyAnimationCounter = 0;

bool islandPlayerAttack = false;
int islandAttackCooldown = 0;

// ======================================================
// IMAGE LOADER
// ======================================================

GLuint loadIslandPNG(const char *filename, int *width, int *height)
{
	int channels;
	unsigned char *imageData = stbi_load(filename, width, height, &channels, 4);
	if (imageData == NULL) return 0;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

void initializeArrows()
{
	for (int i = 0; i < MAX_ARROWS; i++)
	{
		playerArrows[i].active = false;
		playerArrows[i].speed = 36.0f;
	}
	arrowShootCooldown = 0;
}

void shootArrow()
{
	if (currentState != ISLAND_EXPLORATION || arrowShootCooldown > 0) return;

	for (int i = 0; i < MAX_ARROWS; i++)
	{
		if (!playerArrows[i].active)
		{
			playerArrows[i].x = islandPlayerX + islandPlayerWidth - 10;
			playerArrows[i].y = islandPlayerY + (islandPlayerHeight / 2.0f);
			playerArrows[i].active = true;
			arrowShootCooldown = 12;
			playShootSound();
			break;
		}
	}
}

void drawArrows()
{
	for (int i = 0; i < MAX_ARROWS; i++)
	{
		if (playerArrows[i].active)
		{
			float arrowScreenX = playerArrows[i].x - islandCameraX;
			float arrowY = playerArrows[i].y;

			if (arrowScreenX < -50 || arrowScreenX > ISLAND_WIDTH + 50) continue;

			iSetColor(139, 69, 19);
			iFilledRectangle(arrowScreenX, arrowY - 2, 45, 4);

			iSetColor(220, 220, 230);
			double headX[] = { arrowScreenX + 45, arrowScreenX + 60, arrowScreenX + 45 };
			double headY[] = { arrowY + 6, arrowY, arrowY - 6 };
			iFilledPolygon(headX, headY, 3);

			iSetColor(255, 50, 50);
			double fletch1X[] = { arrowScreenX, arrowScreenX - 10, arrowScreenX };
			double fletch1Y[] = { arrowY, arrowY + 6, arrowY + 2 };
			iFilledPolygon(fletch1X, fletch1Y, 3);

			double fletch2X[] = { arrowScreenX, arrowScreenX - 10, arrowScreenX };
			double fletch2Y[] = { arrowY, arrowY - 6, arrowY - 2 };
			iFilledPolygon(fletch2X, fletch2Y, 3);
		}
	}
}

void updateArrows()
{
	if (arrowShootCooldown > 0) arrowShootCooldown--;

	for (int i = 0; i < MAX_ARROWS; i++)
	{
		if (!playerArrows[i].active) continue;

		playerArrows[i].x += playerArrows[i].speed;

		if (playerArrows[i].x - islandCameraX > ISLAND_WIDTH + 100)
		{
			playerArrows[i].active = false;
			continue;
		}

		if (currentLevel == 3 && snakeBossAlive)
		{
			if (playerArrows[i].x >= snakeBossX && playerArrows[i].x <= snakeBossX + snakeBossWidth &&
				playerArrows[i].y >= snakeBossY && playerArrows[i].y <= snakeBossY + snakeBossHeight)
			{
				snakeBossHealth--;
				snakeHurtTimer = 12;
				playerArrows[i].active = false;

				if (snakeBossHealth <= 0)
				{
					snakeBossHealth = 0;
					snakeBossAlive = false;
					playKillSound();
				}
				continue;
			}
		}

		if (islandEnemy1Alive && playerArrows[i].x >= islandEnemy1X && playerArrows[i].x <= islandEnemy1X + islandEnemyWidth &&
			playerArrows[i].y >= islandEnemy1Y && playerArrows[i].y <= islandEnemy1Y + islandEnemyHeight)
		{
			islandEnemy1Health--;
			playerArrows[i].active = false;

			if (islandEnemy1Health <= 0)
			{
				islandEnemy1Alive = false;
				level3KilledCount++;
				playKillSound();

				if (currentLevel == 3 && (level3KilledCount + (islandEnemy2Alive ? 1 : 0)) < level3TotalEnemies)
				{
					islandEnemy1X = islandPlayerX + 900;
					islandEnemy1Health = 3;
					islandEnemy1Alive = true;
				}
			}
			continue;
		}

		if (currentLevel == 3 && islandEnemy2Alive && playerArrows[i].x >= islandEnemy2X && playerArrows[i].x <= islandEnemy2X + islandEnemyWidth &&
			playerArrows[i].y >= islandEnemy2Y && playerArrows[i].y <= islandEnemy2Y + islandEnemyHeight)
		{
			islandEnemy2Health--;
			playerArrows[i].active = false;

			if (islandEnemy2Health <= 0)
			{
				islandEnemy2Alive = false;
				level3KilledCount++;
				playKillSound();

				if (currentLevel == 3 && (level3KilledCount + (islandEnemy1Alive ? 1 : 0)) < level3TotalEnemies)
				{
					islandEnemy2X = islandPlayerX + 900;
					islandEnemy2Health = 3;
					islandEnemy2Alive = true;
				}
			}
			continue;
		}
	}
}

void initializeIsland()
{
	islandPlayerX = 150;
	islandPlayerY = 180;
	islandCameraX = 0;

	generateShootSound();
	generateKillSound();
	initializeArrows();

	islandPlayerTexture[0] = loadIslandPNG("island_player1.png", &islandPlayerImageWidth[0], &islandPlayerImageHeight[0]);
	islandPlayerTexture[1] = loadIslandPNG("island_player2.png", &islandPlayerImageWidth[1], &islandPlayerImageHeight[1]);
	islandPlayerTexture[2] = loadIslandPNG("island_player3.png", &islandPlayerImageWidth[2], &islandPlayerImageHeight[2]);

	islandBackgroundTexture = loadIslandPNG("island_background.png", &islandBackgroundWidth, &islandBackgroundHeight);

	hillTexture = loadIslandPNG("hill.png", &hillImageWidth, &hillImageHeight);
	if (hillTexture == 0) hillTexture = loadIslandPNG("hill.png.png", &hillImageWidth, &hillImageHeight);

	snakeTexture = loadIslandPNG("snake.png", &snakeImageWidth, &snakeImageHeight);
	if (snakeTexture == 0) snakeTexture = loadIslandPNG("snake.png.png", &snakeImageWidth, &snakeImageHeight);

	islandEnemyTexture[0] = loadIslandPNG("indigenous1.png", &islandEnemyImageWidth[0], &islandEnemyImageHeight[0]);
	islandEnemyTexture[1] = loadIslandPNG("indigenous2.png", &islandEnemyImageWidth[1], &islandEnemyImageHeight[1]);
	islandEnemyTexture[2] = loadIslandPNG("indigenous3.png", &islandEnemyImageWidth[2], &islandEnemyImageHeight[2]);

	initializeTreasure();
}

void resetIsland()
{
	islandPlayerX = 150;
	islandPlayerY = 180;
	islandCameraX = 0;
	islandPlayerFrame = 0;
	islandPlayerAnimationCounter = 0;
	islandPlayerMoving = false;
	islandPlayerAttack = false;
	islandAttackCooldown = 0;

	level3KilledCount = 0;
	initializeArrows();
	resetTreasure();

	if (currentLevel == 1)
	{
		islandEnemy1Alive = false;
		islandEnemy2Alive = false;
		snakeBossAlive = false;
	}
	else if (currentLevel == 2)
	{
		islandEnemy1X = 2500;
		islandEnemy1Health = 3;
		islandEnemy1Alive = true;
		islandEnemy2Alive = false;
		snakeBossAlive = false;
	}
	else if (currentLevel == 3)
	{
		islandEnemy1X = 2200;
		islandEnemy1Health = 3;
		islandEnemy1Alive = true;

		islandEnemy2X = 2550;
		islandEnemy2Health = 3;
		islandEnemy2Alive = true;

		islandEnemySpeed = 4.0f;

		snakeBossX = 3450;
		snakeBossY = 220;
		snakeBossHealth = 6;
		snakeBossAlive = true;
		snakeHurtTimer = 0;
	}
}

void drawIslandBackground()
{
	if (islandBackgroundTexture == 0) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, islandBackgroundTexture);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-islandCameraX, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(ISLAND_WORLD_WIDTH - islandCameraX, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(ISLAND_WORLD_WIDTH - islandCameraX, ISLAND_HEIGHT);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-islandCameraX, ISLAND_HEIGHT);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void drawHill()
{
	if (currentLevel != 3) return;

	float screenHillX = hillStartX - islandCameraX;

	if (hillTexture != 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hillTexture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenHillX, 150);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenHillX + hillRenderWidth, 150);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenHillX + hillRenderWidth, 150 + hillRenderHeight);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenHillX, 150 + hillRenderHeight);
		glEnd();

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		iSetColor(24, 90, 36);
		double polyX[] = { screenHillX, screenHillX + 160, screenHillX + hillRenderWidth, screenHillX + hillRenderWidth, screenHillX };
		double polyY[] = { 180, 180 + 150, 180 + 150, 0, 0 };
		iFilledPolygon(polyX, polyY, 5);

		iSetColor(34, 139, 34);
		double ridgeX[] = { screenHillX + 60, screenHillX + 180, screenHillX + hillRenderWidth, screenHillX + hillRenderWidth };
		double ridgeY[] = { 180, 180 + 150, 180 + 150, 180 };
		iFilledPolygon(ridgeX, ridgeY, 4);

		iSetColor(50, 205, 50);
		double grassX[] = { screenHillX + 140, screenHillX + 160, screenHillX + hillRenderWidth, screenHillX + hillRenderWidth };
		double grassY[] = { 180 + 140, 180 + 165, 180 + 165, 180 + 140 };
		iFilledPolygon(grassX, grassY, 4);
	}
}

// ======================================================
// DRAW REALISTIC GOLDEN VIPER (ROUNDED HEAD & JAW)
// ======================================================

void drawGiantSnake()
{
	if (currentLevel != 3 || !snakeBossAlive) return;

	float screenSnakeX = snakeBossX - islandCameraX;

	if (snakeTexture != 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, snakeTexture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (snakeHurtTimer > 0) glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
		else glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenSnakeX, snakeBossY);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenSnakeX + snakeBossWidth, snakeBossY);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenSnakeX + snakeBossWidth, snakeBossY + snakeBossHeight);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenSnakeX, snakeBossY + snakeBossHeight);
		glEnd();

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		float snakeBaseY = snakeBossY + 50;

		// --------------------------------------------------
		// 1. Natural Golden Serpent Coils (Layers of shaded rings)
		// --------------------------------------------------
		int r = 218, g = 165, b = 32;   // Deep Goldenrod
		int r2 = 255, g2 = 200, b2 = 50; // Warm Amber Scale highlight

		if (snakeHurtTimer > 0) { r = 255; g = 70; b = 70; r2 = 255; g2 = 120; b2 = 120; }

		// Dark lower shadow on coils
		iSetColor(110, 75, 10);
		iFilledCircle(screenSnakeX + 90, snakeBaseY + 26, 52);
		iFilledCircle(screenSnakeX + 145, snakeBaseY + 32, 46);
		iFilledCircle(screenSnakeX + 45, snakeBaseY + 56, 40);

		// Main Golden Body Coils
		iSetColor(r, g, b);
		iFilledCircle(screenSnakeX + 90, snakeBaseY + 30, 48);
		iFilledCircle(screenSnakeX + 145, snakeBaseY + 35, 42);
		iFilledCircle(screenSnakeX + 45, snakeBaseY + 60, 36);

		// Pale Cream Belly Scales Underneath
		iSetColor(245, 230, 150);
		iFilledCircle(screenSnakeX + 85, snakeBaseY + 22, 26);
		iFilledCircle(screenSnakeX + 140, snakeBaseY + 28, 22);

		// Amber Scale Tops
		iSetColor(r2, g2, b2);
		iFilledCircle(screenSnakeX + 92, snakeBaseY + 36, 32);
		iFilledCircle(screenSnakeX + 146, snakeBaseY + 40, 26);

		// Natural Reptilian Saddle Markings (Dark Diamond blotches)
		iSetColor(70, 45, 15);
		iFilledCircle(screenSnakeX + 115, snakeBaseY + 38, 12);
		iFilledCircle(screenSnakeX + 68, snakeBaseY + 45, 10);

		// --------------------------------------------------
		// 2. Thick Curved Serpent Neck
		// --------------------------------------------------
		iSetColor(r, g, b);
		iFilledCircle(screenSnakeX + 45, snakeBaseY + 95, 26);
		iFilledCircle(screenSnakeX + 45, snakeBaseY + 125, 28);
		iFilledRectangle(screenSnakeX + 22, snakeBaseY + 80, 46, 55);

		// Light Neck Throat Stripe
		iSetColor(245, 230, 150);
		iFilledRectangle(screenSnakeX + 34, snakeBaseY + 80, 22, 50);

		// --------------------------------------------------
		// 3. ROUNDED VIPER HEAD & CHEEKS (গোলগাল মুখ ও চোয়াল)
		// --------------------------------------------------
		float headCenterX = screenSnakeX + 45;
		float headCenterY = snakeBaseY + 160;

		// Rounded Venom Gland Cheeks (Left & Right round cheeks)
		iSetColor(r, g, b);
		iFilledCircle(headCenterX - 22, headCenterY, 26); // Left rounded cheek
		iFilledCircle(headCenterX + 22, headCenterY, 26); // Right rounded cheek

		// Rounded Crown of the Head
		iFilledCircle(headCenterX, headCenterY + 16, 26);

		// Rounded Curved Snout (Front Nose)
		iSetColor(r2, g2, b2);
		iFilledCircle(headCenterX, headCenterY - 6, 24); // Smooth rounded nose

		// Curved Dark Nose Ridge
		iSetColor(90, 55, 15);
		iFilledCircle(headCenterX, headCenterY + 8, 14);

		// --------------------------------------------------
		// 4. ROUNDED OPEN JAW / MOUTH (গোল বাঁকানো চোয়াল)
		// --------------------------------------------------
		// Dark Pinkish Throat Opening
		iSetColor(120, 20, 40);
		iFilledCircle(headCenterX, headCenterY - 14, 16);

		// Lower Rounded Chin/Jaw
		iSetColor(245, 230, 150);
		iFilledCircle(headCenterX, headCenterY - 26, 12);

		// Sharp Curved White Fangs
		iSetColor(255, 255, 255);
		iFilledRectangle(headCenterX - 11, headCenterY - 18, 4, 14);
		iFilledRectangle(headCenterX + 7, headCenterY - 18, 4, 14);
		iFilledCircle(headCenterX - 9, headCenterY - 18, 3);
		iFilledCircle(headCenterX + 9, headCenterY - 18, 3);

		// --------------------------------------------------
		// 5. Menacing Glowing Red Slit Eyes
		// --------------------------------------------------
		// Rounded Eye Ridge / Scales
		iSetColor(r, g, b);
		iFilledCircle(headCenterX - 18, headCenterY + 10, 9);
		iFilledCircle(headCenterX + 18, headCenterY + 10, 9);

		// Blood Red Iris
		iSetColor(255, 0, 0);
		iFilledCircle(headCenterX - 18, headCenterY + 10, 6);
		iFilledCircle(headCenterX + 18, headCenterY + 10, 6);

		// Vertical Black Slit Pupil
		iSetColor(0, 0, 0);
		iFilledRectangle(headCenterX - 19, headCenterY + 6, 2, 8);
		iFilledRectangle(headCenterX + 17, headCenterY + 6, 2, 8);

		// --------------------------------------------------
		// 6. Flickering Red Forked Tongue
		// --------------------------------------------------
		snakeTongueTimer += 0.15f;
		if (sin(snakeTongueTimer) > 0)
		{
			iSetColor(220, 20, 40);
			iFilledRectangle(headCenterX - 2, headCenterY - 38, 4, 20);
			iLine(headCenterX, headCenterY - 38, headCenterX - 10, headCenterY - 48);
			iLine(headCenterX, headCenterY - 38, headCenterX + 10, headCenterY - 48);
		}
	}

	// Boss Health Bar
	iSetColor(30, 30, 30);
	iFilledRectangle(450, 930, 600, 30);
	iSetColor(255, 215, 0);
	iRectangle(448, 928, 604, 34);

	float healthPct = (float)snakeBossHealth / (float)snakeBossMaxHealth;
	iSetColor(255, 200, 0);
	iFilledRectangle(452, 932, 596 * healthPct, 26);

	iSetColor(0, 0, 0);
	iText(590, 938, "GIANT YELLOW VIPER BOSS", GLUT_BITMAP_HELVETICA_18);
}

void drawHillTreasure()
{
	float treasureScreenX = 3850 - islandCameraX;
	float treasureHillY = 250;

	if (!snakeBossAlive)
	{
		iSetColor(255, 215, 0);
		for (int a = 0; a < 360; a += 45)
		{
			float rad = a * 3.14159f / 180.0f;
			iLine(treasureScreenX + 60, treasureHillY + 60,
				treasureScreenX + 60 + cos(rad) * 90,
				treasureHillY + 60 + sin(rad) * 90);
		}

		iSetColor(139, 69, 19);
		iFilledRectangle(treasureScreenX, treasureHillY, 120, 70);

		iSetColor(255, 215, 0);
		iFilledRectangle(treasureScreenX + 10, treasureHillY, 12, 70);
		iFilledRectangle(treasureScreenX + 98, treasureHillY, 12, 70);
		iFilledCircle(treasureScreenX + 60, treasureHillY + 45, 12);

		iSetColor(160, 82, 45);
		double lidX[] = { treasureScreenX - 10, treasureScreenX + 130, treasureScreenX + 115, treasureScreenX + 5 };
		double lidY[] = { treasureHillY + 70, treasureHillY + 70, treasureHillY + 120, treasureHillY + 120 };
		iFilledPolygon(lidX, lidY, 4);

		iSetColor(255, 255, 100);
		iText(treasureScreenX - 40, treasureHillY + 135, "LEGENDARY TREASURE!", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else
	{
		drawTreasure(islandCameraX);
	}
}

void drawIslandPlayer()
{
	if (islandPlayerTexture[islandPlayerFrame] == 0) return;

	float screenX = islandPlayerX - islandCameraX;
	float playerActualY = islandPlayerY;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, islandPlayerTexture[islandPlayerFrame]);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(screenX, playerActualY);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(screenX + islandPlayerWidth, playerActualY);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(screenX + islandPlayerWidth, playerActualY + islandPlayerHeight);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(screenX, playerActualY + islandPlayerHeight);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (islandPlayerAttack)
	{
		float swordHandX = screenX + islandPlayerWidth - 25;
		float swordHandY = playerActualY + (islandPlayerHeight / 2.0f);

		iSetColor(240, 245, 255);
		iFilledRectangle(swordHandX + 10, swordHandY - 6, 85, 12);

		double tipX[] = { swordHandX + 95, swordHandX + 130, swordHandX + 95 };
		double tipY[] = { swordHandY + 14, swordHandY, swordHandY - 14 };
		iFilledPolygon(tipX, tipY, 3);

		iSetColor(255, 215, 0);
		iFilledRectangle(swordHandX, swordHandY - 15, 10, 30);

		iSetColor(120, 60, 20);
		iFilledRectangle(swordHandX - 18, swordHandY - 5, 18, 10);
		iSetColor(255, 215, 0);
		iFilledCircle(swordHandX - 18, swordHandY, 6);

		iSetColor(100, 220, 255);
		iCircle(swordHandX + 50, swordHandY, 45);
		iCircle(swordHandX + 50, swordHandY, 60);

		iSetColor(255, 255, 255);
		iFilledCircle(swordHandX + 125, swordHandY, 8);
	}
}

void drawSingleEnemy(float enemyX, float enemyY)
{
	if (islandEnemyTexture[islandEnemyFrame] == 0) return;

	float screenX = enemyX - islandCameraX;
	if (screenX + islandEnemyWidth < 0 || screenX > ISLAND_WIDTH) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, islandEnemyTexture[islandEnemyFrame]);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(screenX, enemyY);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(screenX + islandEnemyWidth, enemyY);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(screenX + islandEnemyWidth, enemyY + islandEnemyHeight);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(screenX, enemyY + islandEnemyHeight);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void drawIslandEnemies()
{
	if (islandEnemy1Alive) drawSingleEnemy(islandEnemy1X, islandEnemy1Y);
	if (currentLevel == 3 && islandEnemy2Alive) drawSingleEnemy(islandEnemy2X, islandEnemy2Y);
}

void islandMovePlayerRight()
{
	if (currentState != ISLAND_EXPLORATION) return;

	if (currentLevel == 3 && snakeBossAlive)
	{
		if (islandPlayerX + islandPlayerWidth >= snakeBossX - 10)
		{
			islandPlayerX = snakeBossX - islandPlayerWidth - 10;
			return;
		}
	}

	islandPlayerMoving = true;
	islandPlayerX += islandPlayerSpeed;

	if (islandPlayerX > ISLAND_WORLD_WIDTH - islandPlayerWidth)
	{
		islandPlayerX = ISLAND_WORLD_WIDTH - islandPlayerWidth;
	}

	if (islandPlayerX > 700) islandCameraX = islandPlayerX - 700;
	if (islandCameraX > ISLAND_WORLD_WIDTH - ISLAND_WIDTH) islandCameraX = ISLAND_WORLD_WIDTH - ISLAND_WIDTH;
}

void islandMovePlayerLeft()
{
	if (currentState != ISLAND_EXPLORATION) return;

	islandPlayerMoving = true;
	islandPlayerX -= islandPlayerSpeed;

	if (islandPlayerX < 0) islandPlayerX = 0;

	if (islandPlayerX > 700) islandCameraX = islandPlayerX - 700;
	else islandCameraX = 0;
}

void islandAttack()
{
	if (currentState != ISLAND_EXPLORATION || currentLevel < 2 || islandAttackCooldown > 0) return;

	islandPlayerAttack = true;
	islandAttackCooldown = 16;

	if (currentLevel == 3 && snakeBossAlive)
	{
		float distToSnake = snakeBossX - (islandPlayerX + islandPlayerWidth);
		if (distToSnake < 220 && distToSnake > -100)
		{
			snakeBossHealth--;
			snakeHurtTimer = 12;
			if (snakeBossHealth <= 0)
			{
				snakeBossHealth = 0;
				snakeBossAlive = false;
				playKillSound();
			}
			return;
		}
	}

	float dist1 = islandEnemy1X - (islandPlayerX + islandPlayerWidth);
	bool canHit1 = (islandEnemy1Alive && dist1 < 250 && dist1 > -100);

	float dist2 = islandEnemy2X - (islandPlayerX + islandPlayerWidth);
	bool canHit2 = (currentLevel == 3 && islandEnemy2Alive && dist2 < 250 && dist2 > -100);

	if (canHit1 && (!canHit2 || dist1 <= dist2))
	{
		islandEnemy1Health--;
		if (islandEnemy1Health <= 0)
		{
			islandEnemy1Alive = false;
			level3KilledCount++;
			playKillSound();

			if (currentLevel == 3 && (level3KilledCount + (islandEnemy2Alive ? 1 : 0)) < level3TotalEnemies)
			{
				islandEnemy1X = islandPlayerX + 900;
				islandEnemy1Health = 3;
				islandEnemy1Alive = true;
			}
		}
	}
	else if (canHit2)
	{
		islandEnemy2Health--;
		if (islandEnemy2Health <= 0)
		{
			islandEnemy2Alive = false;
			level3KilledCount++;
			playKillSound();

			if (currentLevel == 3 && (level3KilledCount + (islandEnemy1Alive ? 1 : 0)) < level3TotalEnemies)
			{
				islandEnemy2X = islandPlayerX + 900;
				islandEnemy2Health = 3;
				islandEnemy2Alive = true;
			}
		}
	}
}

void updateIslandPlayerAnimation()
{
	if (!islandPlayerMoving)
	{
		islandPlayerFrame = 0;
		return;
	}

	islandPlayerAnimationCounter++;
	if (islandPlayerAnimationCounter >= 6)
	{
		islandPlayerAnimationCounter = 0;
		islandPlayerFrame = (islandPlayerFrame + 1) % 3;
	}
}

void updateIslandEnemyAnimation()
{
	if (!islandEnemy1Alive && !islandEnemy2Alive) return;

	islandEnemyAnimationCounter++;
	if (islandEnemyAnimationCounter >= 8)
	{
		islandEnemyAnimationCounter = 0;
		islandEnemyFrame = (islandEnemyFrame + 1) % 3;
	}
}

void updateIslandEnemies()
{
	if (currentLevel < 2) return;

	if (islandEnemy1Alive && islandEnemy1X > islandPlayerX + islandPlayerWidth + 60)
	{
		islandEnemy1X -= islandEnemySpeed;
	}

	if (currentLevel == 3 && islandEnemy2Alive)
	{
		float targetStopX = islandEnemy1Alive ? (islandEnemy1X + 160) : (islandPlayerX + islandPlayerWidth + 60);
		if (islandEnemy2X > targetStopX)
		{
			islandEnemy2X -= islandEnemySpeed;
		}
	}
}

void updateIsland()
{
	if (currentState != ISLAND_EXPLORATION) return;

	updateIslandPlayerAnimation();
	updateIslandEnemyAnimation();
	updateIslandEnemies();
	updateArrows();

	if (snakeHurtTimer > 0) snakeHurtTimer--;

	if (islandAttackCooldown > 0)
	{
		islandAttackCooldown--;
		if (islandAttackCooldown <= 4)
		{
			islandPlayerAttack = false;
		}
	}

	if (currentLevel == 3)
	{
		if (!snakeBossAlive && islandPlayerX >= 3650)
		{
			collectTreasure(playerHealth);
		}
	}
	else
	{
		checkTreasureCollection(
			islandPlayerX,
			islandPlayerY,
			islandPlayerWidth,
			islandPlayerHeight,
			playerHealth
			);
	}

	islandPlayerMoving = false;
}

void drawIslandHealth()
{
	char text[100];

	sprintf_s(text, "Player Life: %d", playerHealth);
	iSetColor(0, 255, 100);
	iText(40, 940, text, GLUT_BITMAP_HELVETICA_18);

	if (currentLevel == 2 && islandEnemy1Alive)
	{
		sprintf_s(text, "Enemy Life: %d", islandEnemy1Health);
		iSetColor(255, 50, 50);
		iText(1300, 940, text, GLUT_BITMAP_HELVETICA_18);
	}
	else if (currentLevel == 3)
	{
		if (snakeBossAlive)
		{
			iSetColor(255, 204, 0);
			iText(1050, 890, "SHOOT ARROWS (F) TO SLAY THE GOLDEN VIPER!", GLUT_BITMAP_HELVETICA_18);
		}
		else
		{
			iSetColor(50, 255, 50);
			iText(1120, 890, "VIPER SLAIN! CLAIM THE TREASURE!", GLUT_BITMAP_HELVETICA_18);
		}
	}
}

void drawIsland()
{
	drawIslandBackground();

	if (currentLevel == 3)
	{
		drawHill();
		drawGiantSnake();
		drawHillTreasure();
	}
	else
	{
		drawTreasure(islandCameraX);
	}

	drawIslandEnemies();
	drawIslandPlayer();
	drawArrows();
	drawIslandHealth();

	iSetColor(255, 255, 255);
	iText(40, 40, "RIGHT = FORWARD | LEFT = BACK", GLUT_BITMAP_HELVETICA_18);

	if (currentLevel >= 2)
	{
		iText(40, 70, "F / UP = SHOOT ARROW | SPACE = SWORD", GLUT_BITMAP_HELVETICA_18);
	}
}

void islandSpecialKeyboard(int key)
{
	if (currentState != ISLAND_EXPLORATION) return;

	if (key == GLUT_KEY_RIGHT) islandMovePlayerRight();
	else if (key == GLUT_KEY_LEFT) islandMovePlayerLeft();
	else if (key == GLUT_KEY_UP) shootArrow();
}

void islandKeyboard(unsigned char key)
{
	if (currentState != ISLAND_EXPLORATION) return;

	if (key == ' ') islandAttack();
	else if (key == 'f' || key == 'F' || key == 'a' || key == 'A') shootArrow();
	else if (key == 'm' || key == 'M') currentState = LEVEL_SELECT;
}

void startIsland()
{
	resetIsland();
	currentState = ISLAND_EXPLORATION;
}

#endif // ISLAND_HPP