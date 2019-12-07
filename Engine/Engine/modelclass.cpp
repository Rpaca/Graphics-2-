////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PCH.h"
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	forward = {1,0,0 };
	position = { 0,0,0 };
	speed = 1.0f;
	rMin = rMax = { 0,0,0 };
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, WCHAR* modelFilename, WCHAR* textureFilename)
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;

	// Load in the model data,

	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

int ModelClass::GetPolygonsCount()
{
	return m_polygonsCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}


	for (i = 0; i < m_vertexCount; i++)
	{

		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		if (i == 0)
		{
			vMin = vertices[i].position;
			vMax = vertices[i].position;
		}

		vMin.x = min(vMin.x, m_model[i].x);
		vMin.y = min(vMin.y, m_model[i].y);
		vMin.z = min(vMin.z, m_model[i].z);


		vMax.x = max(vMax.x, m_model[i].x);
		vMax.y = max(vMax.y, m_model[i].y);
		vMax.z = max(vMax.z, m_model[i].z);

		indices[i] = i;

		//1.collison을 초기화에서 만들고 이동할때마다 변환하기
		//2.collison 생성을 매순간마다하고
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(WCHAR* filename)
{
	ifstream fin;
	char input;
	int i = 0, j = 0;


	///////////////////////////////////////////////////////////////////////////////////
	int vertexCount = 0, textureCount = 0, normalCount = 0, faceCount = 0;
	VertexType2 *vertices, *texcoords, *normals;
	FaceType *faces;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input2;
	ofstream fout;
	///////////////////////////////////////////////////////////////////////////////////

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}




	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();
	///////////////////////////////////////////////////////////////////////////////////





	///////////////////////////////////////////////////////////////////////////////////
	// Initialize the four data structures.
	vertices = new VertexType2[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType2[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType2[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	fin.get(input);

	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();
	///////////////////////////////////////////////////////////////////////////////////

	// Set the number of indices to be the same as the vertex count.
	m_vertexCount = faceCount * 3;
	m_indexCount = m_vertexCount;
	m_polygonsCount = faceCount;



	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}


	///////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		m_model[j].x = vertices[vIndex].x;
		m_model[j].y = vertices[vIndex].y;
		m_model[j].z = vertices[vIndex].z;
		m_model[j].tu = texcoords[tIndex].x;
		m_model[j].tv = texcoords[tIndex].y;
		m_model[j].nx = normals[nIndex].x;
		m_model[j].ny = normals[nIndex].y;
		m_model[j].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		j++;

		m_model[j].x = vertices[vIndex].x;
		m_model[j].y = vertices[vIndex].y;
		m_model[j].z = vertices[vIndex].z;
		m_model[j].tu = texcoords[tIndex].x;
		m_model[j].tv = texcoords[tIndex].y;
		m_model[j].nx = normals[nIndex].x;
		m_model[j].ny = normals[nIndex].y;
		m_model[j].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		j++;

		m_model[j].x = vertices[vIndex].x;
		m_model[j].y = vertices[vIndex].y;
		m_model[j].z = vertices[vIndex].z;
		m_model[j].tu = texcoords[tIndex].x;
		m_model[j].tv = texcoords[tIndex].y;
		m_model[j].nx = normals[nIndex].x;
		m_model[j].ny = normals[nIndex].y;
		m_model[j].nz = normals[nIndex].z;
		j++;
	}


	return true;
}

void ModelClass :: TranslateModel(float units)
{
	
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}


//true면 충돌함
bool ModelClass::AABBToAABB(ModelClass* pAABB)
{
	//x축에대하여
	if (vMax.x < pAABB->vMin.x ||
		vMin.x > pAABB->vMax.x)
		return false;


	//y축에대하여
	if (vMax.y < pAABB->vMin.y ||
		vMin.y > pAABB->vMax.y)
		return false;


	//z축에대하여
	if (vMax.z < pAABB->vMin.z ||
		vMin.z > pAABB->vMax.z)
		return false;

	return true;
}


// 절대 이동인줄 알았는데 이것도 상대 이동
void ModelClass::translateCollison(D3DXVECTOR3 position)
{
	D3DXMATRIX objMat;
	D3DXMatrixTranslation(&objMat, position.x, position.y, position.z);
	D3DXVec3TransformCoord(&vMax, &vMax, &objMat);
	D3DXVec3TransformCoord(&vMin, &vMin, &objMat);
}

D3DXMATRIX ModelClass::translatePosition(D3DXVECTOR3 pos)
{
	D3DXMATRIX objMat, scaleMat, rotationMat;
	D3DXMatrixTranslation(&objMat, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXMatrixRotationY(&rotationMat, rotation);
	D3DXMatrixMultiply(&rotationMat, &scaleMat, &rotationMat);
	D3DXMatrixMultiply(&objMat, &rotationMat, &objMat);

	//translateCollison(pos);

	vMin = vMin - position + pos;//충돌박스 이동
	vMax = vMax - position + pos;

	position = pos; // 좌표 업데이트

	return objMat;
}

//상대이동(회전처리 불가)
void ModelClass::updateColliosnPos(D3DXVECTOR3 vec)
{

	if (rMin == rMax)
	{
		rMin = vMin;
		rMax = vMax;
	}

	position += vec;

	vMin += vec;
	vMax += vec;
}

//절대 이동
void ModelClass::updateColliosnPosition(D3DXVECTOR3 vec)
{
	vMin = rMin + vec;
	vMax = rMax + vec;

}



void ModelClass::scalingCollison(D3DXVECTOR3 scale)
{
	D3DXMATRIX scaleMat;
	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXVec3TransformCoord(&vMax, &vMax, &scaleMat);
	D3DXVec3TransformCoord(&vMin, &vMin, &scaleMat);
}

void ModelClass::rotationCollison(float rotation)
{
	int i = 0;
	D3DXVECTOR3 translateVector = vMin - rMin;
	vMin = { rMin.z, rMin.y, rMin.x };
	rMin = vMin;
	vMin += translateVector;

	vMax = { rMax.z, rMax.y, rMax.x };
	rMax = vMax;
	vMax += translateVector;


		//D3DXMATRIX rotationMay;
	//D3DXMatrixRotationY(&rotationMay, rotation);
	//D3DXVec3TransformCoord(&vMax, &vMax, &rotationMay);
	//D3DXVec3TransformCoord(&vMin, &vMin, &rotationMay);

	//if (rotation == 0.0f)
	//	return;
	//D3DXVECTOR3 max, min;
	//max = vMax;
	//min = vMin;
	////vMax = { -vMax.z, vMax.y, -vMax.x };
	////vMin = { -vMin.z, vMin.y, -vMin.x };
	//vMax = { -min.z, max.y, -min.x };
	//vMin = { -max.z, min.y, -max.x };
}

void ModelClass::newMatrixCollison(D3DXMATRIX* mat)
{
	D3DXVec3TransformCoord(&vMax, &vMax, mat);
}


//반사후 법선백터(방향백터)를 알수있음
D3DXVECTOR3 ModelClass::reflect(D3DXVECTOR3 n)
{
	//float x;
	//x = -1 + rand() % 3;
	//forward += {x*0.1,0,x*0.1f}; // 랜덤 범위 설정 필요

	D3DXVECTOR3 r;
	r = forward + 2 *D3DXVec3Dot(&(-forward), &n) *n;
	D3DXVec3Normalize(&r, &r);
	forward = r;
	position += forward;
	return r;
}


//getNormalVector
D3DXVECTOR3 ModelClass::getVector()
{
	D3DXVECTOR3  yMin, xMin;


	return yMin;
}


void ModelClass::getTransform(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 scale)
{
	position = pos;
	rotation = rot;
	this->scale = scale;

}

//콜리더가 원위치로 돌아가지 않음
D3DXMATRIX ModelClass::resetSetting()
{
	forward = { 1,0,0 };
	getTransform({ 0, 2.2,0 }, rotation, scale);

	vMin = rMin;
	vMax = rMax;

	return translatePosition({ 0,  2.2, 0 });
}

//마지막 점수판 관련
//1. 앞뒤 벽에 다으면 점수가 증가
//2. 공의 위치와 방향 초기화