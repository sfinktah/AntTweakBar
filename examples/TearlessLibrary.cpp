// TearlessLibrary.cpp : Defines the Tearless implementation.
//

#include "TearlessLibraryStdAfx.h"

#include "TearlessLibrary.h"
#include "../../CryHTML5/src/CPluginHTML5.h"
#include "../../AntTweakBar/examples/TwSimpleDX11.h"

////////////////////////////////////////////////////////////////////////////////
// Implementation of the ITearless interface.

#if 0
class TearlessImpl : public ITearless
{
	int Foo(int n);
	void Release();
};

int TearlessImpl::Foo(int n)
{
	return n * n;
}

void TearlessImpl::Release()
{
	delete this;
}
#endif

class Tearless_impl : public ISimplePluginHTML5
{
public:
	Tearless_impl() {};
	bool Show();
	bool SetURL(const wchar_t* sURL);
	bool ExecuteJS(const char* sJS);

	/// <summary>
	/// Get DirectX11 Device
	/// </summary>
	ID3D11Device* D11Device() { return g_D3DDev; }  // Getter
	/// <summary>
	/// Set DirectX11 Devic
	/// 
	/// e
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

private:
	HTML5Plugin::CPluginHTML5* plugin;
	// D3D objects
	/*
	ID3D11Device *          m_D3DDev = NULL;
	ID3D11DeviceContext *   m_D3DDevCtx = NULL;
	IDXGISwapChain *        m_SwapChain = NULL;
	*/
};

bool Tearless_impl::Show() {
	// plugin = TearlessInit();
	//plugin = dynamic_cast<HTML5Plugin::CPluginHTML5*>(TearlessInit());
	plugin = TearlessInit();
	printf("Tearless_impl: plugin: %p", plugin);
	printf("Tearless_impl: gChromePlugin: %p", gChromePlugin);
	return true;
}
/**
* @brief set the url of the frame that should be rendered
* @param sURL the url of the website
* @return true if successful
*/
bool Tearless_impl::SetURL(const wchar_t* sURL) {
	printf("Tearless_impl: plugin: %p", plugin);
	printf("Tearless_impl: gChromePlugin: %p", gChromePlugin);
	return plugin->SetURL(sURL);
}

/**
* @brief execute java script code in the current browser frame
* @param sJS the java script code
* @return true if successful
*/
bool Tearless_impl::ExecuteJS(const char* sJS) {
	printf("Tearless_impl: plugin: %p", plugin);
	printf("Tearless_impl: gChromePlugin: %p", gChromePlugin);
	printf("Executing javascript: %s\n", sJS);
	std::string s(sJS);
	std::wstring ws;
	int len = s.length();
	for (int i = 0; i < len; i++) {
		ws.append(1, s[i]);
	}
	return plugin->ExecuteJS(ws.c_str());
}



////////////////////////////////////////////////////////////////////////////////
// Factory function that creates instances of the Tearless object.

// Export both decorated and undecorated names.
//   GetTearless     - Undecorated name, which can be easily used with GetProcAddress
//                Win32 API function.
//   _GetTearless@0  - Common name decoration for __stdcall functions in C language.
//
// For more information on name decoration see here:
// "Format of a C Decorated Name"
// http://msdn.microsoft.com/en-us/library/x7kb4e2f.aspx

#if !defined(_WIN64)
// This pragma is required only for 32-bit builds. In a 64-bit environment,
// C functions are not decorated.
#pragma comment(linker, "/export:GetTearless=_GetTearless@0")
#endif  // _WIN64

TEARLESS_API TEARLESS_HANDLE WINAPI GetTearless()
{
	return new Tearless_impl();
}

TEARLESS_API TEARLESS_HANDLE WINAPI GetTearless2()
{
	return new Tearless_impl();
}

////////////////////////////////////////////////////////////////////////////////
// Regular C++ class implementatin.
// The class is exported.

#if 0
int CTearless::Foo(int n)
{
	return n * n;
}
#endif

#ifdef THIS_IS_REALLY_ANNOYING_ME
CTearless::CTearless() {
	plugin = TearlessInit();
	//plugin = dynamic_cast<HTML5Plugin::CPluginHTML5*>(TearlessInit()); // plugin;
}

bool CTearless::SetURL(const wchar_t* sURL) {
	return dynamic_cast<HTML5Plugin::CPluginHTML5*>(plugin)->SetURL(sURL);
}
bool CTearless::ExecuteJS(const wchar_t* sJS) {
	return dynamic_cast<HTML5Plugin::CPluginHTML5*>(plugin)->ExecuteJS(sJS);
}
#endif

////////////////////////////////////////////////////////////////////////////////
// C interface implementation.
// Internally these functions are written in the C++ language and
// use TearlessImpl instances.

/// <summary>
/// returns 1 if processing should continue, 0 if it should be aborted
/// </summary
TEARLESS_API LRESULT CALLBACK TearlessWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return HTML5Plugin::CWndProc::OsrWndProc(hWnd, message, wParam, lParam);
}

TEARLESS_API INT APIENTRY TearlessSetURL(TEARLESS_HANDLE handle, const wchar_t* ws)
{
	bool nResult = false;

	if (handle)
	{
		nResult = handle->SetURL(ws);
	}

	return nResult;
}

TEARLESS_API INT APIENTRY TearlessExecuteJS(TEARLESS_HANDLE handle, const char* ws)
{
	bool nResult = false;

	if (handle)
	{
		nResult = handle->ExecuteJS(ws);
	}

	return nResult;
}
