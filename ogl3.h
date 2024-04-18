// OGL3.h : main header file for the OGL3 application
//
#if !defined(AFX_TEST_H__D7629C61_B0FF_11D4_A3B0_0050DA731472__INCLUDED_)
#define AFX_OGL3_H__D7629C61_B0FF_11D4_A3B0_0050DA731472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COGL3App:
// See OGL3.cpp for the implementation of this class
//

class COGL3App : public CWinApp
{
public:
	COGL3App();

private:
	LONGLONG	m_lCurTime;			// current time
	DWORD		m_dwTimeCount;		// ms per frame, default if no performance counter
	LONGLONG	m_lPerfCounter;		// performance timer frequency
	BOOL		m_bPerFlag;			// flag determining which timer to use
	LONGLONG	m_lNextTime;		// time to render next frame
	LONGLONG	m_lLastTime;		// time of previous frame
	double		m_dTimeElapsed;		// time since previous frame
	double		m_dTimeScale;		// scaling factor for time
	LONGLONG	m_lFramesPerSecond;	// How many frames per second to run App

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COGL3App)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(COGL3App)
	// NOTE - the ClassWizard will add and remove member functions here.
	// DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGL3_H__D7629C61_B0FF_11D4_A3B0_0050DA731472__INCLUDED_)
