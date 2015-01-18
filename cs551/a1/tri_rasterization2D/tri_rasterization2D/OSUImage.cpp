#include "OSUImage.h"

OSUImage::OSUImage()
{
	
}

OSUImage::~OSUImage()
{
	
}

bool OSUImage::Init(void)
{
	
}

bool OSUImage::CreateTextureAndSRV(int width, int height)
{
	HRESULT res;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SUBRESOURCE_DATA initialData;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	short *rawData = (short *)malloc(width*height*sizeof(Pixel));
	Pixel defaultColor;
	defaultColor.a = 255;
	defaultColor.r = 0;
	defaultColor.b = 0;
	defaultColor.g = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//wrong//memcpy((void *)rawData[(i + 1)*(j + 1)*sizeof(Pixel)-sizeof(Pixel)], &defaultColor, sizeof(Pixel));
		}
	}

	if (FAILED(res = this->device->CreateTexture2D(&textureDesc, &initialData, &this->texture)))
	{
		return false;
	}

	if (FAILED(res = this->device->CreateShaderResourceView(this->texture, &srvDesc, &this->textureSRV)))
	{
		return false;
	}

	return true;
}

void OSUImage::FlushPixels(void)
{

}

DirectX::XMINT2 OSUImage::GetSize()
{

}

void OSUImage::SetSize(DirectX::XMINT2 size) 
{

}

void OSUImage::WritePixel(DirectX::XMINT2 location, short r, short g, short b) 
{
	if (location.x >= this->width || location.y >= this->height || location.x < 0 || location.y < 0)
	{
		log_str("attempting to write to pixel outside image bounds\n");
		return;
	}
	
	Pixel pix;
	pix.r = r;
	pix.g = g;
	pix.b = b;
	pix.a = 255;

	memcpy((void *)this->rawData[location.y*this->width*sizeof(Pixel)+location.x*sizeof(Pixel)], &pix, sizeof(Pixel));
}

Pixel OSUImage::ReadPixel(DirectX::XMINT2 location) 
{
	short *index = &this->rawData[location.y*this->width*sizeof(Pixel)+location.x*sizeof(Pixel)];
	Pixel pix;
	pix.r = index[0];
	pix.g = index[1];
	pix.b = index[2];
	pix.a = index[3];
	return pix;
}