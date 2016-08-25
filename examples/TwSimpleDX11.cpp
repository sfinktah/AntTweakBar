//  ---------------------------------------------------------------------------
//
//  @file       TwSimpleDX11.cpp
//
//  @brief      Example that uses AntTweakBar with DirectX11.
//
//              It draws a Menger sponge, aka Sierpinski cube:
//              http://en.wikipedia.org/wiki/Menger_sponge .
//
//              Cubes shading is augmented with some simple ambient occlusion
//              applied by subdividing each cube face into a 3x3 grid.
//              AntTweakBar is used to add some interacitve controls.
//
//              Note that most of the code here is related to DirectX and
//              Menger sponge generation. AntTweakBar calls are localized
//              in the WinMain function.
//
//              AntTweakBar: http://anttweakbar.sourceforge.net/doc
//              DirectX:     http://msdn.microsoft.com/directx
//  
//  @author     Philippe Decaudin
//
//  ---------------------------------------------------------------------------

#include <AntTweakBar.h>
#include <cmath>
#include <vector>
#include <cassert>

#include "d3d10vs2003.h" // workaround to include D3D10.h and D3D11.h with VS2003
#include <d3d11.h>

// #include "sfinktah/CPluginHTML5.h"
// #include <IPluginManager.h>
// #define XXX_DEBUG
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

// Shaders:
// Vertex and pixel shaders are defined in TwSimpleDX11.hlsl and are compiled 
// in a pre-build step using the fxc.exe compiler (from the DirectX SDK Aug'09 or later).
// Pre-build commands are:
// fxc /T vs_4_0_level_9_1 /E MainVS /Fh $(IntDir)\TwSimpleDX11_VS.h TwSimpleDX11.hlsl
// fxc /T ps_4_0_level_9_1 /E MainPS /Fh $(IntDir)\TwSimpleDX11_PS.h TwSimpleDX11.hlsl
//
#include "TwSimpleDX11_VS.h"
#include "TwSimpleDX11_PS.h"


// D3D objects
ID3D11Device *          g_D3DDev = NULL;
ID3D11DeviceContext *   g_D3DDevCtx = NULL;
IDXGISwapChain *        g_SwapChain = NULL;
DXGI_SWAP_CHAIN_DESC    g_SwapChainDesc;
ID3D11RenderTargetView *g_RenderTargetView = NULL;
ID3D11DepthStencilView *g_DepthStencilView = NULL;
D3D11_TEXTURE2D_DESC    g_DepthStencilDesc;
ID3D11VertexShader *    g_VertexShader = NULL;
ID3D11PixelShader *     g_PixelShader = NULL;
ID3D11InputLayout *     g_InputLayout = NULL;
ID3D11Buffer *          g_VertexBuffer = NULL;
ID3D11Buffer *          g_IndexBuffer = NULL;
ID3D11BlendState *      g_BlendState = NULL;
ID3D11DepthStencilState*g_DepthStencilState = NULL;
ID3D11RasterizerState * g_RasterState = NULL;
ID3D11Buffer *          g_ConstantBuffer = NULL;


// Geometry data structures and objects
struct Vector3
{
    float v[3];
    static Vector3 ZERO;
};
Vector3 Vector3::ZERO = { 0, 0, 0 };
struct Matrix4x4
{
    float m[4][4];
    static Matrix4x4 IDENTITY;
};
Matrix4x4 Matrix4x4::IDENTITY = { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 }; 
struct Quaternion
{
    float q[4];
    static Quaternion IDENTITY;
};
Quaternion Quaternion::IDENTITY = { 0, 0, 0, 1 };
const float FLOAT_PI = 3.14159265f;
struct Vertex
{
    Vector3 Position;
    Vector3 Normal;
    unsigned int AmbientColor;
};
struct ShaderConstants
{
    Matrix4x4 WorldViewProj;
    Matrix4x4 WorldNorm;
    Vector3 LightDir;
    float LightCoeff;
};
// Each cube face is split into a 3x3 grid
const int CUBE_FACE_VERTEX_COUNT = 4 * 4;       // 16 vertices per face
const int CUBE_FACE_TRIANGLE_COUNT = 2 * 3 * 3; // 18 triangles to be drawn for each face
// Faces color of the sponge wrt to recursion level
const unsigned int COLORS[] = { 0xffffffff, 0xff007fff, 0xff7fff00, 0xffff007f, 0xff0000ff, 0xff00ff00, 0xffff0000 };


// Scene globals
/*
int g_SpongeLevel = 2;                       // number of recursions
bool g_SpongeAO = true;                      // apply ambient occlusion
unsigned int g_SpongeIndicesCount = 0;       // set by BuildSponge
bool g_Animate = true;                       // enable animation
float g_AnimationSpeed = 0.2f;               // animation speed
*/
Vector3 g_LightDir = {-0.5f, -0.2f, 1};      // light direction vector
Quaternion g_SpongeRotation;                 // model rotation, set by InitScene
float g_CamDistance = 0.7f;                  // camera distance
float g_BackgroundColor[] = {0, 0, 0.5f, 1}; // background color


// Some math operators and functions. 
// They are defined here to avoid linking with D3DX.
Vector3 operator+(const Vector3& a, const Vector3& b) 
{ 
    Vector3 out; 
    out.v[0] = a.v[0] + b.v[0]; 
    out.v[1] = a.v[1] + b.v[1]; 
    out.v[2] = a.v[2] + b.v[2]; 
    return out;
}
   
Vector3 operator*(float s, const Vector3& a) 
{ 
    Vector3 out; 
    out.v[0] = s * a.v[0]; 
    out.v[1] = s * a.v[1]; 
    out.v[2] = s * a.v[2]; 
    return out; 
}

float Length(const Vector3& a)
{
    return sqrt(a.v[0]*a.v[0] + a.v[1]*a.v[1] + a.v[2]*a.v[2]);
}

Matrix4x4 Projection(float fov, float aspectRatio, float zNear, float zFar) // Left-handed projection
{
    Matrix4x4 out;
    float yScale = 1.0f / tan(fov / 2.0f);
    float xScale = yScale / aspectRatio;
    out.m[0][0] = xScale;
    out.m[0][1] = out.m[0][2] = out.m[0][3] = 0;
    out.m[1][1] = yScale;
    out.m[1][0] = out.m[1][2] = out.m[1][3] = 0;
    out.m[2][0] = out.m[2][1] = 0;
    out.m[2][2] = zFar / (zFar - zNear);
    out.m[2][3] = 1;
    out.m[3][0] = out.m[3][1] = out.m[3][3] = 0;
    out.m[3][2] = - zNear * zFar / (zFar - zNear);
    return out;
}

Matrix4x4 Translation(const Vector3& t)
{
    Matrix4x4 out(Matrix4x4::IDENTITY);
    out.m[3][0] = t.v[0];
    out.m[3][1] = t.v[1];
    out.m[3][2] = t.v[2];
    return out;
}

Matrix4x4 Scale(float s)
{
    Matrix4x4 out(Matrix4x4::IDENTITY);
    out.m[0][0] = out.m[1][1] = out.m[2][2] = s;
    return out;
}

Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
{
    Matrix4x4 out(Matrix4x4::IDENTITY);
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            out.m[i][j] = a.m[i][0]*b.m[0][j] + a.m[i][1]*b.m[1][j] + a.m[i][2]*b.m[2][j] + a.m[i][3]*b.m[3][j];
    return out;
}

Vector3 operator*(const Vector3& p, const Matrix4x4& a)
{
    Vector3 out;
    float rw = 1.f / (p.v[0]*a.m[0][3] + p.v[1]*a.m[1][3] + p.v[2]*a.m[2][3] + a.m[3][3]);
    out.v[0] = rw  * (p.v[0]*a.m[0][0] + p.v[1]*a.m[1][0] + p.v[2]*a.m[2][0] + a.m[3][0]);
    out.v[1] = rw  * (p.v[0]*a.m[0][1] + p.v[1]*a.m[1][1] + p.v[2]*a.m[2][1] + a.m[3][1]);
    out.v[2] = rw  * (p.v[0]*a.m[0][2] + p.v[1]*a.m[1][2] + p.v[2]*a.m[2][2] + a.m[3][2]);
    return out;
}

Quaternion RotationFromAxisAngle(const Vector3& axis, float angle)
{
    Quaternion out;
    float norm = Length(axis);
    float sina2 = sin(0.5f * angle);
    out.q[0] = sina2 * axis.v[0] / norm;
    out.q[1] = sina2 * axis.v[1] / norm;
    out.q[2] = sina2 * axis.v[2] / norm;
    out.q[3] = cos(0.5f * angle);
    return out;
}

void AxisAngleFromRotation(Vector3& outAxis, float& outAngle, const Quaternion& quat)
{
    float sina2 = sqrt(quat.q[0]*quat.q[0] + quat.q[1]*quat.q[1] + quat.q[2]*quat.q[2]);
    outAngle = 2.0f * atan2(sina2, quat.q[3]);
    float r = (sina2 > 0) ? (1.0f / sina2) : 0;
    outAxis.v[0] = r * quat.q[0];
    outAxis.v[1] = r * quat.q[1];
    outAxis.v[2] = r * quat.q[2]; 
}

Matrix4x4 QuaternionToMatrix(const Quaternion& quat)
{
    Matrix4x4 out;
    float yy2 = 2.0f * quat.q[1] * quat.q[1];
    float xy2 = 2.0f * quat.q[0] * quat.q[1];
    float xz2 = 2.0f * quat.q[0] * quat.q[2];
    float yz2 = 2.0f * quat.q[1] * quat.q[2];
    float zz2 = 2.0f * quat.q[2] * quat.q[2];
    float wz2 = 2.0f * quat.q[3] * quat.q[2];
    float wy2 = 2.0f * quat.q[3] * quat.q[1];
    float wx2 = 2.0f * quat.q[3] * quat.q[0];
    float xx2 = 2.0f * quat.q[0] * quat.q[0];
    out.m[0][0] = - yy2 - zz2 + 1.0f;
    out.m[0][1] = xy2 + wz2;
    out.m[0][2] = xz2 - wy2;
    out.m[0][3] = 0;
    out.m[1][0] = xy2 - wz2;
    out.m[1][1] = - xx2 - zz2 + 1.0f;
    out.m[1][2] = yz2 + wx2;
    out.m[1][3] = 0;
    out.m[2][0] = xz2 + wy2;
    out.m[2][1] = yz2 - wx2;
    out.m[2][2] = - xx2 - yy2 + 1.0f;
    out.m[2][3] = 0;
    out.m[3][0] = out.m[3][1] = out.m[3][2] = 0;
    out.m[3][3] = 1;
    return out;
}


// Forward declarations
HRESULT InitDevice(HWND wnd);
HRESULT InitScene();
void Cleanup();
LRESULT CALLBACK MessageProc(HWND, UINT, WPARAM, LPARAM);
void Anim();
void Render();
HRESULT BuildSponge(int levelMax, bool aoEnabled);

// Callback function called by AntTweakBar to set the sponge recursion level
void TW_CALL SetSpongeLevelCB(const void *value, void * /*clientData*/)
{
}


// Callback function called by AntTweakBar to get the sponge recursion level
void TW_CALL GetSpongeLevelCB(void *value, void * /*clientData*/)
{
}


// Callback function called by AntTweakBar to enable/disable ambient occlusion
void TW_CALL SetSpongeAOCB(const void *value, void * /*clientData*/)
{
}


// Callback function called by AntTweakBar to get ambient occlusion state
void TW_CALL GetSpongeAOCB(void *value, void * /*clientData*/)
{
}

#ifndef WINAPI
#define WINAPI __stdcall
#endif
// Main
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmdShow)
{
    // Register our window class
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW|CS_VREDRAW, MessageProc,
                        0L, 0L, instance, NULL, NULL, NULL, NULL, L"TwDX11", NULL };
    RegisterClassEx(&wcex);

    // Create a window
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND wnd = CreateWindow(L"TwDX11", L"AntTweakBar simple example using DirectX11", 
                            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                            rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, instance, NULL);
    if (!wnd)
    {
        MessageBox(NULL, L"Cannot create window", L"Error", MB_OK|MB_ICONERROR);
        return 0;
    }
    ShowWindow(wnd, cmdShow);
    UpdateWindow(wnd);


    // Initialize D3D11
    if (FAILED(InitDevice(wnd)))
    {
        MessageBox(wnd, L"Cannot create D3D11 device", L"Error", MB_OK|MB_ICONERROR);
        Cleanup();
        return 0;
    }

    // Initialize the 3D scene
    if (FAILED(InitScene()))
    {
        MessageBox(wnd, L"Scene initialization failed.", L"Error", MB_OK|MB_ICONERROR);
        Cleanup();
        return 0;
    }

    // Initialize AntTweakBar
    if (!TwInit(TW_DIRECT3D11, g_D3DDev))
    {
        MessageBoxA(wnd, TwGetLastError(), "AntTweakBar initialization failed", MB_OK|MB_ICONERROR);
        Cleanup();
        return 0;
    }
	// We're not adding a new fkn swapchain, I want to see this work.  So we draw over the old one, which should be blank since we got rid of the gay cube


	/*
	From: InitDevice():
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 
                                       NULL, 0, D3D11_SDK_VERSION, &g_SwapChainDesc, &g_SwapChain, 
                                       &g_D3DDev, NULL, &g_D3DDevCtx);

	*/

	/*
	gD3DSystem->SetSwapChain(g_SwapChain);
	gD3DSystem->SetDevice(g_D3DDev);
	gD3DSystem->SetDeviceContext(g_D3DDevCtx);
	*/
#ifndef XXX_DEBUG
	SfinktahsConsole();
	gChromePlugin = new CPluginHTML5;
	gChromePlugin->InitDependencies();  // Calls InitD3DPlugin, and InitCEF (which calls InitCEFBrowser)
#endif
    // gD3DSystem = NULL;
	// gD3DSystem = new CPluginD3D;
	// HTML5Plugin::gD3DPlugin->InitWithoutPluginManager();
	// gD3DPlugin->InitDependencies();

    // Create a tweak bar
    TwBar *bar = TwNewBar("TweakBar");
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
    int barSize[2] = {224, 320};
    TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);

    // Add variables to the tweak bar
    TwAddVarCB(bar, "Level", TW_TYPE_INT32, SetSpongeLevelCB, GetSpongeLevelCB, NULL, "min=0 max=3 group=Sponge keyincr=l keydecr=L");
    TwAddVarCB(bar, "Ambient Occlusion", TW_TYPE_BOOLCPP, SetSpongeAOCB, GetSpongeAOCB, NULL, "group=Sponge key=o");

    // Main message loop
    MSG msg = {0};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Anim();
            Render(); 
        }
    }

    TwTerminate();
    Cleanup();

    return (int)msg.wParam;
}


// Create Direct3D device and swap chain
HRESULT InitDevice(HWND wnd)
{
    HRESULT hr = S_OK;

    // Get window size
    RECT rc;
    GetClientRect(wnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Create D3D11 device and swap chain
    UINT createDeviceFlags = 0;
    #ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    ZeroMemory(&g_SwapChainDesc, sizeof(g_SwapChainDesc));
    g_SwapChainDesc.BufferCount = 1;
    g_SwapChainDesc.BufferDesc.Width = width;
    g_SwapChainDesc.BufferDesc.Height = height;
    g_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    g_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    g_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    g_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    g_SwapChainDesc.OutputWindow = wnd;
    g_SwapChainDesc.SampleDesc.Count = 4;
    g_SwapChainDesc.SampleDesc.Quality = 0;
    g_SwapChainDesc.Windowed = TRUE;
    g_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    // Try to create a hardware accelerated device with multisample antialiasing first
    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 
                                       NULL, 0, D3D11_SDK_VERSION, &g_SwapChainDesc, &g_SwapChain, 
                                       &g_D3DDev, NULL, &g_D3DDevCtx);
    if (FAILED(hr))
    {
        // If failed, try without antialiasing
        g_SwapChainDesc.SampleDesc.Count = 1;
        hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 
                                           NULL, 0, D3D11_SDK_VERSION, &g_SwapChainDesc, &g_SwapChain, 
                                           &g_D3DDev, NULL, &g_D3DDevCtx);
        if (FAILED(hr))
        {
            // If failed, try to create a reference device
            hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, createDeviceFlags, 
                                               NULL, 0, D3D11_SDK_VERSION, &g_SwapChainDesc, &g_SwapChain, 
                                               &g_D3DDev, NULL, &g_D3DDevCtx);
            if (SUCCEEDED(hr))
                MessageBox(wnd, L"No DX11 hardware acceleration found.\nSwitching to REFERENCE driver (very slow).",
                           L"Warning", MB_OK|MB_ICONWARNING);
            else
                return hr;
        }
    }

    // Create a render target and depth-stencil view
    ID3D11Texture2D *backBuffer = NULL, *dsBuffer = NULL;
    hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(hr))
        return hr;

    hr = g_D3DDev->CreateRenderTargetView(backBuffer, NULL, &g_RenderTargetView);
    backBuffer->Release();
    if (FAILED(hr))
        return hr;

	g_DepthStencilDesc.Width = width;
	g_DepthStencilDesc.Height = height;
	g_DepthStencilDesc.MipLevels = 1;
	g_DepthStencilDesc.ArraySize = 1;
    g_DepthStencilDesc.Format = DXGI_FORMAT_D16_UNORM;
	g_DepthStencilDesc.SampleDesc = g_SwapChainDesc.SampleDesc;
	g_DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	g_DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	g_DepthStencilDesc.CPUAccessFlags = 0;
	g_DepthStencilDesc.MiscFlags = 0;
    hr = g_D3DDev->CreateTexture2D(&g_DepthStencilDesc, NULL, &dsBuffer);
    if (FAILED(hr))
        return hr;
    hr = g_D3DDev->CreateDepthStencilView(dsBuffer, NULL, &g_DepthStencilView);
    dsBuffer->Release();
    if (FAILED(hr))
        return hr;

    g_D3DDevCtx->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_D3DDevCtx->RSSetViewports(1, &vp);

    return S_OK;
}


// Initialize the 3D objects & shaders
HRESULT InitScene()
{
    HRESULT hr;

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(Vertex, AmbientColor), D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = g_D3DDev->CreateInputLayout(layout, sizeof(layout)/sizeof(layout[0]), g_MainVS, sizeof(g_MainVS), &g_InputLayout);
    if (FAILED(hr))
        return hr;

    // Set the input layout
    g_D3DDevCtx->IASetInputLayout(g_InputLayout);

    // Create shaders
    hr = g_D3DDev->CreateVertexShader(g_MainVS, sizeof(g_MainVS), NULL, &g_VertexShader);
    if (FAILED(hr))
        return hr;
    hr = g_D3DDev->CreatePixelShader(g_MainPS, sizeof(g_MainPS), NULL, &g_PixelShader);
    if (FAILED(hr))
        return hr;

    // Set shaders
    g_D3DDevCtx->VSSetShader(g_VertexShader, NULL, 0);
    g_D3DDevCtx->PSSetShader(g_PixelShader, NULL, 0);

    // Create vertex and index buffers
    // hr = BuildSponge(g_SpongeLevel, g_SpongeAO);
    // if (FAILED(hr))
    //    return hr;

    // Create constant buffer
    D3D11_BUFFER_DESC bd;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ShaderConstants);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;
    hr = g_D3DDev->CreateBuffer(&bd, NULL, &g_ConstantBuffer);
    if (FAILED(hr))
        return hr;

    // Blend state
    D3D11_BLEND_DESC bsd;
    bsd.AlphaToCoverageEnable = FALSE;
    bsd.IndependentBlendEnable = FALSE;
    for (int i = 0; i < 8; i++)
    {
        bsd.RenderTarget[i].BlendEnable = TRUE;
        bsd.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        bsd.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        bsd.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        bsd.RenderTarget[i].BlendOp =  D3D11_BLEND_OP_ADD;
        bsd.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        bsd.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        bsd.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    }
    g_D3DDev->CreateBlendState(&bsd, &g_BlendState);
    float blendFactors[4] = { 1, 1, 1, 1 };
    g_D3DDevCtx->OMSetBlendState(g_BlendState, blendFactors, 0xffffffff);

    // Depth-stencil state
    D3D11_DEPTH_STENCILOP_DESC od;
    od.StencilFunc = D3D11_COMPARISON_ALWAYS;
    od.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    od.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    od.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    D3D11_DEPTH_STENCIL_DESC dsd;
    dsd.DepthEnable = TRUE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dsd.StencilEnable = FALSE;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dsd.FrontFace = od;
    dsd.BackFace = od;
    g_D3DDev->CreateDepthStencilState(&dsd, &g_DepthStencilState);
    g_D3DDevCtx->OMSetDepthStencilState(g_DepthStencilState, 0);

    // Rasterizer state
    D3D11_RASTERIZER_DESC rs;
    ZeroMemory(&rs, sizeof(rs));
    rs.FillMode = D3D11_FILL_SOLID;
    rs.CullMode = D3D11_CULL_NONE;
    rs.MultisampleEnable = (g_SwapChainDesc.SampleDesc.Count > 0);
    g_D3DDev->CreateRasterizerState(&rs, &g_RasterState);
    g_D3DDevCtx->RSSetState(g_RasterState);

    // Init model rotation
    Vector3 axis = {-1, 1, 0};
    g_SpongeRotation = RotationFromAxisAngle(axis, FLOAT_PI/4);

    return S_OK;
}


// Clean up D3D objects
void Cleanup()
{
    #define RELEASE_CHECK(p) if (p) { ULONG rc = (p)->Release(); assert(rc == 0); (void)rc; (p) = NULL; } 

    if (g_D3DDevCtx) 
        g_D3DDevCtx->ClearState();

    RELEASE_CHECK(g_BlendState);
    RELEASE_CHECK(g_DepthStencilState);
    RELEASE_CHECK(g_RasterState);
    RELEASE_CHECK(g_VertexBuffer);
    RELEASE_CHECK(g_IndexBuffer);
    RELEASE_CHECK(g_ConstantBuffer);
    RELEASE_CHECK(g_InputLayout);
    RELEASE_CHECK(g_VertexShader);
    RELEASE_CHECK(g_PixelShader);
    RELEASE_CHECK(g_RenderTargetView);
    RELEASE_CHECK(g_DepthStencilView);
    if (g_SwapChainDesc.Windowed) 
        RELEASE_CHECK(g_SwapChain);
    RELEASE_CHECK(g_D3DDevCtx);
    RELEASE_CHECK(g_D3DDev);
}


// Called every time the application receives a message
LRESULT CALLBACK MessageProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		// TwEventWin eats some of our keys, so we can't let it have first go at this one.
		CPluginHTML5::OsrWndProc(wnd, message, wParam, lParam);
	}
    // Send event message to AntTweakBar
    if (TwEventWin(wnd, message, wParam, lParam))
        return 0; // Event has been handled by AntTweakBar

    switch (message) 
    {
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                BeginPaint(wnd, &ps);
                EndPaint(wnd, &ps);
                return 0;
            }
        case WM_SIZE: // Window size has been changed
            if (g_D3DDev) // Resize D3D render target
            {
                // Release render target and depth-stencil view
                ID3D11RenderTargetView *nullRTV = NULL;
                g_D3DDevCtx->OMSetRenderTargets(1, &nullRTV, NULL);
                if (g_RenderTargetView)
                {
                    g_RenderTargetView->Release();
                    g_RenderTargetView = NULL;
                }
                if (g_DepthStencilView)
                {
                    g_DepthStencilView->Release();
                    g_DepthStencilView = NULL;
                }

                if (g_SwapChain)
                {
                    // Resize swap chain
                    g_SwapChainDesc.BufferDesc.Width = LOWORD(lParam);
                    g_SwapChainDesc.BufferDesc.Height = HIWORD(lParam);
                    g_SwapChain->ResizeBuffers(g_SwapChainDesc.BufferCount, g_SwapChainDesc.BufferDesc.Width, 
                                               g_SwapChainDesc.BufferDesc.Height, g_SwapChainDesc.BufferDesc.Format, 
                                               g_SwapChainDesc.Flags);

                    // Re-create a render target and depth-stencil view
                    ID3D11Texture2D *backBuffer = NULL, *dsBuffer = NULL;
                    g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
                    g_D3DDev->CreateRenderTargetView(backBuffer, NULL, &g_RenderTargetView);
                    backBuffer->Release();
                    g_DepthStencilDesc.Width = g_SwapChainDesc.BufferDesc.Width;
                    g_DepthStencilDesc.Height = g_SwapChainDesc.BufferDesc.Height;
                    g_D3DDev->CreateTexture2D(&g_DepthStencilDesc, NULL, &dsBuffer);
                    g_D3DDev->CreateDepthStencilView(dsBuffer, NULL, &g_DepthStencilView);
                    dsBuffer->Release();
                    g_D3DDevCtx->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);

                    // Setup the viewport
                    D3D11_VIEWPORT vp;
                    vp.Width = (float)g_SwapChainDesc.BufferDesc.Width;
                    vp.Height = (float)g_SwapChainDesc.BufferDesc.Height;
                    vp.MinDepth = 0.0f;
                    vp.MaxDepth = 1.0f;
                    vp.TopLeftX = 0;
                    vp.TopLeftY = 0;
                    g_D3DDevCtx->RSSetViewports(1, &vp);
                }

                // TwWindowSize has been called by TwEventWin, so it is not necessary to call it again here.
            }
            CPluginHTML5::OsrWndProc(wnd, message, wParam, lParam);
            return 0;
        case WM_CHAR:
            if (wParam == VK_ESCAPE)
                PostQuitMessage(0);
			CPluginHTML5::OsrWndProc(wnd, message, wParam, lParam);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return CPluginHTML5::OsrWndProc(wnd, message, wParam, lParam);
			//DefWindowProc(wnd, message, wParam, lParam);
    }
}

// Build sponge vertex and index buffers 
HRESULT BuildSponge(int levelMax, bool aoEnabled)
{
    if (g_VertexBuffer) 
        g_VertexBuffer->Release();
    if (g_IndexBuffer) 
        g_IndexBuffer->Release();

    // Fill vertex and index memory buffers
    static std::vector<Vertex> vertices;
    static std::vector<unsigned int> indices;
    vertices.clear();
    indices.clear();

    // Create vertex buffer
    D3D11_BUFFER_DESC bd;
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = (UINT)vertices.size() * sizeof(Vertex);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = &vertices[0];
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;
    HRESULT hr = g_D3DDev->CreateBuffer(&bd, &initData, &g_VertexBuffer);
    if (FAILED(hr))
        return hr;

    // Create index buffer
    bd.ByteWidth = (UINT)indices.size() * sizeof(unsigned int);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    initData.pSysMem = &indices[0];
    hr = g_D3DDev->CreateBuffer(&bd, &initData, &g_IndexBuffer);
    if (FAILED(hr))
    {
        g_VertexBuffer->Release();
        return hr;
    }

    return S_OK;
}


// Render the sponge
void DrawSponge()
{
    // Set vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_D3DDevCtx->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

    // Set index buffer
    g_D3DDevCtx->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set primitive topology
    g_D3DDevCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set shaders
    g_D3DDevCtx->VSSetShader(g_VertexShader, NULL, 0);
    g_D3DDevCtx->PSSetShader(g_PixelShader, NULL, 0);

    // Render the primitives
    // g_D3DDevCtx->DrawIndexed(g_SpongeIndicesCount, 0, 0);
}


// Copy world/view/proj matrices and light parameters to shader constants
void SetShaderConstants(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& proj)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = g_D3DDevCtx->Map(g_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr)) 
    {
        ShaderConstants *cst = (ShaderConstants *)mappedResource.pData;
        cst->WorldViewProj = world * view * proj;
        cst->WorldNorm = world;
        cst->LightDir = (1.0f / Length(g_LightDir)) * g_LightDir;
        cst->LightCoeff = 0.85f;
        g_D3DDevCtx->Unmap(g_ConstantBuffer, 0);
    }
    g_D3DDevCtx->VSSetConstantBuffers(0, 1, &g_ConstantBuffer);
}


// Render a frame
void Render()
{
    // Clear the back buffer 
    g_D3DDevCtx->ClearRenderTargetView(g_RenderTargetView, g_BackgroundColor);
    g_D3DDevCtx->ClearDepthStencilView(g_DepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

    // Set world/view/proj matrices and global shader constants
    float aspectRatio = (float)g_DepthStencilDesc.Width / g_DepthStencilDesc.Height;
    Matrix4x4 proj = Projection(FLOAT_PI/4, aspectRatio, 0.1f, 100.0f);
    float dist = g_CamDistance + 0.4f;
    Vector3 camPosInv = { dist * 0.3f, dist * 0.0f, dist * 2.0f };
    Matrix4x4 view = Translation(camPosInv);
    Matrix4x4 world = QuaternionToMatrix(g_SpongeRotation);
    SetShaderConstants(world, view, proj);

    // Draw the sponge
    DrawSponge();

	// Draw tweak bars
    TwDraw();

    // Present the information rendered in the back buffer to the front buffer (the screen)
    g_SwapChain->Present(0, 0);
}


// Rotating sponge
void Anim()
{
    static DWORD s_PrevTick = GetTickCount();
    DWORD tick = GetTickCount(); // msec
    float dt = float(tick - s_PrevTick) / 1000.0f; // sec
	// Stuff in here
	s_PrevTick = tick;
}


/*

This is testing _CRT_WARN output
This is testing _CRT_ERROR output
sfinktah\GuiConsole\guicontest.cpp(107) : Assertion failed: (null)
sfinktah\GuiConsole\guicontest.cpp(109) : Assertion failed: 0 && "testing _ASSERTE"
Notice: CPluginD3D constructed
Notice: Looking for m_pDevice
Error: Here: 264
Error: Here: 266
Error: Here: 274
Error: Here: 281
Error: Here: 300
Notice: found: 0000000000000000
Warning: DX11 device not found
Notice: InitD3DPlugin success
Notice: Initialize success
Notice: CreateBrowser success
Notice: Devtools URL:
[0824/145841:ERROR:dxva_video_decode_accelerator_win.cc(440)] The eglQueryDeviceAttribEXT function failed to get the device
Notice: LoadingStateChange:
[0824/145841:ERROR:singleton_hwnd.cc(34)] Cannot create windows on non-UI thread!
Notice: ProcessReques(UI/TestUI.html) Success Ext(htm) Mime(text/html) Size(576)
Notice: LoadEnd: cry://UI/TestUI.html, 200
Notice: LoadingStateChange: cry://UI/TestUI.html

*/