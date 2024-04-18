#include <dinput.h>

LPDIRECTINPUT8       g_pDI    = NULL;         
LPDIRECTINPUTDEVICE8 g_pMouse = NULL;
LPDIRECTINPUTDEVICE8 g_pKeyb = NULL;
BYTE    diks[256];
DIMOUSESTATE2 dims2;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

void FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pMouse ) 
        g_pMouse->Unacquire();
    if( g_pKeyb )
		g_pKeyb->Unacquire();

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pMouse );
	SAFE_RELEASE( g_pKeyb );
    SAFE_RELEASE( g_pDI );
}

HRESULT CreateInputDevice(HWND hWnd)
{
HRESULT hr;
BOOL bExclusive=true;
BOOL bForeground=false;
DWORD dwCoopFlags;

	FreeDirectInput();
	if( bExclusive ) dwCoopFlags = DISCL_EXCLUSIVE;
		else dwCoopFlags = DISCL_NONEXCLUSIVE;

    if( bForeground ) dwCoopFlags |= DISCL_FOREGROUND;
		else dwCoopFlags |= DISCL_BACKGROUND;

	// Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return hr;

	// Obtain an interface to the system mouse device.
    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) )
        return hr;
	if( FAILED( hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_pKeyb, NULL ) ) )
        return hr;
	if( FAILED( hr = g_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
        return hr;
	if( FAILED( hr = g_pKeyb->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;
	if( FAILED( hr = g_pMouse->SetCooperativeLevel( hWnd, DISCL_FOREGROUND|DISCL_EXCLUSIVE ) ) )
		return hr;
	if( FAILED( hr = g_pKeyb->SetCooperativeLevel( hWnd, dwCoopFlags ) ) )
		return hr;
	g_pMouse->Acquire();
	g_pKeyb->Acquire();
}

HRESULT InputData(HWND hWnd)
{
HRESULT hr;
   ZeroMemory( &diks, sizeof(diks) );
    hr = g_pKeyb->GetDeviceState( sizeof(diks), &diks );
    if( FAILED(hr) ) 
    {        
        hr = g_pKeyb->Acquire();
        while( hr == DIERR_INPUTLOST ) hr = g_pKeyb->Acquire();
        if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
		{MessageBox(hWnd,"Error: Keyboard unacquired!!!","Error",0);}
        //return S_OK; 
    }
ZeroMemory( &dims2, sizeof(dims2) );
hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );
    if( FAILED(hr) ) 
    {
        hr = g_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) hr = g_pMouse->Acquire();

//        if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) {}
    }
return S_OK; 
}