#define _CRT_SECURE_NO_WARNINGS

#include "iGraphics.h"
#include "GameState.hpp"
#include "Loading.hpp"
#include "Menu.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Cannon.hpp"
#include "Collision.hpp"
#include "Instruction.hpp"
#include "Battle.hpp"
#include "Island.hpp"
#include "Treasure.hpp"
#include "Score.hpp"

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#pragma comment(lib, "winmm.lib")

// ======================================================
// SCREEN SIZE
// ======================================================

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000

bool prevKeyState[256] = { false };

// ======================================================
// PROCEDURAL PIRATE MUSIC GENERATOR (Built-in Audio)
// ======================================================

void generateAndPlayPirateTheme()
{
	// 1. Check if audio file already exists
	FILE *check = NULL;
	if (fopen_s(&check, "pirate_bgm.wav", "r") == 0 && check != NULL)
	{
		fclose(check);
		PlaySound(TEXT("pirate_bgm.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		return;
	}

	// 2. Generate Nautical Theme in memory
	int sampleRate = 22050;
	float duration = 12.0f; // 12-second seamless looping theme
	int numSamples = (int)(sampleRate * duration);

	// Pirate note frequencies (D Minor Nautical Scale)
	float notes[] = { 293.66f, 349.23f, 392.00f, 440.00f, 466.16f, 523.25f, 587.33f };

	// Melody sequence
	int melody[] = {
		0, 2, 3, 3, 3, 4, 3, 2,
		0, 2, 3, 3, 5, 4, 3, 2,
		0, 2, 3, 3, 3, 4, 3, 2,
		0, 1, 0, 2, 0, 0, 0, 0
	};
	int totalNotes = sizeof(melody) / sizeof(melody[0]);
	float noteDuration = duration / (float)totalNotes;

	FILE *f = NULL;
	if (fopen_s(&f, "pirate_bgm.wav", "wb") != 0 || f == NULL) return;

	// Write WAV RIFF Header
	int dataSize = numSamples * 2;
	int fileSize = 36 + dataSize;
	short channels = 1;
	short bitsPerSample = 16;
	int byteRate = sampleRate * channels * (bitsPerSample / 8);
	short blockAlign = channels * (bitsPerSample / 8);

	fwrite("RIFF", 1, 4, f);
	fwrite(&fileSize, 4, 1, f);
	fwrite("WAVEfmt ", 1, 8, f);

	int subchunk1Size = 16;
	short audioFormat = 1; // PCM
	fwrite(&subchunk1Size, 4, 1, f);
	fwrite(&audioFormat, 2, 1, f);
	fwrite(&channels, 2, 1, f);
	fwrite(&sampleRate, 4, 1, f);
	fwrite(&byteRate, 4, 1, f);
	fwrite(&blockAlign, 2, 1, f);
	fwrite(&bitsPerSample, 2, 1, f);

	fwrite("data", 1, 4, f);
	fwrite(&dataSize, 4, 1, f);

	// Synthesize sound waves
	for (int i = 0; i < numSamples; i++)
	{
		float time = (float)i / (float)sampleRate;
		int currentNoteIdx = (int)(time / noteDuration) % totalNotes;
		float freq = notes[melody[currentNoteIdx]];

		float notePhase = 2.0f * 3.14159f * freq * time;
		float bassPhase = 2.0f * 3.14159f * (freq / 2.0f) * time;

		float sample = 0.6f * sin(notePhase) + 0.3f * sin(bassPhase);

		float noteProgress = fmod(time, noteDuration) / noteDuration;
		float decay = exp(-3.0f * noteProgress);
		sample *= decay;

		short sample16 = (short)(sample * 16000.0f);
		fwrite(&sample16, 2, 1, f);
	}

	fclose(f);

	// 3. Play generated theme on infinite loop
	PlaySound(TEXT("pirate_bgm.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

// ======================================================
// GAME INITIALIZATION
// ======================================================

void initializeGame()
{
	currentState = LOADING;
	currentLevel = 1;
	previousLevel = 1;
	gameOver = false;
	levelCompleted = false;

	initializeLoading();
	initializeMenu();
	initializeLevel();
	initializeEnemy();
	initializePlayer();
	initializeBattle();
	initializeIsland();
	initializeTreasure();
}

// ======================================================
// DRAW
// ======================================================

void iDraw()
{
	iClear();

	if (currentState == LOADING)
	{
		drawLoading();
	}
	else if (currentState == MAIN_MENU)
	{
		drawMainMenu();
	}
	else if (currentState == INSTRUCTIONS)
	{
		drawInstructions();
	}
	else if (currentState == LEVEL_SELECT)
	{
		drawLevelSelection();
	}
	else if (currentState == LEVEL_INTRO)
	{
		drawLevelIntro();
	}
	else if (currentState == OCEAN_BATTLE)
	{
		drawBattle();
	}
	else if (currentState == ISLAND_EXPLORATION)
	{
		drawIsland();
	}
	else if (currentState == LEVEL_COMPLETE)
	{
		drawLevelComplete();
	}
	else if (currentState == GAME_OVER)
	{
		drawGameOver();
	}
}

// ======================================================
// UPDATE
// ======================================================

void iUpdate()
{
	if (currentState == LOADING)
	{
		updateLoading();
	}
	else if (currentState == OCEAN_BATTLE)
	{
		updateBattleAll();
	}
	else if (currentState == ISLAND_EXPLORATION)
	{
		updateIsland();
	}
}

// ======================================================
// KEYBOARD CONTROLS
// ======================================================

void iKeyboard(unsigned char key)
{
	if (currentState == MAIN_MENU)
	{
		menuKeyboard(key);
	}
	else if (currentState == INSTRUCTIONS)
	{
		instructionKeyboard(key);
	}
	else if (currentState == LEVEL_SELECT)
	{
		levelKeyboard(key);
	}
	else if (currentState == LEVEL_INTRO)
	{
		levelKeyboard(key);
	}
	else if (currentState == OCEAN_BATTLE)
	{
		battleKeyboard(key);
	}
	else if (currentState == ISLAND_EXPLORATION)
	{
		islandKeyboard(key);
	}
	else if (currentState == LEVEL_COMPLETE)
	{
		// On the final Victory Screen (Level 3):
		if (currentLevel >= 3)
		{
			if (key == 'm' || key == 'M')
			{
				resetAllScore();
				currentState = MAIN_MENU;
			}
			else if (key == 'r' || key == 'R')
			{
				initializeGame();
				resetAllScore();
				currentState = MAIN_MENU;
			}
		}
		// For Level 1 and 2: press ENTER to return to Level Select
		else if (key == '\r' || key == 13)
		{
			resetLevelScore();
			currentState = LEVEL_SELECT;
		}
	}
	else if (currentState == GAME_OVER)
	{
		if (key == 'r' || key == 'R')
		{
			initializeGame();
			currentState = MAIN_MENU;
		}
		else if (key == 'm' || key == 'M')
		{
			currentState = MAIN_MENU;
		}
	}
}

void iSpecialKeyboard(int key)
{
	if (currentState == OCEAN_BATTLE)
	{
		battleSpecialKeyboard(key);
	}
	else if (currentState == ISLAND_EXPLORATION)
	{
		islandSpecialKeyboard(key);
	}
}

void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {}
void iMouseDrag(int mx, int my) {}

void pollOneShotKeys()
{
	unsigned char oneShotKeys[] = { 27, 'm', 'M', ' ', 13 };
	int numKeys = sizeof(oneShotKeys) / sizeof(oneShotKeys[0]);

	for (int i = 0; i < numKeys; i++)
	{
		unsigned char key = oneShotKeys[i];
		bool nowPressed = isKeyPressed(key) != 0;
		bool wasPressed = prevKeyState[key];
		if (nowPressed && !wasPressed)
		{
			iKeyboard(key);
		}
		prevKeyState[key] = nowPressed;
	}
}

void pollSpecialKeys()
{
	int specialKeys[] = { GLUT_KEY_LEFT, GLUT_KEY_RIGHT, GLUT_KEY_UP, GLUT_KEY_DOWN };
	int numKeys = sizeof(specialKeys) / sizeof(specialKeys[0]);

	for (int i = 0; i < numKeys; i++)
	{
		if (isSpecialKeyPressed((unsigned char)specialKeys[i]))
		{
			iSpecialKeyboard(specialKeys[i]);
		}
	}
}

void fixedUpdate()
{
	pollOneShotKeys();
	pollSpecialKeys();
	iUpdate();
}

void iMouse(int button, int state, int mx, int my)
{
	if (currentState == MAIN_MENU)
	{
		menuMouse(button, state, mx, my);
	}
	else if (currentState == LEVEL_SELECT)
	{
		levelMouse(button, state, mx, my);
	}
	else if (currentState == OCEAN_BATTLE)
	{
		battleMouse(button, state, mx, my);
	}
}

// ======================================================
// MAIN FUNCTION
// ======================================================

int main()
{
	iInitialize(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		"Pirate Adventure"
		);

	initializeGame();

	// Built-in sea shanty background music
	generateAndPlayPirateTheme();

	// 60 FPS timer
	iSetTimer(16, fixedUpdate);

	iStart();

	return 0;
}