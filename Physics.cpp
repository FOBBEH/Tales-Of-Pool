#include "Physics.h"

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void HitCueBall( Ball *a, float mouseX, float mouseY, D3DXVECTOR3 cameraPos )
{
	Vector3 mouseToBall;

	//-----------------------------------------------------------------------------
	// Chris' wizard maths:
	//-----------------------------------------------------------------------------
	//Move where the ball is into screen space
	D3DXMATRIX world, view, projection, worldViewProjection;
	//World
	D3DXMatrixTranslation(&world, a->m_position.x, a->m_position.y, a->m_position.z);
	//View
	D3DXVECTOR3 vEyePt( cameraPos.x, cameraPos.y, cameraPos.z );
	D3DXVECTOR3 vLookatPt( cameraPos.x, -1.0f, cameraPos.z);
	D3DXVECTOR3 vUpVec( -1.0f, 0.0f, 0.0f );
	D3DXMatrixLookAtLH( &view, &vEyePt, &vLookatPt, &vUpVec );
	//Projection
	D3DXMatrixPerspectiveFovLH( &projection, D3DX_PI / 4, (FLOAT)1280 / (FLOAT)720, 1.0f, 650.0f );
	worldViewProjection = world * view * projection;

	D3DXVECTOR4 finalPos(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVec4Transform(&finalPos, &finalPos, &worldViewProjection);
	finalPos /= finalPos.w;
	D3DXVECTOR3 ballPos(finalPos.x, finalPos.y, 0.0f);

	//Move the mouse position into screen space
	mouseX -= (1280.0f / 2.0f);
	mouseX /= (1280.0f / 2.0f);
	mouseY -= (720.0f / 2.0f);
	mouseY /= (720.0f / 2.0f);
	//mouseY *= -1.0f;
	D3DXVECTOR3 mousePos(mouseX, mouseY, 0.0f);

	//Get vector from mouse TO ball
	//Do the thang

	// The vector from the mouse position to the cue ball
	mouseToBall.x = ballPos.y - mousePos.y;
	mouseToBall.y = 0.0f;
	mouseToBall.z = ballPos.x - mousePos.x;

	float distance = mouseToBall.GetLength();
	mouseToBall.Normalise();

	// Set the cue ball's direction to the mouseToBall vector (normalised)
	a->m_direction.x = mouseToBall.x;
	a->m_direction.y = mouseToBall.y;
	a->m_direction.z = mouseToBall.z;

	// Apply velocity based on the distance
	a->m_velocity = distance * 7.5f;
}

bool AreBallsColliding( Vector3 p1, float r1, Vector3 p2, float r2 )
{
	Vector3 distanceV = p1 - p2;
	float distanceF = distanceV.GetLength();
	float distanceBetween = distanceF - r1 - r2;

	return ( distanceBetween <= 0 );
}

// The Vector from the initial ball to the colliding ball:
Vector3 FromAToB( Ball *a, Ball *b )
{
	Vector3 result;

	result.x = b->m_position.x - a->m_position.x;
	result.y = b->m_position.y - a->m_position.y;
	result.z = b->m_position.z - a->m_position.z;
	result.Normalise();

	return result;
}

// Reflect the vector if the ball collides against the sides
Vector3 Reflection( Vector3 &a, Vector3 &b )
{
	Vector3 result;

	// Fancy vector math to reflect the vector
	// http://www.3dkingdoms.com/weekly/weekly.php?a=2
	result = - 2 * DotProduct( a, b ) * b + a;

	return result;
}