//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// 3DTransformation.cpp: implementation of the C3DTransformation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wsview.h"
#include <math.h>
#include "3DTransformation.h"

//static double SinTable[256],CosTable[256];

#define ABS(a) (((a)<0)?(-(a)):(a))
//#define SIN(x) SinTable[ABS(((int)x)&255)]
//#define COS(x) CosTable[ABS(((int)x)&255)]

#define SIN(x) sin(x/180.*PI);
#define COS(x) cos(x/180.*PI);

#define   PI  3.1415926535

#define N 4

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static void inverse(	double w[N][2*N],        /* input/output - the working matrix */ 
						int    *sol_existsp);     /* output - flag indicating whether or not a */ 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DTransformation::C3DTransformation()
	{
	Reset();
	}

C3DTransformation::~C3DTransformation()
{

}

void C3DTransformation::WorldToLogical(CPoint &cPoint, double fX, double fY, double fZ)
{

	double dX, dY, dZ;

	VEC_MultMatrix(	fX, fY, fZ,
					m_dTransformationMatrix,
					dX, dY, dZ);
	cPoint.x=int(dX);
	cPoint.y=int(dY);
}

bool C3DTransformation::LogicalToWorld(CPoint cPoint, double &fX, double &fY, double &fZ)
{

	double dX, dY, dZ;

	if( !CalculateInverse() ) return false;

	VEC_MultMatrix(	cPoint.x, cPoint.y,  0.0,
					m_dTransformationMatrixInv,
					dX, dY, dZ);
	fX=float(dX);
	fY=float(dY);
	fZ=float(dZ);

	return true;
}


void C3DTransformation::AutoScale(	   double fWorldXMin, 
									   double fWorldXMax, 
									   double fWorldYMin, 
									   double fWorldYMax,
									   int   iMaxScreenX,
									   int   iMaxScreenY,
									   double dHomeLat,
									   double dZScale)
	{

	Reset();

	// Compute the correction factor to correct to the projection error.
	double dCorr=1.0;
	if( dHomeLat!=0 )
		{
		double dLat=dHomeLat;
		double dAng=(dLat/180)*PI;
		dAng=min(dAng, 80);
		dCorr=1/cos(dAng);
		}
	else
		{
		double dLat=((fWorldYMin+fWorldYMax)/2.);
		double dAng=(dLat/180)*PI;
		dAng=min(dAng, 80);
		dCorr=1/cos(dAng);
		}

	// Add a minor tweak to make sure X and Y coordinates are the same scale on the screen
	// I have no idea why this is needed
	dCorr*=1.05; 

	// Compute the scale factor for the Autoscale
	double dXScale=(fWorldXMax-fWorldXMin)/iMaxScreenX;
	double dYScale=(fWorldYMax-fWorldYMin)/iMaxScreenY;

	double dScale=max(dYScale, dXScale );
	dScale*=1.3;
	SetScale( (1./dScale),  (1./dScale)*dCorr, dZScale);

	// Compute the new center point for the Autoscale
	CPoint cPoint;
	WorldToLogical(cPoint, (fWorldXMax+fWorldXMin)/2., (fWorldYMax+fWorldYMin)/2., 0);

	SetTranslation( -(cPoint.x-iMaxScreenX/2), -(cPoint.y-iMaxScreenY/2), 0);



	}

bool C3DTransformation::CalculateInverse()
{
	
//http://cegt201.bradley.edu/~olekmali/courses/uw3070/examples/arr_inv.txt


  double matrix_W[N][2 * N],        /* parameter - working matrix */ 
         ID[N][N];                  /* constant - identity matrix */ 
  int sol_poss,                     /* parameter - flag to indicate whether or not a solution exists */ 
      j,k;                          /* LCV - element subscripts */ 
 
  /* Initialize identity matrix */ 
  for (j = 0; j < N; ++j) 
      for (k = 0; k < N; ++k) 
            if (k == j) 
                   ID[j][k] = 1.0; 
            else 
                   ID[j][k] = 0.0; 
 
  /* Copy matrix and identity matrix into working matrix */ 
  for (j = 0; j < N; ++j) 
      for (k = 0; k < N; ++k) { 
            matrix_W[j][k] = m_dTransformationMatrix[j][k]; 
            matrix_W[j][k + N] = ID[j][k]; 
      } 
 
  /* Find inverse of matrix A, if it exists */ 
  inverse (matrix_W, &sol_poss); 
 
  /* error if no solution exists */ 
  if (!sol_poss) return false;
 
   /* Copy inverse from working matrix into A inverse matrix */ 
  for (j = 0; j < N; ++j) 
        for (k = 0; k < N; ++k) 
               m_dTransformationMatrixInv[j][k] = matrix_W[j][k + N]; 
  
return true;
}

/*
=================
TR_Translate
=================
*/
void C3DTransformation::Translate( double tx,double ty,double tz)
{

   double tmat[4][4];
   double mat1[4][4];

   tmat[0][0]=1;  tmat[0][1]=0;  tmat[0][2]=0;  tmat[0][3]=0;
   tmat[1][0]=0;  tmat[1][1]=1;  tmat[1][2]=0;  tmat[1][3]=0;
   tmat[2][0]=0;  tmat[2][1]=0;  tmat[2][2]=1;  tmat[2][3]=0;
   tmat[3][0]=tx; tmat[3][1]=ty; tmat[3][2]=tz; tmat[3][3]=1;
   MAT_Mult(m_dTransformationMatrix,tmat,mat1);
   MAT_Copy(mat1,m_dTransformationMatrix);
}
/*
=================
TR_Scale
=================
*/
void C3DTransformation::Scale( double sx,double sy, double sz)
{
   double smat[4][4];
   double mat1[4][4];

   smat[0][0]=sx; smat[0][1]=0;  smat[0][2]=0;  smat[0][3]=0;
   smat[1][0]=0;  smat[1][1]=sy; smat[1][2]=0;  smat[1][3]=0;
   smat[2][0]=0;  smat[2][1]=0;  smat[2][2]=sz; smat[2][3]=0;
   smat[3][0]=0;  smat[3][1]=0;  smat[3][2]=0;  smat[3][3]=1;
   MAT_Mult(m_dTransformationMatrix,smat,mat1);
   MAT_Copy(mat1,m_dTransformationMatrix);
}
/*
=================
TR_Rotate
=================
*/
void C3DTransformation::Rotate( double ax, double ay, double az)
{


   double xmat[4][4], ymat[4][4], zmat[4][4];
   double mat1[4][4], mat2[4][4];

   xmat[0][0]=1;        xmat[0][1]=0;        xmat[0][2]=0;        xmat[0][3]=0;
   xmat[1][0]=0;        xmat[1][1]=COS(ax);  xmat[1][2]=SIN(ax);  xmat[1][3]=0;
   xmat[2][0]=0;        xmat[2][1]=-SIN(ax); xmat[2][2]=COS(ax);  xmat[2][3]=0;
   xmat[3][0]=0;        xmat[3][1]=0;        xmat[3][2]=0;        xmat[3][3]=1;

   ymat[0][0]=COS(ay);  ymat[0][1]=0;        ymat[0][2]=-SIN(ay); ymat[0][3]=0;
   ymat[1][0]=0;        ymat[1][1]=1;        ymat[1][2]=0;        ymat[1][3]=0;
   ymat[2][0]=SIN(ay);  ymat[2][1]=0;        ymat[2][2]=COS(ay);  ymat[2][3]=0;
   ymat[3][0]=0;        ymat[3][1]=0;        ymat[3][2]=0;        ymat[3][3]=1;

   zmat[0][0]=COS(az);  zmat[0][1]=SIN(az);  zmat[0][2]=0;        zmat[0][3]=0;
   zmat[1][0]=-SIN(az); zmat[1][1]=COS(az);  zmat[1][2]=0;        zmat[1][3]=0;
   zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=1;        zmat[2][3]=0;
   zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;        zmat[3][3]=1;

   MAT_Mult( m_dTransformationMatrix,ymat,mat1);
   MAT_Mult(mat1,xmat,mat2);
   MAT_Mult(mat2,zmat, m_dTransformationMatrix);
}


void C3DTransformation::SetAbsoluteRotation(int rx, int ry, int rz)
	{
 	MAT_Identity(m_dTransformationMatrix);
	MAT_Identity(m_dTransformationMatrixInv);

	Scale(m_dXScale, m_dYScale, m_dZScale);
	Translate(m_dXTrans, m_dYTrans, m_dZTrans);

	Rotate(rx, ry, rz);

	}



//===========================================================================
//				SOME MATH UTILITIES

/*
=================
M3D_Init
=================
*/
void M3D_Init()
{
//   int d;
  // for(d=0; d<256; d++)
//   {
//       SinTable[d]=sin(d*PI/128.0);
//       CosTable[d]=cos(d*PI/128.0);
//   }
}
/*
=================
MAT_Copy
=================
*/
void MAT_Copy(double source[4][4], double dest[4][4])
{
    int i,j;
    for(i=0; i<4; i++)
      for(j=0; j<4; j++)
         dest[i][j]=source[i][j];
}
/*
=================
MAT_Mult
=================
*/
void MAT_Mult(double mat1[4][4], double mat2[4][4], double dest[4][4])
{
   int i,j;
   for(i=0; i<4; i++)
      for(j=0; j<4; j++)
         dest[i][j]=mat1[i][0]*mat2[0][j]+
                    mat1[i][1]*mat2[1][j]+
                    mat1[i][2]*mat2[2][j]+
                    mat1[i][3]*mat2[3][j];
}
/*
=================
MAT_Identity
=================
*/
void MAT_Identity(double mat[4][4])
{
    mat[0][0]=1; mat[0][1]=0; mat[0][2]=0; mat[0][3]=0;
    mat[1][0]=0; mat[1][1]=1; mat[1][2]=0; mat[1][3]=0;
    mat[2][0]=0; mat[2][1]=0; mat[2][2]=1; mat[2][3]=0;
    mat[3][0]=0; mat[3][1]=0; mat[3][2]=0; mat[3][3]=1;
}

/*
=================
VEC_DotProduct
=================
*/
double  VEC_DotProduct(double v1x, double v1y, double v1z,
					   double v2x, double v2y, double v2z )
{
    return (v1x*v2x+v1y*v2y+v1z*v2z);
}

void VEC_Normalize(double &v1x, double &v1y, double &v1z)
{
   double m=sqrt(v1x*v1x+v1y*v1y+v1z*v1z);
   v1x/=m;
   v1y/=m;
   v1z/=m;
}

/*
=================
VEC_CrossProduct
=================
*/
void VEC_CrossProduct(	double Vector1x,double Vector1y,double Vector1z, 
						double Vector2x,double Vector2y,double Vector2z, 
						double &VectorOx,double &VectorOy,double &VectorOz) 
	{
	VectorOx=  Vector1y*Vector2z-Vector1z*Vector2y;
	VectorOy=  Vector1z*Vector2x-Vector1x*Vector2z;
	VectorOz=  Vector1x*Vector2y-Vector1y*Vector2x;
	}

/*
=================
VEC_MultMatrix
=================
*/
void VEC_MultMatrix(	double dxSource, double dySource, double dzSource,
						double mat[4][4],
						double &dxDest, double &dyDest, double &dzDest)
{
    dxDest= dxSource*mat[0][0]+
            dySource*mat[1][0]+
            dzSource*mat[2][0]+
                     mat[3][0];

    dyDest= dxSource*mat[0][1]+
            dySource*mat[1][1]+
            dzSource*mat[2][1]+
                     mat[3][1];

    dzDest= dxSource*mat[0][2]+
            dySource*mat[1][2]+
            dzSource*mat[2][2]+
                     mat[3][2];
}


/* 
 *  This function does Gaussian elimination on the working matrix until 
 *  the left half of it is equivalent to the identity matrix, or as long 
 *  as a solution is possible.  If no solution is possible, FALSE is 
 *  returned through sol_existsp.  (modified from text - pg. 16.31) 
 */ 


static void inverse(	double w[N][2*N],        /* input/output - the working matrix */ 
				int    *sol_existsp)     /* output - flag indicating whether or not a */ 
                          /*             solution exists (invertible)  */ 
{ 
  int j,k,            /* element subscripts */ 
      p;              /* row indicator */ 
  double piv_recip,   /* reciprocal of the pivot */ 
       xmult;        /* multiplier for Gaussian elimination */ 
 
  /* Assume solution exists */ 
  *sol_existsp = TRUE; 
 
  /* Begin computing inverse matrix */ 
  for (p = 0; *sol_existsp && p < (N - 1);  ++p) { 
 
       /* Test for possible solution */ 
      if (w[p][p] == 0.0) 
            *sol_existsp = FALSE; 
      else 
            *sol_existsp = TRUE; 
 
      if (*sol_existsp) { 
 
              /* Scale pivot row */ 
            piv_recip = 1.0 / w[p][p]; 
            w[p][p] = 1.0; 
            for (k = (p + 1); k < (2 * N); ++k) 
                   w[p][k] *= piv_recip; 
 
              /* Eliminate rest of column beneath pivot */ 
            for (j = (p + 1); j < N; ++j) { 
                   xmult = -w[j][p]; 
                   w[j][p] = 0; 
                   for (k = (p + 1); k < (2 * N); ++k) 
                         w[j][k] += xmult * w[p][k]; 
            } 
 
               /* Eliminate rest of column above pivot if pivot is in middle of matrix */ 
            for (j = (p - 1); j > -1; --j) { 
                   xmult = -w[j][p]; 
                   w[j][p] = 0; 
                   for (k = (p + 1); k < (2 * N); ++k) 
                         w[j][k] += xmult * w[p][k]; 
            } 
      } 
  } 
 
  if (*sol_existsp) { 
 
       /* Scale Nth row */ 
      piv_recip = 1.0 / w[N - 1][N - 1]; 
      w[N - 1][N - 1] = 1.0; 
      for (k = N; k < (2 * N); ++k) 
            w[N - 1][k] *= piv_recip; 
 
       /* Complete inverse matrix - eliminate column above pivot */ 
      p = (N - 1); 
      for (j = (p - 1); j > -1; --j) { 
            xmult = -w[j][p]; 
            w[j][p] = 0; 
            for (k = N; k < (2 * N); ++k) 
                   w[j][k] += xmult * w[p][k]; 
      } 
  } 
} 
 

void C3DTransformation::PointZoom(CPoint &cPoint, double dZoom)
	{
	double dX, dY, dZ;
	CPoint cNewPoint;

	// Set a sane limit to the zooms
	if( m_dXScale>500000. && dZoom>1.) return;

	// Get the world location of the point to zoom
	LogicalToWorld(cPoint, dX, dY, dZ);

	// Do a temporary scale, then check the new local point
	Push();
	Scale( dZoom, dZoom, dZoom );

	// Find the new logical point location after the zoom.
	WorldToLogical(cNewPoint, dX, dY, dZ);
	Pop();

	SetScale( dZoom, dZoom, dZoom );

	SetTranslation( -(cNewPoint.x-cPoint.x),
					-(cNewPoint.y-cPoint.y),
					0);

	}


bool C3DTransformation::IsViewOrthogonal()
{
   return m_dTransformationMatrix[0][1]==0.0  &&
		  m_dTransformationMatrix[0][2]==0.0  &&
		  m_dTransformationMatrix[1][2]==0.0  ;
}

void C3DTransformation::ResetTransformationMatrix()
{
	MAT_Identity(m_dTransformationMatrix);

	// Because in Windows, the Y axis is positive downward.
	// If we will change the sign of the 2*2 element, this will invert the Y axis correctly.
	m_dTransformationMatrix[1][1]= -m_dTransformationMatrix[1][1];


}

const CPoint C3DTransformation::WorldToLogical(double fWorldX, double fWorldY,  double fWorldZ)
{
	CPoint cPoint;
	WorldToLogical(cPoint, fWorldX, fWorldY,  fWorldZ);
	return cPoint;

}

void C3DTransformation::GetScale(double &sx, double &sy, double &sz)
{
	sx=m_dXScale;
	sy=m_dYScale;
	sz=m_dZScale;
}

void C3DTransformation::GetTranslations(double &tx, double &ty, double &tz)
{
	tx=m_dXTrans;
	ty=m_dYTrans;
	tz=m_dZTrans;
}

void C3DTransformation::GetRotations(double &rx, double &ry, double &rz)
{
	rx=m_dXRot;
	ry=m_dYRot;
	rz=m_dZRot;
/*
	double dX, dY, dZ, dXO, dYO, dZO, dXC, dYC, dZC, dXC2, dYC2, dZC2;
	double dCrossLen, dAng;

	VEC_MultMatrix(	0, 0, 0,
					m_dTransformationMatrix,
					dXO, dYO, dZO);

	// Find the angles by crossing each rotated axis with each primary axis

	//=================GET X Rotation ===================

	// Get y unit vector in transformed space
	VEC_MultMatrix(	0, 1, 0,
					m_dTransformationMatrix,
					dX, dY, dZ);
	dX-=dXO;
	dY-=dYO;
	dZ-=dZO;
	VEC_Normalize(dX, dY, dZ);

	//  Cross the unit vector in transformed space, with the untransformed
	VEC_CrossProduct( 0,    1,   0,
				 	  dX,  dY,  dZ,
					  dXC, dYC, dZC);

	// Get the length or the cross product
	dCrossLen=VEC_DotProduct( dXC, dYC, dZC,
							  dXC, dYC, dZC);

//	dCrossLen=VEC_DotProduct( dXC, dYC, dZC,
//							    0,   0,  1);
	dAng=asin(dCrossLen);
	rx= -(dAng/PI)*180;

	//=================GET Z Rotation ===================

	dCrossLen=VEC_DotProduct( dXC, dYC, dZC,
							    0,   0,  1);
	dAng=asin(dCrossLen);
	rz=(dAng/PI)*180;


	//=================GET Y Rotation ===================

	VEC_MultMatrix(	1, 0, 0,
					m_dTransformationMatrix,
					dX, dY, dZ);
	dX-=dXO;
	dY-=dYO;
	dZ-=dZO;
	VEC_Normalize(dX, dY, dZ);

	//  Cross the unit vector in transformed space, with the untransformed
	VEC_CrossProduct( 1,    0,   0,
				 	  dX,  dY,  dZ,
					  dXC, dYC, dZC);

	// Get the length or the cross product
	dCrossLen=VEC_DotProduct( dXC, dYC, dZC,
							    0,   1,  0);
	dAng=asin(dCrossLen);
	ry=(dAng/PI)*180;
	*/
}


void C3DTransformation::SetScale(double sx, double sy, double sz)
{
	m_dXScale*=sx;
	m_dYScale*=sy;
	m_dZScale*=sz;
	Scale(sx, sy, sz);
}

void C3DTransformation::SetTranslation(double tx, double ty, double tz)
{
	m_dXTrans+=tx; 
	m_dYTrans+=ty;
	m_dZTrans+=tz;
	Translate(tx, ty, tz);

	TRACE("autoscaling Translations: %f %f %f \n", tx, ty, tz);

	if( tx >842150000 )
		{
		Sleep(200);
		return;
		}


}

void C3DTransformation::SetRotation(double rx, double ry, double rz)
{
	m_dXRot+=rx; 
	m_dYRot+=ry;
	m_dZRot+=rz;
	Rotate(rx, ry, rz);
}

void C3DTransformation::Push()
{
   MAT_Copy(m_dTransformationMatrix,m_dTransformationMatrixPush);
}

void C3DTransformation::Pop()
{
   MAT_Copy(m_dTransformationMatrixPush,m_dTransformationMatrix);
}

void C3DTransformation::ScaleInv(double sx, double sy, double sz)
{
   double smat[4][4];
   double smatInv[4][4];
   double mat1[4][4];

   smat[0][0]=sx; smat[0][1]=0;  smat[0][2]=0;  smat[0][3]=0;
   smat[1][0]=0;  smat[1][1]=sy; smat[1][2]=0;  smat[1][3]=0;
   smat[2][0]=0;  smat[2][1]=0;  smat[2][2]=sz; smat[2][3]=0;
   smat[3][0]=0;  smat[3][1]=0;  smat[3][2]=0;  smat[3][3]=1;

   if( !CalculateInverse() ) return;

   MAT_Mult(m_dTransformationMatrixInv,smat,smatInv);
   MAT_Mult(smatInv,m_dTransformationMatrix, mat1);
   MAT_Copy(mat1,m_dTransformationMatrix);
}

BOOL C3DTransformation::CheckRotations(double rx, double ry, double rz)
	{
	bool bvalid=true;

	Push();

	Rotate(rx, ry, rz);

	//  Check that the global Z always has a positive dot with the screen

	double dX, dY, dZ, dXO, dYO, dZO;
	double dDot;

	VEC_MultMatrix(	0, 0, 0,
					m_dTransformationMatrix,
					dXO, dYO, dZO);

	// Get z unit vector in transformed space
	VEC_MultMatrix(	0, 0, 1,
					m_dTransformationMatrix,
					dX, dY, dZ);
	dX-=dXO;
	dY-=dYO;
	dZ-=dZO;

	// Get the length or the cross product
	dDot=VEC_DotProduct(   0, 0, 1,
						  dX, dY, dZ);
	if( dDot<0 ) bvalid=false;


	//  make sure the Y dot Z is always positive

	// Get y unit vector in transformed space
	VEC_MultMatrix(	0, 0, 1,
					m_dTransformationMatrix,
					dX, dY, dZ);
	dX-=dXO;
	dY-=dYO;
	dZ-=dZO;

	// Get the length or the cross product
	dDot= -VEC_DotProduct(   0, 1, 0,
							dX, dY, dZ);
	if( dDot<0 ) bvalid=false;

	Pop();
	return bvalid;
	}

void C3DTransformation::Reset()
	{
	M3D_Init();
	MAT_Identity(m_dTransformationMatrix);
	MAT_Identity(m_dTransformationMatrixPush);
	MAT_Identity(m_dTransformationMatrixInv);

	m_dTransformationMatrix[1][1]= -1;

	m_dXScale=1.;
	m_dYScale=1.;
	m_dZScale=1.;

	m_dXTrans=0; 
	m_dYTrans=0;
	m_dZTrans=0;

	m_dXRot=0; 
	m_dYRot=0;
	m_dZRot=0;
	}

bool IsPointVisible(int cx, int cy,  CPoint &cPoint)
	{
	if ( cPoint.x<0 || cPoint.y<0 || cPoint.x>cx || cPoint.y>cy ) 
		return false;
	else
		return true;
	}

bool IsPointClipped(CPoint &cPoint)
	{
//	if ( cPoint.x<SHRT_MIN || cPoint.y<SHRT_MIN || cPoint.x>SHRT_MAX || cPoint.y>SHRT_MAX ) 
	if ( cPoint.x<-25000 || cPoint.y<-25000 || cPoint.x>25000 || cPoint.y>25000 ) 
		return true;
	else
		return false;
	}

CPoint SafePoint(CPoint &cPoint)
	{
	// Make sure we don't get anywhere near the +/- short integer limit on Win95 & 98 & ME systems
	CPoint cLocal=cPoint;
	cLocal.x=max( cLocal.x, -10000 ); //SHRT_MIN );
	cLocal.x=min( cLocal.x,  10000 ); //SHRT_MAX );
	cLocal.y=max( cLocal.y, -10000 ); //SHRT_MIN );
	cLocal.y=min( cLocal.y,  10000 ); //SHRT_MAX );
	return cLocal;
	}


void DrawClippedLine(CDC *pDC, int cx, int cy, CPoint &cPoint1, CPoint &cPoint2 )
	{

	// Do a quick prelim check 
	if ( (cPoint1.x<0  && cPoint2.x<0 ) ||
		 (cPoint1.x>cx && cPoint2.x>cx) ||
	     (cPoint1.y<0  && cPoint2.y<0 ) ||
		 (cPoint1.y>cy && cPoint2.y>cy)   ) return;

	if( IsPointVisible(cx, cy, cPoint1) && IsPointVisible(cx, cy, cPoint2) )
		{
		// Both inside
		pDC->MoveTo(cPoint1);
		pDC->LineTo(cPoint2);
		}
	else
		{
		CPoint cInsidePoint, cCrossingPoint;
		if( IsPointVisible(cx, cy, cPoint1) )
			cInsidePoint=cPoint1;
		else
			cInsidePoint=cPoint2;

		CPoint cCrossingPoints[2];
		int iPoints=0;
		if( LinesIntersect( cPoint1, cPoint2, CPoint(0,0), CPoint(cx,0), cCrossingPoints[iPoints] ) )
			{
			iPoints++;
			}
		if( LinesIntersect( cPoint1, cPoint2, CPoint(cx,0), CPoint(cx,cy), cCrossingPoints[iPoints] ) )
			{
			iPoints++;
			}
		if( iPoints<2 && LinesIntersect( cPoint1, cPoint2, CPoint(cx,cy), CPoint(0,cy), cCrossingPoints[iPoints] ) )
			{
			iPoints++;
			}
		if( iPoints<2 && LinesIntersect( cPoint1, cPoint2, CPoint(0,cy), CPoint(0,0), cCrossingPoints[iPoints] ) )
			{
			iPoints++;
			}

		if( iPoints==1 ) 
			{
			pDC->MoveTo(cInsidePoint);
			pDC->LineTo(cCrossingPoints[0]);
			}
		else if( iPoints==2 ) 
			{
			pDC->MoveTo(cCrossingPoints[0]);
			pDC->LineTo(cCrossingPoints[1]);
			}

		}

	}


bool LinesIntersect(CPoint &cP1, CPoint &cP2, CPoint &cP3, CPoint &cP4, CPoint& cCrossingPoint  )
	{

//http://www.ncgia.ucsb.edu/~good/176b/p08.html#SEC32.3.3

	double x1=cP1.x;
	double y1=cP1.y; 
	double x2=cP2.x;
	double y2=cP2.y;  
	double u1=cP3.x;
	double v1=cP3.y;
	double u2=cP4.x;
	double v2=cP4.y;
	double xi=0.0,yi=0.0,b1,b2,a1,a2;;

	if (x1!=x2)
		{ 
		b1 = (y2-y1)/(x2-x1); 
		if (u1!=u2)
			{
			b2 = (v2-v1)/(u2-u1); 
			a1 = y1-b1*x1; 
			a2 = v1-b2*u1; 
			if (b1==b2)
				{
				return false;
				}
			else
				{
				xi = - (a1-a2)/(b1-b2); 
				yi = a1+b1*xi; 
				}
			} 
		else
			{
			xi = u1; 
			a1 = y1-b1*x1; 
			yi = a1+b1*xi; 
			}
		}
	else
		{ 
		xi = x1; 
		if (u1!=u2) 
			{
			b2 = (v2-v1)/(u2-u1); 
			a2 = v1-b2*u1; 
			yi = a2+b2*xi; 
			}
		else
			{
			return false;
			} 
		} 

		double dToler= -.0001;
		if (	   (x1-xi)*(xi-x2)>= dToler
				&& (u1-xi)*(xi-u2)>= dToler
				&& (y1-yi)*(yi-y2)>= dToler
				&& (v1-yi)*(yi-v2)>= dToler )
			{
			cCrossingPoint.x=(long)xi;
			cCrossingPoint.y=(long)yi;
			return true;
			}


		return false;
	}
