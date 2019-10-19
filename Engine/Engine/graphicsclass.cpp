////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


//코드의 문제점
//1. 모델 갯수 변수를 한번에 변경 못함 -> vector를 사용한 수정 필요
//2. 변환 백터 부분이 너무 복잡함 따라서 간략화 필요

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Text = 0;
	m_Input = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, InputClass* input)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	m_Input = input;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -350.0f);

	// Create the model object.
	for (int i = 0; i < 5; i++)
	{
		m_Model[i] = new ModelClass;

		if (!m_Model)
		{
			return false;
		}
	}



	// Initialize the model object.
	result = m_Model[0]->Initialize(m_D3D->GetDevice(), "../Engine/Dog.obj", L"../Engine/data/Dog_diffuse.dds");
	result = m_Model[1]->Initialize(m_D3D->GetDevice(), "../Engine/Cat.obj", L"../Engine/data/Cat_diffuse.dds");
	result = m_Model[2]->Initialize(m_D3D->GetDevice(), "../Engine/Lamp.obj", L"../Engine/data/lamp.dds");
	result = m_Model[3]->Initialize(m_D3D->GetDevice(), "../Engine/Skull.obj", L"../Engine/data/Skull.dds");
	result = m_Model[4]->Initialize(m_D3D->GetDevice(), "../Engine/table.obj", L"../Engine/data/table.dds");


	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/seafloor.dds", screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	for (int i = 0; i < 5; i++)
	{
		if (m_Model[i])
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	//Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	return;
}


bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;
	static float rotation = 0.0f;
	int dir = 1;
	int deltaX, deltaY;
	static float delay = 0.0f;

	// Run the frame processing for the particle system.
	m_Input->GetMouseDeltaPosition(deltaX, deltaY);
	m_Camera->Yaw(deltaX * frameTime * 0.00018f);
	m_Camera->Pitch(deltaY * frameTime * 0.00018f);

	
	if (m_Input->GetKey(KeyCode::W)) m_Camera->MoveForward(dir * frameTime);
	if (m_Input->GetKey(KeyCode::A)) m_Camera->Strafe(-dir * frameTime);
	if (m_Input->GetKey(KeyCode::S)) m_Camera->MoveForward(-dir * frameTime);
	if (m_Input->GetKey(KeyCode::D)) m_Camera->Strafe(dir * frameTime);


	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
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
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//D3DXMatrixRotationY(&worldMatrix, rotation);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();


	//Render the model using the texture shader.ceiling
	for (int i = 0; i < 4; i++)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Model[i]->Render(m_D3D->GetDeviceContext());


		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), m_Matrix[i], viewMatrix, projectionMatrix,
			m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		
		if (!result)
		{
			return false;
		}
	}



	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}




	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}