#pragma once


#include <d3dx12.h>
#include <dxcapi.h>

#include "MyMath.h"
#include "Struct.h"
#include "DirectXCommon.h"
#include "ShaderManager.h"
#include "GraphicPipelineStructure.h"


class LineGraphicPipeline {

public: // メンバ関数

	/// <summary>
	/// LineGraphicPipelineクラスのインスタンス取得
	/// </summary>
	static LineGraphicPipeline* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// PSOを構築する
	/// </summary>
	static void SetupLightPso();

	/// <summary>
	/// LightPSOの取得
	/// </summary>
	static PsoProperty GetPsoProperty() { return LineGraphicPipeline::GetInstance()->linePso_; }


private:

	/// <summary>
	/// RootSignatureのセットアップ
	/// </summary>
	void SetupRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature);

	/// <summary>
	/// InputLayoutのセットアップ
	/// </summary>
	void SetupInputElementDescs(D3D12_INPUT_ELEMENT_DESC& inputElementDescs, LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, UINT AlignedByteOffset);
	void SetupInputLayout(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements);

	/// <summary>
	/// BlendStateのセットアップ
	/// </summary>
	void SetupBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode);

	/// <summary>
	/// RasterizerStateのセットアップ
	/// </summary>
	void SetupRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc);

	/// <summary>
	/// Shadersのコンパイル
	/// </summary>
	void CompileShaders(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob);


private: // メンバ変数

	PsoProperty linePso_{};
};

