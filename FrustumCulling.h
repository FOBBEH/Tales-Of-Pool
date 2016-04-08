// [FrustumCulling.h]
// Everything for sphere to plane frustum culling
#ifndef FRUSTUMCULLING_H
#define FRUSTUMCULLING_H

#include <d3dx9.h>

//-----------------------------------------------------------------------------
// Typedef
//-----------------------------------------------------------------------------
typedef D3DXVECTOR4		Vector4;
typedef D3DXMATRIXA16	Matrix4x4;

//-----------------------------------------------------------------------------
// Plane definition
//-----------------------------------------------------------------------------
struct Plane
{
	D3DXVECTOR3	normal;   // n
	float		distance; // d
};

//-----------------------------------------------------------------------------
// Frustum definition
//-----------------------------------------------------------------------------
struct Frustum
{
	enum Planes
	{
		Plane_Near,
		Plane_Far,
		Plane_Top,
		Plane_Bottom,
		Plane_Left,
		Plane_Right,
		
		Plane_Count
	};
	
	Plane planes[Plane_Count];
};

//-----------------------------------------------------------------------------
// Sphere definition
//-----------------------------------------------------------------------------
struct Sphere
{
	D3DXVECTOR3	position;
	float		radius;
};

//-----------------------------------------------------------------------------
// Enumeration
//-----------------------------------------------------------------------------
enum Result
{
	Result_InFront,
	Result_Behind,
	Result_Intersecting,
	
	Result_Count
};
 
enum S2FResult
{
	S2FResult_Outside,
	S2FResult_Inside,
	S2FResult_Intersecting,
	
	S2FResult_Count
};

//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
Result SphereToPlane(const Sphere& sphere, const Plane& plane);
S2FResult SphereToFrustum(const Sphere& sphere, const Frustum& frustum);
void MatrixToFrustum(const Matrix4x4& viewProj, Frustum& frustum);

#endif // FRUSTUMCULLING_H