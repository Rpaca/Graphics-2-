////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "convert.h"
GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model[8] = 0;
	m_LightShader = 0;
	m_Light = 0;
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


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -350.0f);
	
	// Create the model object.
	for (int i = 0; i < 8; i++)
	{
		m_Model[i] = new ModelClass;

		if (!m_Model)
		{
			return false;
		}
	}


	// Initialize the model object.
	result = m_Model[0]->Initialize(m_D3D->GetDevice(), L"../Engine/data/woodFloor.dds");
	result = m_Model[1]->Initialize(m_D3D->GetDevice(), L"../Engine/data/wall(1).dds");
	m_Model[2] = m_Model[1];
	m_Model[3] = m_Model[1];
	result = m_Model[4]->Initialize(m_D3D->GetDevice(), L"../Engine/data/seafloor.dds");
	result = m_Model[5]->Initialize(m_D3D->GetDevice(), "../Engine/Lamp.obj", L"../Engine/data/lamp.dds");
	result = m_Model[6]->Initialize(m_D3D->GetDevice(), "../Engine/Skull.obj", L"../Engine/data/Skull.dds");
	result = m_Model[7]->Initialize(m_D3D->GetDevice(), "../Engine/Dog.obj", L"../Engine/data/Dog_diffuse.dds");


	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -4.5f, 4.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	for (int i = 0; i < 7; i++)
	{
		if (m_Model[i])
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXMATRIX lampMatrixR, skullMatrixR, dogMatrixR, floorMatrixR, LeftWallMatrixR, MiddleWallMatrixR, RightWallMatrixR, CeilingMatrixR;
	D3DXMATRIX dogMatrixT, catMatrixT, lampMatrixT, floorMatrixT, LeftWallMatrixT, MiddleWallMatrixT, RightWallMatrixT, CeilingMatrixT;
	D3DXMATRIX floorMatrixS, CeilingMatrixS, LeftWallMatrixS, RightWallMatrixS, MiddleWallMatrixS;
	D3DXMATRIX m_Matrix[8];

	D3DXMatrixScaling(&floorMatrixS, 2, 2, 2);
	D3DXMatrixRotationX(&floorMatrixR, 1.6);
	D3DXMatrixTranslation(&floorMatrixT, 0, -100, -10);
	m_Matrix[0] = floorMatrixS * floorMatrixR * floorMatrixT;

	D3DXMatrixScaling(&LeftWallMatrixS, 2, 2, 2);
	D3DXMatrixRotationY(&LeftWallMatrixR, -1.2);
	D3DXMatrixTranslation(&LeftWallMatrixT, -100, 0, -10);
	m_Matrix[1] = LeftWallMatrixS * LeftWallMatrixR * LeftWallMatrixT;

	D3DXMatrixScaling(&MiddleWallMatrixS, 2, 2, 2);
	D3DXMatrixRotationY(&MiddleWallMatrixR, 0);
	D3DXMatrixTranslation(&MiddleWallMatrixT, 0, 0, 0);
	m_Matrix[2] = MiddleWallMatrixS * MiddleWallMatrixR * MiddleWallMatrixT;

	D3DXMatrixScaling(&RightWallMatrixS, 2, 2, 2);
	D3DXMatrixRotationY(&RightWallMatrixR, 1.2);
	D3DXMatrixTranslation(&RightWallMatrixT, 100, 0, -10);
	m_Matrix[3] = RightWallMatrixS * RightWallMatrixR * RightWallMatrixT;

	D3DXMatrixScaling(&CeilingMatrixS, 2, 2, 2);
	D3DXMatrixRotationX(&CeilingMatrixR, -1.6);
	D3DXMatrixTranslation(&CeilingMatrixT, 0, 100, -10);
	m_Matrix[4] = CeilingMatrixS * CeilingMatrixR * CeilingMatrixT;

	D3DXMatrixTranslation(&m_Matrix[5], -83.2f, -110.0f, -50.0f); // lamp
	D3DXMatrixTranslation(&m_Matrix[6], +40.2f, -100.3f, -40.0f); // skull
	D3DXMatrixTranslation(&m_Matrix[7], -0, -110, -40.0f); // dog


	bool result;



	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);



	//Render the model using the texture shader.ceiling
	for (int i = 0; i < 8; i++)
	{
		m_Model[i]->Render(m_D3D->GetDeviceContext());

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), m_Matrix[i], viewMatrix, projectionMatrix,
			m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}