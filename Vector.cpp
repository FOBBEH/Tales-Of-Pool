#include <iostream>
#include <math.h>
using namespace std;

#include "vector.h"

//-----------------------------------------------------------------------------
// Vector Functions
//-----------------------------------------------------------------------------
float Vector3::GetLength()
{
	return sqrt( x*x + y*y + z*z );
}

void Vector3::Normalise()
{
	float length = GetLength();

	if ( length > 0.0f )
	{
		x /= length;
		y /= length;
		z /= length;
	}
}

D3DXVECTOR3	Vector3::DXVector3()
{
	D3DXVECTOR3 result;

	result.x = x;
	result.y = y;
	result.z = z;

	return result;
}

Vector3 operator +( const Vector3 &a, const Vector3 &b )
{
	Vector3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

Vector3 operator -( const Vector3 &a, const Vector3 &b )
{
	Vector3 result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

Vector3	operator *( const float &a, const Vector3 &b )
{
	Vector3 result;

	result.x = a * b.x;
	result.y = a * b.y;
	result.z = a * b.z;

	return result;
}

float DotProduct( const Vector3 &a, const Vector3 &b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 CrossProduct( const Vector3 &a, const Vector3 &b )
{
	Vector3 result;

	result.x = ( a.y * b.z ) - ( a.z * b.y );
	result.y = -( ( a.x * b.z ) - ( a.z * b.x ) );
	result.z = ( a.x * b.y ) - ( a.y * b.x );

	return result;
}

float GetAngle( Vector3 &a, Vector3 &b )
{
	float lengthA = a.GetLength();
	float lengthB = b.GetLength();
	float dot = DotProduct( a, b );

	return acos( dot / (lengthA * lengthB) );
}