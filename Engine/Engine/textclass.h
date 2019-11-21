////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#include "fontclass.h"
#include "fontshaderclass.h"

//////////////
// INCLUDES //
//////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dinput8.lib")

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <Pdh.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11tex.h>
#include <d3dx11async.h>
#include <dinput.h>
#include <dsound.h>

#include <fstream>
#include <string>
#include <vector>
#include <map>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::map;


////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		int x, y;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

	struct SentenceData {
		string msg;
		int posX, posY;
		float r, g, b;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFPS(int, ID3D11DeviceContext*);
	bool SetCPU(int, ID3D11DeviceContext*);
	bool SetNumOfObjects(int, ID3D11DeviceContext*);
	bool SetNumOfPolygons(int, ID3D11DeviceContext*);
	bool SetScreenSize(int, int, ID3D11DeviceContext*);

	bool SetPosition(D3DXVECTOR3, ID3D11DeviceContext*);
	bool SetScore(int, int, ID3D11DeviceContext*);
	void TurnOnOffRenderInfo();

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	D3DXMATRIX					m_baseViewMatrix;
	FontClass*						m_Font;
	FontShaderClass*				m_FontShader;
	vector<SentenceType*>		m_renderInfo;
	int							m_screenWidth, m_screenHeight;
	bool						isEnableRenderInfo;
};
#endif