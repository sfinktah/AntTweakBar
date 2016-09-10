// #include "../../AuthorityProjectConfig.h"

#include "TwSimpleDX11.h"

#ifdef ENABLE_TEARLESS

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
void D11SwapChain(IDXGISwapChain * v) { g_SwapChain = v; }            // Setter
#ifdef ENABLE_TEARLESS

/// <summary>
/// Inject Tearless with Console
/// </summary>
/// <returns>Tearless Controller</returns>
/*struct ISimplePluginHTML5 * __cdecl TearlessInit(void)*/
/// <summary>
/// Inject Tearless (optional console)
/// </summary>
/// <returns>Tearless Controller</returns>
ISimplePluginHTML5* TearlessInit(bool debugConsole) {
	if (debugConsole)
		SfinktahsConsole();
	gChromePlugin = new CPluginHTML5;
	gChromePlugin->InitDependencies();  // Calls InitD3DPlugin, and InitCEF (which calls InitCEFBrowser)
	return dynamic_cast<ISimplePluginHTML5 *>(gChromePlugin);
}

#else
#error("ENABLE_TEARLESS IS NOT SET")
#endif
