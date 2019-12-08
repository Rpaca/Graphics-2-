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

	numOfPlankP = 0;
	numOfPlankM = 0;
	point = 0;
	OnLeft = 0;
	OnRight = 0;
	OnUp = 0;
	OnDown = 0;
	OnPlank = 0;
	lastPosition;
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


	m_manVoice = new Sound;
	if (!m_manVoice)
	{
		return false;
	}
	result = m_manVoice->Initialize(hwnd, "../Engine/data/Footman.wav");
	if (!result)
	{
		return false;
	}

	m_orcVoice = new Sound;
	if (!m_manVoice)
	{
		return false;
	}
	result = m_orcVoice->Initialize(hwnd, "../Engine/data/Chieften.wav");
	if (!result)
	{
		return false;
	}

	m_step = new Sound;
	if (!m_manVoice)
	{
		return false;
	}
	result = m_step->Initialize(hwnd, "../Engine/data/step.wav");
	if (!result)
	{
		return false;
	}


	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, 0.0f);
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	m_Camera->SetPosition(-110.0f, 80.0f, 0.0f);

	const int NumOfModel = 29;

	WCHAR*	models[NumOfModel] = {
		L"../Engine/data/Stone.obj",
		L"../Engine/data/Knight1.obj",
		L"../Engine/data/Monster1.obj",
		L"../Engine/data/CircularGrass.obj",
		L"../Engine/data/FantasyHouse.obj",
		L"../Engine/data/Chest.obj",
		L"../Engine/data/tree.obj",
		L"../Engine/data/War_Tower.obj",
		L"../Engine/data/Portal.obj",

		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",

		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",
		L"../Engine/data/planks.obj",

	};
	WCHAR* modelTextures[NumOfModel] = {
		L"../Engine/data/stone.dds",
		L"../Engine/data/Axe1.dds",
		L"../Engine/data/Monster1.png",
		L"../Engine/data/CircularGrass.jpg",
		L"../Engine/data/FantasyHouse.dds",
		L"../Engine/data/Chest.dds",
		L"../Engine/data/tree.jpg",
		L"../Engine/data/War_Tower.jpeg",
		L"../Engine/data/Portal.png",

		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",	
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",

		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
		L"../Engine/data/planks.dds",
	};

	D3DXVECTOR3 positions[] = {
		{ 0.0f, 0.0f, 0.0f},
		{ 280.0f, 8.0f, 0.0f},
		{ 280.0f, 7.0f, 550.0f},
		{ 200.0f, -50.0f, 200.0f},
		{ 800.0f, 20.0f, 200.0f},
		{ -50.0f, 0.0f, 0.0f},
		{ -50.0f, 0.0f, 150.0f},
		{ 280.0f, 0.0f, -150.0f},
		{ 280.0f, 130.0f, 700.0f},

		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},

		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
		{ 5595.0f, 0.0f, 35.0f},
	};

	D3DXVECTOR3 scales[] = {
		{ 0.5f, 0.5f, 0.5f},
		{ 0.35f, 0.35f, 0.35f},
		{ 15.0f, 15.0f, 15.0f},
		{ 5.0f, 5.0f, 5.0f},
		{ 10.0f, 10.0f, 10.0f},
		{ 0.1f, 0.1f, 0.1f},
		{ 0.5f, 0.5f, 0.5f},
		{ 50.0f, 80.0f, 50.0f},
		{ 2.0f, 2.0f, 2.0f},

		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},

		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
		{ 0.39f, 0.39f, 0.39},
	};

	float Rotation[] = {
		0.0f, 180.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,	0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,	0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
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

	// 바닥 타일 포지션
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
			positions[0] += {70.0f, 0.0f, 0.0f};
		}
		positions[0].x = 0.0f;
		positions[0] += {0.0f, 0.0f, 70.0f};
	}

	// 모델 포지션
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
	m_PointLight->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_PointLight->SetPosition(0.0f, 50.0f, 0.0f);





	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd, baseViewMatrix);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}




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

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/Pillar.dds", 2048, 2048);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
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
	D3DXMATRIX forward;

	// Run the frame processing for the particle system.
	m_Input->GetMouseDeltaPosition(deltaX, deltaY);
	m_Camera->Yaw(deltaX * frameTime * 0.00018f);
	m_Camera->Pitch(deltaY * frameTime * 0.00018f);


	if (m_Input->GetKey(KeyCode::W)) m_Camera->MoveForward(dir * frameTime);
	if (m_Input->GetKey(KeyCode::A)) m_Camera->Strafe(-dir * frameTime);
	if (m_Input->GetKey(KeyCode::S)) m_Camera->MoveForward(-dir * frameTime);
	if (m_Input->GetKey(KeyCode::D)) m_Camera->Strafe(dir * frameTime);

	if (m_Input->GetKeyDown(KeyCode::ENTER) && point == 0)
	{

		if (OnState == OnPlayer)
		{
			m_orcVoice->PlayWaveFile(-2000, false);
			OnState = OnEnumy;
		}
		else
		{
			m_manVoice->PlayWaveFile(-2000, false);
			OnState = OnPlayer;
		}

		if (OnPlank)
		{
			if (OnState == OnPlayer)
				numOfPlankP++;
			if (OnState == OnEnumy)
				numOfPlankM++;
			OnPlank = false;
		}
	}

	if (m_Input->GetKeyDown(KeyCode::R))
	{
		int num = 9 + numOfPlankM + numOfPlankP;
		D3DXVECTOR3 position;
		if(OnState == OnPlayer)
			position = m_Models[1]->position;
		if (OnState == OnEnumy)
			position = m_Models[2]->position;
		position += {35.0f, -8.0f, 35.0f};

		if (OnPlank)
		{
			position = { -500.0f, -500.0f, -500.0f };
			m_objMatrices[num + 80] = Translate(m_Models[num], position, m_Models[num]->scale, m_Models[num]->rotation);
			m_Models[num]->position = position;
			m_Models[num]->updateColliosnPosition(position);
			OnPlank = false;
		}
		else
		{
			m_objMatrices[num +  80] = Translate(m_Models[num], position, m_Models[num]->scale, m_Models[num]->rotation);
			m_Models[num]->position = position;
			m_Models[num]->updateColliosnPosition(position);
			OnPlank = true;;
		}
	}


	if (m_Input->GetKeyDown(KeyCode::LSHFIT)&&OnPlank)
	{
		int num = 9 + numOfPlankM + numOfPlankP;
		m_Models[num]->rotation += 90;
		m_objMatrices[num + 80] = Translate(m_Models[num], m_Models[num]->position, m_Models[num]->scale, m_Models[num]->rotation);
		m_Models[num]->rotationCollison(90);
	}


	if (m_Input->GetKeyDown(KeyCode::LEFTARROW) && point == 0)
		MoveObject({ -70.0f, 0.0f, 0.0f });

	if (m_Input->GetKeyDown(KeyCode::RIGHTARROW) && point == 0)
		MoveObject({ 70.0f, 0.0f, 0.0f });

	if (m_Input->GetKeyDown(KeyCode::UPARROW) && point == 0)
		MoveObject({ 0.0f, 0.0f, 70.0f });

	if (m_Input->GetKeyDown(KeyCode::DOWNARROW) && point == 0)
		MoveObject({ 0.0f, 0.0f, -70.0f });


	if (OnLeft)
		MoveCharacter({ -1.0f, 0.0f, 0.0f });

	if (OnRight)
		MoveCharacter({ 1.0f, 0.0f, 0.0f });

	if (OnUp)
		MoveCharacter({ 0.0f, 0.0f, 1.0f });

	if (OnDown)
		MoveCharacter({ 0.0f, 0.0f, -1.0f });



	//오브젝트 충돌
	if (m_Models[1]->AABBToAABB(m_Models[9]))//적
	{
		if (OnUp || OnDown || OnLeft || OnRight)
		{
			D3DXVECTOR3 position = lastPosition;
			point = 0;
			OnUp = false;
			OnDown = false;
			OnLeft = false;
			OnRight = false;
			m_objMatrices[1 + 80] = Translate(m_Models[1], position, m_Models[1]->scale, m_Models[1]->rotation);
			m_Models[1]->translatePosition(position);
		}


	}
	//오브젝트 충돌
	if (m_Models[2]->AABBToAABB(m_Models[9]))//적
	{
		if (OnUp || OnDown || OnLeft || OnRight)
		{
			D3DXVECTOR3 position = lastPosition;
			point = 0;
			OnUp = false;
			OnDown = false;
			OnLeft = false;
			OnRight = false;
			m_objMatrices[2 + 80] = Translate(m_Models[2], position, m_Models[2]->scale, m_Models[2]->rotation);
			m_Models[2]->translatePosition(position);
		}
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

		//if (i >=0 && i <9)
		//{
		//	result = m_PointLightShader->Render(m_D3D->GetDeviceContext(), m_Models[0]->GetIndexCount(), m_objMatrices[i], viewMatrix, projectionMatrix,
		//		m_Models[0]->GetTexture(), m_PointLight->GetDiffuseColor(), m_PointLight->GetPosition());
		//}

		//if (i >= 72 && i < 81)
		//{
		//	result = m_PointLightShader->Render(m_D3D->GetDeviceContext(), m_Models[0]->GetIndexCount(), m_objMatrices[i], viewMatrix, projectionMatrix,
		//		m_Models[0]->GetTexture(), m_PointLight->GetDiffuseColor(), m_PointLight->GetPosition());
		//}

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[0]->GetIndexCount(), m_objMatrices[i], viewMatrix, projectionMatrix,
				m_Models[0]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
		{
			return false;
		}
	}

	// creat model
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

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();


	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());

	if (!result)
	{
		return false;
	}

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}



	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

D3DXMATRIX GraphicsClass::Translate(ModelClass* model, D3DXVECTOR3 position, D3DXVECTOR3 scale, float rotation)
{
	D3DXMATRIX objMat, scaleMat, rotationMat;


	m_D3D->GetWorldMatrix(objMat);

	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);
	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXMatrixRotationY(&rotationMat, rotation * (PI / 180));
	D3DXMatrixMultiply(&rotationMat, &scaleMat, &rotationMat);
	D3DXMatrixMultiply(&objMat, &rotationMat, &objMat);

	//model->scalingCollison(scale);
	//model->rotationCollison(rotation);
	//model->updateColliosnPos(position);
	//model->getTransform(position, rotation, scale);

	return objMat;
}


void GraphicsClass::MoveCharacter(D3DXVECTOR3 position)
{
	D3DXMATRIX objMat;
	int num;
	D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);
	if (OnState == OnPlayer)
		num = 1;
	else
		num = 2;

	// (-1.0f, 0.0f, 0.0f) 만큼 상대이동
	D3DXMatrixMultiply(&m_objMatrices[num + 80], &m_objMatrices[num + 80], &objMat);
	m_Models[num]->updateColliosnPos(position);
	point--;
	if (point == 0)
		OnLeft = OnRight = OnDown = OnUp = false;
}



void GraphicsClass::MoveObject(D3DXVECTOR3 position)
{
	D3DXMATRIX objMat;
	D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);

	int num;
	float rotaiotn;
	point = 70;
	if (OnState == OnPlayer)
		num = 1;
	else
		num = 2;

	if (OnPlank)
	{
		num = 9 + numOfPlankM + numOfPlankP;
		lastPosition = m_Models[num]->position;
		m_objMatrices[num + 80] = Translate(m_Models[num], m_Models[num]->position, m_Models[num]->scale, m_Models[num]->rotation);


		D3DXMatrixMultiply(&m_objMatrices[num + 80], &m_objMatrices[num + 80], &objMat);
		m_Models[num]->updateColliosnPos(position);
		point = 0;
	}

	else
	{
		m_step->PlayWaveFile(-2000, false); // 발소리 재생
		//이동전 좌표  기억
		lastPosition = m_Models[num]->position;


		if (position.x == 70.0f)
		{
			OnRight = true;
			rotaiotn = -90.0f;
		}
		if (position.x == -70.0f)
		{
			OnLeft = true;
			rotaiotn = 90.0f;
		}
		if (position.z == 70.0f)
		{
			OnUp = true;
			rotaiotn = 180.0f;
		}
		if (position.z == -70.0f)
		{
			OnDown = true;
			rotaiotn = 0.0f;
		}

		m_objMatrices[num + 80] = Translate(m_Models[num], m_Models[num]->position, m_Models[num]->scale, rotaiotn);
	}
}