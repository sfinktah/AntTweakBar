/* CryHTML5 - for licensing and copyright see license.txt */

#pragma once

struct ID3D11ShaderResourceView;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11BlendState;

namespace HTML5Plugin
{

	class CFullscreenTriangleDrawer
	{
	public:
		CFullscreenTriangleDrawer();
		~CFullscreenTriangleDrawer();

		void Draw(void* pTexture);

	private:
		void CreateDX11Resources();
		void DrawDX11(ID3D11ShaderResourceView* pTexture);

	private:
		// DX11
		ID3D11VertexShader* m_pVertexShader11;
		ID3D11PixelShader* m_pPixelShader11;
		ID3D11BlendState* m_pBlendState11;
	};
};