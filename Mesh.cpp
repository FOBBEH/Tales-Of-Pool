#include "Mesh.h"

#include <assert.h>

Mesh::Mesh ( std::wstring filename, LPDIRECT3DDEVICE9 directX )
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	D3DXLoadMeshFromX( filename.c_str(), D3DXMESH_SYSTEMMEM,
					   directX, NULL,
					   &pD3DXMtrlBuffer, NULL, &m_numMaterials,
					   &m_mesh );

	// We need to extract the material properties and texture names from the pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
	m_materials = new D3DMATERIAL9[m_numMaterials];
	m_textures = new LPDIRECT3DTEXTURE9[m_numMaterials];
	
	for( DWORD i = 0; i < m_numMaterials; i++ )
	{
		// Copy the material
		m_materials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		m_materials[i].Ambient = m_materials[i].Diffuse;

		m_textures[i] = NULL;
		if( d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
		{
			// Create the texture found in the assets folder
			const CHAR* strPrefix = "assets\\meshes\\";
			CHAR strTexture[MAX_PATH];
			strcpy_s( strTexture, MAX_PATH, strPrefix );
			strcat_s( strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename );
			// If texture is not in current folder, try parent folder
			if( FAILED( D3DXCreateTextureFromFileA( directX,
													strTexture,
													&m_textures[i] ) ) )
			{
				MessageBox( NULL, L"Could not find texture map", L"TalesOfPool.exe", MB_OK );
			}
		}
	}

	// Assert checks
	assert( m_materials != NULL );
	assert( m_textures != NULL );

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
}

Mesh::~Mesh()
{
	delete [] m_materials;		// Delete an array

	for (int i = 0; i < m_numMaterials; ++i)
	{
		m_textures[i]->Release();
	}
	delete [] m_textures;

	m_mesh->Release();
}

void Mesh::Render( LPDIRECT3DDEVICE9 directX )
{
	// Loop over all materials:
	for ( int i = 0; i < m_numMaterials; ++i )
	{
		// Draw this subset of the mesh:
		directX->SetMaterial( &m_materials[i] );
		directX->SetTexture( 0, m_textures[i] );
		m_mesh->DrawSubset( i );
	}
}