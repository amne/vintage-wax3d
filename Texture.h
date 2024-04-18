// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "BMP.h"

#if !defined(AFX_TEXTURE_H__FB03BE80_B34C_11D4_A3B0_0050DA731472__INCLUDED_)
#define AFX_TEXTURE_H__FB03BE80_B34C_11D4_A3B0_0050DA731472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTexture  
{
public:
	CTexture();
	virtual ~CTexture();
	void InitTexHashTable(int nTextures);
	AUX_RGBImageRec* LoadBMP(TCHAR* Filename);
	void LoadGLTexImage2DNearest(TCHAR* Filename,TCHAR* LookUpName);
	void LoadGLTexImage2DLinear(TCHAR* Filename,TCHAR* LookUpName);
	void LoadGLTexImage2DMipMap(TCHAR* Filename,TCHAR* LookUpName);
	void Use(TCHAR* LookUpName);
private:
	struct t_tex 
	{
		AUX_RGBImageRec* image;
		GLuint bindTexID;
	};	
	CMap<CString, LPCSTR, GLuint,GLuint&> m_TexMap;

};

#endif // !defined(AFX_TEXTURE_H__FB03BE80_B34C_11D4_A3B0_0050DA731472__INCLUDED_)
