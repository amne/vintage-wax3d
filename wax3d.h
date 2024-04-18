#if !defined(__WAX_3D_H_1880108)
#define __WAX_3D_H_1880108
#define __WIN32__
#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>
//#include "BMP.h"
#include <math.h>
#include <memory.h>
#include <vector>


#include "texture.h"
#include "wax3d_core.h"


//#using <System.dll>

using namespace std;


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif


#define pi 3.1415926535897932384626433832795
#define rad pi/180

UINT lastTick=GetTickCount(),thisTick=GetTickCount();

GLfloat MyMatrix[16];




inline double sgn(double val){return fabs(val)/val;}
inline float sgn(float val){return (float)fabs((double)val)/(float)val;}
inline long int sgn(long int val){return (long int)fabs((double)val)/(long int)val;}

class glQuaternion  
{
public:
	glQuaternion operator *(glQuaternion q);
	void CreateMatrix(GLfloat *pMatrix);
	void CreateFromAxisAngle(GLfloat x, GLfloat y, GLfloat z, GLfloat degrees);
	glQuaternion();
	virtual ~glQuaternion();
private:
	GLfloat m_w;
	GLfloat m_z;
	GLfloat m_y;
	GLfloat m_x;
};

glQuaternion::glQuaternion(){m_x = m_y = m_z = 0.0f;	m_w = 1.0f;}
glQuaternion::~glQuaternion(){}

void glQuaternion::CreateFromAxisAngle(GLfloat x, GLfloat y, GLfloat z, GLfloat degrees)
{
	GLfloat angle = GLfloat((degrees / 180.0f) * pi);

	GLfloat result = (GLfloat)sin( angle / 2.0f );
		
	m_w = (GLfloat)cos( angle / 2.0f );

	m_x = GLfloat(x * result);
	m_y = GLfloat(y * result);
	m_z = GLfloat(z * result);
}

void glQuaternion::CreateMatrix(GLfloat *pMatrix)
{
	if(!pMatrix) return;

	pMatrix[ 0] = 1.0f - 2.0f * ( m_y * m_y + m_z * m_z ); 
	pMatrix[ 1] = 2.0f * (m_x * m_y + m_z * m_w);
	pMatrix[ 2] = 2.0f * (m_x * m_z - m_y * m_w);
	pMatrix[ 3] = 0.0f;  

	pMatrix[ 4] = 2.0f * ( m_x * m_y - m_z * m_w );  
	pMatrix[ 5] = 1.0f - 2.0f * ( m_x * m_x + m_z * m_z ); 
	pMatrix[ 6] = 2.0f * (m_z * m_y + m_x * m_w );  
	pMatrix[ 7] = 0.0f;  

	pMatrix[ 8] = 2.0f * ( m_x * m_z + m_y * m_w );
	pMatrix[ 9] = 2.0f * ( m_y * m_z - m_x * m_w );
	pMatrix[10] = 1.0f - 2.0f * ( m_x * m_x + m_y * m_y );  
	pMatrix[11] = 0.0f;  

	pMatrix[12] = 0;  
	pMatrix[13] = 0;  
	pMatrix[14] = 0;  
	pMatrix[15] = 1.0f;
}

glQuaternion glQuaternion::operator *(glQuaternion q)
{
	glQuaternion r;

	r.m_w = m_w*q.m_w - m_x*q.m_x - m_y*q.m_y - m_z*q.m_z;
	r.m_x = m_w*q.m_x + m_x*q.m_w + m_y*q.m_z - m_z*q.m_y;
	r.m_y = m_w*q.m_y + m_y*q.m_w + m_z*q.m_x - m_x*q.m_z;
	r.m_z = m_w*q.m_z + m_z*q.m_w + m_x*q.m_y - m_y*q.m_x;
	
	return(r);
}



class Vector3D
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat tu;
	GLfloat tv;
	Vector3D()
	{		x = 0;		y = 0;		z = 0;		tu=0;		tv=0;   };
	~Vector3D(){x=0; y=0; z=0; tu=0; tv=0;}

	Vector3D(GLfloat xx, GLfloat yy, GLfloat zz)
	{		x = xx;		y = yy;		z = zz;		tu=0;		tv=0;	};
	
	Vector3D(GLfloat xx,GLfloat yy,GLfloat zz,GLfloat tuu,GLfloat tvv)
	{		x=xx;		y=yy;		z=zz;		tu=tuu;		tv=tvv;	};

	Vector3D operator= (Vector3D v)
	{		this->x = v.x;	this->y = v.y;	this->z = v.z;	this->tu=v.tu;	this->tv=v.tv;	return *this;	};

	Vector3D operator+ (Vector3D v)
	{		return Vector3D(x + v.x, y + v.y, z + v.z);	};

	Vector3D operator- (Vector3D v)
	{		return Vector3D(x - v.x, y - v.y, z - v.z);	};

	Vector3D operator* (GLfloat value)
	{		return Vector3D(x * value, y * value, z * value);	};
	
	float operator* (Vector3D v)
	{		return GLfloat(x*v.x+y*v.y+z*v.z);	};
	
	float operator~ ()
	{		return (GLfloat)sqrtf(x*x+y*y+z*z);	};

	Vector3D operator^ (Vector3D v)//cross-product of this and v
	{    return Vector3D(GLfloat(y*v.z-z*v.y),GLfloat(z*v.x-x*v.z),GLfloat(x*v.y-y*v.x));	};

	Vector3D operator/ (GLfloat value)
	{		return Vector3D(GLfloat(x / value), GLfloat(y / value), GLfloat(z / value));	};

	int operator== (Vector3D v)
	{		return((x==v.x) && (y==v.y) && (z==v.z));	};

	Vector3D operator+= (Vector3D v)
	{		this->x += v.x;		this->y += v.y;		this->z += v.z;		return *this;	};

	Vector3D operator-= (Vector3D v)
	{		this->x -= v.x;		this->y -= v.y;		this->z -= v.z;		return *this;	};

	Vector3D operator*= (GLfloat value)
	{		this->x *= value;		this->y *= value;		this->z *= value;		return *this;	};

	Vector3D operator/= (GLfloat value)
	{		this->x /= value;		this->y /= value;		this->z /= value;		return *this;	};

	Vector3D operator- ()
	{		return Vector3D(-x, -y, -z);	};

	GLfloat length()
	{		return (GLfloat)sqrtf(x*x + y*y + z*z);	};

	void unitize()
	{		GLfloat length = this->length();
		if (length == 0)	return;
		this->x /= length;		this->y /= length;		this->z /= length;	};

	Vector3D unit()
	{
		GLfloat length = this->length();
		if (length == 0) return *this;		
		return Vector3D(GLfloat(this->x / length), GLfloat(this->y / length), GLfloat(this->z / length));
	}
	void project(Vector3D *p,float p_s)
	{
		if(*p==Vector3D(0,0,0)){return;}
		Vector3D v2=Vector3D(0,0,0);
		v2=((*this)^(*p));
		*this=((*p)^(v2));
	}
};

class TPlane
{
public:
	GLfloat s;
	Vector3D v;
	TPlane(Vector3D = Vector3D(),Vector3D = Vector3D(),Vector3D = Vector3D());
	float DistFromPoint(Vector3D p)
	{
		return (v.x*p.x+v.y*p.y+v.z*p.z-s)/v.length();
	}
	float operator| (Vector3D p)
	{
		return (v.x*p.x+v.y*p.y+v.z*p.z-s)/v.length();
	}
};


TPlane::TPlane(Vector3D v1,Vector3D v2,Vector3D v3)
{
Vector3D va,vb;
va=v2-v1;
vb=v3-v1;
v=va^vb;
v.unitize();
s=v*v1;
}

/*
float TPlane::DistFromPoint(Vector3D p)
{
}*/

class W_Eye
{
public:
	float fHeading,fPitch,fTilt;
	float sensLR,sensUD,sensFB;
	float sensMouse;
	Vector3D vPos,vLookAt,vUp,vFocus,vLastPos;
	float dFov;
	W_Eye(Vector3D v_Pos,Vector3D v_LookAt,Vector3D v_Up,float d_Fov);
	void SetPos(float x,float y,float z);
	void LookAt(float x,float y, float z);
	void UpV(float x,float y,float z);
	void DoAction(HWND hWnd);
	void CalcFocus();
	W_Eye operator= (W_Eye);
};
W_Eye::W_Eye(Vector3D v_Pos, Vector3D v_LookAt, Vector3D v_Up,float d_Fov)
{
	sensLR=2.0f/1000.0f;
	sensUD=2.0f/1000.0f;
	sensFB=2.0f/1000.0f;
	sensMouse=20.0f/1000.0f;
	vPos=v_Pos;
	vLookAt=v_LookAt;
	vUp=v_Up;
	vFocus=Vector3D(0.0f,0.0f,-1.0f);
	dFov=d_Fov;
	fHeading=0;
	fPitch=0;
	fTilt=0;
}
void W_Eye::SetPos(float x,float y,float z)
{
	vPos.x=x;
	vPos.y=y;
	vPos.z=z;
}
void W_Eye::LookAt(float x, float y,float z)
{
	vLookAt.x=x;
	vLookAt.y=y;
	vLookAt.z=z;
}
void W_Eye::UpV(float x,float y,float z)
{
	vUp.x=x;
	vUp.y=y;
	vUp.z=z;
}
void W_Eye::CalcFocus()
{
Vector3D vSide;
vSide=vFocus^vUp;
glQuaternion m_qPitch,m_qHeading;
m_qPitch.CreateFromAxisAngle(1.0f, 0.0f, 0.0f, fPitch);
m_qHeading.CreateFromAxisAngle(0.0f, 1.0f, 0.0f, fHeading);
m_qPitch.CreateMatrix(MyMatrix);
vFocus.y=MyMatrix[9];
m_qHeading.CreateMatrix(MyMatrix);
vFocus.x=MyMatrix[8];
vFocus.z=MyMatrix[10];
}

void W_Eye::DoAction(HWND hWnd)
{
Vector3D vSide;
float fX,fY;
unsigned long tickCount;

vSide=vFocus^vUp;
//QueryPerformanceCounter((unsigned long *)thisTick)
lastTick=thisTick;
thisTick=GetTickCount();
tickCount=thisTick-lastTick;
InputData(hWnd);

if(diks[DIK_A] && 0x80)
	{
		vPos=vPos-vSide*(float)tickCount*sensLR;
	}
if(diks[DIK_D] && 0x80)
	{
		vPos=vPos+vSide*(float)tickCount*sensLR;
	}
if(diks[DIK_W] && 0x80)
	{
		vPos=vPos+vFocus*(float)tickCount*sensFB;
	}
if(diks[DIK_S] && 0x80)
	{
		vPos=vPos-vFocus*(float)tickCount*sensFB;
	}
fX=(float)dims2.lX*tickCount*sensMouse;
fY=(float)dims2.lY*tickCount*sensMouse;
fHeading=fHeading-fX;
fPitch=fPitch+fY;
if(fHeading>360) fHeading-=360;
if(fHeading<0) fHeading+=360;
if(fPitch>80) fPitch=80;
if(fPitch<-80) fPitch=-80;
if (fX||fY) {CalcFocus();}
LookAt(vPos.x+vFocus.x,vPos.y+vFocus.y,vPos.z+vFocus.z);
}


class Mass
{
public:
	float m;
	Vector3D pos;
	Vector3D vel;
	Vector3D force;

	Mass(float m)
	{
		this->m = m;
	}
	void applyForce(Vector3D force)
	{
		this->force += force;
	}
	void init()
	{
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}
	void simulate(float dt)
	{
		vel += (force / m) * dt;
		pos += vel * dt;
	}
};




class TPolygon
{
private:
	int processed;
public:
	vector<Vector3D*> vlist;
	float marked;
	unsigned long polyID;
	TPlane mP;
	vector<TPlane*> sP;
	TPolygon(){marked=0.0f;polyID=0;processed=0;}
	~TPolygon()
		{
			register unsigned int i=0;
			for(i=0;i<vlist.size();i++) if (vlist[i]) {delete vlist[i]; vlist[i]=NULL;}
			for(i=0;i<sP.size();i++) if(sP[i]) {delete sP[i]; sP[i]=NULL;}
		}
	void ProcessPolygon()
	{
		register unsigned int i=0;
		mP=TPlane(*vlist[0],*vlist[1],*vlist[2]);
		for(i=0;i<sP.size();i++) if(sP[i]) {delete sP[i]; sP[i]=NULL;}
		while(sP.size()){sP.pop_back();}
		for(i=0;i<vlist.size();i++)
		{
//wrong ordering but left if needed			//sP.push_back(new TPlane(*vlist[i],*vlist[(i+1)%vlist.size()], *(vlist[(i+1)%vlist.size()])+mP.v));
			sP.push_back(new TPlane(*vlist[(i+1)%vlist.size()],*vlist[i], *(vlist[i])+mP.v));
		}
		return;
	}
	float ClassifyPoint(Vector3D* p1,float thickness=0.0f)
	{
		if(vlist.size()<3) return 0;
		if(processed!=vlist.size()) { ProcessPolygon(); } //process the poly planes only if new points. manual process if shape changes
		//TPlane* tP=new TPlane(*vlist[0],*vlist[1],*vlist[2]);
		float d=(mP) | (*p1);
		//delete tP;
//		tP=NULL;
		return (fabs(d)<fabs(thickness))?0.0f:d;
	}
	float PointInArea(Vector3D *v1)
	{
		register unsigned int i=0; //yeah yeah .. register because it's local has a short lifetime.	
		float d=0;
		for(i=0;i<sP.size();i++)
		{
			d=(*sP[i]) | (*v1); // | = overloaded to return the distance from point v1 to plane sP[i]. see TPlane decl.
			if(d<0){ return 0; }
		}
		return 1;
	}
	float ClassifyPoly(TPolygon* tP,float thickness=0.0f)
	{
		if(vlist.size()<3) return 0;
		if(tP->vlist.size()<3) return 0;
		float d=0,d1=0,nF=0,nB=0;		
		for(register unsigned int i=0;i<tP->vlist.size();i++)
		{
			d1=ClassifyPoint(tP->vlist[i],thickness);
			if((d1>0) || nF){nF++;} //if point is on plane but no back points yet then consider it in front to avoid split
			else {if((d1<0) || nB)nB++;} //same here .. if no front points but d1==0 then take it as in the back
		}
		if((nF==0)&&(nB>0)) {return -1.0f;}
		if((nF>0)&&(nB==0)) {return 1.0f;}
		if((nF==0)&&(nB==0)) {return 0.0f;}
		if((nF>0)&&(nB>0)) {return 1.1f;}
		return -2.0f;
	}
	void SplitPoly(TPolygon* tP)
	{
	}
};

struct TCollisionInfo {
	Vector3D pN,vC;
	float pS;
	float collision;
};

class CBSPTreeNode
{
public:
	CBSPTreeNode *left ,*right;
	int depth,lr;
	vector<TPolygon*> polylist;
	CBSPTreeNode(){left=0; right=0; depth=0; lr=0;}
	~CBSPTreeNode()
		{			
			for(register unsigned int i=0;i<polylist.size();i++) if(polylist[i]){polylist[i]->~TPolygon(); if(polylist[i]) {delete polylist[i]; polylist[i]=NULL;}}
			if(left){delete left; left=NULL;}if(right){delete right; right=NULL;}
		}
	int ListIsConvex(vector<TPolygon*> plist1)
	{
		if(plist1.size()<1){return 0;}
		if(plist1.size()==1){return 1;}
		unsigned long i=0,j=0;
		float clsf=0;
		for(i=0;i<plist1.size();i++)
		{
			for(j=0;j<plist1.size();j++)
			{
				if(i!=j)
				{
					clsf=plist1[i]->ClassifyPoly(plist1[j]);
					if((clsf!=1.0f) && (clsf!=0.0f)) return 0;
				}
			}
		}
		return 1;
	}
	int ListIsCoplanar(vector<TPolygon*> plist1)
	{
		if(plist1.size()<1){return 0;}
		if(plist1.size()==1){return 1;}
		unsigned long i=0,j=0;
		float clsf=0;
//		pl1=plist1->GetElem(i);
		for(j=1;j<plist1.size();j++)
		{
//			pl2=plist1->GetElem(j);
			clsf=plist1[0]->ClassifyPoly(plist1[j],0.02f);
			if(clsf!=0.0f) return 0;
		}
		return 1;
	}	
	unsigned long ChooseBestPoly(vector<TPolygon*> plist1)
	{
		if(plist1.size()<1){return 0;}
//		if(ListIsConvex(plist1)){return 0;}  we get here only if it's not convex. so it's marked out
		float minrelation=1.0f/2.0f,relationdivider=2.18f;
		unsigned long bestpoly=0;
		unsigned long leastsplits=4200000000;
		float bestrelation=0.0f,tVal=0.0f,relation=0.0f;
		unsigned long nF=0,nB=0,nS=0,i=0,j=0;
		while(!bestpoly)
		{
			for(i=0;i<plist1.size();i++)
			{
				nF=0;nB=0;nS=0;
				for(j=0;j<plist1.size();j++)
				{
					if(i!=j)
					{
//						pl1=plist1->GetElem(i);
						if(plist1[i]->marked==0)
						{
//							pl2=plist1->GetElem(j);
							tVal=plist1[i]->ClassifyPoly(plist1[j]);
							if(tVal==1.0f)nF++;
							if(tVal==-1.0f)nB++;
							if(tVal==1.1f)nS++;
						}
					}
				}
				if(nF<nB){relation=float(nF/nB);}else{if(nF)relation=float(nB/nF);}
				if(relation>minrelation && (nS<leastsplits||nS==leastsplits && relation>bestrelation))
				{
					bestpoly=i;
					leastsplits=nS;
					bestrelation=relation;
				}
				minrelation/=relationdivider;
			}
		}
	return bestpoly;
	}
	void GenerateBSP(vector<TPolygon*> polyset)
	{
		TPolygon* divpoly=0;
		vector<TPolygon*> frontSet;
		vector<TPolygon*> backSet;
		unsigned long divpolyID=0,i;
		float d1=0;
		if(polyset.size()<1) return;
		if((polyset.size()==1) || ListIsConvex(polyset))
		{
			if((polyset.size()==1) || ListIsCoplanar(polyset))
			{
				polylist=polyset;
			} else
			{
				divpolyID=0;
			}
		} else {divpolyID=ChooseBestPoly(polyset);}
		if(divpolyID)
		{
			divpoly=polyset[divpolyID];
			divpoly->marked=1.0f;
			polylist.push_back(divpoly);			
			for(i=0;i<polyset.size();i++)
			{
				if(i!=divpolyID)
				{
					d1=divpoly->ClassifyPoly(polyset[i]);
					if(d1==1.0f)
					{
						frontSet.push_back(polyset[i]);
					}
					if(d1==-1.0f)
					{
						backSet.push_back(polyset[i]);
					}
				}
			}
			if(backSet.size()>0)
			{
				left=new CBSPTreeNode;
				left->depth=depth+1;
				left->lr=lr-1;
				left->GenerateBSP(backSet);
			}
			if(frontSet.size()>0)
			{
				right=new CBSPTreeNode;
				right->depth=depth+1;
				right->lr=lr+1;
				right->GenerateBSP(frontSet);
			}
		}
	}
	void CollisionTest(Vector3D *startP,Vector3D *endP, TCollisionInfo *s)
	{
		float rVal1=0.0f,rVal2=0.0f;
		register int i=0,k1=0;
		memset(s,0,sizeof(TCollisionInfo)); //remember to set everything to 0 ... bad things can happen
		if((*startP)==(*endP)) {return;} // just get out if we are not moving ... doooh
		s->pN=Vector3D(0.0f,0.0f,0.0f);
		s->vC=Vector3D(0.0f,0.0f,0.0f);
		s->pS=0.0f;
		s->collision=0;		
		i=0;
		rVal1=polylist[i]->ClassifyPoint(startP,0.01f); //since all polys are on the same plane just check one
		rVal2=polylist[i]->ClassifyPoint(endP,0.01f);	//and if crossed then one of them is colliding
		i=-1;
		if((rVal1*rVal2)<=0)
		{
			while((i==-1) && (k1<polylist.size()))
			{
				if(polylist[k1]->PointInArea(startP)) //although right is using the intersection point
				{									 //we use the startP because it doesn't require any calcs.
					i=k1;
				}
				k1++;
			}
			if(i>-1)
			{
//			CDynList<Vector3D>::TElemList *t1=polylist->GetElem(i)->elem->vlist->GetElem(1)->first;
			TPlane *colPl=new TPlane(*(polylist[i]->vlist[0]),*(polylist[i]->vlist[1]),*(polylist[i]->vlist[2]));
			Vector3D v1=Vector3D(endP->x-startP->x,endP->y-startP->y,endP->z-startP->z);
			s->pN=colPl->v;
			s->pS=colPl->s;			
			s->vC=v1;
			s->vC.unitize();
			s->collision=rVal1;
			SAFE_DELETE(colPl);
			return;
			}
		}
		if(rVal1<0.0f){if(left){left->CollisionTest(startP,endP, s);}}
		if(rVal1>0.0f){if(right){right->CollisionTest(startP,endP, s);}}
		return;
	}
};



#endif