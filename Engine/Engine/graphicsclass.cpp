////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PCH.h"
#include "graphicsclass.h"


//개선 포인트
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
	playerPoint =0;
	EnmeyPoint = 0;
	OnState = OnPlayer;
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


	//m_gunShot = new Sound;
	//if (!m_gunShot)
	//{
	//	return false;
	//}
	//result = m_gunShot->Initialize(hwnd, "../Engine/data/gunshot.wav");
	//if (!result)
	//{
	//	return false;
	//}


	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, 0.0f);

	const int NumOfModel = 7;

	WCHAR*	models[NumOfModel] = {
		L"../Engine/data/Stone.obj",
		L"../Engine/data/Knight.obj",
		L"../Engine/data/Monster.obj",
		L"../Engine/data/CircularGrass.obj",
		L"../Engine/data/FantasyHouse.obj",
		L"../Engine/data/Chest.obj",
		L"../Engine/data/tree.obj",
	};
	WCHAR* modelTextures[NumOfModel] = {
		L"../Engine/data/Stone.dds",
		L"../Engine/data/Pillar.dds",
		L"../Engine/data/Pillar.dds",
		L"../Engine/data/CircularGrass.jpg",
		L"../Engine/data/FantasyHouse.dds",
		L"../Engine/data/Chest.dds",
		L"../Engine/data/tree.jpg",
	};

	D3DXVECTOR3 positions[] = {
		{ 0.0f, 0.0f, 0.0f},
		{ 200.0f, 8.0f, 0.0f},
		{ 200.0f, 8.0f, 400.0f},
		{ 0.0f, -50.0f, 0.0f},
		{ 500.0f, 20.0f, 200.0f},
		{ -50.0f, 0.0f, 0.0f},
		{ -50.0f, 0.0f, -50.0f},
	};

	D3DXVECTOR3 scales[] = {
		{ 0.5f, 0.5f, 0.5f},
		{ 5.5f, 5.5f, 5.5f},
		{ 8.5f, 5.5f, 8.5f},
		{ 5.0f, 5.0f, 5.0f},
		{ 10.0f, 10.0f, 10.0f},
		{ 0.1f, 0.1f, 0.1f},
		{ 0.5f, 0.5f, 0.5f},
	};

	float Rotation[] = {
		0.0f, 180.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
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
		m_Models.push_back(newModel);
	}
	// Create the model transform.
	for (int i = 0; i < 9; ++i)
	{
		for (int i = 0; i < 9; ++i)
		{
			m_D3D->GetWorldMatrix(objMat);

			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixTranslation(&objMat, positions[0].x, positions[0].y, positions[0].z);
			D3DXMatrixScaling(&scaleMat, scales[0].x, scales[0].y, scales[0].z);
			D3DXMatrixRotationY(&rotationMat, Rotation[0] * (PI / 180));
			D3DXMatrixMultiply(&rotationMat, &scaleMat, &rotationMat);
			D3DXMatrixMultiply(&objMat, &rotationMat, &objMat);

			m_objMatrices.push_back(objMat);
			positions[0] += {0.0f, 0.0f, 50.0f};
		}
		positions[0].z = 0.0f;
		positions[0] += {50.0f, 0.0f, 0.0f};
	}

	for (int i = 1; i < NumOfModel; ++i)
	{
		m_D3D->GetWorldMatrix(objMat);

		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixTranslation(&objMat, positions[i].x, positions[i].y, positions[i].z);
		D3DXMatrixScaling(&scaleMat, scales[i].x, scales[i].y, scales[i].z);
		D3DXMatrixRotationY(&rotationMat, Rotation[i] * (PI / 180));
		D3DXMatrixMultiply(&rotationMat, &scaleMat, &rotationMat);
		D3DXMatrixMultiply(&objMat, &rotationMat, &objMat);
		m_objMatrices.push_back(objMat);

		m_Models[i]->scalingCollison(scales[i]);
		m_Models[i]->rotationCollison(Rotation[i]);
		m_Models[i]->updateColliosnPos(positions[i]);
		m_Models[i]->getTransform(positions[i], Rotation[i], scales[i]);
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

	m_Camera->SetPosition(-110.0f, 80.0f, 0.0f);
	m_Camera->Yaw(1.5708f);
	m_Camera->Pitch(0.75f);


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
	D3DXMATRIX forward;


	// Run the frame processing for the particle system.
	m_Input->GetMouseDeltaPosition(deltaX, deltaY);
	m_Camera->Yaw(deltaX * frameTime * 0.00018f);
	m_Camera->Pitch(deltaY * frameTime * 0.00018f);


	//공 이동
	//D3DXVECTOR3 A = {m_Models[1]->speed*m_Models[1]->forward.x, m_Models[1]->speed*m_Models[1]->forward.y,m_Models[1]->speed* m_Models[1]->forward.z };
	//D3DXMatrixTranslation(&forward, m_Models[1]->speed*m_Models[1]->forward.x, m_Models[1]->forward.y, m_Models[1]->forward.z);
	//D3DXMatrixMultiply(&m_objMatrices[1], &m_objMatrices[1], &forward);
	//m_Models[1]->translateCollison(A);
	//m_Models[1]->updateColliosnPos(A);

	//몬스터 ai
	//float random = rand() % 3 -1;
	//m_objMatrices[7] = m_Models[7]->translatePosition({ m_Models[7]->position.x, m_Models[7]->position.y , (float)m_Models[1]->position.z*0.9f});

	//if (m_Models[7]->position.z > m_Models[1]->position.z + 1)
	//{
	//	D3DXMATRIX objMat;
	//	D3DXVECTOR3 position = { 0.0f, 0.0f, -0.3f };//left
	//	D3DXMatrixTranslation(&objMat, 0.0f, 0.0f, -0.3f);
	//	D3DXMatrixMultiply(&m_objMatrices[7], &m_objMatrices[7], &objMat);
	//	m_Models[7]->updateColliosnPos(position);
	//}
	//else if (m_Models[7]->position.z < m_Models[1]->position.z - 1)
	//{
	//	D3DXMATRIX objMat;
	//	D3DXVECTOR3 position = { 0.0f, 0.0f, 0.3f };//left
	//	D3DXMatrixTranslation(&objMat, 0.0f, 0.0f, 0.3f);
	//	D3DXMatrixMultiply(&m_objMatrices[7], &m_objMatrices[7], &objMat);
	//	m_Models[7]->updateColliosnPos(position);
	//}

	////오브젝트 충돌
	//if (m_Models[1]->AABBToAABB(m_Models[2]))//L
	//{
	//	m_Models[1]->reflect({0,0,1});
	//	m_gunShot->PlayWaveFile(-2000, false);
	//}

	////오브젝트 충돌
	//if (m_Models[1]->AABBToAABB(m_Models[3]))//R
	//{
	//	m_Models[1]->reflect({ 0,0,-1 });
	//	m_gunShot->PlayWaveFile(-2000, false);
	//}

	////오브젝트 충돌
	//if (m_Models[1]->AABBToAABB(m_Models[4]))//B
	//{
	//	m_Models[1]->reflect({ 1,0,0 });
	//	m_gunShot->PlayWaveFile(-2000, false);
	//	EnmeyPoint++;
	//	m_objMatrices[1] = m_Models[1]->resetSetting();
	//	m_objMatrices[7] = m_Models[7]->translatePosition({ m_Models[7]->position.x, m_Models[7]->position.y , (float)m_Models[1]->position.z });
	//}

	////오브젝트 충돌
	//if (m_Models[1]->AABBToAABB(m_Models[5]))//F
	//{
	//	m_Models[1]->reflect({ -1,0,0 });
	//	m_gunShot->PlayWaveFile(-2000, false);
	//	playerPoint++;
	//	m_objMatrices[1] = m_Models[1]->resetSetting();
	//	m_objMatrices[7] = m_Models[7]->translatePosition({ m_Models[7]->position.x, m_Models[7]->position.y , (float)m_Models[1]->position.z });
	//}


	////오브젝트 충돌
	//if (m_Models[1]->AABBToAABB(m_Models[6]))//나
	//{
	//	float x;
	//	x = -2 + rand() % 4;
	//	x = x * 0.3f;

	//	m_Models[1]->reflect({ 1,0,0 });
	//	m_Models[1]->forward += {0, 0, x};

	//	m_gunShot->PlayWaveFile(-2000, false);
	//}

	////오브젝트 충돌
	//if (m_Models[1]->AABBToAABB(m_Models[7]))//적
	//{
	//	float x;
	//	x = -2 + rand() % 3;
	//	x = x * 0.3f;

	//	m_Models[1]->reflect({ -1,0,0 });
	//	m_Models[1]->forward += {0,0,x};

	//	
	//	m_gunShot->PlayWaveFile(-2000, false);
	//}



	if (m_Input->GetKey(KeyCode::W)) m_Camera->MoveForward(dir * frameTime);
	if (m_Input->GetKey(KeyCode::A)) m_Camera->Strafe(-dir * frameTime);
	if (m_Input->GetKey(KeyCode::S)) m_Camera->MoveForward(-dir * frameTime);
	if (m_Input->GetKey(KeyCode::D)) m_Camera->Strafe(dir * frameTime);

	if (m_Input->GetKeyDown(KeyCode::ENTER))
	{
		if (OnState == OnPlayer)
			OnState = OnEnumy;
		else
			OnState = OnPlayer;
	}

	if (m_Input->GetKeyDown(KeyCode::LEFTARROW))
	{
		D3DXMATRIX objMat;
		D3DXVECTOR3 position = { -50.0f, 0.0f, 0.0f };
		int num;
		D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);
		if (OnState == OnPlayer)
			num = 1;
		else
			num = 2;
		D3DXMatrixMultiply(&m_objMatrices[num + 80], &m_objMatrices[num + 80], &objMat);
		m_Models[num]->updateColliosnPos(position);
	}

	if (m_Input->GetKeyDown(KeyCode::RIGHTARROW))
	{
		D3DXMATRIX objMat;
		D3DXVECTOR3 position = { 50.0f, 0.0f, 0.0f };
		int num;
		D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);
		if (OnState == OnPlayer)
			num = 1;
		else
			num = 2;
		D3DXMatrixMultiply(&m_objMatrices[num + 80], &m_objMatrices[num + 80], &objMat);
		m_Models[num]->updateColliosnPos(position);
	}
	if (m_Input->GetKeyDown(KeyCode::UPARROW))
	{
		D3DXMATRIX objMat;
		D3DXVECTOR3 position = { 0.0f, 0.0f, 50.0f };
		int num;
		D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);
		if (OnState == OnPlayer)
			num = 1;
		else
			num = 2;
		D3DXMatrixMultiply(&m_objMatrices[num + 80], &m_objMatrices[num + 80], &objMat);
		m_Models[num]->updateColliosnPos(position);
	}
	if (m_Input->GetKeyDown(KeyCode::DOWNARROW))
	{
		D3DXMATRIX objMat;
		D3DXVECTOR3 position = { 0.0f, 0.0f, -50.0f };
		int num;
		D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);
		if (OnState == OnPlayer)
			num = 1;
		else
			num = 2;
		D3DXMatrixMultiply(&m_objMatrices[num + 80], &m_objMatrices[num + 80], &objMat);
		m_Models[num]->updateColliosnPos(position);
	}


	//if (m_Input->GetKey(KeyCode::RIGHTARROW) && playerPosZ > -100)
	//{
	//	D3DXMATRIX objMat;
	//	D3DXVECTOR3 position = { 0.0f, 0.0f, -0.7f };
	//	D3DXMatrixTranslation(&objMat, 0.0f, 0.0f, -0.7f);
	//	//D3DXMatrixMultiply(&m_objMatrices[6], &m_objMatrices[6], &objMat);
	//	playerPosZ--;
	//	//m_Models[6]->translateCollison(position);
	//	m_Models[6]->updateColliosnPos(position);
	//}




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


	result = m_Text->SetScore(playerPoint, EnmeyPoint,m_D3D->GetDeviceContext());
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
	//m_Models[0]->Render(m_D3D->GetDeviceContext());


	// Render the model using the light shader.
	/*result = m_PointLightShader->Render(m_D3D->GetDeviceContext(), m_Models[0]->GetIndexCount(), m_objMatrices[0], viewMatrix, projectionMatrix,
	m_Models[0]->GetTexture(), m_PointLight->GetDiffuseColor(), m_PointLight->GetPosition());
	if (!result)
	{
		return false;
	}*/


	//Render the model using the texture shader.ceiling
	//for (int i = 0; i < m_Models.size(); i++)
	//{
	//	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//	m_Models[i]->Render(m_D3D->GetDeviceContext());


	//	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), m_objMatrices[i], viewMatrix, projectionMatrix,
	//		m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
	//		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	//	if (!result)
	//	{
	//		return false;
	//	}
	//}

	// creat map.
	for (int i = 0; i < 81; i++)
	{
		m_Models[0]->Render(m_D3D->GetDeviceContext());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[0]->GetIndexCount(), m_objMatrices[i], viewMatrix, projectionMatrix,
				m_Models[0]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if (!result)
		{
			return false;
		}
	}

	for (int i = 1; i < m_Models.size(); i++)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Models[i]->Render(m_D3D->GetDeviceContext());

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), m_objMatrices[i+80], viewMatrix, projectionMatrix,
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

