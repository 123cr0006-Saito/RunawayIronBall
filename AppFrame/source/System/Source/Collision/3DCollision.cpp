#include "../../Header/Collision/3DCollision.h"

bool Collision3D::PlaneCollision(VECTOR pos1, float w1,float d1, VECTOR pos2, float w2, float d2)
{
	VECTOR receiv = VSub(pos1, VGet(w1 / 2, 0, d1 / 2));
	VECTOR give = VSub(pos2, VGet(w2 / 2, 0, d2 / 2));

	return Collision2D::IsHitBox(receiv.x, receiv.z, w1, d1, give.x, give.z, w2, d2);
}

bool Collision3D::AABBCollision(VECTOR pos1, float w1, float h1, float d1, VECTOR pos2, float w2, float h2, float d2)
{
	VECTOR receiv = VSub(pos1, VGet(w1 / 2, h1 / 2, d1 / 2));
	VECTOR give = VSub(pos2, VGet(w2 / 2, h2 / 2, d2 / 2));

	if(Collision2D::IsHitBox(receiv.x, receiv.z, w1, d1, give.x, give.z, w2, d2)) {
		if(Collision2D::IsHitBox(receiv.x, receiv.y, w1, h1, give.x, give.y, w2, h2)) {
			return true;
		}
	}

	return false;
}

bool Collision3D::CircleCollision(VECTOR pos1, float r1, VECTOR pos2, float r2){
	VECTOR col = VSub(pos1, pos2); col.y = 0;
	float r = r1 + r2;
	if (r * r > col.x * col.x + col.z * col.z) {
		return true;
	}
	return false;
}

bool Collision3D::CylindCollision(VECTOR pos1, float r1, float h1, VECTOR pos2, float r2, float h2) {
	VECTOR col = VSub(pos1, pos2); 
	float r = r1 + r2;
	if (r * r > col.x * col.x + col.z * col.z) {
		if (pos1.y < pos2.y && pos1.y + h1 > pos2.y ||
			pos1.y < pos2.y + h2 && pos1.y + h1 > pos2.y + h2) {
			return true;
		}
	}
	return false;
}

float Collision3D::AABBShortLength(VECTOR Box, float wide, float height ,float depth ,VECTOR Point) {
	float SqLen = 0;   // 長さのべき乗の値を格納
	float point[3] = { Point.x,Point.y,Point.z};
	float box[3] = { Box.x,Box.y,Box.z };
	float length[3] = { wide/2,height/2,depth/2 };

	for (int i = 0; i < 3; i++)
	{
		// 各軸で点が最小値以下もしくは最大値以上ならば、差を考慮
		if (point[i] < box[i] - length[i])  // i=0はX、1はY、2はZの意味です
			SqLen += ((point[i] - (box[i] - length[i])) * (point[i] - (box[i] - length[i])));
		if (point[i] > box[i] + length[i])
			SqLen += ((point[i] - (box[i] + length[i])) * (point[i] - (box[i] + length[i])));
	}
	return sqrt(SqLen);
}

bool Collision3D::OBBCollision(OBB obb_1, OBB obb_2, bool flag) {
	// lengthは全体の長さなので使うときは半分にする
	//NAe は 方向ベクトル Ae は 方向ベクトルにその長さをかけたもの　方向ベクトルはマトリクスから持ってきたもので正規化されている
	MATRIX matrix_A = Math::MMultXYZ(obb_1.direction[0], obb_1.direction[1], obb_1.direction[2]);
	MATRIX matrix_B = Math::MMultXYZ(obb_2.direction[0], obb_2.direction[1], obb_2.direction[2]);

	if (flag) {
		matrix_B = MMult(matrix_B, GetCameraBillboardMatrix());
	}

	VECTOR NAe1 = VGet(matrix_A.m[0][0],matrix_A.m[0][1],matrix_A.m[0][2]), Ae1 = VScale(NAe1, obb_1.length[0]/2);
	VECTOR NAe2 = VGet(matrix_A.m[1][0],matrix_A.m[1][1],matrix_A.m[1][2]), Ae2 = VScale(NAe2, obb_1.length[1]/2);
	VECTOR NAe3 = VGet(matrix_A.m[2][0],matrix_A.m[2][1],matrix_A.m[2][2]), Ae3 = VScale(NAe3, obb_1.length[2]/2);
	VECTOR NBe1 = VGet(matrix_B.m[0][0],matrix_B.m[0][1],matrix_B.m[0][2]), Be1 = VScale(NBe1, obb_2.length[0]/2);
	VECTOR NBe2 = VGet(matrix_B.m[1][0],matrix_B.m[1][1],matrix_B.m[1][2]), Be2 = VScale(NBe2, obb_2.length[1]/2);
	VECTOR NBe3 = VGet(matrix_B.m[2][0],matrix_B.m[2][1],matrix_B.m[2][2]), Be3 = VScale(NBe3, obb_2.length[2]/2);
	//Misalignmentでおなかの中心にモデルをずらしている 大体ｙ軸だけで済むと思うがフライト系などの3軸回転を入れるときは
	//VTransformの中の回転行列をy軸だけでなくｘとｚを含めた3軸で回転してください
	VECTOR Interval = VSub(obb_1.pos , VSub(obb_2.pos, VTransform(obb_2.Misalignment,MGetRotY(obb_2.direction[1]))));

	//VECTOR Interval = VSub(obb_1.pos, obb_2.pos);

	//  各軸の方向ベクトルと分離軸との絶対値の内積の和をまとめたものと 中心点間の距離を比較し中心点間距離より短かったら衝突している

	// 分離軸 : Ae1
	FLOAT rA = VSize(Ae1);
	FLOAT rB = Math::LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	FLOAT L = fabs(VDot(Interval, NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	// 分離軸 : Ae2
	rA = VSize(Ae2);
	rB = Math::LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabs(VDot(Interval,NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = VSize(Ae3);
	rB = Math::LenSegOnSeparateAxis(NAe3, Be1,Be2,Be3);
	L = fabs(VDot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = Math::LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = VSize(Be1);
	L = fabs(VDot(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = Math::LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = VSize(Be2);
	L = fabs(VDot(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = Math::LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = VSize(Be3);
	L = fabs(VDot(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// ↓内積での計算を含めると重いので今回は使わない　9割の計算は上の方向ベクトルだけでもできる
	//  OBBが辺同士ひねった関係で近づくと方向ベクトルの分離軸では判定できなくなるそうです

	//  今回は複雑な処理はないので軽くするためにコメントアウトしておく

	//// 分離軸 : C11   Ae1*Be1に垂直
	//VECTOR Cross;
	//Cross = VCross(NAe1, NBe1);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be2, Be3);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C12   Ae1*Be2
	//Cross = VCross(NAe1, NBe2);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be1, Be3);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C13   Ae1*Be3
	//Cross = VCross(NAe1, NBe3);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be1, Be2);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C21   Ae2*Be1
	//Cross = VCross(NAe2, NBe1);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be2, Be3);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C22   Ae2*Be2
	//Cross = VCross(NAe2, NBe2);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be1, Be3);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C23   Ae2*Be3
	//Cross = VCross(NAe2, NBe3);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be1, Be2);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C31   Ae3*Be1
	//Cross = VCross(NAe3, NBe1);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be2, Be3);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C32   Ae3*Be2
	//Cross = VCross(NAe3, NBe2);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be1, Be3);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C33   Ae3*Be3
	//Cross = VCross(NAe3, NBe3);
	//rA = Math::LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	//rB = Math::LenSegOnSeparateAxis(Cross, Be1, Be2);
	//L = fabs(VDot(Interval, Cross));
	//if (L > rA + rB)
	//	return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}

//最短距離は大体点と点を垂直に下した線になる
//点と直線の最短距離
POINT_LINE_SHORT Collision3D::PointLineShortLength(VECTOR line_start, VECTOR line_end, VECTOR point) {
	POINT_LINE_SHORT Re;
	VECTOR line = VSub(line_end, line_start);
	float length = VSquareSize(line);

	Re.coefficient = 0.0f;
	if (length > 0.0f) {
		Re.coefficient = VDot(line, VSub(point, line_start)) / length;
	}

	Re.hit_point = VAdd(line_start, VScale(line, Re.coefficient));
	Re.length = VSize(VSub(Re.hit_point, point));

	return Re;
};

//点と線分の最短距離   私的な作りは始点と終点はワールド座標で入れる
POINT_LINE_SHORT Collision3D::PointLineSegShortLength(VECTOR line_start, VECTOR line_end, VECTOR point) {
	VECTOR end = VSub(line_end, line_start);
	POINT_LINE_SHORT Re = Collision3D::PointLineShortLength(line_start, line_end, point);

	if (!Math::CheckAcuteAngle(point, line_start, line_end)) {
		//始点側
		Re.hit_point = line_start;
		Re.length = VSize(VSub(line_start, point));
	}
	else if (!Math::CheckAcuteAngle(point, VAdd(line_start, end), line_start)) {
		//終点側
		Re.hit_point = VAdd(line_start, end);
		Re.length = VSize(VSub(end, point));
	}

	return Re;
}

//2直線の垂直になる場所とベクトル係数
TWOLINE_SHORT Collision3D::TwoLineShortPoint(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end) {
	TWOLINE_SHORT Re;
	VECTOR one_way = VSub(line_1_end, line_1_start);
	VECTOR two_way = VSub(line_2_end, line_2_start);

	//平行な直線か調べる
	if (Math::CheckParallelRelation(line_1_start, line_1_end, line_2_start, line_2_end)) {
		POINT_LINE_SHORT pls = Collision3D::PointLineSegShortLength(line_1_start, line_1_end, line_2_start);
		Re.length = pls.length;
		Re.line_1_point = line_1_start;
		Re.line_1_coefficient = 0.0f;
		Re.line_2_point = pls.hit_point;
		Re.line_2_coefficient = pls.coefficient;

		return Re;
	}

	float Dot_OwTw = VDot(one_way, two_way);//方向1と方向2の垂直な距離
	float Dot_OsOw = VSquareSize(one_way);//線分1と方向1の垂直な距離
	float Dot_TsTw = VSquareSize(two_way);//線分2と方向2の垂直な距離
	VECTOR Sub_OsTs = VSub(line_1_start, line_2_start);//始点1から始点2の方向

	Re.line_1_coefficient = (Dot_OwTw * VDot(two_way, Sub_OsTs) - Dot_TsTw * VDot(one_way, Sub_OsTs)) / (Dot_OsOw * Dot_TsTw - Dot_OwTw * Dot_OwTw);
	Re.line_1_point = VAdd(line_1_start, VScale(one_way, Re.line_1_coefficient));
	Re.line_2_coefficient = VDot(two_way, VSub(Re.line_1_point, line_2_start)) / Dot_TsTw;
	Re.line_2_point = VAdd(line_2_start, VScale(two_way, Re.line_2_coefficient));
	Re.length = VSize(VSub(Re.line_2_point, Re.line_1_point));

	return Re;
}

TWOLINE_SHORT Collision3D::TwoSegmentShortPoint(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end) {

	TWOLINE_SHORT Re;

	VECTOR one_way = VSub(line_1_end, line_1_start);
	VECTOR two_way = VSub(line_2_end, line_2_start);

	float effective_range = 0.001f;

	//縮退　小さすぎて点になっている　点で考える
	//線分1が縮退しているか
	if (VSquareSize(one_way) < effective_range) {
		//線分1が縮退していた
		//線分2も縮退しているか
		if (VSquareSize(two_way) < effective_range) {
			// 線分2は縮退していた
			Re.line_1_point = line_1_start;
			Re.line_2_point = line_2_start;
			Re.line_1_coefficient = 0.0f;
			Re.line_2_coefficient = 0.0f;
			Re.length = VSize(VSub(line_2_start, line_1_start));
			return Re;
		}
		else {
			// 線分2は縮退していなかった
			//線分1の始点と直線2の最短距離
			POINT_LINE_SHORT value = Collision3D::PointLineSegShortLength(line_2_start, line_2_end, line_1_start);
			Re.line_1_point = line_1_start;
			Re.line_1_coefficient = 0.0f;
			Re.line_2_point = value.hit_point;
			Re.line_2_coefficient = value.coefficient;
			Re.length = value.length;
			Re.line_2_coefficient = Math::Clamp(0, 1.0f, Re.line_2_coefficient);
			return Re;
		}
	}
	//線分2が縮退しているか
	else if (VSquareSize(two_way) < effective_range) {
		// 線分2は縮退していた
		//上で線分1は確認したのでここはしない
		POINT_LINE_SHORT value = Collision3D::PointLineSegShortLength(line_1_start, line_1_end, line_2_start);
		Re.line_1_point = value.hit_point;
		Re.line_1_coefficient = value.coefficient;
		Re.line_2_point = line_2_start;
		Re.line_2_coefficient = 0.0f;
		Re.length = value.length;
		Re.line_1_coefficient = Math::Clamp(0, 1.0f, Re.line_1_coefficient);
		return Re;
	}

	//縮退がなかったので線分同士で考える
	//線分1と線分2が並行か
	if (Math::CheckParallelRelation(line_1_start, line_1_end, line_2_start, line_2_end)) {
		//平行だったので垂線の端の一つをＰ1とする
		POINT_LINE_SHORT value = Collision3D::PointLineSegShortLength(line_1_start, line_1_end, line_2_start);
		Re.line_1_point = line_1_start;
		Re.line_1_coefficient = 0.0f;
		Re.line_2_point = value.hit_point;
		Re.line_2_coefficient = value.coefficient;
		Re.length = value.length;
		if (0.0f <= Re.line_2_coefficient && Re.line_2_coefficient <= 1.0f) {
			return Re;
		}
	}
	else {
		//平行でなかったので線分二つの最短距離を求める
		Re = Collision3D::TwoLineShortPoint(line_1_start, line_1_end, line_2_start, line_2_end);
		if (0.0f <= Re.line_1_coefficient && Re.line_1_coefficient <= 1.0f &&
			0.0f <= Re.line_2_coefficient && Re.line_2_coefficient <= 1.0f) {
			return Re;
		}
	}

	//垂線の端が線分の外側にあるので
	//線分1側のベクトル係数を0から1の間にクランプして垂線を降ろす

	Re.line_1_coefficient = Math::Clamp(0, 1, Re.line_1_coefficient);
	Re.line_1_point = VAdd(line_1_start, VScale(one_way, Re.line_1_coefficient));
	POINT_LINE_SHORT value = Collision3D::PointLineSegShortLength(line_2_start, line_2_end, Re.line_1_point);
	Re.line_2_coefficient = value.coefficient;
	Re.line_2_point = value.hit_point;
	Re.length = value.length;

	if (0.0f <= Re.line_2_coefficient && Re.line_2_coefficient <= 1.0f) {
		return Re;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------------------
	Re.line_2_coefficient = Math::Clamp(0, 1, Re.line_2_coefficient);
	Re.line_2_point = VAdd(line_2_start, VScale(two_way, Re.line_2_coefficient));
	value = Collision3D::PointLineSegShortLength(line_1_start, line_1_end, Re.line_2_point);

	Re.line_1_coefficient = value.coefficient;
	Re.line_1_point = value.hit_point;
	Re.length = value.length;

	if (0.0f <= Re.line_1_coefficient && Re.line_1_coefficient <= 1.0f) {
		return Re;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	Re.line_1_coefficient = Math::Clamp(0, 1, Re.line_1_coefficient);
	Re.line_1_point = VAdd(line_1_start, VScale(one_way, Re.line_1_coefficient));
	Re.length = VSize(VSub(Re.line_2_point, Re.line_1_point));
	return Re;
}

bool Collision3D::SphereCol(VECTOR pos1, float r1, VECTOR pos2, float r2) {
	return VSquareSize(VSub(pos2, pos1)) <= (r1 + r2) * (r1 + r2);
}

bool Collision3D::TwoCapselCol(VECTOR line_1_start, VECTOR line_1_end, float r_1, VECTOR line_2_start, VECTOR line_2_end, float r_2) {
	TWOLINE_SHORT value = Collision3D::TwoSegmentShortPoint(line_1_start, line_1_end, line_2_start, line_2_end);
	return Collision3D::SphereCol(value.line_1_point, r_1, value.line_2_point, r_2);
}

//点とＯＢＢの最接近点
VECTOR Collision3D::PointOBB(VECTOR point, OBB obb) {
	VECTOR pos = VSub(obb.pos, VTransform(obb.Misalignment, MGetRotY(obb.direction[1])));
	VECTOR cp = VSub(point, pos);
	VECTOR Re = pos;
	float length = 0.0f;
	for (int i = 0; i < 3; i++) {
		length = VDot(cp, obb.dir_vec[i]);

		if (length > (obb.length[i] / 2)) {
			length = obb.length[i] / 2;
		}
		else if (length < -obb.length[i] / 2) {
			length = -obb.length[i] / 2;
		}
		Re = VAdd(Re, VScale(obb.dir_vec[i], length));
	}
	return Re;
}

VECTOR Collision3D::PointOBBToBillBoard(VECTOR point, OBB obb) {
	VECTOR pos = obb.pos;
	VECTOR cp = VSub(point, pos);
	VECTOR Re = pos;
	float length = 0.0f;
	for (int i = 0; i < 3; i++) {
		length = VDot(cp, obb.dir_vec[i]);

		if (length > (obb.length[i] / 2)) {
			length = obb.length[i] / 2;
		}
		else if (length < -obb.length[i] / 2) {
			length = -obb.length[i] / 2;
		}
		Re = VAdd(Re, VScale(obb.dir_vec[i], length));
	}
	return Re;
}

bool Collision3D::OBBSphereCol(OBB obb, VECTOR point, float r) {
	VECTOR pos = Collision3D::PointOBB(point, obb);

	VECTOR vector = VSub(pos, point);

	if (VDot(vector, vector) <= r * r) {
		return true;
	}

	return false;
}

bool Collision3D::OBBCapselCol(VECTOR line_start, VECTOR line_end, OBB obb, float r) {
	POINT_LINE_SHORT  a = Collision3D::PointLineSegShortLength(line_start, line_end, VSub(obb.pos, VTransform(obb.Misalignment, MGetRotY(obb.direction[1]))));

	VECTOR pos = Collision3D::PointOBB(a.hit_point, obb);

	VECTOR vector = VSub(pos, a.hit_point);

	if (VDot(vector, vector) <= r * r) {
		return true;
	}

	return false;
}

bool Collision3D::OBBCapselCol(Capsule capsule, OBB obb) {
	POINT_LINE_SHORT  cap_pos = Collision3D::PointLineSegShortLength(capsule.down_pos, capsule.up_pos, VSub(obb.pos, VTransform(obb.Misalignment, MGetRotY(obb.direction[1]))));

	VECTOR obb_pos = Collision3D::PointOBB(cap_pos.hit_point, obb);

	VECTOR vector = VSub(obb_pos, cap_pos.hit_point);


	if (VDot(vector, vector) <= capsule.r * capsule.r) {
		return true;
	}

	return false;
}

bool Collision3D::OBBToBillBoardCapselCol(Capsule capsule, OBB obb) {
	POINT_LINE_SHORT  cap_pos = Collision3D::PointLineSegShortLength(capsule.down_pos, capsule.up_pos,obb.pos);

	VECTOR obb_pos = Collision3D::PointOBBToBillBoard(cap_pos.hit_point, obb);

	VECTOR vector = VSub(obb_pos, cap_pos.hit_point);


	if (VDot(vector, vector) <= capsule.r * capsule.r) {
		return true;
	}

	return false;
}