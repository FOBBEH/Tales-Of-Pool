#include "FrustumCulling.h"

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
Result SphereToPlane(const Sphere& sphere, const Plane& plane)
{
	float distance = D3DXVec3Dot( &plane.normal, &sphere.position );
	
	if (distance > plane.distance + sphere.radius)
	{
		return Result_InFront;
	}
	else if (distance < plane.distance - sphere.radius)
	{
		return Result_Behind;
	}

	return Result_Intersecting;
}

// Assumes frustum plane normals face *outward*
S2FResult SphereToFrustum(const Sphere& sphere, const Frustum& frustum)
{
	S2FResult result = S2FResult_Inside;
	
	for (int i = 0; i < Frustum::Plane_Count; i++)
	{
		Result spherePlane = SphereToPlane(sphere, frustum.planes[i]);
		switch (spherePlane)
		{
		case Result_InFront:
			return S2FResult_Outside;
		case Result_Intersecting:
			result = S2FResult_Intersecting;
		default:
			break;
		}
	}
	
	return result;
}

// Takes model * view * projection as input
void MatrixToFrustum(const Matrix4x4 &viewProj, Frustum &frustum)
{
	// The viewProj matrix converts world to projection space
	// Therefore the inverse will convert from projection space to world space
	//Matrix invVP = viewProj.Invert();
	Matrix4x4 invVP;
	D3DXMatrixInverse( &invVP, NULL, &viewProj );
	
	//-----------------------------------------------------------------------------
	// Near plane
	//-----------------------------------------------------------------------------
	Vector4 nearNormal			( 0,0,1,0 );
	Vector4 nearPosition		( 0,0,0,1 );
	
	// Transform to world space
	//nearNormal = invVP * nearNormal;
	//nearPosition = invVP * nearPosition;
	D3DXVec4Transform			( &nearNormal,		&nearNormal,		&invVP );
	D3DXVec4Transform			( &nearPosition,	&nearPosition,		&invVP );
	
	frustum.planes[Frustum::Plane_Near].normal =		D3DXVECTOR3( nearNormal.x, nearNormal.y, nearNormal.z );
	
	D3DXVECTOR3 nearPosition3	( nearPosition.x, nearPosition.y, nearPosition.z );
	D3DXVECTOR3 nearNormal3		( nearNormal.x, nearNormal.y, nearNormal.z );
	
	frustum.planes[Frustum::Plane_Near].distance =		D3DXVec3Dot( &nearPosition3, &nearNormal3 );

	//-----------------------------------------------------------------------------
	// Far plane
	//-----------------------------------------------------------------------------
	Vector4 farNormal			( 0,0,-1,0 );
	Vector4 farPosition			( 0,0,150,0 );

	D3DXVec4Transform			( &farNormal,		&farNormal,			&invVP );
	D3DXVec4Transform			( &farPosition,		&farPosition,		&invVP );
	
	frustum.planes[Frustum::Plane_Far].normal =			D3DXVECTOR3( farNormal.x, farNormal.y, farNormal.z );
	
	D3DXVECTOR3 farPosition3	( farPosition.x,	farPosition.y,		farPosition.z );
	D3DXVECTOR3 farNormal3		( farNormal.x,		farNormal.y,		farNormal.z );
	
	frustum.planes[Frustum::Plane_Far].distance =		D3DXVec3Dot( &farPosition3, &farNormal3 );

	//-----------------------------------------------------------------------------
	// Top plane
	//-----------------------------------------------------------------------------
	Vector4 topNormal			( 0,-1,0,0 );
	Vector4 topPosition			( 0,1,0,1 );
	
	D3DXVec4Transform			( &topNormal,		&topNormal,			&invVP );
	D3DXVec4Transform			( &topPosition,		&topPosition,		&invVP );
	
	frustum.planes[Frustum::Plane_Top].normal =			D3DXVECTOR3( topNormal.x, topNormal.y, topNormal.z );
	
	D3DXVECTOR3 topPosition3	( topPosition.x,	topPosition.y,		topPosition.z );
	D3DXVECTOR3 topNormal3		( topNormal.x,		topNormal.y,		topNormal.z );
	
	frustum.planes[Frustum::Plane_Top].distance =		D3DXVec3Dot( &topPosition3, &topNormal3 );

	//-----------------------------------------------------------------------------
	// Bottom plane
	//-----------------------------------------------------------------------------
	Vector4 bottomNormal		( 0,1,0,0 );
	Vector4 bottomPosition		( 0,-1,0,1 );
	
	D3DXVec4Transform			( &bottomNormal,	&bottomNormal,		&invVP );
	D3DXVec4Transform			( &bottomPosition,	&bottomPosition,	&invVP );
	
	frustum.planes[Frustum::Plane_Bottom].normal =		D3DXVECTOR3( bottomNormal.x, bottomNormal.y, bottomNormal.z );
	
	D3DXVECTOR3 bottomPosition3	( bottomPosition.x,	bottomPosition.y,	bottomPosition.z );
	D3DXVECTOR3 bottomNormal3	( bottomNormal.x,	bottomNormal.y,		bottomNormal.z );
	
	frustum.planes[Frustum::Plane_Bottom].distance =	D3DXVec3Dot( &bottomPosition3, &bottomNormal3 );

	//-----------------------------------------------------------------------------
	// Left plane
	//-----------------------------------------------------------------------------
	Vector4 leftNormal			( 1,0,0,0 );
	Vector4 leftPosition		( -1,0,0,1 );
	
	D3DXVec4Transform			( &leftNormal,		&leftNormal,		&invVP );
	D3DXVec4Transform			( &leftPosition,	&leftPosition,		&invVP );
	
	frustum.planes[Frustum::Plane_Left].normal =	D3DXVECTOR3( leftNormal.x, leftNormal.y, leftNormal.z );
	
	D3DXVECTOR3 leftPosition3	( leftPosition.x,	leftPosition.y,		leftPosition.z );
	D3DXVECTOR3 leftNormal3		( leftNormal.x,		leftNormal.y,		leftNormal.z );
	
	frustum.planes[Frustum::Plane_Left].distance =	D3DXVec3Dot( &leftPosition3, &leftNormal3 );

	//-----------------------------------------------------------------------------
	// Right plane
	//-----------------------------------------------------------------------------
	Vector4 rightNormal			( -1,0,0,0 );
	Vector4 rightPosition		( 1,0,0,1 );
	
	D3DXVec4Transform			( &rightNormal,		&rightNormal,		&invVP );
	D3DXVec4Transform			( &rightPosition,	&rightPosition,		&invVP );
	
	frustum.planes[Frustum::Plane_Right].normal =	D3DXVECTOR3( rightNormal.x, rightNormal.y, rightNormal.z );
	
	D3DXVECTOR3 rightPosition3	( rightPosition.x,	rightPosition.y,	rightPosition.z );
	D3DXVECTOR3 rightNormal3	( rightNormal.x,	rightNormal.y,		rightNormal.z );
	
	frustum.planes[Frustum::Plane_Right].distance =	D3DXVec3Dot( &rightPosition3, &rightNormal3 );
}