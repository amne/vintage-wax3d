// MainFrame.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRAME_H__D7629C65_B0FF_11D4_A3B0_0050DA731472__INCLUDED_)
#define AFX_MAINFRAME_H__D7629C65_B0FF_11D4_A3B0_0050DA731472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Texture.h"


class CMainFrame : public CFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

//Attributes
protected:
	HDC m_hDC;								// Device Context
	HGLRC m_hRC;							// Rendering Context
	UINT m_cxClient;
	UINT m_cyClient;
	HPALETTE m_hPal;
	int VB_WIDTH;
	int VB_HEIGHT;
	int VB_DEPTH;
	float vpWidth,vpHeight;
	DEVMODE	m_DMsaved;				        // Saves The Previous Screen Settings 
	BOOL m_bFullScreen;
	CTexture m_appTextures;	
	GLuint	m_iBaseFont;				// Base Display List For The Font Set

// Operations
public:
	void RenderGLScene();
	BOOL m_bAppIsActive;
	GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	int InitGL(GLvoid);	
	GLvoid KillGLWindow(GLvoid);
	void calculateFramesPerSec ();
	GLvoid BuildFont(GLvoid);
	GLvoid KillFont(GLvoid);
	GLvoid glPrint(const char *fmt, ...);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__D7629C65_B0FF_11D4_A3B0_0050DA731472__INCLUDED_)
