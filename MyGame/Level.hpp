#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "iGraphics.h"
#include "GameState.hpp"

#include <stdio.h>
#include <stdlib.h>

// ======================================================
// SCREEN SIZE
// ======================================================

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000

// ======================================================
// FORWARD DECLARATIONS (To reset game modules)
// ======================================================

void resetIsland();
void resetBattle();
void initializeBattle();

// ======================================================
// LEVEL SCREEN TEXTURE
// ======================================================

GLuint levelTexture = 0;
int levelImageWidth = 0;
int levelImageHeight = 0;

// ======================================================
// POPUP STATE
// ======================================================

int levelPopup = 0;

void drawLockedPopup()
{
	iSetColor(255, 215, 0);
	iFilledRectangle(445, 415, 610, 190);

	iSetColor(20, 20, 20);
	iFilledRectangle(450, 420, 600, 180);

	iSetColor(255, 255, 255);
	iText(580, 530, "LOCKED!", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(480, 480, "Complete Level 2 first", GLUT_BITMAP_HELVETICA_18);
	iText(510, 440, "Click BACK to return", GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// COMPLETED LEVEL
// ======================================================

int completedLevel = 0;

// ======================================================
// LOAD PNG TEXTURE
// ======================================================

GLuint loadLevelPNG(const char *filename, int *width, int *height)
{
	int channels;
	unsigned char *imageData = stbi_load(filename, width, height, &channels, 4);

	if (imageData == NULL)
	{
		printf("ERROR: Cannot load image: %s\n", filename);
		return 0;
	}

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

// ======================================================
// INITIALIZE LEVEL SCREEN
// ======================================================

void initializeLevel()
{
	levelPopup = 0;
	completedLevel = 0;
	currentLevel = 1;

	levelTexture = loadLevelPNG("LevelScreen.png", &levelImageWidth, &levelImageHeight);
}

// ======================================================
// DRAW TEXTURE FULL SCREEN
// ======================================================

void drawLevelTexture(GLuint texture)
{
	if (texture == 0) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, SCREEN_HEIGHT);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

// ======================================================
// DRAW COMING SOON POPUP
// ======================================================

void drawComingSoonPopup()
{
	iSetColor(255, 215, 0);
	iFilledRectangle(445, 415, 610, 190);

	iSetColor(20, 20, 20);
	iFilledRectangle(450, 420, 600, 180);

	iSetColor(255, 255, 255);
	iText(580, 530, "LEVEL 3", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(540, 480, "COMING SOON!", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(510, 440, "Click BACK to return", GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// DRAW LEVEL SCREEN
// ======================================================

void drawLevelScreen()
{
	drawLevelTexture(levelTexture);

	if (levelPopup == 1)
	{
		drawLockedPopup();
	}
	else if (levelPopup == 2)
	{
		drawComingSoonPopup();
	}
}

// ======================================================
// COMPLETE LEVEL FUNCTIONS
// ======================================================

void completeLevel1()
{
	if (completedLevel < 1) completedLevel = 1;
	levelCompleted = true;
}

void completeLevel2()
{
	if (completedLevel < 2) completedLevel = 2;
	levelCompleted = true;
}

void completeLevel3()
{
	if (completedLevel < 3) completedLevel = 3;
	levelCompleted = true;
}

void closeLevelPopup()
{
	levelPopup = 0;
}

// ======================================================
// LEVEL MOUSE CLICK
// ======================================================

void levelMouseClick(int mx, int my)
{
	// Close popup if click back
	if (levelPopup != 0)
	{
		if (mx >= 8 && mx <= 200 && my >= 850 && my <= 985)
		{
			levelPopup = 0;
		}
		return;
	}

	// Back to Main Menu
	if (mx >= 8 && mx <= 200 && my >= 850 && my <= 985)
	{
		currentState = MAIN_MENU;
		return;
	}

	// LEVEL 1 BUTTON
	if (mx >= 203 && mx <= 451 && my >= 390 && my <= 695)
	{
		currentLevel = 1;
		previousLevel = 1;
		gameOver = false;
		levelCompleted = false;

		currentState = LEVEL_INTRO;
		return;
	}

	// LEVEL 2 BUTTON
	if (mx >= 629 && mx <= 879 && my >= 390 && my <= 695)
	{
		currentLevel = 2;
		previousLevel = 2;
		gameOver = false;
		levelCompleted = false;

		currentState = LEVEL_INTRO;
		return;
	}

	// LEVEL 3 BUTTON
	if (mx >= 846 && mx <= 1171 && my >= 450 && my <= 742)
	{
		if (completedLevel < 2)
		{
			levelPopup = 1; // Locked
			return;
		}

		currentLevel = 3;
		previousLevel = 3;
		gameOver = false;
		levelCompleted = false;

		currentState = LEVEL_INTRO;
		return;
	}
}

// ======================================================
// START SELECTED LEVEL (RESETS EVERYTHING FRESH)
// ======================================================

void startSelectedLevel()
{
	gameOver = false;
	levelCompleted = false;

	// Reset Island and battle completely for the new level
	resetIsland();

	// Start from the beginning
	currentState = OCEAN_BATTLE;
}

// ======================================================
// RETRY CURRENT LEVEL
// ======================================================

void retryCurrentLevel()
{
	currentLevel = previousLevel;
	gameOver = false;
	levelCompleted = false;

	resetIsland();

	currentState = LEVEL_INTRO;
}

void returnAfterLevelComplete()
{
	levelPopup = 0;
	currentState = LEVEL_SELECT;
}

void drawLevelSelection()
{
	drawLevelScreen();
}

void drawLevelIntro()
{
	iSetColor(10, 30, 55);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	iSetColor(255, 215, 0);
	char levelText[50];
	sprintf_s(levelText, "LEVEL %d", currentLevel);
	iText(667, 542, levelText, GLUT_BITMAP_TIMES_ROMAN_24);

	iSetColor(255, 255, 255);
	iText(542, 458, "Press ENTER to Start", GLUT_BITMAP_HELVETICA_18);
}

void levelKeyboard(unsigned char key)
{
	if (key == 13) // ENTER key
	{
		if (currentState == LEVEL_INTRO)
		{
			startSelectedLevel();
		}
		else if (currentState == GAME_OVER)
		{
			retryCurrentLevel();
		}
		else if (currentState == LEVEL_COMPLETE)
		{
			returnAfterLevelComplete();
		}
	}
	else if (key == 27) // ESC key
	{
		currentState = MAIN_MENU;
	}
}

void levelMouse(int button, int state, int mx, int my)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
	{
		return;
	}

	levelMouseClick(mx, my);
}

#endif 