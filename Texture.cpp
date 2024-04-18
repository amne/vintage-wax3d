// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OGL3.h"
#include "Texture.h"
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CTexture construction/destruction

CTexture::CTexture()
{
	
}

CTexture::~CTexture()
{
	m_TexMap.RemoveAll();	

}

void CTexture::InitTexHashTable(int nTextures)
{

	// add 20 percent larger number to hash table to optimize lookup efficiency

	if(nTextures <= 10)
	{
		nTextures = 13;
	}
	else if(nTextures > 10)
	{ 
		nTextures = int(nTextures + (0.2 * nTextures));
	}

	if(nTextures % 2 == 0)
		nTextures++;

	m_TexMap.InitHashTable(nTextures);


}

//---------------------------------------------------------------------------
// LoadBMP
//---------------------------------------------------------------------------
AUX_RGBImageRec* CTexture::LoadBMP(TCHAR* Filename)
{
	if (!Filename)									// Make Sure A Filename Was Given
    {
		return NULL;								// If Not Return NULL
    }

	CFile file;
	CFileException fileError;

	if(file.Open(_T(Filename), CFile::modeRead, &fileError))
	{
		file.Close ();
		return auxDIBImageLoad(Filename);			// Load The Bitmap And Return A Pointer
	}
	else
	{
		fileError.ReportError();
		return NULL;								// If Load Failed Return NULL
	}

	return NULL;									// If Load Failed Return NULL

}


//---------------------------------------------------------------------------
// LoadGLTexImage2DNearest
//---------------------------------------------------------------------------
void CTexture::LoadGLTexImage2DNearest(TCHAR* Filename, TCHAR* LookUpName)
{
	// Check to see if the LookUpName is already added to the map

	GLuint BindTexID;
	if(m_TexMap.Lookup(LPCSTR (LookUpName),BindTexID))
	{
		CString errString(LookUpName);
		errString += _T(" already added to Texture lookup Map.");
		MessageBox(NULL,errString, "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;

	}

	t_tex currTex;
	if (currTex.image = LoadBMP(Filename))	
    {	
		glGenTextures(1, &currTex.bindTexID);				// Create One Texture
		glBindTexture(GL_TEXTURE_2D, currTex.bindTexID);

		// Create Nearest Filtered Texture

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, currTex.image->sizeX, currTex.image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, currTex.image->data);
        if (currTex.image)							// If Texture Exists
		{
			if (currTex.image->data)					// If Texture Image Exists
			{
				free(currTex.image->data);			// Free The Texture Image Memory
			}
			free(currTex.image);						// Free The Image Structure
		}
	}
	m_TexMap[LookUpName] = currTex.bindTexID;

}

//---------------------------------------------------------------------------
// LoadGLTexImage2DLinear
//---------------------------------------------------------------------------
void CTexture::LoadGLTexImage2DLinear(TCHAR* Filename, TCHAR* LookUpName)
{
	// Check to see if the LookUpName is already added to the map

	GLuint BindTexID;
	if(m_TexMap.Lookup(LPCSTR (LookUpName),BindTexID))
	{
		CString errString(LookUpName);
		errString += _T(" already added to Texture lookup Map.");
		MessageBox(NULL,errString, "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;

	}

	t_tex currTex;
	if (currTex.image = LoadBMP(Filename))	
    {	
		glGenTextures(1, &currTex.bindTexID);				// Create One Texture
		glBindTexture(GL_TEXTURE_2D, currTex.bindTexID);

		// Create Linear Filtered Texture

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, currTex.image->sizeX, currTex.image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, currTex.image->data);

        if (currTex.image)							// If Texture Exists
		{
			if (currTex.image->data)					// If Texture Image Exists
			{
				free(currTex.image->data);			// Free The Texture Image Memory
			}
			free(currTex.image);						// Free The Image Structure
		}
	}
	m_TexMap[LookUpName] = currTex.bindTexID;
}


//---------------------------------------------------------------------------
// LoadGLTexImage2DMipMap
//---------------------------------------------------------------------------
void CTexture::LoadGLTexImage2DMipMap(TCHAR* Filename, TCHAR* LookUpName)
{
	// Check to see if the LookUpName is already added to the map

	GLuint BindTexID;
	if(m_TexMap.Lookup(LPCSTR (LookUpName),BindTexID))
	{
		CString errString(LookUpName);
		errString += _T(" already added to Texture lookup Map.");
		MessageBox(NULL,errString, "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;

	}

	t_tex currTex;
	if (currTex.image = LoadBMP(Filename))	
    {	
		glGenTextures(1, &currTex.bindTexID);				// Create One Texture
		glBindTexture(GL_TEXTURE_2D, currTex.bindTexID);

		// Create MipMapped Texture

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, currTex.image->sizeX, currTex.image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, currTex.image->data);

        if (currTex.image)							// If Texture Exists
		{
			if (currTex.image->data)					// If Texture Image Exists
			{
				free(currTex.image->data);			// Free The Texture Image Memory
			}
			free(currTex.image);						// Free The Image Structure
		}
	}
	m_TexMap[LookUpName] = currTex.bindTexID;
}


//---------------------------------------------------------------------------
// Use
//---------------------------------------------------------------------------
void CTexture::Use(TCHAR* LookUpName)
{
	glBindTexture(GL_TEXTURE_2D,m_TexMap[LookUpName]);

}

