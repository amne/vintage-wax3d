// MainFrame.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OGL3.h"

#include "wax3d.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
//	ON_WM_ACTIVATEAPP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
W_Eye* Eye=new W_Eye(Vector3D(0,0,5),Vector3D(0,0,0),Vector3D(0,1,0),60.0f);
TPolygon* poly1=new TPolygon();
TPolygon* poly2=new TPolygon();
TPolygon* poly3=new TPolygon();
CBSPTreeNode* bsp1=new CBSPTreeNode();
vector<TPolygon*> polylist1;
TCollisionInfo tCI;

CMainFrame::CMainFrame()
{
	VB_WIDTH = 640;
	VB_HEIGHT = 480;
	VB_DEPTH  = 32;
	m_bFullScreen = FALSE;
	m_hDC = NULL;
	m_hRC = NULL;
	m_cxClient = 0;
	m_cyClient = 0;
	m_hPal = NULL;
	m_bAppIsActive = FALSE;
}

CMainFrame::~CMainFrame()
{
	KillGLWindow();	// Shutdown	
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame PreCreateWindow

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &m_DMsaved);					// save the current display state 
	cs.cx = VB_WIDTH;
	cs.cy = VB_HEIGHT;
	cs.dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Window Extended Style
	cs.style=WS_OVERLAPPEDWINDOW;										// Windows Style
	cs.y = (int) GetSystemMetrics(SM_CYSCREEN) / 2 - cs.cy / 2; 
	cs.x = (int) GetSystemMetrics(SM_CXSCREEN) / 2 - cs.cx / 2;

	cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		LoadCursor(NULL,IDC_ARROW), NULL, NULL);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnCreateClient



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRet = CFrameWnd::OnCreateClient(lpcs, pContext);
	if (bRet)
	{
		GLuint	PixelFormat;												// Holds The Results After Searching For A Match
		static	PIXELFORMATDESCRIPTOR pfd=									// pfd Tells Windows How We Want Things To Be
		{																		
			sizeof(PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
			1,																// Version Number
			PFD_DRAW_TO_WINDOW |											// Format Must Support Window
			PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,												// Must Support Double Buffering
			PFD_TYPE_RGBA,													// Request An RGBA Format
			VB_DEPTH,														// Select Our Color Depth
			0, 0, 0, 0, 0, 0,												// Color Bits Ignored
			0,																// No Alpha Buffer
			0,																// Shift Bit Ignored
			0,																// No Accumulation Buffer
			0, 0, 0, 0,														// Accumulation Bits Ignored
			16,																// 16Bit Z-Buffer (Depth Buffer)  
			0,																// No Stencil Buffer
			0,																// No Auxiliary Buffer
			PFD_MAIN_PLANE,													// Main Drawing Layer
			0,																// Reserved
			0, 0, 0															// Layer Masks Ignored
		};																		
	
		
		if ( !( m_hDC = ::GetDC ( m_hWnd ) ) )	{							// Did We Get A Device Context?																	
			KillGLWindow ();													// Reset The Display
			MessageBox ( "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;												
		}
																			
		if ( !( PixelFormat = ChoosePixelFormat ( m_hDC, &pfd ) ) )	{		// Did Windows Find A Matching Pixel Format?																		
			KillGLWindow ();												// Reset The Display
			MessageBox ( "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;												
		}
																			
		if ( !SetPixelFormat ( m_hDC, PixelFormat, &pfd ) ){				// Are We Able To Set The Pixel Format?																		
			KillGLWindow ();												// Reset The Display
			MessageBox ( "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;												
		}
																			
		if ( !( m_hRC = wglCreateContext ( m_hDC ) ) ) {					// Are We Able To Get A Rendering Context?																	
			KillGLWindow ();												// Reset The Display
			MessageBox( " Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;													
		}
																			
		if ( !wglMakeCurrent ( m_hDC, m_hRC ) )	{							// Try To Activate The Rendering Context																		
			KillGLWindow ();												// Reset The Display
			MessageBox ( "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;													
		}

		if ( !InitGL () ) {													// Initialize Our Newly Created GL Window																		
			KillGLWindow ();												// Reset The Display
			MessageBox ( "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;													
		}


		m_bAppIsActive = TRUE;
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnPaint

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	// Do not call CMainFrame::OnPaint() for painting messages

	::ValidateRect ( m_hWnd, NULL );
	
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnSize

void CMainFrame::OnSize(UINT, int cx, int cy)
{
	m_cxClient = cx;
	m_cyClient = cy;
	ReSizeGLScene( cx, cy );
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnSetFocus

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	OnQueryNewPalette();
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnCreateClient

BOOL CMainFrame::OnQueryNewPalette()
{
	Invalidate();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnPaletteChanged

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
	if ((pFocusWnd != this) && (!IsChild(pFocusWnd)))
		OnQueryNewPalette();
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnActivateApp

void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask)
{
	CFrameWnd::OnActivateApp(bActive, (DWORD)hTask);
	m_bAppIsActive = bActive;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame RenderGLScene

void CMainFrame::RenderGLScene()
{	
	if (!m_bAppIsActive)
		return;
//	CDynList<Vector3D>::TElemList *t2=poly1->vlist->elemlist->first;	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer
	glLoadIdentity();	

	
	// EXAMPLE GLFONT CODE START ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	int j=0;
	glPushMatrix();
			glDisable(GL_TEXTURE_2D);							// if your App is using Textures disable it before displaying font
			// glDisable(GL_LIGHTING);							// if your App is using lighting disable it before displaying font
			glLoadIdentity();									// Reset The Current Modelview Matrix
			glTranslatef(0.0f,0.0f,-1.0f);						// Move One Unit Into The Screen
			glColor3f(255.0f,255.0f,255.0f);
			glRasterPos2f(-0.95f, 0.65f);
			glPrint("%g : %g  FOV=%g",vpWidth,vpHeight,Eye->dFov);
			glRasterPos2f(-0.95f, 0.60f);
			glPrint("d1=%g",poly1->ClassifyPoint(&Eye->vPos));
			glRasterPos2f(-0.95f, 0.55f);
			glPrint("d2=%g",poly1->ClassifyPoint(&Eye->vLastPos));
			glRasterPos2f(-0.95f, 0.50f);
			glPrint("c[1][2]=%g",poly2->ClassifyPoly(poly1));
			glRasterPos2f(-0.95f, 0.45f);
			glPrint("c[2][1]=%g",poly1->ClassifyPoly(poly2));
			glRasterPos2f(-0.95f, 0.40f);
			glPrint("inarea=%g",poly1->PointInArea(&Eye->vPos));
			/*glRasterPos2f(-0.95f, 0.40f); 
			glPrint("pN.x=%g   pN.y=%g    pN.z=%g",tCI->pN.x,tCI->pN.y,tCI->pN.z);
			glRasterPos2f(-0.95f, 0.35f);
			glPrint("vC.x=%g   vC.y=%g    vC.z=%g",tCI->vC.x,tCI->vC.y,tCI->vC.z);
			glRasterPos2f(-0.95f, 0.30f);			
			glPrint("pS=%g   collision=%d",tCI->pS, tCI->collision);
			//char *text=(char *)malloc(250);
			//sprintf(text,"Collision:%d\nPlane normal: x=%f  y=%f  z=%f",tCI->collision,tCI->pN.x,tCI->pN.y,tCI->pN.z);
			//if(tCI->collision){MessageBox(text);}
			//free(text);*/
			glEnable(GL_TEXTURE_2D);							// if your App is using Textures enable it after displaying font
			// glEnable(GL_LIGHTING);							// if your App is using lighting enable it after displaying font		
	glPopMatrix();	

	// EXAMPLE GLFONT CODE END ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	// EXAMPLE TEXTURE CODE START ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	glPushMatrix();															// Push Matrix Onto Stack (Copy The Current Matrix)
		glLoadIdentity();													// Reset The Current Modelview Matrix
		//glTranslatef(0.0f,0.0f,-6.0f);										// Move Left 1.5 Units And Into The Screen 6.0
		//glRotatef(1,1.0f,0.0f,0.0f);
		//glRotatef(1,0.0f,1.0f,0.0f);
		//glRotatef(1,0.0f,0.0f,1.0f);
		Eye->vLastPos=Eye->vPos;
		Eye->DoAction(m_hWnd);

		bsp1->CollisionTest(&Eye->vLastPos,&Eye->vPos, &tCI);
		if(tCI.collision)
		{
			tCI.vC.project(&(tCI.pN),tCI.pS);
			tCI.vC.unitize();
			Eye->vPos=Eye->vLastPos+tCI.vC*abs(tCI.collision);
			//Eye->vPos=tCI->vC+tCI->pN*tCI->collision;
			Eye->CalcFocus();
			Eye->LookAt(Eye->vPos.x+Eye->vFocus.x,Eye->vPos.y+Eye->vFocus.y,Eye->vPos.z+Eye->vFocus.z);
		}

		gluLookAt(Eye->vPos.x,Eye->vPos.y,Eye->vPos.z,Eye->vLookAt.x,Eye->vLookAt.y,Eye->vLookAt.z,Eye->vUp.x,Eye->vUp.y,Eye->vUp.z);
		
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);			// Keep the font color from coloring the textures

		// Front Face
		m_appTextures.Use("FRONT");
		glBegin(GL_QUADS);
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  -1.0f);
		glEnd();
		register unsigned int i;
		glBegin(GL_QUADS);
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			for(i=0;i<poly1->vlist.size();i++)
			{
				glTexCoord2f(poly1->vlist[i]->tu,poly1->vlist[i]->tv);
				glVertex3f(poly1->vlist[i]->x,poly1->vlist[i]->y,poly1->vlist[i]->z);
			}
		glEnd();		
		glBegin(GL_QUADS);
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			for(i=0;i<poly2->vlist.size();i++)
			{
				glTexCoord2f(poly2->vlist[i]->tu,poly2->vlist[i]->tv);
				glVertex3f(poly2->vlist[i]->x,poly2->vlist[i]->y,poly2->vlist[i]->z);
			}
		glEnd();
		glBegin(GL_QUADS);
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			for(i=0;i<poly3->vlist.size();i++)
			{
				glTexCoord2f(poly3->vlist[i]->tu,poly3->vlist[i]->tv);
				glVertex3f(poly3->vlist[i]->x,poly3->vlist[i]->y,poly3->vlist[i]->z);
			}
		glEnd();
	glPopMatrix();														// Pop Matrix Off The Stack
	//////////////////////////////////////////////////////////////////////////////
	// EXAMPLE TEXTURE CODE END //////////////////////////////////////////////////////////

	// Swap our scene to the front

	SwapBuffers(m_hDC);
	Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnCreateClient

GLvoid CMainFrame::ReSizeGLScene(GLsizei width, GLsizei height)				// Resize And Initialize The GL Window
{																			
	if ( height==0) {														// Prevent A Divide By Zero By																	
		height=1;															// Making Height Equal One
	}																		
	vpWidth=(float)width;
	vpHeight=(float)height;
	glViewport(0,0,width,height);											// Reset The Current Viewport
																			
	glMatrixMode(GL_PROJECTION);											// Select The Projection Matrix
	glLoadIdentity();														// Reset The Projection Matrix

	gluPerspective(70.0f,(GLfloat)width/(GLfloat)height,0.0001f,100.0f);		// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);												// Select The Modelview Matrix
	glLoadIdentity();														// Reset The Modelview Matrix
}


//////////////////////////////////////////////////////////////////////////////
// CMainFrame InitGL

int CMainFrame::InitGL(GLvoid)												// All Setup For OpenGL Goes Here
{	
	CreateInputDevice(m_hWnd);
	m_appTextures.InitTexHashTable(1);		
	m_appTextures.LoadGLTexImage2DMipMap("data/exTexFront.bmp","FRONT");	
	
	poly1->vlist.push_back(new Vector3D(-1.0f,-1.0f,0.0f,0.0f,0.0f));
	poly1->vlist.push_back(new Vector3D(-1.0f,1.0f,0.0f,1.0f,0.0f));
	poly1->vlist.push_back(new Vector3D(1.0f,1.0f,0.0f,1.0f,1.0f));
	poly1->vlist.push_back(new Vector3D(1.0f,-1.0f,0.0f,0.0f,1.0f));
	
	poly2->vlist.push_back(new Vector3D(-1.0f,-1.0f,-1.0f,0.0f,0.0f));
	poly2->vlist.push_back(new Vector3D(1.0f,-1.0f,-1.0f,1.0f,0.0f));
	poly2->vlist.push_back(new Vector3D(1.0f,-2.0f,0.0f,1.0f,1.0f));
	poly2->vlist.push_back(new Vector3D(-1.0f,-2.0f,0.0f,0.0f,1.0f));

	poly3->vlist.push_back(new Vector3D(-1.0f,-3.0f,0.0f,0.0f,0.0f));
	poly3->vlist.push_back(new Vector3D(-1.0f,-1.0f,0.0f,1.0f,0.0f));
	poly3->vlist.push_back(new Vector3D(1.0f,-1.0f,0.0f,1.0f,1.0f));
	poly3->vlist.push_back(new Vector3D(1.0f,-3.0f,0.0f,0.0f,1.0f));

	poly1->polyID=1;
	poly2->polyID=2;
	poly3->polyID=3;
	polylist1.push_back(poly1);
	polylist1.push_back(poly2);
	polylist1.push_back(poly3);
	bsp1->GenerateBSP(polylist1);


	Eye->fHeading=180;
	BuildFont();															// Build your GL Font
	
	glShadeModel(GL_SMOOTH);												// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);									// Black Background
	glClearDepth(1.0f);														// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);												// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);													// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);						// Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);												// Enable Texture Mapping
	return 1;															// Initialization Went OK
}

//////////////////////////////////////////////////////////////////////////////
// CMainFrame KillGLWindow

void getridofdata()
{
	delete bsp1;
}

GLvoid CMainFrame::KillGLWindow(GLvoid)										// Properly Kill The Window
{
	delete Eye;
	/*poly1->~TPolygon();
	poly3->~TPolygon();
	poly2->~TPolygon();	*/
	getridofdata();

	
	
	

	if ( m_hRC ) {															// Do We Have A Rendering Context?																		
		if ( !wglMakeCurrent ( NULL, NULL ) )	{							// Are We Able To Release The DC And RC Contexts?
			MessageBox ( "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}

		if ( !wglDeleteContext ( m_hRC ) ) {								// Are We Able To Delete The RC?
			MessageBox ( "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}
		m_hRC = NULL;														// Set RC To NULL
	}

	if ( m_hDC && !::ReleaseDC ( m_hWnd, m_hDC ) ) {						// Are We Able To Release The DC
		MessageBox ( "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		m_hDC = NULL;														// Set DC To NULL
	}

	if ( m_hWnd && !::DestroyWindow ( m_hWnd ) )	{							// Are We Able To Destroy The Window?
		MessageBox( "Could Not Release m_hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		m_hWnd = NULL;														// Set m_hWnd To NULL
	}

	KillFont();											// killing the GL Font
	FreeDirectInput();

}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame BuildFont

GLvoid CMainFrame::BuildFont(GLvoid)					// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID

	m_iBaseFont = glGenLists(96);						// Storage For 96 Characters

	font = CreateFont(	20,								// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Westminster");					// Font Name

	SelectObject(m_hDC, font);							// Selects The Font We Want

	wglUseFontBitmaps(m_hDC, 32, 96, m_iBaseFont);		// Builds 96 Characters Starting At Character 32
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame KillFont

GLvoid CMainFrame::KillFont(GLvoid)						// Delete The Font
{
	glDeleteLists(m_iBaseFont, 96);						// Delete All 96 Characters
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame glPrint

GLvoid CMainFrame::glPrint(const char *fmt, ...)		// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf_s(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(m_iBaseFont - 32);								// Sets The m_iBaseFont Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame OnKeyDown

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
		CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	// TODO: Add your message handler code here and/or call default

	switch ( nChar ) {
	case VK_ESCAPE:	
		PostMessage ( WM_CLOSE );
		break;
	} // end switch
	
}
