#ifndef MENU_HPP
#define MENU_HPP

#include "iGraphics.h"
#include "GameState.hpp"

#include <stdio.h>
#include <stdlib.h>


// ==========================================
// SCREEN SIZE
// ==========================================

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000


// ==========================================
// MENU IMAGE
// ==========================================

GLuint menuTexture = 0;

int menuWidth = 0;
int menuHeight = 0;


// ==========================================
// LOAD MENU PNG
// ==========================================

void loadMenuImage()
{
	int channels;

	unsigned char *imageData = stbi_load(
		"Menu.png",
		&menuWidth,
		&menuHeight,
		&channels,
		4
		);

	if (imageData == NULL)
	{
		printf("Menu.png could not be loaded!\n");
		return;
	}


	glGenTextures(1, &menuTexture);

	glBindTexture(
		GL_TEXTURE_2D,
		menuTexture
		);


	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR
		);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR
		);


	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		menuWidth,
		menuHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		imageData
		);


	stbi_image_free(imageData);

	glBindTexture(GL_TEXTURE_2D, 0);
}


// ==========================================
// INITIALIZE MENU
// ==========================================

void initializeMenu()
{
	loadMenuImage();
}


// ==========================================
// DRAW MENU IMAGE
// ==========================================

void drawMainMenu()
{
	if (menuTexture == 0)
		return;


	glEnable(GL_TEXTURE_2D);

	glBindTexture(
		GL_TEXTURE_2D,
		menuTexture
		);


	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(
		0,
		0
		);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(
		SCREEN_WIDTH,
		0
		);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(
		SCREEN_WIDTH,
		SCREEN_HEIGHT
		);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(
		0,
		SCREEN_HEIGHT
		);

	glEnd();


	glBindTexture(
		GL_TEXTURE_2D,
		0
		);

	glDisable(GL_TEXTURE_2D);
}


// ==========================================
// MENU BUTTON HITBOXES
// ==========================================
//
// Measured directly from Menu.png (1800 x 1200)
// by locating the wooden plank edges of each
// button.
//
// iGraphics mouse coordinates share the same
// origin as drawing (bottom-left = 0,0), so
// "my" here is already bottom-up, matching the
// glVertex2f() calls used elsewhere.
//
// Conversion used while measuring:
//     gameY = SCREEN_HEIGHT - imageY(top-down)
//
// A small padding is added on every side so
// clicks near a button's edge still register.
// ==========================================

#define MENU_BTN_X_MIN 613
#define MENU_BTN_X_MAX 888

#define START_BTN_Y_MIN      421
#define START_BTN_Y_MAX      508

#define INSTRUCTIONS_Y_MIN   328
#define INSTRUCTIONS_Y_MAX   417

#define UPGRADE_Y_MIN        233
#define UPGRADE_Y_MAX        327

#define EXIT_Y_MIN           138
#define EXIT_Y_MAX           232


// ==========================================
// MENU MOUSE CLICK
// ==========================================

void menuMouseClick(int mx, int my)
{
	// Helpful for calibration - safe to remove later
	printf("Menu click at (%d, %d)\n", mx, my);


	// ======================================
	// START GAME
	// ======================================

	if (mx >= MENU_BTN_X_MIN && mx <= MENU_BTN_X_MAX &&
		my >= START_BTN_Y_MIN && my <= START_BTN_Y_MAX)
	{
		currentState = LEVEL_SELECT;

		return;
	}


	// ======================================
	// INSTRUCTIONS
	// ======================================

	if (mx >= MENU_BTN_X_MIN && mx <= MENU_BTN_X_MAX &&
		my >= INSTRUCTIONS_Y_MIN && my <= INSTRUCTIONS_Y_MAX)
	{
		currentState = INSTRUCTIONS;

		return;
	}


	// ======================================
	// UPGRADE SHIP
	// ======================================

	if (mx >= MENU_BTN_X_MIN && mx <= MENU_BTN_X_MAX &&
		my >= UPGRADE_Y_MIN && my <= UPGRADE_Y_MAX)
	{
		currentState = UPGRADE_MENU;

		return;
	}


	// ======================================
	// EXIT
	// ======================================

	if (mx >= MENU_BTN_X_MIN && mx <= MENU_BTN_X_MAX &&
		my >= EXIT_Y_MIN && my <= EXIT_Y_MAX)
	{
		exit(0);

		return;
	}
}

// ==========================================
// MENU KEYBOARD
// ==========================================

void menuKeyboard(unsigned char key)
{
	if (key == 27) // ESC
	{
		exit(0);
	}
}


// ==========================================
// MENU MOUSE (wrapper to match Main.cpp signature)
// ==========================================

void menuMouse(int button, int state, int mx, int my)
{
	menuMouseClick(mx, my);
}

#endif