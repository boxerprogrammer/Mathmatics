#include "Dice.h"
#include<algorithm>
namespace {
VECTOR Vector3ToVECTOR(const Vector3& vec) {
	return { vec.x, vec.y, vec.z };
}
Vector3 VECTORToVector3(const VECTOR& vec) {
	return {vec.x, vec.y, vec.z};
}
} // namespace

Dice::Dice()
{
	//VECTOR					pos ;							// 座標
	//VECTOR					norm ;							// 法線
	//COLOR_U8				dif ;							// ディフューズカラー
	//COLOR_U8				spc ;							// スペキュラカラー
	//float					u, v ;							// テクスチャ座標
	//float					su, sv ;						// 補助テクスチャ座標
	constexpr float dice_size = 2.0f; // ダイスのサイズ
	// 頂点の定義
	
	COLOR_U8 difCol = { 200, 200, 200, 255 }; // ディフューズカラー
	COLOR_U8 spcCol = { 32, 32, 32, 255 }; // スペキュラカラー
	
	vertices_ = {
		{
		// 上面
		{ { -dice_size, dice_size, -dice_size }, { 0, 1, 0 },difCol, spcCol, 0.25f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, dice_size, -dice_size }, { 0, 1, 0 }, difCol, spcCol,  0.5f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, dice_size, dice_size }, { 0, 1, 0 }, difCol, spcCol,  0.5f, 0.0f, 0.0f, 0.0f },
		{ { -dice_size, dice_size, dice_size }, { 0, 1, 0 }, difCol, spcCol, 0.25f, 0.0f, 0.0f, 0.0f },
		// 底面
		{ { -dice_size, -dice_size, -dice_size }, { 0, -1, 0 },difCol,spcCol, 0.25f, 0.5f, 0.0f, 0.0f },
		{ { dice_size, -dice_size, -dice_size }, { 0, -1, 0 },difCol,spcCol, 0.5f, 0.5f, 0.0f, 0.0f },
		{ { dice_size, -dice_size, dice_size }, { 0, -1, 0 },difCol,spcCol, 0.5f, 0.75f, 0.0f, 0.0f },
		{ { -dice_size, -dice_size, dice_size }, { 0, -1, 0 },difCol,spcCol, 0.25f, 0.75f, 0.0f, 0.0f },
		// 前面
		{ { -dice_size, -dice_size, -dice_size }, { 0, 0, -1 },difCol,spcCol, 0.25f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, -dice_size, -dice_size }, { 0, 0, -1 },difCol,spcCol, 0.5f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, dice_size, -dice_size }, { 0, 0, -1 },difCol,spcCol, 0.5f, 0.5f, 0.0f, 0.0f },
		{ { -dice_size, dice_size, -dice_size }, { 0, 0, -1 },difCol,spcCol, 0.25f, 0.5f, 0.0f, 0.0f },
		// 後面
		{ { -dice_size, -dice_size, dice_size }, { 0, 0, 1 },difCol,spcCol, 0.75f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, -dice_size, dice_size }, { 0, 0, 1 },difCol,spcCol, 1.0f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, dice_size, dice_size }, { 0, 0, 1 },difCol,spcCol, 1.0f, 0.5f, 0.0f, 0.0f },
		{ { -dice_size, dice_size, dice_size }, { 0, 0, 1 },difCol,spcCol, 0.75f, 0.5f, 0.0f, 0.0f },
		
		// 左面
		{ { -dice_size, -dice_size, -dice_size }, { -1, 0, 0 },difCol,spcCol, 0.0f, 0.25f, 0.0f, 0.0f },
		{ { -dice_size, -dice_size, dice_size }, { -1, 0, 0 },difCol,spcCol, 0.25f, 0.25f, 0.0f, 0.0f },
		{ { -dice_size, dice_size, dice_size }, { -1, 0, 0 },difCol,spcCol, 0.25f, 0.5f, 0.0f, 0.0f },
		{ { -dice_size, dice_size, -dice_size }, { -1, 0, 0 },difCol,spcCol, 0.0f, 0.5f, 0.0f, 0.0f },
		// 右面
		{ { dice_size, -dice_size, -dice_size }, { 1, 0, 0 },difCol,spcCol, 0.5f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, -dice_size, dice_size }, { 1, 0, 0 },difCol,spcCol, 0.75f, 0.25f, 0.0f, 0.0f },
		{ { dice_size, dice_size, dice_size }, { 1, 0, 0 },difCol,spcCol, 0.75f, 0.5f, 0.0f, 0.0f },
		{ { dice_size, dice_size, -dice_size }, { 1, 0, 0 },difCol,spcCol, 0.5f, 0.5f, 0.0f, 0.0f }
	
	}};
	//座標は左手系、表面は時計回りとする
	// インデックスの定義
	indices_ = {
		//上面
		0,3,1,1,3,2,
		//底面
		4,5,7,5,6,7,
		//前面
		8,11,10,8,10,9,
		//後面
		12,13,15,13,14,15,
		//左面
		16,17,19,17,18,19,
		//右面
		20,23,21,23,22,21
	};
	imgH_ = LoadGraph(L"img/dice.png");
}

void Dice::Update(const Matrix4x4& mat)
{
	for(auto& v : vertices_) {
		Vector3 vec={v.pos.x, v.pos.y, v.pos.z};
		vec = mat * vec; // 行列とベクトルの積
		v.pos = Vector3ToVECTOR(vec); // 変換後のベクトルをVECTORに戻す
		auto rot = mat;
		rot.m[0][3] = rot.m[1][3] = rot.m[2][3] = 0.0f; // 平行移動成分をゼロにする
		auto norm = VECTORToVector3(v.norm);
		v.norm = Vector3ToVECTOR(rot * norm);
	}
}

void Dice::Draw() const
{
	DrawPrimitiveIndexed3D(
		vertices_.data(), vertices_.size(),
		indices_.data(),indices_.size(),
		DX_PRIMTYPE_TRIANGLELIST,
		imgH_, // テクスチャなし
		false
	);
	//DrawPolygonIndexed3D(
	//	vertices_.data(), vertices_.size(),
	//	indices_.data(), indices_.size() / 3, // 3つの頂点で1つのポリゴンを形成
	//	-1, // テクスチャなし
	//	false
	//);

}

BoundingBox Dice::GetBoundingBox() const
{
	BoundingBox bb={{FLT_MAX,FLT_MAX,FLT_MAX},{-FLT_MAX,-FLT_MAX,-FLT_MAX}};
	for(const auto& v : vertices_) {
		bb.min.x = std::min(bb.min.x,v.pos.x);
		bb.max.x = std::max(bb.max.x,v.pos.x);
		bb.min.y = std::min(bb.min.y, v.pos.y);
		bb.max.y = std::max(bb.max.y, v.pos.y);
		bb.min.z = std::min(bb.min.z, v.pos.z);
		bb.max.z = std::max(bb.max.z, v.pos.z);
	}
	return bb;
}
