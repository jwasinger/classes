#ifndef MISC_H
#define MISC_H

#include <string>
#include <Windows.h>
#include <comdef.h>

std::string get_COM_err_str(HRESULT hres);
std::wstring GetShaderPath(const std::wstring &shader_name);
template <class T> void SafeRelease(T **ppT);
template <class T> void SafeDelete(T **ppT);

void convertColor(short r, short b, short g, short a, float out_color[4]);
#endif