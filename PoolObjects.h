// [PoolObjects.h]
// Stores the balls + table + others
#ifndef POOLOBJECTS_H
#define POOLOBJECTS_H

#include "GameObjects.h"

// Total number of balls, tables for decorations
#define NUM_BALLS				16
#define NUM_TABLES				9

struct Ball : public GameObject
{
	Ball( float x, float y, float z );
	~Ball() {}
	
	Vector3						m_initPos;			// Only accessed for setting the initial rotation
	Vector3						m_direction;
	float						m_radius;
	float						m_velocity, m_friction;
	bool						m_stripes, m_solids;

	void						Update();
	void						SetPosition();
	void						SetRotation();
};

struct Table : public GameObject
{
	Table( float x, float y, float z );
	~Table() {}

	void						Update() {}			// Position or rotation never changes
	void						SetPosition() {}	// Need to define because of abstract class
	void						SetRotation() {}	// ie. GameObject inheritence
};

// Set the initial positions of everything
void							InitialiseObjects();
void							CheckMovement();
void							ResetCueBall();
void							ResetBalls();

// Vector lists
extern std::vector<GameObject*>	ObjectList;			// Keep track of all the world objects (incl. balls)
extern std::vector<Ball*>		BallList;			// Keep track of ONLY the balls
extern std::vector<Ball*>		P1BallList;			// Keep track of Player 1's balls
extern std::vector<Ball*>		P2BallList;			// Keep track of Player 2's balls

#endif // POOLOBJECTS_H