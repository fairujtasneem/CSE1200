#ifndef LOADING_HPP
#define LOADING_HPP

#include "iGraphics.h"
#include "GameState.hpp"

#include <stdio.h>




// ==========================================
// SCREEN SIZE
// ==========================================

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000


// ==========================================
// LOADING IMAGE
// ==========================================

GLuint loadingTexture = 0;

int loadingImageWidth = 0;
int loadingImageHeight = 0;


// ==========================================
// LOADING PROGRESS
// ==========================================

int loadingProgress = 0;


// ==========================================
// LOAD LOADING IMAGE
// ==========================================

void loadLoadingImage()
{
	int channels;

	unsigned char *imageData = stbi_load(
		"Loading.png",
		&loadingImageWidth,
		&loadingImageHeight,
		&channels,
		4
		);

	if (imageData == NULL)
	{
		printf("ERROR: Loading.png could not be loaded!\n");
		return;
	}


	// Create OpenGL texture
	glGenTextures(
		1,
		&loadingTexture
		);


	glBindTexture(
		GL_TEXTURE_2D,
		loadingTexture
		);


	// Texture settings
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


	// Send image to GPU
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		loadingImageWidth,
		loadingImageHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		imageData
		);


	// Free image memory
	stbi_image_free(imageData);


	glBindTexture(
		GL_TEXTURE_2D,
		0
		);


	printf("Loading.png loaded successfully!\n");
}


// ==========================================
// INITIALIZE LOADING
// ==========================================

void initializeLoading()
{
	loadingProgress = 0;

	loadLoadingImage();
}


// ==========================================
// UPDATE LOADING ANIMATION
// ==========================================

void updateLoading()
{
	// Only update when loading screen is active
	if (currentState != LOADING)
		return;


	if (loadingProgress < 100)
	{
		loadingProgress += 1;
	}


	// Loading finished
	if (loadingProgress >= 100)
	{
		loadingProgress = 100;

		currentState = MAIN_MENU;
	}
}


// ==========================================
// DRAW LOADING BACKGROUND
// ==========================================

void drawLoadingBackground()
{
	if (loadingTexture == 0)
		return;


	glEnable(GL_TEXTURE_2D);

	glBindTexture(
		GL_TEXTURE_2D,
		loadingTexture
		);


	glBegin(GL_QUADS);


	// Bottom Left
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(
		0,
		0
		);


	// Bottom Right
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(
		SCREEN_WIDTH,
		0
		);


	// Top Right
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(
		SCREEN_WIDTH,
		SCREEN_HEIGHT
		);


	// Top Left
	glTexCoord2f(0.0f, 0.0f);
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
// DRAW LOADING ANIMATION
// ==========================================

void drawLoadingBar()
{
	// --------------------------------------
	// Position based on your Loading.png
	// --------------------------------------

	int barX = 333;
	int barY = 54;

	int barWidth = 833;
	int barHeight = 32;


	// --------------------------------------
	// Loading progress
	// --------------------------------------

	int progressWidth =
		(loadingProgress * barWidth) / 100;


	// --------------------------------------
	// Draw animated loading line
	// --------------------------------------

	iSetColor(
		255,
		190,
		60
		);


	iFilledRectangle(
		barX,
		barY,
		progressWidth,
		barHeight
		);
}


// ==========================================
// DRAW LOADING SCREEN
// ==========================================

void drawLoading()
{
	// Draw complete Loading.png
	drawLoadingBackground();


	// Draw animated progress
	// between the two lines
	drawLoadingBar();
}


#endif