// Light shader.h
// Basic single light shader setup
#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_

#include "DXF.h"

using namespace std;
using namespace DirectX;


class ShadowShader : public BaseShader
{
private:
	static const int MAX_LIGHTS = 2;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView[MAX_LIGHTS];
		XMMATRIX lightProjection[MAX_LIGHTS];
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[MAX_LIGHTS];
		XMFLOAT4 diffuse[MAX_LIGHTS];
		XMFLOAT4 direction[MAX_LIGHTS];
	};

public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap[], Light* lights[]);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow[MAX_LIGHTS];
	ID3D11Buffer* lightBuffer;
};

#endif