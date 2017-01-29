#pragma once
#include <d3d11.h>
#include "../../AuthorityProjectConfig.h"
#include "TearlessLibrary.h"
#include "../../CryHTML5/src/CPluginHTML5.h"

extern ID3D11Device *          g_D3DDev;
extern ID3D11DeviceContext *   g_D3DDevCtx;
extern IDXGISwapChain *        g_SwapChain;
extern JSON_CALLBACK           g_fnJsonCallback;
//extern DXGI_SWAP_CHAIN_DESC    g_SwapChainDesc;

ID3D11Device* D11Device();  
void D11Device(ID3D11Device* v);            

ID3D11DeviceContext* D11DeviceContext();  
void D11DeviceContext(ID3D11DeviceContext* v);            
																			   
IDXGISwapChain* D11SwapChain();  
void D11SwapChain(IDXGISwapChain* v);

#ifdef ENABLE_TEARLESS
HTML5Plugin::CPluginHTML5 * TearlessInit(bool debugConsole = false);
extern HTML5Plugin::CPluginHTML5 * gChromePlugin;
#else
#error("ENABLE_TEARLESS IS NOT SET")
#endif



