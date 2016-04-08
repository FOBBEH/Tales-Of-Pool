// [UI.h]
// User interface n shit
#ifndef UI_H
#define UI_H

#include <map>
#include <string>
#include "DirectX.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class Button;

// Map of strings to button pointers
typedef std::map<std::string, Button*> ButtonMap;

// Pointer to a function
typedef void ( *VoidFunction )();


//-----------------------------------------------------------------------------
// Classes
//-----------------------------------------------------------------------------
class UI
{
public:
	static void					AddButton( std::string, Button* );
	static bool					MsgProc( UINT msg, WPARAM wParam, LPARAM lParam );
	static void					Render( LPDIRECT3DDEVICE9 renderDevice );

private:
	static ButtonMap			m_buttons;
	static Button *				m_active;
};


class Button
{
public:
	Button( LPDIRECT3DDEVICE9 renderDevice, float _x, float _y, float _w, float _h, std::wstring filename );
	~Button();
	
	void						PositionMe();

	bool						MsgProc( UINT msg, WPARAM wParam, LPARAM lParam );
	void						Render( LPDIRECT3DDEVICE9 renderDevice );

	float						x, y, w, h;
	VoidFunction				onPress;
	LPDIRECT3DTEXTURE9			m_texture;

private:
	LPDIRECT3DVERTEXBUFFER9		m_vertexBuffer;
};


struct ButtonVertex
{
	float						x, y, z;
	float						tu, tv;
};

#define ButtonFVF (D3DFVF_XYZ|D3DFVF_TEX1)

#endif // UI_H