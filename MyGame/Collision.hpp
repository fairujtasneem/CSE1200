#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "iGraphics.h"
#include "GameState.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Cannon.hpp"

#include <math.h>
#include <stdio.h>


// ======================================================
// SCREEN SIZE
// ======================================================

#define COLLISION_SCREEN_WIDTH 1500
#define COLLISION_SCREEN_HEIGHT 1000


// ======================================================
// COLLISION SETTINGS
// ======================================================

// Cannonball collision radius

float collisionBallRadius = 8;


// ======================================================
// CHECK POINT INSIDE RECTANGLE
// ======================================================

bool pointInsideShip(
	float pointX,
	float pointY,
	float shipX,
	float shipY,
	float shipWidth,
	float shipHeight
	)
{
	if (
		pointX >= shipX &&
		pointX <= shipX + shipWidth &&
		pointY >= shipY &&
		pointY <= shipY + shipHeight
		)
	{
		return true;
	}

	return false;
}


// ======================================================
// PLAYER CANNONBALL vs ENEMY
// ======================================================

bool playerBallHitsEnemy(
	CannonBall &ball
	)
{
	if (!ball.active)
	{
		return false;
	}

	if (!enemyAlive)
	{
		return false;
	}


	// Check cannonball position
	// inside enemy ship

	if (
		pointInsideShip(
		ball.x,
		ball.y,
		enemyX,
		enemyY,
		enemyWidth,
		enemyHeight
		)
		)
	{
		return true;
	}

	return false;
}


// ======================================================
// ENEMY CANNONBALL vs PLAYER
// ======================================================

bool enemyBallHitsPlayer(
	CannonBall &ball
	)
{
	if (!ball.active)
	{
		return false;
	}

	if (!playerAlive)
	{
		return false;
	}


	// Check cannonball position
	// inside player ship

	if (
		pointInsideShip(
		ball.x,
		ball.y,
		playerX,
		playerY,
		playerWidth,
		playerHeight
		)
		)
	{
		return true;
	}

	return false;
}


// ======================================================
// CHECK PLAYER CANNONBALL COLLISION
// ======================================================

void checkPlayerCannonCollisions()
{
	int i;


	for (
		i = 0;
		i < MAX_PLAYER_BALLS;
	i++
		)
	{
		if (!playerBalls[i].active)
		{
			continue;
		}


		// ------------------------------------------
		// Player ball hit Enemy
		// ------------------------------------------

		if (
			playerBallHitsEnemy(
			playerBalls[i]
			)
			)
		{
			// Enemy loses 1 health

			damageEnemy();


			// Cannonball disappears

			playerBalls[i].active = false;


			printf(
				"Enemy was hit!\n"
				);

			printf(
				"Enemy Health: %d\n",
				enemyHealth
				);
		}
	}
}


// ======================================================
// CHECK ENEMY CANNONBALL COLLISION
// ======================================================

void checkEnemyCannonCollisions()
{
	int i;


	for (
		i = 0;
		i < MAX_ENEMY_BALLS;
	i++
		)
	{
		if (!enemyBalls[i].active)
		{
			continue;
		}


		// ------------------------------------------
		// Enemy ball hit Player
		// ------------------------------------------

		if (
			enemyBallHitsPlayer(
			enemyBalls[i]
			)
			)
		{
			// Player loses 1 health

			damagePlayer();


			// Cannonball disappears

			enemyBalls[i].active = false;


			printf(
				"Player was hit!\n"
				);

			printf(
				"Player Health: %d\n",
				playerHealth
				);
		}
	}
}


// ======================================================
// CHECK SHIP vs SHIP COLLISION
// ======================================================
//
// Optional.
// ======================================================

bool shipsCollide()
{
	if (!playerAlive)
	{
		return false;
	}

	if (!enemyAlive)
	{
		return false;
	}


	// Rectangle collision

	if (
		playerX <
		enemyX + enemyWidth &&

		playerX + playerWidth >
		enemyX &&

		playerY <
		enemyY + enemyHeight &&

		playerY + playerHeight >
		enemyY
		)
	{
		return true;
	}


	return false;
}


// ======================================================
// CHECK ALL COLLISIONS
// ======================================================

void checkAllCollisions()
{
	// Only check during Ocean Battle

	if (
		currentState !=
		OCEAN_BATTLE
		)
	{
		return;
	}


	// ------------------------------------------
	// Player cannonball ? Enemy
	// ------------------------------------------

	checkPlayerCannonCollisions();


	// ------------------------------------------
	// Enemy cannonball ? Player
	// ------------------------------------------

	checkEnemyCannonCollisions();
}


// ======================================================
// UPDATE COLLISION
// ======================================================

void updateCollision()
{
	if (
		currentState !=
		OCEAN_BATTLE
		)
	{
		return;
	}


	checkAllCollisions();
}


#endif