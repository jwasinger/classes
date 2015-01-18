#include "osuGraphics.h"

OSUGraphics::OSUGraphics()
{}

OSUGraphics::~OSUGraphics()
{}

bool OSUGraphics::createShadersAndInputLayouts(void)
{
	UINT flags1 = 0;
	HRESULT res;
	ID3DBlob *vShaderCode = nullptr;
	ID3DBlob *pShaderCode = nullptr;
	ID3DBlob *errorCode = nullptr;

#if _DEBUG

	flags1 |= D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;

#endif

	//create Texture shaders------------------------------------------------------------------------------------------------------------

	if (FAILED(res = D3DCompileFromFile(GetShaderPath(L"Texture.hlsl").data(), NULL, NULL, "VShader", "vs_5_0", flags1, 0, &vShaderCode, &errorCode)))
	{
		log_str("D3DCompileFromFile failed.  Error code: %s\n", get_COM_err_str(res));
		if (errorCode)
			log_str("Error Message: %s\n", (LPCSTR)errorCode->GetBufferPointer());

		goto failed;
	}

	if (FAILED(D3DCompileFromFile(GetShaderPath(L"Texture.hlsl").data(), NULL, NULL, "PShader", "ps_5_0", flags1, 0, &pShaderCode, &errorCode)))
	{
		log_str("D3DCompileFromFile failed.  Error code: %s\n", get_COM_err_str(res));
		if (errorCode)
			log_str("Error Message: %s\n", (LPCSTR)errorCode->GetBufferPointer());
		
		goto failed;
	}

	if (FAILED(res = device->CreatePixelShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &this->texturePShader)))
	{
		log_str("CreatePixelShader failed.  Error: %s\n", get_COM_err_str(res));
		goto failed;
	}

	if (FAILED(res = device->CreateVertexShader(vShaderCode->GetBufferPointer(), vShaderCode->GetBufferSize(), NULL, &this->textureVShader)))
	{
		log_str("CreateVertexShader failed.  Error: %s\n", get_COM_err_str(res));
		goto failed;
	}
	
	D3D11_INPUT_ELEMENT_DESC positionInputElement;
	positionInputElement.AlignedByteOffset = 0;
	positionInputElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	positionInputElement.InputSlot = 0;
	positionInputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	positionInputElement.InstanceDataStepRate = 0;
	positionInputElement.SemanticName = "POSITION";
	positionInputElement.SemanticIndex = 0;

	D3D11_INPUT_ELEMENT_DESC textureInputElement;
	textureInputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	textureInputElement.Format = DXGI_FORMAT_R32G32_FLOAT;
	textureInputElement.InputSlot = 0;
	textureInputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	textureInputElement.InstanceDataStepRate = 0;
	textureInputElement.SemanticIndex = 0;
	textureInputElement.SemanticName = "TEXCOORD";

	D3D11_INPUT_ELEMENT_DESC textureShaderInput[] = { positionInputElement, textureInputElement };

	if (FAILED(res = this->device->CreateInputLayout(textureShaderInput, 2, vShaderCode->GetBufferPointer(), vShaderCode->GetBufferSize(), &this->textureInputLayout)))
	{
		log_str("CreateInputLayout failed: %s", get_COM_err_str(res));
		goto failed;
	}

failed:
	SafeRelease<ID3DBlob>(&vShaderCode);
	SafeRelease<ID3DBlob>(&pShaderCode);
	SafeRelease<ID3DBlob>(&errorCode);
	SafeRelease<ID3D11PixelShader>(&this->texturePShader);
	SafeRelease<ID3D11VertexShader>(&this->textureVShader);
	SafeRelease<ID3D11InputLayout>(&this->textureInputLayout);
	return false;

	return true;
}

bool OSUGraphics::Init(void)
{
	HRESULT res;
	D3D_FEATURE_LEVEL featureLvl11;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D *bbSurfacePtr = NULL;

	hWnd = GetActiveWindow();
	if (!hWnd)
		return false;

	if (FAILED(res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory)))
	{
		log_str("CreateDXGIFactory Error: %s\n", get_COM_err_str(res));
		return false;
	}

	//set up device and swap chain
	D3D_FEATURE_LEVEL featureLvl11 = D3D_FEATURE_LEVEL_11_0;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Width = bbWidth;
	swapChainDesc.BufferDesc.Height = bbHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = hWnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

#ifdef _DEBUG 
	res = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		&featureLvl11,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		(IDXGISwapChain**)&swapChain,
		(ID3D11Device**)&device,
		NULL,
		(ID3D11DeviceContext**)&deviceContext);

	if (FAILED(res))
	{
		log_str("\nD3D11CreateDeviceAndSwapChain error: %s\n", get_COM_err_str(res));
		return false;
	}
#else
	res = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		&featureLvl11,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		(IDXGISwapChain**)&swapChain,
		(ID3D11Device**)&device,
		nullptr,
		(ID3D11DeviceContext**)&deviceContext);

	if (FAILED(res))
	{
		log_str("\nD3D11CreateDeviceAndSwapChain error: %s\n", get_COM_err_str(res));
		return false;
	}
#endif

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bbSurfacePtr);
	if (FAILED(res))
	{
		log_str("\nswapChain->GetBuffer error: %s\n", get_COM_err_str(res));
		return false;
	}

	res = device->CreateRenderTargetView(bbSurfacePtr, NULL, (ID3D11RenderTargetView**)&RTView);
	if (FAILED(res))
	{
		log_str("\ndevice->createRenderTargetView failed: %s\n", get_COM_err_str(res));
		return false;
	}

	if FAILED(res = createShadersAndInputLayouts())
		return false;
	
	return true;
}

void OSUGraphics::Clear(short r, short g, short b)
{
	float color[4];
	convertColor(r, g, b, 255, color);
	
	this->deviceContext->ClearRenderTargetView(this->RTView, color);
}

void OSUGraphics::WritePixel(int x, int y, short r, short g, short b)
{

}

void OSUGraphics::FlushScreen(void)
{

}

void OSUGraphics::SetWriteMode(int /* OSUWriteMode */ mode)
{

}

void OSUGraphics::SetColor(short r, short g, short b)
{

}

void OSUGraphics::Vertex(float x, float y)
{

}