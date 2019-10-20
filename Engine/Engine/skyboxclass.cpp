//////////////////////////////////////////////////////////////////////////////////
//// Filename: skybox.cpp
//////////////////////////////////////////////////////////////////////////////////
//#include "skyboxclass.h"
//#include "PCH.h"
//
//SkyboxClass::SkyboxClass()
//{
//}
//
//SkyboxClass::SkyboxClass(const SkyboxClass&)
//{
//}
//
//
//SkyboxClass::~SkyboxClass()
//{
//}
//
//bool SkyboxClass::Initialize()
//{
//	///////////////**************new**************////////////////////
//	// Call the Create Sphere Function
//	// Here we will call the function to create our sphere. We will give it 10 latitude lines, and 10 longitude lines.
//	// By the way, when the sphere is created, its actually created on its side, so the "south" and "north" poles are 
//	// parallel to the ground.
//	CreateSphere(10, 10);
//	///////////////**************new**************////////////////////
//
//	//Compile Shaders from shader file
//	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
//	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
//	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "D2D_PS", "ps_4_0", 0, 0, 0, &D2D_PS_Buffer, 0, 0);
//	///////////////**************new**************////////////////////
//	// The Skymap's VS and PS
//	// Next, we need to create our new shaders.
//	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_VS", "vs_4_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
//	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_PS", "ps_4_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);
//	///////////////**************new**************////////////////////
//	//Create the Shader Objects
//	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
//	hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
//	hr = d3d11Device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
//	///////////////**************new**************////////////////////
//	hr = d3d11Device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
//	hr = d3d11Device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
//	///////////////**************new**************////////////////////
//
//	///////////////**************new**************////////////////////
//	// Tell D3D we will be loading a cube texture
//	// Loading the Cube Map
//	// A Cube Map is actually an array of six 2D textures, two for each axis(x, -x, y, -y, z, -z). The first thing we 
//	// do when loading a cube map is tell D3D we will be loading a texture cube, by creating a D3DX11_IMAGE_LOAD_INFO 
//	// structure and setting its MiscFlags member with D3D11_RESOURCE_MISC_TEXTURECUBE.
//	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
//	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
//
//	// Load the texture
//	// Then we will create a 2D texture from the file. This 2D texture will actually be an array now since we said 
//	// we are loading a texture cube.
//	ID3D11Texture2D* SMTexture = 0;
//	hr = D3DX11CreateTextureFromFile(d3d11Device, L"skymap1.dds",
//		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);
//
//	// Create the textures description
//	// Next we get the description of our texture so we can create a resource view description that matches the 
//	// texture we loaded in.
//	D3D11_TEXTURE2D_DESC SMTextureDesc;
//	SMTexture->GetDesc(&SMTextureDesc);
//
//	// Tell D3D We have a cube texture, which is an array of 2D textures
//	// Now we will create the shader resource view description. We will say that this resource view is a texture cube,
//	// or an array of 2D textures, so when the pixel shader is texturing a pixel, it will know how to use the 3D 
//	// coordinates we give it, which are used to find the texel on the texture cube. Remember a 2D texture uses (u, v)
//	// coordinates, well a 3D texture uses (u, v, w) coordinates.
//	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
//	SMViewDesc.Format = SMTextureDesc.Format;
//	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
//	SMViewDesc.TextureCube.MostDetailedMip = 0;
//
//	// Create the Resource view
//	// And finally we create the resource view using the texture we loaded in from a file, the shader resource views 
//	// description, and storing the shader resource view in smrv.
//	hr = d3d11Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
//	///////////////**************new**************////////////////////
//
//	return true;
//}
//void SkyboxClass::CreateSphere(int LatLines, int LongLines)
//{
//	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
//	NumSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);
//
//	float sphereYaw = 0.0f;
//	float spherePitch = 0.0f;
//
//	std::vector<Vertex> vertices(NumSphereVertices);
//
//	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//
//	vertices[0].pos.x = 0.0f;
//	vertices[0].pos.y = 0.0f;
//	vertices[0].pos.z = 1.0f;
//
//	for (DWORD i = 0; i < LatLines - 2; ++i)
//	{
//		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
//		Rotationx = XMMatrixRotationX(spherePitch);
//		for (DWORD j = 0; j < LongLines; ++j)
//		{
//			sphereYaw = j * (6.28 / (LongLines));
//			Rotationy = XMMatrixRotationZ(sphereYaw);
//			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
//			currVertPos = XMVector3Normalize(currVertPos);
//			vertices[i*LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
//			vertices[i*LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
//			vertices[i*LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
//		}
//	}
//
//	vertices[NumSphereVertices - 1].pos.x = 0.0f;
//	vertices[NumSphereVertices - 1].pos.y = 0.0f;
//	vertices[NumSphereVertices - 1].pos.z = -1.0f;
//
//
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
//
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA vertexBufferData;
//
//	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
//	vertexBufferData.pSysMem = &vertices[0];
//	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);
//
//
//	std::vector<DWORD> indices(NumSphereFaces * 3);
//
//	int k = 0;
//	for (DWORD l = 0; l < LongLines - 1; ++l)
//	{
//		indices[k] = 0;
//		indices[k + 1] = l + 1;
//		indices[k + 2] = l + 2;
//		k += 3;
//	}
//
//	indices[k] = 0;
//	indices[k + 1] = LongLines;
//	indices[k + 2] = 1;
//	k += 3;
//
//	for (DWORD i = 0; i < LatLines - 3; ++i)
//	{
//		for (DWORD j = 0; j < LongLines - 1; ++j)
//		{
//			indices[k] = i * LongLines + j + 1;
//			indices[k + 1] = i * LongLines + j + 2;
//			indices[k + 2] = (i + 1)*LongLines + j + 1;
//
//			indices[k + 3] = (i + 1)*LongLines + j + 1;
//			indices[k + 4] = i * LongLines + j + 2;
//			indices[k + 5] = (i + 1)*LongLines + j + 2;
//
//			k += 6; // next quad
//		}
//
//		indices[k] = (i*LongLines) + LongLines;
//		indices[k + 1] = (i*LongLines) + 1;
//		indices[k + 2] = ((i + 1)*LongLines) + LongLines;
//
//		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
//		indices[k + 4] = (i*LongLines) + 1;
//		indices[k + 5] = ((i + 1)*LongLines) + 1;
//
//		k += 6;
//	}
//
//	for (DWORD l = 0; l < LongLines - 1; ++l)
//	{
//		indices[k] = NumSphereVertices - 1;
//		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
//		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
//		k += 3;
//	}
//
//	indices[k] = NumSphereVertices - 1;
//	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
//	indices[k + 2] = NumSphereVertices - 2;
//
//	D3D11_BUFFER_DESC indexBufferDesc;
//	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
//
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA iinitData;
//
//	iinitData.pSysMem = &indices[0];
//	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);
//
//}
//
//void SkyboxClass::Render(ID3D11DeviceContext* deviceContext)
//{
//	XMMATRIX WVP;
//	XMMATRIX camView;
//	XMMATRIX camProjection;
//	//Set the d2d vertex buffer
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//	ID3D11Buffer* cbPerObjectBuffer;
//	ID3D11SamplerState* CubesTexSamplerState;
//	///////////////**************new**************////////////////////
//// Drawing the Sky
//// First, we set the spheres index and vertex buffer. Then we set its shader variables, WVP and World matrices, 
//// texture, and sampler. After that we set its VS, PS, Depth / Stencil State and Rasterizer state. Then we draw 
//// the sphere. After, we need to make sure we set the states back to default in case other geometry drawn after 
//// this one do not explicitly set the default states and shaders.
//
//// Set the spheres index buffer
//	deviceContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	// Set the spheres vertex buffer
//	deviceContext->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);
//
//	// Set the world view projection matrix and send it to the constant buffer in effect file
//	WVP = sphereWorld * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	cbPerObj.World = XMMatrixTranspose(sphereWorld);
//	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//	// Send our skymap resource view to pixel shader
//	deviceContext->PSSetShaderResources(0, 1, &smrv);
//	deviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
//
//	// Set the new VS and PS shaders
//	deviceContext->VSSetShader(SKYMAP_VS, 0, 0);
//	deviceContext->PSSetShader(SKYMAP_PS, 0, 0);
//	// Set the new depth/stencil and RS states
//	deviceContext->OMSetDepthStencilState(DSLessEqual, 0);
//	deviceContext->RSSetState(RSCullNone);
//	deviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0);
//
//	// Set the default VS shader and depth/stencil state
//	deviceContext->VSSetShader(VS, 0, 0);
//	deviceContext->OMSetDepthStencilState(NULL, 0);
//	///////////////**************new**************////////////////////
//}