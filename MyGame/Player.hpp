#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "iGraphics.h"
#include "GameState.hpp"

#include <stdio.h>


// ======================================================
// SCREEN SIZE
// ======================================================

#define PLAYER_SCREEN_WIDTH 1500
#define PLAYER_SCREEN_HEIGHT 1000


// ======================================================
// PLAYER SHIP
// ======================================================

// Ship position

float playerX = 625;
float playerY = 125;


// Ship size

int playerWidth = 200;
int playerHeight = 250;


// Ship movement speed

float playerSpeed = 13;


// Player health

int playerHealth = 3;


// Player alive status

bool playerAlive = true;


// ======================================================
// PLAYER IMAGE
// ======================================================

GLuint playerTexture = 0;

int playerImageWidth = 0;
int playerImageHeight = 0;


// ======================================================
// LOAD PLAYER PNG
// ======================================================

GLuint loadPlayerPNG(
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


	// ------------------------------------------
	// Check image
	// ------------------------------------------

	if (imageData == NULL)
	{
		printf(
			"ERROR: Cannot load player image: %s\n",
			filename
			);

		return 0;
	}


	// ------------------------------------------
	// Create texture
	// ------------------------------------------

	GLuint texture;


	glGenTextures(
		1,
		&texture
		);


	glBindTexture(
		GL_TEXTURE_2D,
		texture
		);


	// ------------------------------------------
	// Texture filter
	// ------------------------------------------

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


	// ------------------------------------------
	// Upload image
	// ------------------------------------------

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


	// ------------------------------------------
	// Free image data
	// ------------------------------------------

	stbi_image_free(
		imageData
		);


	glBindTexture(
		GL_TEXTURE_2D,
		0
		);


	printf(
		"Player image loaded: %s\n",
		filename
		);


	return texture;
}


// ======================================================
// INITIALIZE PLAYER
// ======================================================

void initializePlayer()
{
	// ------------------------------------------
	// Starting position
	// ------------------------------------------

	playerX = 625;
	playerY = 125;


	// ------------------------------------------
	// Ship size
	// ------------------------------------------

	playerWidth = 150;
	playerHeight = 150;


	// ------------------------------------------
	// Movement speed
	// ------------------------------------------

	playerSpeed = 13;


	// ------------------------------------------
	// Health
	// ------------------------------------------

	playerHealth = 3;

	playerAlive = true;


	// ------------------------------------------
	// Load pirate ship
	// ------------------------------------------

	playerTexture = loadPlayerPNG(
		"pirateShip.png",
		&playerImageWidth,
		&playerImageHeight
		);
}


// ======================================================
// DRAW PLAYER SHIP
// ======================================================

void drawPlayer()
{
	// ------------------------------------------
	// Don't draw if player is dead
	// ------------------------------------------

	if (!playerAlive)
	{
		return;
	}


	// ------------------------------------------
	// Check texture
	// ------------------------------------------

	if (playerTexture == 0)
	{
		return;
	}


	glEnable(
		GL_TEXTURE_2D
		);


	glBindTexture(
		GL_TEXTURE_2D,
		playerTexture
		);


	// Reset color

	glColor3f(
		1.0f,
		1.0f,
		1.0f
		);


	// ------------------------------------------
	// Draw ship
	// ------------------------------------------

	glBegin(
		GL_QUADS
		);


	// Bottom Left

	glTexCoord2f(
		0.0f,
		1.0f
		);

	glVertex2f(
		playerX,
		playerY
		);


	// Bottom Right

	glTexCoord2f(
		1.0f,
		1.0f
		);

	glVertex2f(
		playerX + playerWidth,
		playerY
		);


	// Top Right

	glTexCoord2f(
		1.0f,
		0.0f
		);

	glVertex2f(
		playerX + playerWidth,
		playerY + playerHeight
		);


	// Top Left

	glTexCoord2f(
		0.0f,
		0.0f
		);

	glVertex2f(
		playerX,
		playerY + playerHeight
		);


	glEnd();


	glBindTexture(
		GL_TEXTURE_2D,
		0
		);


	glDisable(
		GL_TEXTURE_2D
		);
}


// ======================================================
// MOVE PLAYER LEFT
// ======================================================

void movePlayerLeft()
{
	playerX -= playerSpeed;


	// ------------------------------------------
	// Stop at left boundary
	// ------------------------------------------

	if (playerX < 0)
	{
		playerX = 0;
	}
}


// ======================================================
// MOVE PLAYER RIGHT
// ======================================================

void movePlayerRight()
{
	playerX += playerSpeed;


	// ------------------------------------------
	// Stop at right boundary
	// ------------------------------------------

	if (
		playerX + playerWidth >
		PLAYER_SCREEN_WIDTH
		)
	{
		playerX =
			PLAYER_SCREEN_WIDTH -
			playerWidth;
	}
}


// ======================================================
// MOVE PLAYER UP
// ======================================================

void movePlayerUp()
{
	playerY += playerSpeed;


	// ------------------------------------------
	// Stop at top boundary
	// ------------------------------------------

	if (
		playerY + playerHeight >
		PLAYER_SCREEN_HEIGHT
		)
	{
		playerY =
			PLAYER_SCREEN_HEIGHT -
			playerHeight;
	}
}


// ======================================================
// MOVE PLAYER DOWN
// ======================================================

void movePlayerDown()
{
	playerY -= playerSpeed;


	// ------------------------------------------
	// Stop at bottom boundary
	// ------------------------------------------

	if (playerY < 0)
	{
		playerY = 0;
	}
}


// ======================================================
// PLAYER KEYBOARD CONTROL
// ======================================================
//
// Arrow Keys:
// LEFT  -> Move Left
// RIGHT -> Move Right
// UP    -> Move Up
// DOWN  -> Move Down
//
// ======================================================

void playerSpecialKeyboard(
	int key
	)
{
	// ------------------------------------------
	// Only move during Ocean Battle
	// ------------------------------------------

	if (currentState != OCEAN_BATTLE)
	{
		return;
	}


	// ------------------------------------------
	// Don't move if dead
	// ------------------------------------------

	if (!playerAlive)
	{
		return;
	}


	// ------------------------------------------
	// LEFT
	// ------------------------------------------

	if (key == GLUT_KEY_LEFT)
	{
		movePlayerLeft();
	}


	// ------------------------------------------
	// RIGHT
	// ------------------------------------------

	else if (key == GLUT_KEY_RIGHT)
	{
		movePlayerRight();
	}


	// ------------------------------------------
	// UP
	// ------------------------------------------

	else if (key == GLUT_KEY_UP)
	{
		movePlayerUp();
	}


	// ------------------------------------------
	// DOWN
	// ------------------------------------------

	else if (key == GLUT_KEY_DOWN)
	{
		movePlayerDown();
	}
}


// ======================================================
// DAMAGE PLAYER
// ======================================================

void damagePlayer()
{
	if (!playerAlive)
	{
		return;
	}


	// Reduce health

	playerHealth--;


	printf(
		"Player Health: %d\n",
		playerHealth
		);


	// ------------------------------------------
	// Health reaches zero
	// ------------------------------------------

	if (playerHealth <= 0)
	{
		playerHealth = 0;

		playerAlive = false;

		gameOver = true;

		currentState = GAME_OVER;
	}
}


// ======================================================
// RESET PLAYER
// ======================================================

void resetPlayer()
{
	playerX = 625;
	playerY = 125;


	playerHealth = 3;

	playerAlive = true;
}


// ======================================================
// DRAW PLAYER HEALTH
// ======================================================

void drawPlayerHealth()
{
	char healthText[50];


	sprintf_s(
		healthText,
		"Pirate Health: %d",
		playerHealth
		);


	iSetColor(
		255,
		255,
		255
		);


	iText(
		25,
		950,
		healthText,
		GLUT_BITMAP_HELVETICA_18
		);
}


#endif