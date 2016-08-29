#pragma once
#include <d3d11.h>

extern ID3D11Device *          g_D3DDev;
extern ID3D11DeviceContext *   g_D3DDevCtx;
extern IDXGISwapChain *        g_SwapChain;
extern DXGI_SWAP_CHAIN_DESC    g_SwapChainDesc;

ID3D11Device* D11Device();  
void D11Device(ID3D11Device* v);            

ID3D11DeviceContext* D11DeviceContext();  
void D11DeviceContext(ID3D11DeviceContext* v);            
																			   
IDXGISwapChain* D11SwapChain();  
void D11SwapChain(IDXGISwapChain* v);

void * TearlessInit();

