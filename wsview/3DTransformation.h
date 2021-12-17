//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// 3DTransformation.h: interface for the C3DTransformation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DTRANSFORMATION_H__BDAC6345_6492_11D4_89F2_00C04F79ECB9__INCLUDED_)
#define AFX_3DTRANSFORMATION_H__BDAC6345_6492_11D4_89F2_00C04F79ECB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C3DTransformation  
{
public:
	void Reset();
	BOOL CheckRotations(double, double, double);
	void ScaleInv(double, double, double);
	void Pop();
	void Push();

	void GetRotations(double &rx, double &ry, double &rz);
	void GetTranslations(double &tx, double &ty, double &tz);
	void GetScale(double &sx, double &sy, double &sz);

	void SetTranslation(double, double, double );
	void SetScale(double, double, double);
	void SetRotation(double rx, double ry, double rz);

	const CPoint WorldToLogical( double fWorldX, double fWorldY, double fWorldZ);
	void ResetTransformationMatrix();
	bool IsViewOrthogonal();

	void SetAbsoluteRotation( int, int, int );
	void PointZoom( CPoint &cPoint, double dZoom);
	bool CalculateInverse();
	void AutoScale( double, double, double, double, int, int, double dHomeLat=0, double dZScale=.012  );
	void WorldToLogical(CPoint &cPoint, double fX, double fY, double fZ);
	bool LogicalToWorld(CPoint cPoint, double &fX, double &fY, double &fZ);

	virtual ~C3DTransformation();

	C3DTransformation( );



private:
	double m_dXScale,	m_dYScale, m_dZScale;
	double m_dXTrans,	m_dYTrans, m_dZTrans;
	double m_dXRot,		m_dYRot, m_dZRot;

	void C3DTransformation::Translate(double tx,double ty,double tz);
	void C3DTransformation::Scale(double sx,double sy, double sz);
	void C3DTransformation::Rotate(double ax, double ay, double az);

	double  m_dTransformationMatrix[4][4];
	double  m_dTransformationMatrixPush[4][4];
	double  m_dTransformationMatrixInv[4][4];

};

bool IsPointVisible(int cx, int cy,  CPoint &cPoint);
bool IsPointClipped(CPoint &cPoint);
bool LinesIntersect(CPoint &cP1, CPoint &cP2, CPoint &cP3, CPoint &cP4, CPoint& cCrossingPoint  );
void DrawClippedLine(CDC *pDC, int cx, int cy, CPoint &cPoint1, CPoint &cPoint2 );
CPoint SafePoint(CPoint &cPoint);

//============================================================================
//  Math utility headers

//General 3D Mathematics
void M3D_Init();

//Matrices
void MAT_Copy(double source[4][4], double dest[4][4]);
void MAT_Mult(double mat1[4][4], double mat2[4][4], double dest[4][4]);
void MAT_Identity(double mat[4][4]);
void VEC_MultMatrix(	double dxSource, double dySource, double dzSource,
						double mat[4][4],
						double &dxDest, double &dyDest, double &dzDest);
double  VEC_DotProduct(double v1x, double v1y, double v1z,
					   double v2x, double v2y, double v2z );
void VEC_Normalize(double &v1x, double &v1y, double &v1z);

void VEC_CrossProduct(	double Vector1x,double Vector1y,double Vector1z, 
						double Vector2x,double Vector2y,double Vector2z, 
						double &VectorOx,double &VectorOy,double &VectorOz);


#endif // !defined(AFX_3DTRANSFORMATION_H__BDAC6345_6492_11D4_89F2_00C04F79ECB9__INCLUDED_)

