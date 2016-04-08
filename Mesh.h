// [Mesh.h]
// Store and draw meshes
#ifndef MESH_H
#define MESH_H

#include <d3dx9.h>
#include <string>
#include <strsafe.h>

struct Mesh
{
	Mesh( std::wstring filename, LPDIRECT3DDEVICE9 directX );
	~Mesh();

	void Render( LPDIRECT3DDEVICE9 directX );

	// Material count
	DWORD					m_numMaterials;

	// Mesh
	LPD3DXMESH				m_mesh;			// Long pointer to mesh

	// Buffer
	D3DMATERIAL9 *			m_materials;
	LPDIRECT3DTEXTURE9 *	m_textures;
};

#endif // MESH_H