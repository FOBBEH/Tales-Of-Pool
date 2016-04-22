#include <WindowsX.h>
#include <d3dx9.h>

#include <assert.h>
#include <time.h>
#include "..\FMOD\fmod.hpp"
#include "..\FMOD\common.h"

#include "DirectX.h"
#include "Physics.h"
#include "PoolObjects.h"
#include "FrustumCulling.h"
#include "UI.h"




//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
HWND						hWnd =			NULL;
float						startTime =		(FLOAT)timeGetTime();

// Game settings
bool						finished =		false;
bool						firstTurn =		true;
bool						moveCueBall =	true;
bool						hitCueBall =	true;
bool						p1Turn =		true;
bool						p2Turn =		false;

// Camera settings
float						cameraX =		0.0f;
float						cameraY	=		140.0f;
float						cameraZ =		-45.0f;

// FMOD sound settings
FMOD::System *				g_system;
FMOD::Sound *				g_sound1;
FMOD::Channel *				g_channel = 0;
FMOD_RESULT					g_result;
void *						g_extradriverdata = 0;

// Mesh for all the balls + pool table
Mesh *						g_mesh[NUM_BALLS + 1];

// UI
Button *					g_UI[5];

// Vector template to keep track of everything
std::vector<GameObject*>	ObjectList;
std::vector<Ball*>			BallList;
std::vector<Ball*>			P1BallList;
std::vector<Ball*>			P2BallList;

// Matrix stack, can't remember its use HEHEHEH
MatrixStack					g_matrixStack;

// Matrices to be accessed during frustum culling stage
D3DXMATRIXA16				matView, matProj;

// DirectX::m_renderDevice (condense the code down!)
inline LPDIRECT3DDEVICE9	RenderDevice()	{ return DirectX::Get().GetRenderDevice(); }


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
void						Initialise();
void						InitialiseBalls();
void						InitialiseUI();
void						SetupMatrices();
void						SetupTalesOfPool();
void						UpdateInput();
void						UpdateGame();
void						CameraDefault();
void						Render();
LRESULT WINAPI				HandleWindowsEvents( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void						RunGame();
void						Cleanup();
int							wWinMain( HINSTANCE, HINSTANCE, LPWSTR, INT );

// Register the window class, uses HandeWindowsEvents!
WNDCLASSEX wc =
{
    sizeof( WNDCLASSEX ), CS_CLASSDC, HandleWindowsEvents, 0L, 0L,
    GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
    L"GAM203/GAM204 ASSIGNMENT 2", NULL
};

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    D3DXVECTOR3 position;				// The position
    D3DCOLOR color;						// The color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX	(D3DFVF_XYZ|D3DFVF_DIFFUSE)


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void Initialise()
{
	// Create a window:
	// Register the window class
	RegisterClassEx( &wc );

	// Create the application's window
	hWnd = CreateWindow( L"GAM203/GAM204 ASSIGNMENT 2", 
						 L"Tales Of Pool: SPEED GAMING FUCKING WON EE-SAMA NO MERCY",
                         WS_OVERLAPPEDWINDOW, 100, 100, WIDTH, HEIGHT,
                         NULL, NULL, wc.hInstance, NULL );

	// Create DirectX:
	// Create the D3D object
	DirectX::Get().InitialiseDirectX( hWnd );

	// Create the balls
	InitialiseObjects();

	// Create the UI
	InitialiseUI();

	// Create the ball meshes
	// If the first ball isn't assigned, assign them all for initialisation stage
	// ie. Skip initialisation for replays
	if ( g_mesh[0] == NULL )
	{
		g_mesh[0]	= new Mesh( L"assets\\meshes\\white.x",			RenderDevice() );
		g_mesh[1]	= new Mesh( L"assets\\meshes\\black.x",			RenderDevice() );
		g_mesh[2]	= new Mesh( L"assets\\meshes\\solidyellow.x",	RenderDevice() );
		g_mesh[3]	= new Mesh( L"assets\\meshes\\stripeyellow.x",	RenderDevice() );
		g_mesh[4]	= new Mesh( L"assets\\meshes\\solidblue.x",		RenderDevice() );
		g_mesh[5]	= new Mesh( L"assets\\meshes\\stripeblue.x",	RenderDevice() );
		g_mesh[6]	= new Mesh( L"assets\\meshes\\solidred.x",		RenderDevice() );
		g_mesh[7]	= new Mesh( L"assets\\meshes\\stripered.x",		RenderDevice() );
		g_mesh[8]	= new Mesh( L"assets\\meshes\\solidpurple.x",	RenderDevice() );
		g_mesh[9]	= new Mesh( L"assets\\meshes\\stripepurple.x",	RenderDevice() );
		g_mesh[10]	= new Mesh( L"assets\\meshes\\solidorange.x",	RenderDevice() );
		g_mesh[11]	= new Mesh( L"assets\\meshes\\stripeorange.x",	RenderDevice() );
		g_mesh[12]	= new Mesh( L"assets\\meshes\\solidgreen.x",	RenderDevice() );
		g_mesh[13]	= new Mesh( L"assets\\meshes\\stripegreen.x",	RenderDevice() );
		g_mesh[14]	= new Mesh( L"assets\\meshes\\soliddred.x",		RenderDevice() );
		g_mesh[15]	= new Mesh( L"assets\\meshes\\stripedred.x",	RenderDevice() );
		g_mesh[16]	= new Mesh( L"assets\\meshes\\pooltable.x",		RenderDevice() );
	}

	// Assign the mesh for the pool table
	for (int i = 16; i < NUM_TABLES + 16; ++i)
	{
		ObjectList[i]->m_mesh = g_mesh[16];
		ObjectList[i]->m_isActive = true;
	}

	// Set the meshes to the balls
	InitialiseBalls();

	// Create a System object and initialise
	g_result = FMOD::System_Create(&g_system);
	ERRCHECK(g_result);

	g_result = g_system->init(32, FMOD_INIT_NORMAL, g_extradriverdata);
    ERRCHECK(g_result);

	g_result = g_system->createSound(Common_MediaPath("theme.mp3"), FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &g_sound1);
    ERRCHECK(g_result);

	// Play music
	g_result = g_system->playSound(g_sound1, 0, false, &g_channel);
}


// Easy to reset the game with this function!
void InitialiseBalls()
{
	// Assign the white and black balls:
	// White ball: 1st ball in the list
	BallList[0]->m_mesh = g_mesh[0];
	BallList[0]->m_isActive = true;
	// Black ball: 6th ball in the list
	BallList[5]->m_mesh = g_mesh[1];
	BallList[5]->m_isActive = true;
	
	int selector = NULL;
	srand( (UINT)time(NULL) );

	// Assign the rest of the balls randomly
	for (int i = 2; i < NUM_BALLS; ++i)		// i starts at 2, previous ball mesh has been assigned
	{
		do
		{
			selector = rand() % 15 + 1;		// 1 to 15, ObjectList[0] already assigned to white ball
											// Even though ObjectList[5] is assigned, too lazy
			
			// If the ball hasn't been activated, assign one of the ball meshes
			if ( !BallList[selector]->m_isActive )
			{
				if ( i % 2 == 0 )
				{
					BallList[selector]->m_solids = true;
				}
				else if ( i % 2 == 1 )
				{
					BallList[selector]->m_stripes = true;
				}
				BallList[selector]->m_mesh = g_mesh[i];
				BallList[selector]->m_isActive = true;
				break;
			}
		} while ( BallList[selector]->m_isActive );
	}

	//-----------------------------------------------------------------------------
	// ALTERNATIVE RANDOMISER FOR BALLS
	//-----------------------------------------------------------------------------
	//Chris' (AKA GOD OF PROGRAMMING) special flavour magic
	/*std::vector<D3DXVECTOR3> positionsToBeUsed;
	std::vector<D3DXVECTOR3> goodPositions;

	for(int i = 0; i < 14; ++i)
	{
		D3DXVECTOR3 pos = {1.0f*i, 0.0f, 0.0f};
		positionsToBeUsed.push_back(pos);
	}

	while(positionsToBeUsed.size() > 0)
	{
		//Assign the good position
		unsigned int selector = (rand() % postionsToBeUsed.size());
		goodPositions.push_back(positionsToBeUsed[selector]);

		//Remove the position that was just used
		positionsToBeUsed[i] = positionToBeUsed.back();
		positionsToBeUsed.pop_back();
	}
	
	//Assign the positions
	for(int i = 0; i < 14; ++i)
	{
		ObjectList[i].m_position = goodPositions[i];
	}*/
}


void InitialiseUI()
{
	// Make buttons (takes texture from previous UI button???)
	g_UI[0] = new Button( RenderDevice(), -1.0f, -1.0f, 1.0f, 1.0f, L"assets\\textures\\mainbar.png" );
	g_UI[0]->onPress = NULL;
	UI::AddButton( "DUD BUTTON", g_UI[0] );

	g_UI[1] = new Button( RenderDevice(), 780.0f, 0.0f, 500.0f, 50.0f, L"assets\\textures\\menu.png" );
	g_UI[1]->onPress = NULL;
	UI::AddButton( "Main bar", g_UI[1] );

	g_UI[2] = new Button( RenderDevice(), 0.0f, 0.0f, 130.0f, 50.0f, L"assets\\textures\\player1.png" );
	g_UI[2]->onPress = NULL;
	UI::AddButton( "Menu", g_UI[2] );

	g_UI[3] = new Button( RenderDevice(), 1080.0f, 0.0f, 100.0f, 50.0f, L"assets\\textures\\player2.png" );
	g_UI[3]->onPress = NULL;
	UI::AddButton( "Player 1", g_UI[3] );

	g_UI[3] = new Button( RenderDevice(), 1180.0f, 0.0f, 100.0f, 50.0f, L"assets\\textures\\player2.png" );
	g_UI[3]->onPress = NULL;
	UI::AddButton( "Player 2", g_UI[3] );
}


void SetupMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	RenderDevice()->SetTransform( D3DTS_WORLD, &matWorld );
	
	// Using global variables to move the camera around
	D3DXVECTOR3 vEyePt( cameraX, cameraY, cameraZ );
	D3DXVECTOR3 vLookatPt( cameraX, -1.0f, cameraZ );		// LookatPt has to move with the camera
	D3DXVECTOR3 vUpVec( -1.0f, 0.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	RenderDevice()->SetTransform( D3DTS_VIEW, &matView );
	
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, (FLOAT)WIDTH / (FLOAT)HEIGHT, 1.0f, 650.0f );
	RenderDevice()->SetTransform( D3DTS_PROJECTION, &matProj );
}


void SetupTalesOfPool()
{
    // Set up material
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    RenderDevice()->SetMaterial( &mtrl );

    // Set up spotlight
    D3DXVECTOR3 vecDir;
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
    light.Type =			D3DLIGHT_SPOT;
    light.Diffuse =			D3DXCOLOR	( 1.0f, 1.0f, 1.0f, 0.0f );
	light.Position =		D3DXVECTOR3	( 0.0f, 5.0f, 0.0f );
	light.Direction =		D3DXVECTOR3	( 0.0f, -1.0f, 0.0f );
	light.Specular =		D3DXCOLOR	( 1.0f, 1.0f, 1.0f, 0.0f );
	light.Range =			50.0f;
	light.Attenuation0 =	0.1f;
	light.Attenuation1 =	0.1f;
	light.Attenuation2 =	0.02f;
	light.Phi =				10.0f;
	light.Theta =			90.0f;
	light.Falloff =			1.0f;
	
	// Enable lights
	RenderDevice()->SetLight( 0, &light );
    RenderDevice()->LightEnable( 0, TRUE );
    RenderDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Turn on some ambient light
    RenderDevice()->SetRenderState( D3DRS_AMBIENT, 0x00202020 );
}


void UpdateInput()
{
	// Enter the message loop
    MSG msg;
    ZeroMemory( &msg, sizeof( msg ) );

	// Get message from Windows and handle them
    if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

	// Quit game if the player quits
	if ( msg.message == WM_QUIT )
	{
		finished = true;
	}
}


void UpdateGame()
{
	// Restrict the camera
	if ( cameraY > 600.0f )
	{
		cameraY = 600.0f;
	}
	if ( cameraY < 10.0f )
	{
		cameraY = 10.0f;
	}
}


void CameraDefault()
{
	// Reset camera to original settings
	cameraX = 0.0f;
	cameraY	= 140.0f;
	cameraZ = -45.0f;
}


void Render()
{
	// Clear the backbuffer and the zbuffer
	RenderDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
							D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	// Begin the scene
	if( SUCCEEDED( RenderDevice()->BeginScene() ) )
	{
		// Setup the TalesOfPool and materials
		SetupTalesOfPool();

		// Setup the world, view, and projection matrices
		SetupMatrices();
				
		// Updating once updates them all, too lazy to fix
		// Only have to update the BALLS, not all the OBJECTS
		BallList[0]->Update();

		// Render everything that is still active
		for (int i = 0; i < ObjectList.size(); ++i)
		{
			// Sphere to frustum culling
			Sphere sphere;
			Frustum frustum;

			sphere.position = ObjectList[i]->m_position.DXVector3();
			if ( i >= 0 && i <= NUM_BALLS - 1 )
			{
				sphere.radius = BallList[i]->m_radius;
			}
			else
			{
				sphere.radius = 100.0f;		// MAGIC NUMBERRRRR
			}

			D3DXMATRIXA16 viewProj = matView * matProj;
			MatrixToFrustum( viewProj, frustum );

			bool visible = ( SphereToFrustum( sphere, frustum ) == S2FResult_Outside );
			
			if ( visible )
			{
				if ( ObjectList[i]->m_isActive )
				{
					ObjectList[i]->SetRotation();
					ObjectList[i]->Render( RenderDevice() );
				}
			}
		}

		// If the cue ball has been pocketed, reset it
		if ( !BallList[0]->m_isActive )
		{
			ResetCueBall();
		}

		// Setup and draw UI
		UI::Render( RenderDevice() );

		// End the scene
		RenderDevice()->EndScene();
	}

	// Present the backbuffer contents to the display
	RenderDevice()->Present( NULL, NULL, NULL, NULL );
}


LRESULT WINAPI HandleWindowsEvents( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	bool paused = false;
	
	switch( msg )
    {
		case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
		case WM_KEYDOWN:
			{
				//-----------------------------------------------------------------------------
				// Camera movement
				//-----------------------------------------------------------------------------
				if ( wParam == 'W' )
				{
					cameraX -= 2.0f;
					break;
				}
				if ( wParam == 'S' )
				{
					cameraX += 2.0f;
					break;
				}
				if ( wParam == 'A' )
				{
					cameraZ -= 2.0f;
					break;
				}
				if ( wParam == 'D' )
				{
					cameraZ += 2.0f;
					break;
				}
				if ( wParam == 'R' )
				{
					cameraY -= 2.0f;
					break;
				}
				if ( wParam == 'F' )
				{
					cameraY += 2.0f;
					break;
				}
				if ( wParam == 32 )						// Spacebar
				{
					CameraDefault();
					break;
				}

				//-----------------------------------------------------------------------------
				// Cue ball movement
				//-----------------------------------------------------------------------------
				if ( wParam == 38 && moveCueBall )		// Up arrow
				{
					BallList[0]->m_position.x -= 0.5f;
					break;
				}
				if ( wParam == 40 && moveCueBall )		// Down arrow
				{
					BallList[0]->m_position.x += 0.5f;
					break;
				}

				//-----------------------------------------------------------------------------
				// Sound settings
				//-----------------------------------------------------------------------------
				if ( wParam == 187 )					// Equal
				{
					// Doesn't unpause, just resets the song
					g_result = g_system->playSound(g_sound1, 0, false, &g_channel);
					break;
				}
				if ( wParam == 189 )					// Minus
				{
					g_result = g_channel->setPaused(&paused);
					break;
				}

				if ( wParam == 8 )						// Backspace
				{
					for (int i = 0; i < BallList.size(); ++i)
					{
						BallList[i]->m_isActive = false;
						BallList[i]->m_velocity = 0.0f;
					}
					
					// Reset all the positions
					ResetBalls();

					// Change the mesh/randomise the balls again
					InitialiseBalls();
					break;
				}

				default:
					break;
			}
			return 0;
		case WM_LBUTTONDOWN:
			{
				//-----------------------------------------------------------------------------
				// Hitting the cue ball
				//-----------------------------------------------------------------------------
				// Cursor's position in world space:
				float mouseX = (FLOAT)GET_X_LPARAM( lParam );
				float mouseY = (FLOAT)GET_Y_LPARAM( lParam );
				
				CheckMovement();

				D3DXVECTOR3 cameraPos(cameraX, cameraY, cameraZ);
				HitCueBall( BallList[0], mouseX, mouseY, cameraPos );

				CameraDefault();

				if ( p1Turn && !p2Turn )
				{
					p1Turn = false;
					p2Turn = true;
				}

				else if ( !p1Turn && p2Turn )
				{
					p1Turn = true;
					p2Turn = false;
				}

				moveCueBall = false;
				//g_UI[3] = NULL;

				break;
			}

			return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


void RunGame()
{
	// Show and update window
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	// Main game loop
	while ( !finished )
	{
		UpdateInput();
		UpdateGame();
		Render();
	}
}


void Cleanup()
{
	// Delete and deallocate everything	
	DirectX::Get().Cleanup();
	DirectX::Delete();

	for (int i = 0; i < NUM_BALLS + 1; ++i)
	{
		delete g_mesh[i];
		g_mesh[i] = NULL;
	}

	// Delete all the pointers in ObjectList
	while ( ObjectList.size() > 0 )
	{
		delete ObjectList[ ObjectList.size() - 1 ];
		ObjectList[ ObjectList.size() - 1 ] = NULL;
		ObjectList.pop_back();
	}

	// Delete the UI button pointers
	for (int i = 0; i < 5; ++i)
	{
		delete g_UI[i];
		g_UI[i] = NULL;
	}

	// Delete sound
	g_result = g_sound1->release();
	ERRCHECK(g_result);
	g_result = g_system->close();
	ERRCHECK(g_result);
	g_result = g_system->release();
	ERRCHECK(g_result);

	Common_Close();

	// Assert check
	assert( ObjectList.empty() );

	UnregisterClass( L"GAM203/GAM204 ASSIGNMENT 2", wc.hInstance );
}


//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
int wWinMain( HINSTANCE, HINSTANCE, LPWSTR, INT )
{
    // Initialise the program
	Initialise();

	// Run the game
	RunGame();

	// Terminate
	Cleanup();

    return 0;
}
