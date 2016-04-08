#include "DirectX.h"

#include <assert.h>

DirectX *DirectX::m_instance = NULL;

DirectX &DirectX::Get()
{
	if ( m_instance == NULL )
	{
		m_instance = new DirectX;
	}

	return *m_instance;
}

void DirectX::Delete()
{
	delete m_instance;
	m_instance = NULL;
}

void DirectX::InitialiseDirectX( HWND hWnd )
{
	// Create the D3D object
	m_D3DDevice = Direct3DCreate9( D3D_SDK_VERSION );

	// Create the D3D/render device
	ZeroMemory( &m_D3DParameters, sizeof( m_D3DParameters ) );
    m_D3DParameters.Windowed = TRUE;
    m_D3DParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_D3DParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    m_D3DParameters.EnableAutoDepthStencil = TRUE;
    m_D3DParameters.AutoDepthStencilFormat = D3DFMT_D16;
	
	m_D3DDevice->CreateDevice( D3DADAPTER_DEFAULT,
							   D3DDEVTYPE_HAL,
							   hWnd,
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							   &m_D3DParameters,
							   &m_renderDevice );

	// Turn off culling
    m_renderDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn on the zbuffer
    m_renderDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	// Assert checks
	assert( m_D3DDevice != NULL );
}

void DirectX::Cleanup()
{
	if ( m_D3DDevice != NULL )
		 m_D3DDevice->Release();

	if ( m_renderDevice != NULL )
		 m_renderDevice->Release();
}