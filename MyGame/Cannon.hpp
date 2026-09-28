#ifndef CANNON_HPP
#define CANNON_HPP

#include "iGraphics.h"
#include "GameState.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

#include <stdio.h>
#include <math.h>


// ======================================================
// SCREEN SIZE
// ======================================================

#define CANNON_SCREEN_WIDTH 1500
#define CANNON_SCREEN_HEIGHT 1000


// ======================================================
// PLAYER CANNON SETTINGS
// ======================================================

#define MAX_PLAYER_BALLS 30

float playerCannonSpeed = 10;


// ======================================================
// ENEMY CANNON SETTINGS
// ======================================================

#define MAX_ENEMY_BALLS 5

float enemyCannonSpeed = 6;


//
// Timer = 20 ms
// Interval = 150
//
// 150 × 20 ms = 3000 ms
// = 3 seconds

int enemyFireTimer = 0;

int enemyFireInterval = 150;


// ======================================================
// CANNONBALL STRUCT
// ======================================================

struct CannonBall
{
	bool active;

	float x;
	float y;

	float vx;
	float vy;
};


// ======================================================
// PLAYER CANNONBALL ARRAY
// ======================================================

CannonBall playerBalls[MAX_PLAYER_BALLS];


// ======================================================
// ENEMY CANNONBALL ARRAY
// ======================================================

CannonBall enemyBalls[MAX_ENEMY_BALLS];


// ======================================================
// INITIALIZE CANNON
// ======================================================

void initializeCannon()
{
	int i;


	// --------------------------------------------------
	// Player cannonballs reset
	// --------------------------------------------------

	for (i = 0; i < MAX_PLAYER_BALLS; i++)
	{
		playerBalls[i].active = false;

		playerBalls[i].x = 0;
		playerBalls[i].y = 0;

		playerBalls[i].vx = 0;
		playerBalls[i].vy = 0;
	}


	// --------------------------------------------------
	// Enemy cannonballs reset
	// --------------------------------------------------

	for (i = 0; i < MAX_ENEMY_BALLS; i++)
	{
		enemyBalls[i].active = false;

		enemyBalls[i].x = 0;
		enemyBalls[i].y = 0;

		enemyBalls[i].vx = 0;
		enemyBalls[i].vy = 0;
	}


	// Reset enemy firing timer

	enemyFireTimer = 0;
}


// ======================================================
// PLAYER FIRE CANNON
// ======================================================

void firePlayerCannon()
{
	int i;

	if (currentState != OCEAN_BATTLE)
	{
		return;
	}

	if (!playerAlive)
	{
		return;
	}

	for (i = 0; i < MAX_PLAYER_BALLS; i++)
	{
		if (!playerBalls[i].active)
		{
			playerBalls[i].active = true;

			float startX = playerX + playerWidth / 2;
			float startY = playerY + playerHeight;

			playerBalls[i].x = startX;
			playerBalls[i].y = startY;

			// --------------------------------------------------
			// Target enemy center
			// --------------------------------------------------

			float targetX = enemyX + enemyWidth / 2;
			float targetY = enemyY + enemyHeight / 2;

			float dx = targetX - startX;
			float dy = targetY - startY;

			float distance = sqrt(dx * dx + dy * dy);

			if (distance == 0)
			{
				distance = 1;
			}

			playerBalls[i].vx = (dx / distance) * playerCannonSpeed;
			playerBalls[i].vy = (dy / distance) * playerCannonSpeed;

			break;
		}
	}
}


// ======================================================
// DRAW PLAYER CANNONBALL
// ======================================================

void drawPlayerCannonballs()
{
	int i;


	for (i = 0; i < MAX_PLAYER_BALLS; i++)
	{
		if (playerBalls[i].active)
		{
			// White cannonball

			iSetColor(
				255,
				255,
				255
				);


			iFilledCircle(
				playerBalls[i].x,
				playerBalls[i].y,
				8
				);
		}
	}
}


// ======================================================
// UPDATE PLAYER CANNONBALL
// ======================================================

void updatePlayerCannonballs()
{
	int i;


	for (i = 0; i < MAX_PLAYER_BALLS; i++)
	{
		if (!playerBalls[i].active)
		{
			continue;
		}


		// --------------------------------------------------
		// Move cannonball
		// --------------------------------------------------

		playerBalls[i].x +=
			playerBalls[i].vx;


		playerBalls[i].y +=
			playerBalls[i].vy;


		// --------------------------------------------------
		// Remove if it leaves screen
		// --------------------------------------------------

		if (
			playerBalls[i].y >
			CANNON_SCREEN_HEIGHT
			)
		{
			playerBalls[i].active = false;

			continue;
		}


		// --------------------------------------------------
		// Collision with enemy
		// --------------------------------------------------

		if (enemyAlive)
		{
			if (
				playerBalls[i].x >= enemyX &&
				playerBalls[i].x <=
				enemyX + enemyWidth &&

				playerBalls[i].y >= enemyY &&
				playerBalls[i].y <=
				enemyY + enemyHeight
				)
			{
				// Enemy takes one damage

				damageEnemy();


				// Remove cannonball

				playerBalls[i].active = false;


				printf(
					"Player cannonball hit Enemy!\n"
					);


				printf(
					"Enemy Health = %d\n",
					enemyHealth
					);
			}
		}
	}
}


// ======================================================
// ENEMY FIRE CANNON
// ======================================================
//
// ======================================================

void fireEnemyCannon()
{
	int i;


	// --------------------------------------------------
	// Only during Ocean Battle
	// --------------------------------------------------

	if (currentState != OCEAN_BATTLE)
	{
		return;
	}



	if (!enemyAlive)
	{
		return;
	}



	if (!playerAlive)
	{
		return;
	}


	// --------------------------------------------------
	// Find inactive enemy cannonball
	// --------------------------------------------------

	for (i = 0; i < MAX_ENEMY_BALLS; i++)
	{
		if (!enemyBalls[i].active)
		{
			enemyBalls[i].active = true;


			// --------------------------------------------------
			// Enemy cannonball starting position
			// --------------------------------------------------

			float startX =
				enemyX + enemyWidth / 2;


			float startY =
				enemyY;


			enemyBalls[i].x =
				startX;


			enemyBalls[i].y =
				startY;


			// --------------------------------------------------
			// --------------------------------------------------

			float targetX =
				playerX + playerWidth / 2;


			float targetY =
				playerY + playerHeight / 2;


			// --------------------------------------------------
			// Calculate direction
			// --------------------------------------------------

			float dx =
				targetX - startX;


			float dy =
				targetY - startY;


			float distance =
				sqrt(
				dx * dx +
				dy * dy
				);


			// --------------------------------------------------
			// Prevent division by zero
			// --------------------------------------------------

			if (distance == 0)
			{
				distance = 1;
			}


			// --------------------------------------------------
			// Set velocity towards Player
			// --------------------------------------------------

			enemyBalls[i].vx =
				(dx / distance) *
				enemyCannonSpeed;


			enemyBalls[i].vy =
				(dy / distance) *
				enemyCannonSpeed;


			printf(
				"Enemy fired cannonball!\n"
				);


			break;
		}
	}
}


// ======================================================
// DRAW ENEMY CANNONBALL
// ======================================================

void drawEnemyCannonballs()
{
	int i;


	for (i = 0; i < MAX_ENEMY_BALLS; i++)
	{
		if (enemyBalls[i].active)
		{
			// Red enemy cannonball

			iSetColor(
				255,
				80,
				80
				);


			iFilledCircle(
				enemyBalls[i].x,
				enemyBalls[i].y,
				8
				);
		}
	}
}


// ======================================================
// UPDATE ENEMY CANNONBALL
// ======================================================

void updateEnemyCannonballs()
{
	int i;


	for (i = 0; i < MAX_ENEMY_BALLS; i++)
	{
		if (!enemyBalls[i].active)
		{
			continue;
		}


		// --------------------------------------------------
		// Move cannonball
		// --------------------------------------------------

		enemyBalls[i].x +=
			enemyBalls[i].vx;


		enemyBalls[i].y +=
			enemyBalls[i].vy;


		// --------------------------------------------------
		// Remove if outside screen
		// --------------------------------------------------

		if (
			enemyBalls[i].x < 0 ||

			enemyBalls[i].x >
			CANNON_SCREEN_WIDTH ||

			enemyBalls[i].y < 0 ||

			enemyBalls[i].y >
			CANNON_SCREEN_HEIGHT
			)
		{
			enemyBalls[i].active = false;

			continue;
		}


		// --------------------------------------------------
		// Collision with Player
		// --------------------------------------------------

		if (playerAlive)
		{
			if (
				enemyBalls[i].x >= playerX &&
				enemyBalls[i].x <=
				playerX + playerWidth &&

				enemyBalls[i].y >= playerY &&
				enemyBalls[i].y <=
				playerY + playerHeight
				)
			{
				// Player takes damage

				damagePlayer();


				// Remove cannonball

				enemyBalls[i].active = false;


				printf(
					"Enemy cannonball hit Player!\n"
					);


				printf(
					"Player Health = %d\n",
					playerHealth
					);
			}
		}
	}
}


// ======================================================
// AUTOMATIC ENEMY FIRE
// ======================================================
//
// ======================================================

void updateEnemyFire()
{
	// --------------------------------------------------
	// Only during Ocean Battle
	// --------------------------------------------------

	if (currentState != OCEAN_BATTLE)
	{
		enemyFireTimer = 0;

		return;
	}



	if (!enemyAlive)
	{
		return;
	}



	if (!playerAlive)
	{
		return;
	}


	// --------------------------------------------------
	// Increase timer
	// --------------------------------------------------

	enemyFireTimer++;


	// --------------------------------------------------
	// Time reached
	// --------------------------------------------------

	if (
		enemyFireTimer >=
		enemyFireInterval
		)
	{
		fireEnemyCannon();


		// Reset timer

		enemyFireTimer = 0;
	}
}


// ======================================================
// UPDATE ALL CANNON SYSTEM
// ======================================================

void updateCannon()
{
	// Only update during Ocean Battle

	if (currentState != OCEAN_BATTLE)
	{
		return;
	}


	// Player cannonballs

	updatePlayerCannonballs();


	// Enemy cannonballs

	updateEnemyCannonballs();


	// Enemy automatic firing

	updateEnemyFire();
}


// ======================================================
// DRAW ALL CANNON SYSTEM
// ======================================================

void drawCannon()
{
	// Only draw during Ocean Battle

	if (currentState != OCEAN_BATTLE)
	{
		return;
	}


	drawPlayerCannonballs();

	drawEnemyCannonballs();
}


// ======================================================
// PLAYER SPACE KEY
// ======================================================

void cannonKeyboard(
	unsigned char key
	)
{
	// SPACE

	if (key == ' ')
	{
		if (
			currentState ==
			OCEAN_BATTLE
			)
		{
			firePlayerCannon();
		}
	}
}


// ======================================================
// RESET CANNON SYSTEM
// ======================================================

void resetCannon()
{
	int i;


	// --------------------------------------------------
	// Reset player cannonballs
	// --------------------------------------------------

	for (i = 0; i < MAX_PLAYER_BALLS; i++)
	{
		playerBalls[i].active = false;

		playerBalls[i].x = 0;
		playerBalls[i].y = 0;

		playerBalls[i].vx = 0;
		playerBalls[i].vy = 0;
	}


	// --------------------------------------------------
	// Reset enemy cannonballs
	// --------------------------------------------------

	for (i = 0; i < MAX_ENEMY_BALLS; i++)
	{
		enemyBalls[i].active = false;

		enemyBalls[i].x = 0;
		enemyBalls[i].y = 0;

		enemyBalls[i].vx = 0;
		enemyBalls[i].vy = 0;
	}


	// Reset enemy firing timer

	enemyFireTimer = 0;
}


#endif