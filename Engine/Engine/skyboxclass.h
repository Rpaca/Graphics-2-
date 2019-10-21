////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYBOXCLASS_H_
#define _SKYBOXCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyboxClass
////////////////////////////////////////////////////////////////////////////////
class SkyboxClass
{
public:
	SkyboxClass();
	SkyboxClass(const SkyboxClass&);
	~SkyboxClass();

	bool Initialize(int, int, ID3D11Device*, ID3D11DeviceContext* );
	bool ShaderInitialize(ID3D11Device*, ID3D11DeviceContext*);
	bool UpdatePos(D3DXVECTOR3);
	void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
	ID3D11Buffer* squareIndexBuffer;
	ID3D11Buffer* squareVertBuffer;
	XMMATRIX groundWorld;

	XMMATRIX Rotationx;
	XMMATRIX Rotationy;
	XMMATRIX Rotationz;
	ID3D11InputLayout* vertLayout;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11PixelShader* D2D_PS;
	ID3D10Blob* D2D_PS_Buffer;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NumSphereVertices;
	int NumSphereFaces;

	D3DXMATRIX sphereWorld;

	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	IDXGISwapChain* SwapChain;
	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11Buffer* cbPerFrameBuffer;
	ID3D11SamplerState* CubesTexSamplerState;
	ID3D11Texture2D *BackBuffer11;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

	void CreateSphere(ID3D11Device*, int LatLines, int LongLines);



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


	////Create effects constant buffer's structure//
	struct cbPerObject
	{
		//XMMATRIX  WVP;
		//XMMATRIX World;

		D3DXMATRIX  WVP;
		D3DXMATRIX World;
	};

	cbPerObject cbPerObj;

	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		XMFLOAT3 dir;
		float pad;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
	};

	Light light;

	struct cbPerFrame
	{
		Light  light;
	};

	XMMATRIX camView;
	XMMATRIX camProjection;
};

#endif