#ifndef _TextureManager_H_
#define _TextureManager_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textClass.h"

class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager&);
	~TextureManager();

	bool Initialize(int);
	void Shutdown();

	bool LoadTexture(ID3D11Device*, WCHAR*, int);

	ID3D11ShaderResourceView* GetTexture(int);

private:
	TextureClass *m_TextureArray;
	int		m_textureCount;
};
#endif