#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_d3d = 0;
	m_camera = 0;
	m_model = 0;
	m_colorShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other) 
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_d3d = new D3DClass;
	
	if (!m_d3d)
		return false;

	result = m_d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_camera = new CameraClass;
	if (!m_camera)
		return false;
	
	///Set initial position of camera
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_model = new ModelClass;
	if (!m_model)
		return false;

	result = m_model->Initialize(m_d3d->GetDevice());
	if (!result) 
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_colorShader = new ColorShaderClass;
	if (!m_colorShader)
		return false;

	result = m_colorShader->Initialize(m_d3d->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize color shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	///Release the color shader object.
	if (m_colorShader)
	{
		m_colorShader->Shutdown();
		delete m_colorShader;
		m_colorShader = 0;
	}

	///Release the model object.
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}

	///Release the camera object.
	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	if (m_d3d)
	{
		m_d3d->Shutdown();
		delete m_d3d;
		m_d3d = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
		return false;
	
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	m_d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	///Generates the view matrix based on camera pos
	m_camera->Render();

	m_camera->GetViewMatrix(viewMatrix);
	m_d3d->GetWorldMatrix(worldMatrix);
	m_d3d->GetProjectionMatrix(projectionMatrix);
	
	///Put model vertex and index buffers on graphics pipeline to prepare for drawing
	m_model->Render(m_d3d->GetDeviceContext());

	result = m_colorShader->Render(m_d3d->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	m_d3d->EndScene();

	return true;
}