// #include <AntTweakBar.h>
// #include <cmath>
// #include <vector>
// #include <cassert>
// #include "d3d10vs2003.h" // workaround to include D3D10.h and D3D11.h with VS2003
#include "../../AuthorityProjectConfig.h"
#include <d3d11.h>

#include "TwSimpleDX11.h"


// #include "sfinktah/CPluginHTML5.h"
// #include <IPluginManager.h>
// #define XXX_DEBUG
#ifndef ENABLE_TEARLESS
#define XXX_DEBUG
#endif
#ifndef XXX_DEBUG

// #include <platform_impl.h>
#include <CPluginD3D.h>
#include "FullscreenTriangleDrawer.h"
#include "sfinktah/GuiConsole/guicon.h"
#include "sfinktah/GuiConsole/guicontest.h"
#include <CPluginHTML5.h>
using namespace HTML5Plugin;
using namespace D3DPlugin;
CPluginHTML5 *gChromePlugin = NULL;
#endif

// D3D objects
ID3D11Device *          g_D3DDev = NULL;
ID3D11DeviceContext *   g_D3DDevCtx = NULL;
IDXGISwapChain *        g_SwapChain = NULL;

/// <summary>
/// Get DirectX11 Device
/// </summary>
ID3D11Device* D11Device() { return g_D3DDev; }  // Getter
/// <summary>
/// Set DirectX11 Device
/// </summary>
void D11Device(ID3D11Device* v) { g_D3DDev = v; }            // Setter

/// <summary>
/// Get DirectX11 DeviceContext
/// </summary>
ID3D11DeviceContext* D11DeviceContext() { return g_D3DDevCtx; }  // Getter
/// <summary>
/// Set DirectX11 DeviceContext
/// </summary>
void D11DeviceContext(ID3D11DeviceContext * v) { g_D3DDevCtx = v; }            // Setter

/// <summary>
/// (Unused) Get DirectX11 DeviceContext
/// </summary>
IDXGISwapChain* D11SwapChain() { return g_SwapChain; }  // Getter
/// <summary>
/// (Unused) Set DirectX11 DeviceContext
/// </summary>
void D11SwapChain(IDXGISwapChain * v) { g_SwapChain = v; }            // Sette
#ifndef XXX_DEBUG

/// <summary>
/// Inject Tearless
/// </summary>
/// <returns>Tearless Controller</returns>
void* TearlessInit(bool debugConsole = true) {
	if (debugConsole)
		SfinktahsConsole();
	gChromePlugin = new CPluginHTML5;
	gChromePlugin->InitDependencies();  // Calls InitD3DPlugin, and InitCEF (which calls InitCEFBrowser)
	return gChromePlugin;
}

#endif
