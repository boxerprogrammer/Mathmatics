#include "Graphics2D.h"
#include<DxLib.h>	
#include<array>

/// <summary>
/// 指定された座標に画像を描画します。
/// </summary>
/// <param name="x">描画するX座標。</param>
/// <param name="y">描画するY座標。</param>
/// <param name="grHandle">描画する画像のグラフィックハンドル。</param>
/// <param name="ps">描画パラメータまたはステータス。</param>
/// <param name="transFlag">透過処理を行うかどうかのフラグ。</param>
void 
Graphics2D::DrawGraph(int x, int y, const std::vector<int>& handles, int ps, bool transFlag) {
	std::array<VERTEX2DSHADER, 4> vertices;
	if (handles.empty()) {
		return;
	}
	float w, h;
	//グラフィクスハンドルから幅と高さを取得
	GetGraphSizeF(handles[0], &w, &h);

	for (auto& vertex : vertices) {
		vertex.rhw = 1.0f;
		vertex.pos.z = 0.0f; // 2D描画なのでZ座標は0
		vertex.dif = { 255, 255, 255, 255 }; // 白色
		vertex.spc = { 0, 0, 0, 0 }; // スペキュラは使用しない
		vertex.su = 1.0f; // テクスチャの幅に対する比率
		vertex.sv = 1.0f; // テクスチャの高さに対する比率
	}
	//頂点の並びは左上、右上、左下、右下のZ字の順番で指定します
	//左上
	vertices[0].pos.x = static_cast<float>(x);
	vertices[0].pos.y = static_cast<float>(y);
	vertices[0].u = 0.0f; // テクスチャの左端
	vertices[0].v = 0.0f; // テクスチャの上端
	//右上
	vertices[1].pos.x = static_cast<float>(x + w);
	vertices[1].pos.y = static_cast<float>(y);
	vertices[1].u = 1.0f; // テクスチャの右端
	vertices[1].v = 0.0f; // テクスチャの上端
	//左下
	vertices[2].pos.x = static_cast<float>(x);
	vertices[2].pos.y = static_cast<float>(y + h);
	vertices[2].u = 0.0f; // テクスチャの左端
	vertices[2].v = 1.0f; // テクスチャの下端
	//右下
	vertices[3].pos.x = static_cast<float>(x + w);
	vertices[3].pos.y = static_cast<float>(y + h);
	vertices[3].u = 1.0f; // テクスチャの右端
	vertices[3].v = 1.0f; // テクスチャの下端

	for (int i = 0; i < handles.size(); ++i) {
		SetUseTextureToShader(i, handles[i]); // シェーダーのテクスチャスロット0に画像をセットします。grHandleは画像のグラフィックハンドルです。
	}

	SetUsePixelShader(ps); // 使用するピクセルシェーダーを設定します。psはシェーダーのハンドルです。

	if (transFlag) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	// シェーダを使用して画像を描画するにはDrawPrimitive2DToShader関数を使用します
	DxLib::DrawPrimitive2DToShader(vertices.data(), // 頂点配列の先頭アドレス
		static_cast<int>(vertices.size()), // 頂点数
		DX_PRIMTYPE_TRIANGLESTRIP // 描画するプリミティブの種類（ここでは三角形ストリップ）
	);
	for (int i = 0; i < handles.size(); ++i) {
		SetUseTextureToShader(i, -1);
	}

	SetUsePixelShader(-1); // シェーダーの使用を解除します。
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}