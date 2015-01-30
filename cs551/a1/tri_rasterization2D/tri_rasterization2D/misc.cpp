#include "misc.h"

std::wstring base_path = L"C:\\Users\\Jared\\classes\\cs551\\project1\\tri_rasterization2D";
std::wstring media_path = L"\\media";

std::string get_COM_err_str(HRESULT hres)
{
	_com_error err(hres);
	return std::string(err.ErrorMessage()); //NOTE: if there is an error on this line, change settings (in VS 2013) to Character Set: Use Multi-Byte Character Set
}

std::wstring GetShaderPath(const std::wstring &shader_name)
{
	std::wstring rv = std::wstring(base_path).append(media_path).append(L"\\shaders\\").append(shader_name);
	return rv;
}

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
};

template <class T> void SafeDelete(T **ppT)
{
	if (*ppT)
	{
		delete(*ppT);
		*ppT = nullptr;
	}
};

float shortToFloat(short num)
{
	return (float)num / 255.0f;
}

void convertColor(short r, short b, short g, short a, float out_color[4])
{
	out_color[0] = shortToFloat(r);
	out_color[1] = shortToFloat(g);
	out_color[2] = shortToFloat(b);
	out_color[3] = shortToFloat(a);
}