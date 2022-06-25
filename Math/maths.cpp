#include "maths.h"

const float Math_L::Pi = 3.1415926f;

//ƽ�ƾ���
Eigen::Matrix4f Math_L::MatrixSetTranslate(float x, float y, float z)
{
	Eigen::Matrix4f TranslateMatrix;
	TranslateMatrix << 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1;
	return TranslateMatrix;
}
//���ž���
Eigen::Matrix4f Math_L::MatrixSetScale(float x, float y, float z)
{
	Eigen::Matrix4f ScaleMatrix;
	ScaleMatrix <<
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1;
	return ScaleMatrix;
}
//��X����ת����
Eigen::Matrix4f Math_L::MatrixSetRotationX(float angle)
{
	Eigen::Matrix4f RotationXMatrix;
	RotationXMatrix <<
		1, 0, 0, 0,
		0, cos(angle), sin(angle), 0,
		0, -sin(angle), cos(angle), 0,
		0, 0, 0, 1;
	return RotationXMatrix;
}
//��Y����ת����
Eigen::Matrix4f Math_L::MatrixSetRotationY(float angle)
{
	Eigen::Matrix4f RotationYMatrix;
	RotationYMatrix <<
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1;
	return RotationYMatrix;
}
//��Z����ת����
Eigen::Matrix4f Math_L::MatrixSetRotationZ(float angle)
{
	Eigen::Matrix4f RotationZMatrix;
	RotationZMatrix <<
		cos(angle), sin(angle), 0, 0,
		-sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
	return RotationZMatrix;
}
//��ȡ�������������ӽǾ���,view matrix
Eigen::Matrix4f Math_L::MatrixSetLookAt(const Eigen::Vector4f& look, const Eigen::Vector4f& at, const Eigen::Vector4f& up)
{
	Eigen::Vector4f zaxis = (at - look).normalized();
	Eigen::Vector4f xaxis = up.cross(zaxis).normalized();
	Eigen::Vector4f yaxis = zaxis.cross(xaxis);
	Eigen::Matrix4f ViewMatrix;
	ViewMatrix <<
		xaxis[0], yaxis[0], zaxis[0], 0,
		xaxis[1], yaxis[1], zaxis[1], 0,
		xaxis[2], yaxis[2], zaxis[2], 0,
		-xaxis.dot(look), -yaxis.dot(look), -zaxis.dot(look), 1;
	return ViewMatrix;
}
/***********************************************
�ο�D3D11 ����
��ȡͶӰ����  D3DXMatrixPerspectiveFovLH 
	fovY ----vertical field of view  angle in radians
	aspectRatio = width / height
	nearZ -----distance to near plane
	farZ -------distance to far plane
************************************************/
Eigen::Matrix4f Math_L::MatrixPerspectiveFovLH(float fovY, float aspectRatio, float nearZ, float farZ)
{
	Eigen::Matrix4f mat;
	float fac = 1.0f / tan(fovY*0.5f);
	mat <<
		fac / aspectRatio, 0, 0, 0,
		0, fac, 0, 0,
		0, 0, farZ / (farZ - nearZ), 1.0f,
		0, 0, -nearZ * farZ / (farZ - nearZ), 0;
	return mat;
}
//��ɫת��ΪUINT, argb order
UINT Math_L::ColorToUINT(const Eigen::Vector4f& color)
{
	int r = int(255 * color[0]);
	int g = int(255 * color[1]);
	int b = int(255 * color[2]);
	r = Clamp(r, 0, 255);
	g = Clamp(g, 0, 255);
	b = Clamp(b, 0, 255);
	return UINT((r << 16) | (g << 8) | (b));
}
