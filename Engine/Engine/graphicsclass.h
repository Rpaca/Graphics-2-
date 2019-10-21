////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "textclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "inputclass.h"
#include "skyboxclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;



////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND, InputClass*);
	void Shutdown();
	bool Frame(int, int, float);
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	vector<ModelClass*>		 m_Models;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	InputClass* m_Input;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	SkyboxClass* m_Skybox;
	vector<D3DXMATRIX>	 m_objMatrices;
	int					 m_numOfPolygons;
};
#endif