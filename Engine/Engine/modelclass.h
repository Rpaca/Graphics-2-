////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_



///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"



typedef struct
{
	float x, y, z;
}VertexType2;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};



public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void TranslateModel(float units);

	int GetIndexCount();
	int GetPolygonsCount();
	ID3D11ShaderResourceView* GetTexture();
	D3DXVECTOR3 vMin, vMax, rMax, rMin;
	bool AABBToAABB(ModelClass*);
	void translateCollison(D3DXVECTOR3);
	void scalingCollison(D3DXVECTOR3);
	void rotationCollison(float);
	D3DXMATRIX translatePosition(D3DXVECTOR3);

	D3DXVECTOR3 position;
	D3DXVECTOR3 forward;
	float rotation;
	D3DXVECTOR3 scale;
	float speed;

	D3DXVECTOR3 getVector();

	D3DXVECTOR3 reflect(D3DXVECTOR3);
	void newMatrixCollison(D3DXMATRIX*);
	void getTransform(D3DXVECTOR3, float, D3DXVECTOR3);
	void updateColliosnPos(D3DXVECTOR3);
	void updateColliosnPosition(D3DXVECTOR3);
	D3DXMATRIX resetSetting();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(WCHAR*);
	void ReleaseModel();



private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int			  m_vertexCount, m_indexCount, m_polygonsCount;
	TextureClass* m_Texture;
	ModelType* m_model;
};

#endif