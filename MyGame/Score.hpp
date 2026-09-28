#ifndef SCORE_HPP
#define SCORE_HPP

#include "iGraphics.h"
#include "GameState.hpp"
#include <stdio.h>
#include <math.h>

// ======================================================
// SCORES
// ======================================================

int totalScore = 0;
int levelScore = 0;
int levelTreasurePoints = 0;
int healthBonus = 0;
bool scoreAddedForLevel = false;

// ======================================================
// GET TREASURE POINTS
// ======================================================

int getTreasurePoints(int level)
{
	if (level == 1) return 1000;
	else if (level == 2) return 1500;
	else if (level == 3) return 3000; // Extra jackpot points for Level 3
	return 0;
}

// ======================================================
// GET HEALTH BONUS
// ======================================================

int getHealthBonus(int playerLife)
{
	if (playerLife >= 3) return 500;
	else if (playerLife == 2) return 300;
	else if (playerLife == 1) return 100;
	return 0;
}

// ======================================================
// CALCULATE LEVEL SCORE
// ======================================================

void calculateLevelScore(int level, int playerLife)
{
	levelTreasurePoints = getTreasurePoints(level);
	healthBonus = getHealthBonus(playerLife);
	levelScore = levelTreasurePoints + healthBonus;
}

// ======================================================
// ADD SCORE
// ======================================================

void addLevelScore(int level, int playerLife)
{
	if (scoreAddedForLevel) return;

	calculateLevelScore(level, playerLife);
	totalScore += levelScore;
	scoreAddedForLevel = true;
}

// ======================================================
// RESET SCORES
// ======================================================

void resetLevelScore()
{
	levelScore = 0;
	levelTreasurePoints = 0;
	healthBonus = 0;
	scoreAddedForLevel = false;
}

void resetAllScore()
{
	totalScore = 0;
	levelScore = 0;
	levelTreasurePoints = 0;
	healthBonus = 0;
	scoreAddedForLevel = false;
}

// ======================================================
// DRAW IN-GAME SCORE HUD
// ======================================================

void drawScore()
{
	char scoreText[100];
	sprintf_s(scoreText, "Score: %d", totalScore);
	iSetColor(255, 255, 255);
	iText(33, 917, scoreText, GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// DRAW STANDARD LEVEL COMPLETE (Levels 1 & 2)
// ======================================================

void drawStandardLevelComplete()
{
	iSetColor(10, 30, 55);
	iFilledRectangle(0, 0, 1500, 1000);

	iSetColor(255, 215, 0);
	iText(583, 792, "LEVEL COMPLETE!", GLUT_BITMAP_TIMES_ROMAN_24);

	char text[100];
	iSetColor(255, 255, 255);

	sprintf_s(text, "Treasure Points: +%d", levelTreasurePoints);
	iText(583, 583, text, GLUT_BITMAP_HELVETICA_18);

	sprintf_s(text, "Health Bonus: +%d", healthBonus);
	iText(583, 542, text, GLUT_BITMAP_HELVETICA_18);

	sprintf_s(text, "Level Score: +%d", levelScore);
	iText(583, 500, text, GLUT_BITMAP_HELVETICA_18);

	sprintf_s(text, "Total Score: %d", totalScore);
	iText(583, 458, text, GLUT_BITMAP_HELVETICA_18);

	iSetColor(255, 255, 255);
	iText(542, 375, "Press ENTER to Continue", GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// DRAW EPIC FULL-SCREEN "BRAVO! YOU WON!" (Level 3 Victory)
// ======================================================

void drawGrandVictoryScreen()
{
	// 1. Royal Navy Deep Space Celebration Background
	iSetColor(8, 12, 30);
	iFilledRectangle(0, 0, 1500, 1000);

	// 2. Celebration Fireworks & Stars
	for (int i = 0; i < 28; i++)
	{
		float starX = (float)((i * 137 + 75) % 1450 + 25);
		float starY = (float)((i * 229 + 150) % 850 + 80);
		int r = (i % 2 == 0) ? 255 : 100;
		int g = (i % 3 == 0) ? 215 : 220;
		int b = (i % 2 == 0) ? 0 : 255;
		iSetColor(r, g, b);
		iFilledCircle(starX, starY, (i % 3) + 3);
	}

	// 3. Golden Laurel / Trophy Banner Box
	iSetColor(255, 215, 0);
	iRectangle(350, 200, 800, 680);
	iRectangle(354, 204, 792, 672);

	// Trophy Cup
	iSetColor(255, 215, 0); // Gold cup
	double cupX[] = { 670, 830, 800, 700 };
	double cupY[] = { 720, 720, 630, 630 };
	iFilledPolygon(cupX, cupY, 4);

	// Trophy Stand
	iFilledRectangle(735, 590, 30, 40);
	iFilledRectangle(700, 570, 100, 20);

	// Trophy Handles
	iCircle(660, 680, 20);
	iCircle(840, 680, 20);

	// 4. "BRAVO!" Title
	iSetColor(255, 50, 80);
	iText(660, 810, "B R A V O !", GLUT_BITMAP_TIMES_ROMAN_24);

	// 5. "YOU WON!" Subtitle
	iSetColor(255, 215, 0);
	iText(590, 760, "YOU WON THE GAME!", GLUT_BITMAP_TIMES_ROMAN_24);

	iSetColor(50, 255, 120);
	iText(600, 520, "YOU ARE THE PIRATE KING!", GLUT_BITMAP_HELVETICA_18);

	// 6. Final Score Display
	char scoreBuf[100];
	sprintf_s(scoreBuf, "FINAL SCORE: %d", totalScore);
	iSetColor(255, 255, 255);
	iText(630, 450, scoreBuf, GLUT_BITMAP_TIMES_ROMAN_24);

	iSetColor(200, 200, 200);
	iText(580, 390, "Serpent Slain: +3000 Treasure Points", GLUT_BITMAP_HELVETICA_18);
	iText(580, 350, "All Islands Explored & Treasures Claimed!", GLUT_BITMAP_HELVETICA_18);

	// 7. Navigation Prompts
	iSetColor(255, 215, 0);
	iText(520, 260, "Press 'M' for Main Menu  |  Press 'R' to Play Again", GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// MASTER DRAW LEVEL COMPLETE SWITCH
// ======================================================

void drawLevelComplete()
{
	// If final level (Level 3) is finished -> Show Grand Victory Screen!
	if (currentLevel >= 3)
	{
		drawGrandVictoryScreen();
	}
	else
	{
		// Level 1 and 2 normal completion
		drawStandardLevelComplete();
	}
}

// ======================================================
// DRAW GAME OVER SCREEN
// ======================================================

void drawGameOver()
{
	iSetColor(20, 0, 0);
	iFilledRectangle(0, 0, 1500, 1000);

	iSetColor(255, 0, 0);
	iText(650, 600, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

	char finalScoreText[100];
	sprintf_s(finalScoreText, "Final Score: %d", totalScore);
	iSetColor(255, 255, 255);
	iText(640, 520, finalScoreText, GLUT_BITMAP_HELVETICA_18);

	iSetColor(200, 200, 200);
	iText(560, 420, "Press 'R' to Restart or 'M' for Menu", GLUT_BITMAP_HELVETICA_18);
}

// ======================================================
// COMPLETE LEVEL HELPER
// ======================================================

void completeLevelWithScore(int level, int playerLife)
{
	addLevelScore(level, playerLife);
	levelCompleted = true;
	currentState = LEVEL_COMPLETE;
}

#endif // SCORE_HPP