#pragma once
#include "MyStructure.h"
#include "../../Header/Collision/2DCollision.h"
#include"dxlib.h"
class Collision3D
{
public:

	//最短距離は大体点と点を垂直に下した線になる

	//平面上での四角同士のあたり判定
	static bool PlaneCollision(VECTOR pos1, float w1, float d1, VECTOR pos2, float w2, float d2);

	//AABBの当たり判定
	static bool AABBCollision(VECTOR pos1, float w1, float h1, float d1, VECTOR pos2, float w2, float h2, float d2);

	//平面上での円のあたり判定
	static bool CircleCollision(VECTOR pos1, float r1, VECTOR pos2, float r2);

	//円柱の当たり判定
	static bool CylindCollision(VECTOR pos1, float r1,float h1, VECTOR pos2, float r2,float h2);

	//AABBの最短距離
	static float AABBShortLength(VECTOR Box, float wide, float height, float depth, VECTOR Point);

	//OBB同士の当たり判定
	static bool OBBCollision(OBB box_1,OBB box_2);

    //点と直線の最短距離
	static POINT_LINE_SHORT PointLineShortLength(VECTOR line_start, VECTOR line_end, VECTOR point);

	//点と線分の最短距離   私的な作りは始点と終点はワールド座標で入れる
	static POINT_LINE_SHORT PointLineSegShortLength(VECTOR line_start, VECTOR line_end, VECTOR point);

	//2直線の垂直になる場所とベクトル係数
	static TWOLINE_SHORT TwoLineShortPoint(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	//2線分の垂直になる場所とベクトル係数
	static TWOLINE_SHORT TwoSegmentShortPoint(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	//球と球の当たり判定
	static bool SphereCol(VECTOR pos1, float r1, VECTOR pos2, float r2);
	static bool SphereCol(const Sphere& sphere1, const Sphere& sphere2);

	//カプセル同士の当たり判定
	static bool TwoCapsuleCol(VECTOR line_1_start, VECTOR line_1_end, float r_1, VECTOR line_2_start, VECTOR line_2_end, float r_2);
	static bool TwoCapsuleCol(const Capsule& capsule1, const Capsule& capsule2);

	// 球とカプセルの当たり判定
	static bool SphereCapsuleCol(VECTOR spherePos, float sphereR, VECTOR capsuleStartPos, VECTOR capsuleEndPos, float capsuleR, VECTOR* shortestPos = nullptr);
	static bool SphereCapsuleCol(const Sphere& sphere, const Capsule& capsule, VECTOR* shortestPos = nullptr);

	//点とＯＢＢの最接近点　OBBと点が一番近いOBBの位置を返します。
	static  VECTOR PointOBB(VECTOR point, OBB obb);
	
	//OBBと球の当たり判定
	static bool OBBSphereCol(OBB obb, VECTOR point, float r, VECTOR * hitPos = nullptr);
	static bool OBBSphereCol(const OBB& obb, const Sphere& sphere, VECTOR* hitPos = nullptr);

	//OBBとカプセルの当たり判定
	static bool OBBCapsuleCol(OBB obb, VECTOR line_start, VECTOR line_end, float r, VECTOR* hitPos = nullptr);
	//オーバーロード
	static bool OBBCapsuleCol(OBB obb, Capsule capsule, VECTOR* hitPos = nullptr);

};