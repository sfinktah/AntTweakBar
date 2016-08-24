#pragma once
#include <d3d11.h>
// D3D objects
extern ID3D11Device *          g_D3DDev;
extern ID3D11DeviceContext *   g_D3DDevCtx;
extern IDXGISwapChain *        g_SwapChain;
extern DXGI_SWAP_CHAIN_DESC    g_SwapChainDesc;
extern ID3D11RenderTargetView *g_RenderTargetView;
extern ID3D11DepthStencilView *g_DepthStencilView;
extern D3D11_TEXTURE2D_DESC    g_DepthStencilDesc;
extern ID3D11VertexShader *    g_VertexShader;
extern ID3D11PixelShader *     g_PixelShader;
extern ID3D11InputLayout *     g_InputLayout;
extern ID3D11Buffer *          g_VertexBuffer;
extern ID3D11Buffer *          g_IndexBuffer;
extern ID3D11BlendState *      g_BlendState;
extern ID3D11DepthStencilState*g_DepthStencilState;
extern ID3D11RasterizerState * g_RasterState;
extern ID3D11Buffer *          g_ConstantBuffer;
