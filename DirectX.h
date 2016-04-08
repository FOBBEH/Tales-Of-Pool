// [DirectX.h]
// Singleton class to create DirectX variables
#ifndef DIRECTX_H
#define DIRECTX_H

#include <d3dx9.h>

class DirectX
{
public:
	static DirectX &			Get();
	static void					Delete();
	void						InitialiseDirectX( HWND hWnd );
	void						Cleanup();

	// Functions to access the private variables:
	LPDIRECT3D9					GetD3DDevice()		{ return m_D3DDevice; }
	LPDIRECT3DDEVICE9			GetRenderDevice()	{ return m_renderDevice; }

	void						SetHWND( HWND hWnd );

private:
	static DirectX *			m_instance;

	LPDIRECT3D9					m_D3DDevice;		// Used to create the D3DDevice
	LPDIRECT3DDEVICE9			m_renderDevice;		// Our rendering device

	D3DPRESENT_PARAMETERS		m_D3DParameters;	// Parameters for the render device
};

// Resolution values
#define WIDTH 1280
#define HEIGHT 720

#endif // DIRECTX_H