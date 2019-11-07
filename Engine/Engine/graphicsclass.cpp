////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PCH.h"
#include "graphicsclass.h"


//개선 포인트
//1. sky dome의 한면의 색이 이상하게 출력되는 문제 수정- 완료
//2. 모델들을 하늘과 어울리는 오브젝트로 교체(새나 비행기)
//3. 카메라가 움직일때 skydome이 잠깐 확대되는 현상 수정  - 완료
//4. SKY DOME 메모리 해제 함수 생성
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
	m_numOfPolygons = 0;
	m_Skybox = 0;
	m_PointLightShader = 0;
	m_PointLight = 0;
	playerPosZ = 0; 
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
	m_Camera->SetPosition(-90.0f, 20.0f, 0.0f);

	const int NumOfModel = 4;

	WCHAR*	models[NumOfModel] = {
		L"../Engine/data/Tennis-Court.obj",
		L"../Engine/data/Pokemon.obj",
		L"../Engine/data/ball.obj",
		L"../Engine/data/Pokemon.obj"
	};
	WCHAR* modelTextures[NumOfModel] = {
		L"../Engine/data/TennisCourt.dds",
		L"../Engine/data/Pokemon.dds",
		L"../Engine/data/ball.dds",
		L"../Engine/data/Pokemon.dds"
	};

	D3DXVECTOR3 positions[] = {
		{ 0.0f, 0.0f, 0.0f },
		{ -60.0f, 0.0f, 0.0f},
		{ 0.0f, 2.2f, 0.0f},
		{ 50.0f, 0.2f, 0.0f}
	};

	D3DXVECTOR3 scales[] = {
		{ 1.0f, 1.0f, 1.0f},
		{ 3.2f, 3.2f, 3.2f},
		{ 0.3f, 0.3f, 0.3f},
		{ 7.0f, 7.0f, 7.0f}
	};

	float Rotation[] = {
		0.0f,1.5708f, 0.0f,-1.5708f
	};


	D3DXMATRIX objMat, scaleMat, rotationMat;
	// Create the model object.
	for (int i = 0; i < NumOfModel; ++i)
	{
		ModelClass* newModel = new ModelClass;
		result = newModel->Initialize(m_D3D->GetDevice(), models[i], modelTextures[i]);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_D3D->GetWorldMatrix(objMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixTranslation(&objMat, positions[i].x, positions[i].y, positions[i].z);
		D3DXMatrixScaling(&scaleMat, scales[i].x, scales[i].y, scales[i].z);
		D3DXMatrixRotationY(&rotationMat, Rotation[i]);
		D3DXMatrixMultiply(&rotationMat, &scaleMat, &rotationMat);
		D3DXMatrixMultiply(&objMat, &rotationMat, &objMat);

		m_Models.push_back(newModel);
		m_objMatrices.push_back(objMat);
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


	// Create the light shader object.
	m_PointLightShader = new PointLightShaderClass;
	if (!m_PointLightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_PointLightShader->Initialize(m_D3D->GetDevice(), hwnd);
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
	m_Light->SetAmbientColor(0.50f, 0.50f, 0.50f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-30.0f, -10.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
	



	// Create the light object.
	m_PointLight = new PointLightClass;
	if (!m_PointLight)
	{
		return false;
	}

	// Initialize the light object.
	m_PointLight->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_PointLight->SetPosition(0.0f, 3.0f, 0.0f);





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

	//// Create the bitmap object.
	//m_Bitmap = new BitmapClass;
	//if (!m_Bitmap)
	//{
	//	return false;
	//}
	//// Initialize the bitmap object.
	//result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/seafloor.dds", screenWidth, screenHeight);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
	//	return false;
	//}


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

	m_Skybox = new SkyboxClass;
	if (!m_Skybox)
	{
		return false;
	}

	result = m_Skybox->Initialize(screenWidth, screenHeight, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Skybox object.", L"Error", MB_OK);
		return false;
	}

	m_Camera->Yaw(1.5708f);
	m_Camera->Pitch(0.2f);
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
	for (auto it = m_Models.begin(); it != m_Models.end(); )
	{
		if (*it)
		{
			(*it)->Shutdown();
			delete (*it);
			it = m_Models.erase(it);
		}
		else ++it;
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

	////Release the bitmap object.
	//if (m_Bitmap)
	//{
	//	m_Bitmap->Shutdown();
	//	delete m_Bitmap;
	//	m_Bitmap = 0;
	//}

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


	if (m_Input->GetKey(KeyCode::LEFTARROW)&& playerPosZ<100)
	{
		D3DXMATRIX objMat;
		D3DXMatrixTranslation(&objMat, 0.0f, 0.0f, 0.2f);
		D3DXMatrixMultiply(&m_objMatrices[1], &m_objMatrices[1], &objMat);
		playerPosZ++;
	}


	if (m_Input->GetKey(KeyCode::RIGHTARROW) && playerPosZ > -100)
	{
		D3DXMATRIX objMat;
		D3DXMatrixTranslation(&objMat, 0.0f, 0.0f, -0.2f);
		D3DXMatrixMultiply(&m_objMatrices[1], &m_objMatrices[1], &objMat);
		playerPosZ--;
	}



	// Set the frames per second.
	result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetNumOfObjects(m_Models.size(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	m_numOfPolygons = 0;
	for (auto model : m_Models) m_numOfPolygons += model->GetPolygonsCount();
	result = m_Text->SetNumOfPolygons(m_numOfPolygons, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	D3DXVECTOR3 position = m_Camera->GetPosition();
	float height;

	result = m_Text->SetPosition(position, m_D3D->GetDeviceContext());
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

	m_Skybox->UpdatePos(position);

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX lampMatrixR, skullMatrixR, dogMatrixR, floorMatrixR, LeftWallMatrixR, MiddleWallMatrixR, RightWallMatrixR, CeilingMatrixR;
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

	//// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	//if (!result)
	//{
	//	return false;
	//}

	//// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}
		// Set the frames per second.

	m_Skybox->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);



	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Models[0]->Render(m_D3D->GetDeviceContext());


	// Render the model using the light shader.
	result = m_PointLightShader->Render(m_D3D->GetDeviceContext(), m_Models[0]->GetIndexCount(), m_objMatrices[0], viewMatrix, projectionMatrix,
	m_Models[0]->GetTexture(), m_PointLight->GetDiffuseColor(), m_PointLight->GetPosition());
	if (!result)
	{
		return false;
	}


	//Render the model using the texture shader.ceiling
	for (int i = 1; i < m_Models.size(); i++)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Models[i]->Render(m_D3D->GetDeviceContext());


		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), m_objMatrices[i], viewMatrix, projectionMatrix,
			m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
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

