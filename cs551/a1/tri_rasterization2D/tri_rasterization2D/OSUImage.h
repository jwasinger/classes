#ifndef OSU_IMAGE_H
#define OSU_IMAGE_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "log.h"

struct Pixel
{
	short r, g, b, a;
};

class OSUImage
{
public:
	OSUImage();
	~OSUImage();

	bool Init(void);

	DirectX::XMINT2 GetSize();
	void SetSize(DirectX::XMINT2 size);
	void WritePixel(DirectX::XMINT2 location, short r, short g, short b);
	Pixel ReadPixel(DirectX::XMINT2 location);
	
	void FlushPixels(void); //write raw image data to the GPU
	
	ID3D11ShaderResourceView *GetSRV(void);
private:
	bool CreateTextureAndSRV(int width, int height);
	
private:
	int width, height;
	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext;
	short *rawData;

	ID3D11Texture2D *texture;
	ID3D11ShaderResourceView *textureSRV;
};

#endif