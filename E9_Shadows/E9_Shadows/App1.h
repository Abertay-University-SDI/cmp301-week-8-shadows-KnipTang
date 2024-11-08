// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TextureShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void depthPass();
	void finalPass();
	void gui();

private:
	TextureShader* textureShader;
	PlaneMesh* mesh;
	OrthoMesh* orthoMesh;

	Light* light;
	AModel* model;
	CubeMesh* cubeMesh;
	SphereMesh* sunMesh;
	ShadowShader* shadowShader;
	DepthShader* depthShader;

	float rotation = 5;
	float lampDir[3] = { 0.0f, -0.7f, 0.7f };
	float lampPos[3] = { 0.f, 0.f, -10.f };

	ShadowMap* shadowMap;
};

#endif