////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYBOXCLASS_H_
#define _SKYBOXCLASS_H_



struct Vertex	//Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};

//Create effects constant buffer's structure//
struct cbPerObject
{
	XMMATRIX  WVP;
	XMMATRIX World;
};

cbPerObject cbPerObj;


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyboxClass
////////////////////////////////////////////////////////////////////////////////
class SkyboxClass
{
public:
	SkyboxClass();
	SkyboxClass(const SkyboxClass&);
	~SkyboxClass();

	bool Initialize();
	void Render(ID3D11DeviceContext*);
private:
	XMMATRIX Rotationx;
	XMMATRIX Rotationy;

	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	ID3D10Blob* D2D_PS_Buffer;

	HRESULT hr;

	ID3D11Device* d3d11Device;

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11PixelShader* D2D_PS;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;
	void CreateSphere(int LatLines, int LongLines);
};

#endif