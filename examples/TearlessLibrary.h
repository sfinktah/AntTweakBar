// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TEARLESS_LIBRARY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TEARLESS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef TEARLESS_LIBRARY_INCLUDED_1349857
#define TEARLESS_LIBRARY_INCLUDED_1349857
#ifdef TEARLESS_LIBRARY_EXPORTS
#define TEARLESS_API __declspec(dllexport)
#else
#define TEARLESS_API __declspec(dllimport)
#endif

#include <d3d11.h>

////////////////////////////////////////////////////////////////////////////////
// 

#ifdef __cplusplus

// COM-Like abstract interface.
// This interface doesn't require __declspec(dllexport/dllimport) specifier.
// Method calls are dispatched via virtual table.
// Any C++ compiler can use it.
// Instances are obtained via factory function
struct ISimplePluginHTML5
{
	/**
	* @brief Init and Show overlayed browser
	* @return true if successful
	*/
	virtual bool Show() = 0;
	/**
	* @brief set the url of the frame that should be rendered
	* @param sURL the url of the website
	* @return true if successful
	*/
	virtual bool SetURL(const wchar_t* sURL) = 0;

	/**
	* @brief execute java script code in the current browser frame
	* @param sJS the java script code
	* @return true if successful
	*/
	virtual bool ExecuteJS(const wchar_t* sJS) = 0;

	/// <summary>
	/// Get DirectX11 Device
	/// </summary>
	virtual ID3D11Device* D11Device() = 0;
	/// <summary>
	/// Set DirectX11 Device
	/// </summary>
	virtual void D11Device(ID3D11Device* v) = 0;

	/// <summary>
	/// Get DirectX11 DeviceContext
	/// </summary>
	virtual ID3D11DeviceContext* D11DeviceContext() = 0;
	/// <summary>
	/// Set DirectX11 DeviceContext
	/// </summary>
	virtual void D11DeviceContext(ID3D11DeviceContext * v) = 0;

	/// <summary>
	/// (Unused) = 0;
	/// </summary>
	virtual IDXGISwapChain* D11SwapChain() = 0;
	/// <summary>
	/// (Unused) = 0;
	/// </summary>
	virtual void D11SwapChain(IDXGISwapChain * v) = 0;

};


#if 0
struct ITearless
{
	virtual int Foo(int n) = 0;
	virtual void Release() = 0;
};
#endif

// Regular C++ class.
// The __declspec specifier is required, otherwise clients won't be able to
// call methods of the class.
// Both DLL and EXE must use the same version of a C++ compiler.
// No need to use factory to make an instance of this class.
class TEARLESS_API CTearless
{
public:
	CTearless();
	/**
	* @brief set the url of the frame that should be rendered
	* @param sURL the url of the website
	* @return true if successful
	*/
	virtual bool SetURL(const wchar_t* sURL) = 0;

	/**
	* @brief execute java script code in the current browser frame
	* @param sJS the java script code
	* @return true if successful
	*/
	virtual bool ExecuteJS(const wchar_t* sJS) = 0;
private:
	//void* plugin;
	ISimplePluginHTML5* plugin;
};

// Handle type. In C++ language the iterface type is used.
typedef ISimplePluginHTML5* TEARLESS_HANDLE;

#else   // C

// Handle type. In C language there are no classes, so empty struct is used
// in order to ensure type safety.
typedef struct tagTEARLESS_HANDLE {} *TEARLESS_HANDLE;

#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////
// 
#ifdef __cplusplus
#   define EXTERN_C     extern "C"
#else
#   define EXTERN_C
#endif // __cplusplus

// Factory function that creates instances of the Tearless object.
EXTERN_C TEARLESS_API TEARLESS_HANDLE WINAPI GetTearless(VOID);

////////////////////////////////////////////////////////////////////////////////
// Classic C interface.
// These functions can be used with any compiler that knows how to call
// an exported C routine.

EXTERN_C TEARLESS_API INT APIENTRY TearlessSetURL(TEARLESS_HANDLE handle, const wchar_t *);
EXTERN_C TEARLESS_API INT APIENTRY TearlessExecuteJS(TEARLESS_HANDLE handle, const wchar_t *);
////////////////////////////////////////////////////////////////////////////////
#endif
