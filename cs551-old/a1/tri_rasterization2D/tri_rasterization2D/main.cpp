#include <stdio.h>
#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <comdef.h>
#include <string>

#include "log.h"
#include "misc.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool initD3d(void);

bool createShadersAndInputLayouts(void);
std::wstring GetShaderPath(void);

HWND hWnd;
int windowPosX, windowPosY;
int windowWidth, windowHeight;
int bbWidth, bbHeight;

IDXGIFactory *factory = NULL;
IDXGISwapChain *swapChain = NULL;
ID3D11DeviceContext *deviceContext = NULL;
ID3D11Device *device = NULL;
ID3D11RenderTargetView *RTView = NULL;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!init_log(NULL))
	{
		return -1;
	}

	HWND hWnd;
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		"Our First Windowed Program",   // title of the window
		(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX),    // window style
		windowPosX,    // x-position of the window
		windowPosY,    // y-position of the window
		windowWidth,    // width of the window
		windowHeight,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

	if (!hWnd)
		return false;

	// display the window on the screen
	ShowWindow(hWnd, SW_MAXIMIZE);

	//init directX
	return 0;
}

std::string get_COM_err_str(HRESULT hres)
{
	_com_error err(hres);
	return std::string(err.ErrorMessage()); //NOTE: if there is an error on this line, change settings (in VS 2013) to Character Set: Use Multi-Byte Character Set
}

std::wstring GetShaderPath(const std::wstring &shader_name)
{
	std::wstring rv = std::wstring(base_path).append(shader_path).append(shader_name);
	//C:\Users\Jared\projects\d3d_nbody\shaders
	return rv;
}

bool createShadersAndInputLayouts(void)
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

	if (FAILED(res = D3DCompileFromFile(GetShaderPath(L"Texture.hlsl").data(), nullptr, nullptr, "VShader", "vs_5_0", flags1, 0, &vShaderCode, &errorCode)))
	{
		SafeRelease<ID3DBlob>(&vShaderCode);
		if (errorCode)
		{
			OutputDebugStringA((LPCSTR)errorCode->GetBufferPointer());
			SafeRelease<ID3DBlob>(&errorCode);
		}

		return false;
	}

	if (FAILED(D3DCompileFromFile(GetShaderPath(L"Texture.hlsl").data(), nullptr, nullptr, "PShader", "ps_5_0", flags1, 0, &pShaderCode, &errorCode)))
	{
		SafeRelease<ID3DBlob>(&pShaderCode);
		if (errorCode)
		{
			OutputDebugStringA((LPCSTR)errorCode->GetBufferPointer());
			SafeRelease<ID3DBlob>(&errorCode);
		}

		return false;
	}

	if (FAILED(res = device->CreatePixelShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), NULL, &this->texturePShader)))
	{
		OutputDebugString("\n create texture pixel shader failed \n");
		return false;
	}

	if (FAILED(res = device->CreateVertexShader(vShaderCode->GetBufferPointer(), vShaderCode->GetBufferSize(), NULL, &this->textureVShader)))
	{
		OutputDebugString("\n create texture vertex shader failed \n");
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC textureInputElement;
	textureInputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	textureInputElement.Format = DXGI_FORMAT_R32G32_FLOAT;
	textureInputElement.InputSlot = 0;
	textureInputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	textureInputElement.InstanceDataStepRate = 0;
	textureInputElement.SemanticIndex = 0;
	textureInputElement.SemanticName = "TEXCOORD";

	D3D11_INPUT_ELEMENT_DESC textureShaderInput[] = { positionInputElement, textureInputElement };

	if (FAILED(this->device->CreateInputLayout(textureShaderInput, 2, vShaderCode->GetBufferPointer(), vShaderCode->GetBufferSize(), &this->textureInputLayout)))
	{
		OutputDebugString("\n Create textureShaderInput failed \n");
		return false;
	}

	SafeRelease<ID3DBlob>(&vShaderCode);
	SafeRelease<ID3DBlob>(&pShaderCode);

	return true;
}

bool initD3d(void)
{
	HRESULT res;
	D3D_FEATURE_LEVEL featureLvl11;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D *bbSurfacePtr = NULL;

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
}

