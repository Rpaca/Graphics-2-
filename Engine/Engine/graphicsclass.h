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
#include "pointlightshaderclass.h"
#include "pointlightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "inputclass.h"
#include "skyboxclass.h"
#include "Sound.h"


#include "Terrain.h"
#include "TextureManager.h"
#include "Frustum.h"
#include "QuadTree.h" 
#include "ShaderManager.h"
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


enum State
{
	OnPlayer,
	OnEnumy,
};

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
	int playerPoint;
	int EnmeyPoint;

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	vector<ModelClass*>		 m_Models;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	PointLightShaderClass* m_PointLightShader;
	PointLightClass* m_PointLight;
	D3DXVECTOR3 lastPosition;
	Sound				*m_bgm;
	Sound				*m_step;
	Sound				*m_manVoice;
	Sound				*m_orcVoice;

	InputClass* m_Input;
	TextureShaderClass* m_TextureShader;

	BitmapClass* m_OnPlayer1;
	BitmapClass* m_OnPlayer2;

	BitmapClass* P0;
	BitmapClass* P1;
	BitmapClass* P2;
	BitmapClass* P3;
	BitmapClass* P4;
	BitmapClass* P5;
	BitmapClass* P6;
	BitmapClass* P7;
	BitmapClass* P8;
	BitmapClass* P9;

	BitmapClass* M0;
	BitmapClass* M1;
	BitmapClass* M2;
	BitmapClass* M3;
	BitmapClass* M4;
	BitmapClass* M5;
	BitmapClass* M6;
	BitmapClass* M7;
	BitmapClass* M8;
	BitmapClass* M9;

	BitmapClass* start;
	BitmapClass* info;
	BitmapClass* player1W;
	BitmapClass* player2W;


	TextClass* m_Text;
	SkyboxClass* m_Skybox;
	vector<D3DXMATRIX>	 m_objMatrices;
	int					 m_numOfPolygons;
	
	int					point;
	int					numOfPlankP;
	int					numOfPlankM;

	float				playerPosZ;
	State			   	OnState;
	bool				OnLeft;
	bool				OnRight;
	bool				OnUp;
	bool				OnDown;
	bool				OnPlank;

	bool				isStart;
	bool				isInfo;
	bool				IsEnd;
	bool				isPlayer1Win;

	bool				isMoved;

	void				MoveCharacter(D3DXVECTOR3);
	void				MoveObject(D3DXVECTOR3);
	D3DXMATRIX			Translate(ModelClass*, D3DXVECTOR3, D3DXVECTOR3, float);


	Terrain				*m_Terrain;
	TextureManager		*m_TerrainTextures;
	QuadTree			*m_QuadTree;
	Frustum				*m_Frustum;
	ShaderManager		*m_ShaderManager;
	bool				 m_wireFrame;
};
#endif