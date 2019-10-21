////////////////////////////////////////////////////////////////////////////////
// Filename: skybox.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PCH.h"
#include "skyboxclass.h"

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
UINT numElements = ARRAYSIZE(layout);

SkyboxClass::SkyboxClass()
{
}

SkyboxClass::SkyboxClass(const SkyboxClass& other)
{
}


SkyboxClass::~SkyboxClass()
{
}


//스카이 박스 정보 초기화
bool SkyboxClass::Initialize(int screenWidth, int screenHeight, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{	//Camera information
	XMVECTOR camPosition = XMVectorSet(0.0f, 5.0f, -8.0f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//Set the View matrix
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//Set the Projection matrix
	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, screenWidth / screenHeight, 1.0f, 1000.0f);


	CreateSphere(device, 10, 10);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;

	device->CreateRasterizerState(&cmdesc, &CWcullMode);

	cmdesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&cmdesc, &RSCullNone);

	///////////////**************new**************////////////////////
	// Tell D3D we will be loading a cube texture
	// Loading the Cube Map
	// A Cube Map is actually an array of six 2D textures, two for each axis(x, -x, y, -y, z, -z). The first thing we 
	// do when loading a cube map is tell D3D we will be loading a texture cube, by creating a D3DX11_IMAGE_LOAD_INFO 
	// structure and setting its MiscFlags member with D3D11_RESOURCE_MISC_TEXTURECUBE.
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	// Load the texture
	// Then we will create a 2D texture from the file. This 2D texture will actually be an array now since we said 
	// we are loading a texture cube.
	ID3D11Texture2D* SMTexture = 0;
	D3DX11CreateTextureFromFile(device, L"../Engine/data/skymap.dds",
		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

	// Create the textures description
	// Next we get the description of our texture so we can create a resource view description that matches the 
	// texture we loaded in.
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	// Tell D3D We have a cube texture, which is an array of 2D textures
	// Now we will create the shader resource view description. We will say that this resource view is a texture cube,
	// or an array of 2D textures, so when the pixel shader is texturing a pixel, it will know how to use the 3D 
	// coordinates we give it, which are used to find the texel on the texture cube. Remember a 2D texture uses (u, v)
	// coordinates, well a 3D texture uses (u, v, w) coordinates.
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	// Create the Resource view
	// And finally we create the resource view using the texture we loaded in from a file, the shader resource views 
	// description, and storing the shader resource view in smrv.
	device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
	///////////////**************new**************////////////////////

	ShaderInitialize(device, deviceContext);


	return true;
}



bool SkyboxClass::ShaderInitialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	///////////////**************new**************////////////////////

	//Compile Shaders from shader file
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "D2D_PS", "ps_4_0", 0, 0, 0, &D2D_PS_Buffer, 0, 0);
	///////////////**************new**************////////////////////
	// The Skymap's VS and PS
	// Next, we need to create our new shaders.
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_VS", "vs_4_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_PS", "ps_4_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);
	///////////////**************new**************////////////////////

	//Create the Shader Objects
	device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
	///////////////**************new**************////////////////////
	device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
	///////////////**************new**************////////////////////

	return true;
}


void SkyboxClass::CreateSphere(ID3D11Device* device, int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

}


bool SkyboxClass::UpdatePos(D3DXVECTOR3 camPosition)
{
	XMMATRIX Scale;
	XMMATRIX Translation;

	//Reset cube1World
	groundWorld = XMMatrixIdentity();

	//Define cube1's world space matrix
	Scale = XMMatrixScaling(500.0f, 10.0f, 500.0f);
	Translation = XMMatrixTranslation(0.0f, 10.0f, 10.0f);

	//Set cube1's world space using the transformations
	groundWorld = Scale * Translation;

	///////////////**************new**************////////////////////
	//Reset sphereWorld
	sphereWorld = XMMatrixIdentity();

	//Define sphereWorld's world space matrix
	Scale = XMMatrixScaling(50.0f, 50.0f, 50.0f);
	//Make sure the sphere is always centered around camera
	//Translation = XMMatrixTranslation(camPosition.x, camPosition.y, camPosition.z);
	Translation = XMMatrixTranslation(0.0f, 0.0f, -10.0f);

	//Set sphereWorld's world space using the transformations
	sphereWorld = Scale * Translation;
	///////////////**************new**************////////////////////

	return true;
}


void SkyboxClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	XMMATRIX WVP;

	//Set the grounds index buffer;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

	//SetShaderParameters
	// Set the world view projection matrix and send it to the constant buffer in effect file
	//WVP = sphereWorld * camView * camProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(sphereWorld);
	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	//Send our skymap resource view to pixel shader
	deviceContext->PSSetShaderResources(0, 1, &smrv);
	//d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

	// Set the new VS and PS shaders
	deviceContext->VSSetShader(SKYMAP_VS, 0, 0);
	deviceContext->PSSetShader(SKYMAP_PS, 0, 0);
	// Set the new depth/stencil and RS states
	deviceContext->OMSetDepthStencilState(DSLessEqual, 0);
	deviceContext->RSSetState(RSCullNone);
	deviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0);

	// Set the default VS shader and depth/stencil state
	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->OMSetDepthStencilState(NULL, 0);

	// Render the triangle.
	deviceContext->DrawIndexed(NumSphereFaces, 0, 0);
	return;
}

