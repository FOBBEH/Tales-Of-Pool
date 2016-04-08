// [Physics.h]
// Deals with the ball collision
#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector.h"
#include "PoolObjects.h"

//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
void HitCueBall( Ball *a, float mouseX, float mouseY, D3DXVECTOR3 cameraPos);
bool AreBallsColliding( Vector3 p1, float r1, Vector3 p2, float r2 );
Vector3 FromAToB( Ball *a, Ball *b );
Vector3 Reflection( Vector3 &a, Vector3 &b );

#endif // PHYSICS_H