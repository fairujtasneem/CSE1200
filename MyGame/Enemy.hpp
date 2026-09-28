#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "iGraphics.h"
#include "GameState.hpp"

#include <stdio.h>


// ======================================================
// SCREEN SIZE
// ======================================================

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000


// ======================================================
// ENEMY SHIP POSITION
// ======================================================


float enemyX = 1000;
float enemyY = -167;


// ======================================================
// ENEMY SHIP SIZE
// ======================================================

int enemyWidth = 200;
int enemyHeight = 250;


// ======================================================
// ENEMY SPEED
// ======================================================

// Slow movement

float enemySpeed = 2;


// ======================================================
// ENEMY HEALTH
// ======================================================

int enemyHealth = 3;


// Enemy alive status

bool enemyAlive = true;


// ======================================================
// ENEMY IMAGE
// ======================================================

GLuint enemyTexture = 0;

int enemyImageWidth = 0;
int enemyImageHeight = 0;


// ======================================================
// LOAD ENEMY PNG
// ======================================================

GLuint loadEnemyPNG(
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
	// Image loading failed
	// ------------------------------------------

	if (imageData == NULL)
	{
		printf(
			"ERROR: Cannot load enemy image: %s\n",
			filename
			);

		return 0;
	}


	// ------------------------------------------
	// Generate texture
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
	// Texture settings
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
	// Free image memory
	// ------------------------------------------

	stbi_image_free(
		imageData
		);


	glBindTexture(
		GL_TEXTURE_2D,
		0
		);


	printf(
		"Enemy image loaded: %s\n",
		filename
		);


	return texture;
}


// ======================================================
// INITIALIZE ENEMY
// ======================================================

void initializeEnemy()
{
	// ------------------------------------------
	// Starting position
	// ------------------------------------------

	enemyX = 1000;

	enemyY = -167;


	// ------------------------------------------
	// Enemy size
	// ------------------------------------------

	enemyWidth = 167;

	enemyHeight = 167;


	// ------------------------------------------
	// Enemy speed
	// ------------------------------------------

	enemySpeed = 3;


	// ------------------------------------------
	// Enemy health
	// ------------------------------------------

	enemyHealth = 3;

	enemyAlive = true;


	// ------------------------------------------
	// Load PNG
	// ------------------------------------------

	enemyTexture = loadEnemyPNG(
		"enemyShip.png",
		&enemyImageWidth,
		&enemyImageHeight
		);
}


// ======================================================
// DRAW ENEMY SHIP
// ======================================================

void drawEnemy()
{
	// Don't draw dead enemy

	if (!enemyAlive)
	{
		return;
	}


	// Check texture

	if (enemyTexture == 0)
	{
		return;
	}


	glEnable(
		GL_TEXTURE_2D
		);


	glBindTexture(
		GL_TEXTURE_2D,
		enemyTexture
		);


	glColor3f(
		1.0f,
		1.0f,
		1.0f
		);


	// ------------------------------------------
	// Draw enemy
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
		enemyX,
		enemyY
		);


	// Bottom Right

	glTexCoord2f(
		1.0f,
		1.0f
		);

	glVertex2f(
		enemyX + enemyWidth,
		enemyY
		);


	// Top Right

	glTexCoord2f(
		1.0f,
		0.0f
		);

	glVertex2f(
		enemyX + enemyWidth,
		enemyY + enemyHeight
		);


	// Top Left

	glTexCoord2f(
		0.0f,
		0.0f
		);

	glVertex2f(
		enemyX,
		enemyY + enemyHeight
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
// ENEMY MOVEMENT
// ======================================================

void updateEnemy()
{
	// Only move during ocean battle

	if (
		currentState != OCEAN_BATTLE
		)
	{
		return;
	}


	// Don't move if dead

	if (!enemyAlive)
	{
		return;
	}


	// ------------------------------------------
	// Move upward
	// ------------------------------------------

	enemyY += enemySpeed;


	// ------------------------------------------
	// When enemy reaches top
	// ------------------------------------------

	if (enemyY > SCREEN_HEIGHT)
	{
		

		enemyY = -enemyHeight;

		enemyX =
			583 +
			rand() % 583;
	}
}


// ======================================================
// DAMAGE ENEMY
// ======================================================

void damageEnemy()
{
	if (!enemyAlive)
	{
		return;
	}


	// Reduce health

	enemyHealth--;


	printf(
		"Enemy Health: %d\n",
		enemyHealth
		);


	// ------------------------------------------
	// Enemy destroyed
	// ------------------------------------------

	if (enemyHealth <= 0)
	{
		enemyHealth = 0;

		enemyAlive = false;


		printf(
			"Enemy Ship Destroyed!\n"
			);
	}
}


// ======================================================
// RESET ENEMY
// ======================================================

void resetEnemy()
{
	enemyX = 1000;

	enemyY = -enemyHeight;


	if (currentLevel == 1)
	{
		enemyHealth = 3;

		enemySpeed = 2;
	}
	else if (currentLevel == 2)
	{
		enemyHealth = 4;

		enemySpeed = 2.5f;
	}
	else if (currentLevel == 3)
	{
		enemyHealth = 6;

		enemySpeed = 3.5f;
	}


	enemyAlive = true;
}



// ======================================================
// DRAW ENEMY HEALTH
// ======================================================

void drawEnemyHealth()
{
	char healthText[50];


	sprintf_s(
		healthText,
		"Enemy Health: %d",
		enemyHealth
		);


	iSetColor(
		255,
		255,
		255
		);


	iText(
		1480,
		1140,
		healthText,
		GLUT_BITMAP_HELVETICA_18
		);
}


#endif