#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "iGraphics.h"
#include "GameState.hpp"

// ======================================================
// INSTRUCTION SCREEN SIZE
// ======================================================

#define INSTRUCTION_WIDTH 1500
#define INSTRUCTION_HEIGHT 1000


// ======================================================
// DRAW INSTRUCTIONS
// ======================================================

void drawInstructions()
{
	// ------------------------------------------
	// Background
	// ------------------------------------------

	iSetColor(10, 30, 55);

	iFilledRectangle(
		0,
		0,
		INSTRUCTION_WIDTH,
		INSTRUCTION_HEIGHT
		);


	// ------------------------------------------
	// Title
	// ------------------------------------------

	iSetColor(255, 215, 0);

	iText(
		583,
		900,
		"HOW TO PLAY",
		GLUT_BITMAP_TIMES_ROMAN_24
		);


	// ==================================================
	// GAME FLOW
	// ==================================================

	iSetColor(255, 255, 255);

	iText(
		542,
		817,
		"GAME FLOW",
		GLUT_BITMAP_HELVETICA_18
		);


	iText(
		417,
		775,
		"1. Loading Screen",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		742,
		"2. Main Menu",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		708,
		"3. Select Level",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		675,
		"4. Level Intro",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		642,
		"5. Ocean Battle",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		608,
		"6. Island Exploration",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		575,
		"7. Treasure Collection",
		GLUT_BITMAP_HELVETICA_18
		);


	// ==================================================
	// OCEAN BATTLE
	// ==================================================

	iSetColor(255, 215, 0);

	iText(
		542,
		817,
		"OCEAN BATTLE",
		GLUT_BITMAP_HELVETICA_18
		);


	iSetColor(255, 255, 255);

	iText(
		417,
		467,
		"Arrow Keys  : Move Pirate Ship",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		433,
		"SPACE       : Fire Cannon",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		400,
		"Enemy Ship  : Moves and Fires Cannonballs",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		367,
		"Defeat the Enemy Ship to continue",
		GLUT_BITMAP_HELVETICA_18
		);


	// ==================================================
	// ISLAND EXPLORATION
	// ==================================================

	iSetColor(255, 215, 0);

	iText(
		542,
		300,
		"ISLAND EXPLORATION",
		GLUT_BITMAP_HELVETICA_18
		);


	iSetColor(255, 255, 255);

	iText(
		417,
		258,
		"Move forward through the island",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		225,
		"Collect the treasure to complete the level",
		GLUT_BITMAP_HELVETICA_18
		);


	// ==================================================
	// OTHER CONTROLS
	// ==================================================

	iSetColor(255, 215, 0);

	iText(
		542,
		158,
		"OTHER CONTROLS",
		GLUT_BITMAP_HELVETICA_18
		);


	iSetColor(255, 255, 255);

	iText(
		417,
		121,
		"M : Return to Level Menu",
		GLUT_BITMAP_HELVETICA_18
		);

	iText(
		417,
		88,
		"ESC : Return to Main Menu",
		GLUT_BITMAP_HELVETICA_18
		);
}


// ======================================================
// INSTRUCTION KEYBOARD
// ======================================================

void instructionKeyboard(
	unsigned char key
	)
{
	// ------------------------------------------
	// ESC = MAIN MENU
	// ------------------------------------------

	if (key == 27)
	{
		currentState = MAIN_MENU;
	}


	// ------------------------------------------
	// M = LEVEL MENU
	// ------------------------------------------

	else if (
		key == 'm' ||
		key == 'M'
		)
	{
		currentState = LEVEL_SELECT;
	}
}


// ======================================================
// INITIALIZE INSTRUCTION
// ======================================================

void initializeInstructions()
{
	currentState = INSTRUCTIONS;
}


#endif