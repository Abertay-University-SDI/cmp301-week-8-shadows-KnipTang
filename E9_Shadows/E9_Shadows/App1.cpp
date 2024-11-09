// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), 300,300, -screenWidth/2.5, screenHeight/3);
	orthoMesh2 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), 300, 300, +screenWidth / 2.5, screenHeight / 3);
	model = new AModel(renderer->getDevice(), "res/teapot.obj");
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sunMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());


	textureMgr->loadTexture(L"brick", L"res/brick1.dds");

	// initial shaders
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	// Variables for defining shadow map
	int shadowmapWidth = 1024 *10;
	int shadowmapHeight = 1024*10;
	int sceneWidth = 100;
	int sceneHeight = 100;

	// This is your shadow map
	shadowMap = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	shadowMap2 = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);

	// Configure directional light
	light = new Light();
	light->setAmbientColour(0.8f, 0.3f, 0.3f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(lampDir[0], lampDir[1], lampDir[2]);
	light->setPosition(lampPos[0], lampPos[1], lampPos[2]);
	light->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	light2 = new Light();
	light2->setAmbientColour(0.3f, 0.3f, 0.8f, 1.0f);
	light2->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light2->setDirection(lampDir2[0], lampDir2[1], lampDir2[2]);
	light2->setPosition(lampPos2[0], lampPos2[1], lampPos2[2]);
	light2->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{

	// Perform depth pass
	depthPass();
	// Render scene
	finalPass();

	return true;
}

void App1::depthPass()
{
	// Set the render target to be the render to texture.
	shadowMap->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the camera and d3d objects.
	light->generateViewMatrix();
	XMMATRIX lightViewMatrix = light->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render model
	model->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(5.f, 2.f, 5.f);
	scaleMatrix = XMMatrixScaling(5.f, 5.f, 0.5f);
	XMMATRIX rotationMatrix = XMMatrixRotationX(rotation);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, rotationMatrix);
	cubeMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());


	shadowMap2->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the camera and d3d objects.
	light2->generateViewMatrix();
	lightViewMatrix = light2->getViewMatrix();
	lightProjectionMatrix = light2->getOrthoMatrix();
	worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render model
	model->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(5.f, 2.f, 5.f);
	scaleMatrix = XMMatrixScaling(5.f, 5.f, 0.5f);
	rotationMatrix = XMMatrixRotationX(rotation);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, rotationMatrix);
	cubeMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());


	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.0f, 0.58f, 0.92f, 1.0f);
	camera->update();

	ID3D11ShaderResourceView* depthMapsArr[] = { shadowMap->getDepthMapSRV(), shadowMap2->getDepthMapSRV() };

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	worldMatrix = XMMatrixTranslation(lampPos[0], lampPos[1], lampPos[2]);
	sunMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"brick"));
	textureShader->render(renderer->getDeviceContext(), sunMesh->getIndexCount());

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture(L"brick"), depthMapsArr, light);
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Render model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	model->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), depthMapsArr, light);
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(5.f, 2.f, 5.f);
	scaleMatrix = XMMatrixScaling(5.f, 5.f, 0.5f);
	XMMATRIX rotationMatrix = XMMatrixRotationX(rotation);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, rotationMatrix);
	cubeMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), depthMapsArr, light);
	shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	renderer->setZBuffer(false);
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();
	worldMatrix = XMMatrixIdentity();

	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap->getDepthMapSRV());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	orthoMesh2->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap2->getDepthMapSRV());
	textureShader->render(renderer->getDeviceContext(), orthoMesh2->getIndexCount());
	renderer->setZBuffer(true);

	gui();
	renderer->endScene();
}



void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	ImGui::SliderFloat3("lamp dir", lampDir, -1, 1);
	ImGui::SliderFloat3("lamp pos", lampPos, -50, 50);

	light->setDirection(lampDir[0], lampDir[1], lampDir[2]);
	light->setPosition(lampPos[0], lampPos[1], lampPos[2]);

	if (rotation < 360)
		rotation+= 0.001f;
	else
		rotation = 0;

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

