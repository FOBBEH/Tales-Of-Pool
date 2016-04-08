#include "GameObjects.h"

//-----------------------------------------------------------------------------
// Class: GameObject 
//-----------------------------------------------------------------------------
void GameObject::Render( LPDIRECT3DDEVICE9 renderDevice )
{
	D3DXMATRIXA16 matFinal, matPos, matYaw, matPitch, matRoll;

	// Make matrices:
	D3DXMatrixTranslation( &matPos, m_position.x, m_position.y, m_position.z );
	D3DXMatrixRotationX( &matPitch, m_pitch );
	D3DXMatrixRotationY( &matYaw, m_yaw );
	D3DXMatrixRotationZ( &matRoll, m_roll );

	// Multiply matrices:
	matFinal = matRoll * matPitch * matYaw * matPos;

	// Push to matrix stack:
	g_matrixStack.Push( matFinal );

	// Set world matrix in directX:
	renderDevice->SetTransform( D3DTS_WORLD, &g_matrixStack.m_stack.top() );

	// Render:
	m_mesh->Render( renderDevice );

	// Pop matrix from matrix stack:
	g_matrixStack.Pop();
}