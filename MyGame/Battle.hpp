#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "iGraphics.h"
#include "GameState.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Cannon.hpp"
#include "Score.hpp"
#include "Level.hpp"

#include <stdio.h>

// Forward declaration for Island transition
void startIsland();

// ======================================================
// SCREEN SIZE
// ======================================================

#define BATTLE_WIDTH 1500
#define BATTLE_HEIGHT 1000

// ======================================================
// BATTLE BACKGROUND
// ======================================================

GLuint battleBackgroundTexture = 0;
int battleBackgroundWidth = 0;
int battleBackgroundHeight = 0;

// ======================================================
// LOAD BATTLE BACKGROUND
// ======================================================

void loadBattleBackground()
{
	int channels;

	unsigned char *imageData = stbi_load(
		"ocean background 1.jpg",
		&battleBackgroundWidth,
		&battleBackgroundHeight,
		&channels,
		4
		);

	if (imageData == NULL)
	{
		printf("ERROR: Cannot load ocean background!\n");
		return;
	}

	glGenTextures(1, &battleBackgroundTexture);
	glBindTexture(GL_TEXTURE_2D, battleBackgroundTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		battleBackgroundWidth,
		battleBackgroundHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		imageData
		);

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	printf("Battle background loaded successfully!\n");
}

// ======================================================
// DRAW BATTLE BACKGROUND
// ======================================================

void drawBattleBackground()
{
	if (battleBackgroundTexture == 0) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, battleBackgroundTexture);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(BATTLE_WIDTH, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(BATTLE_WIDTH, BATTLE_HEIGHT);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, BATTLE_HEIGHT);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

// ======================================================
// INITIALIZE BATTLE
// ======================================================

void initializeBattle()
{
	loadBattleBackground();
	resetPlayer();
	resetEnemy();
	resetCannon();
}

// ======================================================
// RESET BATTLE
// ======================================================

void resetBattle()
{
	resetPlayer();
	resetEnemy();
	resetCannon();

	gameOver = false;
	levelCompleted = false;
}

// ======================================================
// DRAW PLAYER HEALTH
// ======================================================

void drawBattlePlayerHealth()
{
	char healthText[50];
	sprintf_s(healthText, "Player Health: %d", playerHealth);
	iSetColor(255, 255, 255);
	iText(33, 950, healthText, GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// DRAW ENEMY HEALTH
// ======================================================

void drawBattleEnemyHealth()
{
	char healthText[50];
	sprintf_s(healthText, "Enemy Health: %d", enemyHealth);
	iSetColor(255, 255, 255);
	iText(1233, 950, healthText, GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// DRAW BATTLE CONTROLS
// ======================================================

void drawBattleControls()
{
	iSetColor(255, 255, 255);
	iText(33, 33, "ARROW KEYS = MOVE", GLUT_BITMAP_HELVETICA_18);
	iText(33, 58, "SPACE = FIRE CANNON", GLUT_BITMAP_HELVETICA_18);
	iText(33, 83, "M = LEVEL MENU", GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// DRAW BATTLE SCREEN
// ======================================================

void drawBattle()
{
	drawBattleBackground();
	drawPlayer();
	drawEnemy();
	drawCannon();
	drawBattlePlayerHealth();
	drawBattleEnemyHealth();
	drawBattleControls();
}

// ======================================================
// UPDATE BATTLE
// ======================================================

void updateBattle()
{
	if (currentState != OCEAN_BATTLE) return;

	updateEnemy();
	updateCannon();
}

// ======================================================
// CHECK BATTLE STATUS
// ======================================================

void checkBattleStatus()
{
	// 1. Player ship destroyed -> Game Over
	if (!playerAlive)
	{
		gameOver = true;
		currentState = GAME_OVER;
		return;
	}

	// 2. Enemy ship destroyed!
	if (!enemyAlive)
	{
		// Level 1: Only Ocean Battle! Completes right here!
		if (currentLevel == 1)
		{
			completeLevel1();
			completeLevelWithScore(1, playerHealth);
		}
		// Level 2 & Level 3: Proceed to Jungle Island Exploration!
		else
		{
			startIsland();
		}
		return;
	}
}

// ======================================================
// KEYBOARD CONTROLS
// ======================================================

void battleKeyboard(unsigned char key)
{
	if (key == ' ')
	{
		cannonKeyboard(key);
	}
	else if (key == 'm' || key == 'M')
	{
		currentState = LEVEL_SELECT;
	}
}

void battleSpecialKeyboard(int key)
{
	playerSpecialKeyboard(key);
}

void battleMouse(int button, int state, int mx, int my)
{
}

// ======================================================
// START BATTLE
// ======================================================

void startBattle()
{
	resetBattle();
	currentState = OCEAN_BATTLE;
}

// ======================================================
// BATTLE UPDATE ALL
// ======================================================

void updateBattleAll()
{
	if (currentState != OCEAN_BATTLE) return;

	updateBattle();
	checkBattleStatus();
}

#endif // BATTLE_HPP