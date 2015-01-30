/*==================================================================*/
/* CS551 Graphics Support Code					    */
/* Header File							    */
/* Version 3.0                                                      */
/*==================================================================*/

#ifndef OSU_GRAPHICS_H
#define OSU_GRAPHICS_H

#include <stdio.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <Windows.h>
#include <dxgi.h>

#include "log.h"
#include "misc.h"

/*------------------------------------------------------------------*/

/*==================================================================*/
/* osuImage Data Type Definition                                     */
/*------------------------------------------------------------------*/

//typedef struct {
//   /* an image structure holds the image size */
//   /* and a dynamic array of pixels */
//   int w, h;
//   int *values;
//} osuImage;

/*void osuImageInit_d3d ( osuImage *I );
void osuImageDestroy_d3d ( osuImage *I );

void osuImageGetSize_d3d ( osuImage *I, int *w, int *h );
void osuImageSetSize_d3d ( osuImage *I, int w, int h );

void osuImageWritePixel_d3d ( osuImage *I, int w, int h, int r, int g, int b );
void osuImageReadPixel_d3d ( osuImage *I, int w, int h, int *r, int *g, int *b );
*/


/*==================================================================*/
/* A Single Window Interface
/*------------------------------------------------------------------*/

enum OSUWriteMode { OSU_OVERWRITE, OSU_REPLACE, OSU_XOR };
enum OSUColor { OSU_RED, OSU_GREEN, OSU_BLUE };
enum OSUEnable { OSU_DEPTH_TEST }; 

class OSUGraphics
{
public:
	OSUGraphics();
	~OSUGraphics();
	
	bool Init(void);
	
	/*
	 *
	 *
	 *	
	 */
	void Clear(short r, short g, short b);
	
	/*
	 *
	 *
	 *
	 */
	void WritePixel(int x, int y, short r, short g, short b);
	
	/*
	 *
	 *
	 *
	 */
	void FlushScreen(void);
	
	/*
	 *
	 *
	 *
	 */
	void SetWriteMode(int /* OSUWriteMode */ mode);
	
	/*
	 *
	 *
	 *
	 */
	void SetColor(short r, short g, short b);
	
	/*
	 *
	 *
	 *
	 */
	void Vertex(float x, float y);

private:
	int bbWidth, bbHeight;
	HWND hWnd;
	IDXGIFactory *factory = NULL;
	IDXGISwapChain *swapChain = NULL;
	ID3D11DeviceContext *deviceContext = NULL;
	ID3D11Device *device = NULL;
	ID3D11RenderTargetView *RTView = NULL;

	ID3D11PixelShader *texturePShader;
	ID3D11VertexShader *textureVShader;

	ID3D11InputLayout *textureInputLayout;

private:
	bool createShadersAndInputLayouts(void);
};

void osuBeginGraphics_d3d ( int w, int h );
void osuEndGraphics_d3d ();
void osuClear_d3d ( int r, int g, int b );
void osuFlush_d3d ();
void osuWritePixel_d3d ( int x, int y, int r, int g, int b );
void osuSetWriteMode_d3d ( int /* OSUWriteMode */ mode );
void osuWaitOnEscape_d3d ();
void osuGetFramebufferSize_d3d ( int *w, int *h );

/*==================================================================*/
/* Declarations of polygon drawing routines
/*------------------------------------------------------------------*/

enum OSUDrawable { OSU_NONE, OSU_TRIANGLE ,OSU_LINES };

void osuColor3f ( double r, double g, double b );
void osuVertex2f ( double x, double y );
void osuBegin ( int /* OSUDrawable */ );
void osuEnd ();

/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/

#endif

