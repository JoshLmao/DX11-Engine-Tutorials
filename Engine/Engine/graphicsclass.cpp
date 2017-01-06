#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_d3d = 0;
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
	return true;
}

void GraphicsClass::Shutdown()
{
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
	m_d3d->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_d3d->EndScene();

	return true;
}