// [Vector.h]
#ifndef VECTOR_H
#define VECTOR_H

#include <d3dx9.h>

//-----------------------------------------------------------------------------
// Vector [3D]
//-----------------------------------------------------------------------------
struct Vector3
{
	//Vector3( float _x, float _y, float _z )
	//	: x(_x), y(_y), z(_z) {}				// x = _x, y = _y, z = _z, SO FAST!
	
	Vector3()		{}

	float			x, y, z;

	float			GetLength();
	void			Normalise();
	D3DXVECTOR3		DXVector3();
};

//-----------------------------------------------------------------------------
// Vector Functions
//-----------------------------------------------------------------------------
Vector3				operator +( const Vector3 &a, const Vector3 &b );
Vector3				operator -( const Vector3 &a, const Vector3 &b );
Vector3				operator *( const float &a, const Vector3 &b );
float				DotProduct( const Vector3 &a, const Vector3 &b );
Vector3				CrossProduct( const Vector3 &a, const Vector3 &b );
float				GetAngle( const Vector3 &a, const Vector3 &b );

#endif // VECTOR_H