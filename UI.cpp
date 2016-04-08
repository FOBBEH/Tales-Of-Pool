#include "UI.h"

#include <WindowsX.h>

//-----------------------------------------------------------------------------
// Class: UI
//-----------------------------------------------------------------------------
void UI::AddButton( std::string buttonName, Button* button )
{
	m_buttons[ buttonName ] = button;
}

bool UI::MsgProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	for ( ButtonMap::iterator i = m_buttons.begin();
		  i != m_buttons.end(); ++i )
	{
		bool handled = (*i).second->MsgProc( msg, wParam, lParam );

		if (handled) return true;
	}

	return false;
}

void UI::Render( LPDIRECT3DDEVICE9 renderDevice )
{
	for ( ButtonMap::iterator i = m_buttons.begin();
		  i != m_buttons.end(); ++i )
	{
		(*i).second->Render( renderDevice );
	}
}

// Need to declare static variables in the cpp file
ButtonMap					UI::m_buttons;
Button *					UI::m_active = NULL;


//-----------------------------------------------------------------------------
// Class: Button
//-----------------------------------------------------------------------------
Button::Button( LPDIRECT3DDEVICE9 renderDevice, float _x, float _y, float _w, float _h, std::wstring filename )
	: x(_x), y(_y), w(_w), h(_h), onPress(NULL), m_vertexBuffer(NULL)
{
	renderDevice->CreateVertexBuffer( 4 * sizeof( ButtonVertex ), 
									  0, 
									  ButtonFVF, 
									  D3DPOOL_DEFAULT, 
									  &m_vertexBuffer, 
									  NULL );
	PositionMe();

	D3DXCreateTextureFromFile( renderDevice, filename.c_str(), &m_texture );
}


void Button::PositionMe()
{
	ButtonVertex *vertexData;
	m_vertexBuffer->Lock( 0,
						  4 * sizeof( ButtonVertex ),
						  (VOID**)&vertexData,
						  0 );
	{
		vertexData[0].x = x;
		vertexData[0].y = y;
		vertexData[0].z = 0.0f;

		vertexData[0].tu = 0.0f;
		vertexData[0].tv = 0.0f;

		vertexData[1].x = x;
		vertexData[1].y = y + h;
		vertexData[1].z = 0.0f;

		vertexData[1].tu = 0.0f;
		vertexData[1].tv = 1.0f;

		vertexData[2].x = x + w;
		vertexData[2].y = y;
		vertexData[2].z = 0.0f;

		vertexData[2].tu = 1.0f;
		vertexData[2].tv = 0.0f;

		vertexData[3].x = x + w;
		vertexData[3].y = y + h;
		vertexData[3].z = 0.0f;

		vertexData[3].tu = 1.0f;
		vertexData[3].tv = 1.0f;
	}
	m_vertexBuffer->Unlock();
}

Button::~Button()
{
	if ( m_vertexBuffer != NULL )
		 m_vertexBuffer->Release();
	if ( m_texture != NULL )
		 m_texture->Release();
}
	
bool Button::MsgProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	int mouseX = GET_X_LPARAM( lParam );
	int mouseY = GET_Y_LPARAM( lParam );

	if ( mouseX > x && mouseX < x + w && 
		 mouseY > y && mouseY < y + h )
	{
		if ( msg == WM_LBUTTONDOWN )
		{
			if ( onPress != NULL )
			{
				onPress();
				return true;
			}
		}
	}

	return false;
}

void Button::Render( LPDIRECT3DDEVICE9 renderDevice )
{
	// Set our matrices
	D3DXMATRIXA16 matIdentity, matScale, matOffset;
	D3DXMatrixIdentity( &matIdentity );

	D3DXMatrixScaling( &matScale, 2.0f / (FLOAT)WIDTH, -2.0f / (FLOAT)HEIGHT, 1.0f );
	D3DXMatrixTranslation( &matOffset, -1, 1, 0 );
	D3DXMatrixMultiply( &matScale, &matScale, &matOffset );

	renderDevice->SetTransform( D3DTS_WORLD,		&matIdentity );
	renderDevice->SetTransform( D3DTS_VIEW,			&matIdentity );
	renderDevice->SetTransform( D3DTS_PROJECTION,	&matScale );

	// Render
	renderDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	// Never cull the back face of a triangle
	renderDevice->SetFVF( ButtonFVF );
	renderDevice->SetStreamSource( 0,
								   m_vertexBuffer,
								   0,
								   sizeof( ButtonVertex ) );
	renderDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,
								 0, 
								 2 );
	renderDevice->SetTexture( 0, m_texture );
}