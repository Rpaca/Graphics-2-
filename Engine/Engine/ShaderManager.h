#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TerrainShader.h"


class ShaderManager
{
public:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	bool Initialize(ID3D11Device*, HWND, D3DXMATRIX);
	void Shutdown();

	bool RenderTerrainShader(ID3D11DeviceContext*, int);
	TerrainShader* GetTerrainShader() { return m_TerrainShader; }
private:

	TerrainShader		*m_TerrainShader;

};
#endif