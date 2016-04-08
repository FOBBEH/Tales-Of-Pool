// [GameObjects.h]
// Generic game object class for greatest walrus
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3dx9.h>
#include <vector>			// Gets rid of C4996 wut???

#include "Vector.h"
#include "Mesh.h"
#include "MatrixStack.h"

struct GameObject
{
	GameObject() {}
	virtual ~GameObject() {}
	
	Vector3					m_position;	// x, y, z
	float					m_roll, m_pitch, m_yaw;
	bool					m_isActive;

	Mesh *					m_mesh;

	virtual void			Update() = 0;
	virtual void			SetPosition() = 0;
	virtual void			SetRotation() = 0;
	virtual void			Render( LPDIRECT3DDEVICE9 renderDevice );
};

#endif // GAMEOBJECT_H