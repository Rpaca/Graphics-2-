#include "PCH.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{

	m_TerrainShader = 0;

}

ShaderManager::ShaderManager(const ShaderManager& other)
{
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd, D3DXMATRIX	baseViewMatrix)
{
	bool result;


	m_TerrainShader = new TerrainShader;
	if (!m_TerrainShader)
	{
		return false;
	}

	result = m_TerrainShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}
	
	return true;
}

void ShaderManager::Shutdown()
{
	

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}
	return;
}


bool ShaderManager::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	return m_TerrainShader->RenderShader(deviceContext, indexCount);
}
