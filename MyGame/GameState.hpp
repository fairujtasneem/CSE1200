#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

// ======================================
// GAME STATES
// ======================================

enum GameState
{
	LOADING,              // Loading screen
	MAIN_MENU,            // Main menu
	INSTRUCTIONS,         // Instructions screen
	UPGRADE_MENU,         // Upgrade ship screen

	LEVEL_SELECT,         // Level selection screen
	LEVEL_INTRO,          // Selected level intro screen

	OCEAN_BATTLE,         // Fight enemy ship
	ISLAND_EXPLORATION,   // Explore island
	LEVEL_COMPLETE,       // Treasure collected

	GAME_OVER             // Player health = 0
};


// ======================================
// CURRENT GAME STATE
// ======================================


GameState currentState = LOADING;


// ======================================
// CURRENT LEVEL
// ======================================

// 1 = Level 1
// 2 = Level 2
// 3 = Level 3

int currentLevel = 1;


// ======================================
// PREVIOUS LEVEL
// ======================================



int previousLevel = 1;


// ======================================
// GAME CONTROL
// ======================================


bool gameOver = false;


bool levelCompleted = false;


#endif