#include "../../Header/Function/3DDraw.h"
#include"../../Header/Collision/2DCollision.h"


bool TDD::ThrDimPosDraw(VECTOR pos) {
	VECTOR line_w[2] = {
		VGet(-25,0,0),
		VGet(25,0,0)
	};
	VECTOR line_h[2] = {
		VGet(0,-25,0),
		VGet(0,25,0)
	};
	VECTOR line_d[2] = {
		VGet(0,0,-25),
		VGet(0,0,25)
	};

	for (int i = 0; i < 2; i++) {
		line_w[i] = VAdd(line_w[i], pos);
		line_h[i] = VAdd(line_h[i], pos);
		line_d[i] = VAdd(line_d[i], pos);
	}

	DrawLine3D(line_w[0], line_w[1], GetColor(255, 0, 0));
	DrawLine3D(line_h[0], line_h[1], GetColor(0, 255, 0));
	DrawLine3D(line_d[0], line_d[1], GetColor(0, 0, 255));

	return true;
}

bool TDD::TweDimColDraw(VECTOR pos, float w,float d) {
	VECTOR line[5] = {
		// 通常用（左上から右回り）
		VGet(-w / 2.0f,0,-d / 2.0f),	// 左上
		VGet(w / 2.0f,0,-d / 2.0f),	// 右上
		VGet(w / 2.0f,0, d / 2.0f),	// 右下
		VGet(-w / 2.0f,0, d / 2.0f),	// 左下
		VGet(-w / 2.0f,0,-d / 2.0f),	// 左上
	};

	for (int i = 0; i < 5; i++) {
		line[i] = VAdd(line[i], pos);
	}
	for (int i = 0; i < 4; i++) {
		DrawLine3D(line[i], line[i + 1], GetColor(255, 0, 0));
		DrawLine3D(line[0], line[0 + 1], GetColor(255, 255, 0));
		DrawLine3D(line[1], line[1 + 1], GetColor(255, 0, 255));
		DrawLine3D(line[3], line[3 + 1], GetColor(255, 255, 255));
	}
	return true;
}

bool TDD::ThrDimColDraw(VECTOR pos,float w, float h, float d, float direction) {
	MATRIX matrix = MGetRotY(direction);
	VECTOR line_down[5] = {
		// 通常用（左上から右回り）
		VGet(-w / 2.0f,0,-d / 2.0f),	// 左上
		VGet(w / 2.0f,0,-d / 2.0f),	// 右上
		VGet(w / 2.0f,0, d / 2.0f),	// 右下
		VGet(-w / 2.0f,0, d / 2.0f),	// 左下
		VGet(-w / 2.0f,0,-d / 2.0f),	// 左上
	};
	VECTOR line_up[5] = {
		// 通常用（左上から右回り）
		VGet(-w / 2.0f,h,-d / 2.0f),	// 左上
		VGet(w / 2.0f,h,-d / 2.0f),	// 右上
		VGet(w / 2.0f,h, d / 2.0f),	// 右下
		VGet(-w / 2.0f,h, d / 2.0f),	// 左下
		VGet(-w / 2.0f,h,-d / 2.0f),	// 左上
	};

	
	for (int i = 0; i < 5; i++) {
		line_down[i] = VTransform(line_down[i], matrix);
		line_up[i] = VTransform(line_up[i], matrix);
	}

	for (int i = 0; i < 4; i++) {
		DrawLine3D(line_down[i], line_down[i + 1], GetColor(255, 0, 0));
		DrawLine3D(line_up[i], line_up[i + 1], GetColor(255, 0, 0));
		DrawLine3D(line_down[i], line_up[i], GetColor(255, 0, 0));
	}
	return true;
}


bool TDD::ThrDimColToBillBoradDraw(VECTOR pos, float w, float h, float d, float direction_z = 0) {
	VECTOR line_down[5] = {
		// 通常用（左上から右回り）
		VGet(-w / 2.0f,-h/2,-d / 2.0f),	// 左上
		VGet( w / 2.0f,-h/2,-d / 2.0f),	// 右上
		VGet( w / 2.0f,-h/2, d / 2.0f),	// 右下
		VGet(-w / 2.0f,-h/2, d / 2.0f),	// 左下
		VGet(-w / 2.0f,-h/2,-d / 2.0f),	// 左上
	};
	VECTOR line_up[5] = {
		// 通常用（左上から右回り）
		VGet(-w / 2.0f,h/2,-d / 2.0f),	// 左上
		VGet( w / 2.0f,h/2,-d / 2.0f),	// 右上
		VGet( w / 2.0f,h/2, d / 2.0f),	// 右下
		VGet(-w / 2.0f,h/2, d / 2.0f),	// 左下
		VGet(-w / 2.0f,h/2,-d / 2.0f),	// 左上
	};

	MATRIX matrix = MGetRotZ(direction_z);
	matrix = MMult(matrix, GetCameraBillboardMatrix());

	for (int i = 0; i < 5; i++) {
		line_down[i] = VTransform(line_down[i], matrix);
		line_up[i] = VTransform(line_up[i], matrix);
	}

	for (int i = 0; i < 5; i++) {
		line_down[i] = VAdd(line_down[i], pos);
		line_up[i] = VAdd(line_up[i], pos);
	}

	for (int i = 0; i < 4; i++) {
		DrawLine3D(line_down[i], line_down[i + 1], GetColor(255, 0, 0));
		DrawLine3D(line_up[i], line_up[i + 1], GetColor(255, 0, 0));
		DrawLine3D(line_down[i], line_up[i], GetColor(255, 0, 0));
	}

	return true;
};

bool TDD::TweDimColOfCircleDraw(VECTOR pos, float r) {
	for (int i = 0; i < 360; i++) {
		DrawPixel3D(VGet(pos.x + r * cos(i), pos.y, pos.z + r * sin(i)), GetColor(255, 0, 0));
	}
	return true;
}

bool TDD::ThrDimColOfCircleDraw(VECTOR pos, float r, float h,bool flag) {
	for (int i = 0; i < 360; i++) {
		DrawPixel3D(VGet(pos.x + r * cos(Math::DegToRad(i)), pos.y, pos.z + r * sin(Math::DegToRad(i))), GetColor(255, 0, 0));
		DrawPixel3D(VGet(pos.x + r * cos(Math::DegToRad(i)), pos.y + (h / 2), pos.z + r * sin(Math::DegToRad(i))), GetColor(255, 0, 0));
		DrawPixel3D(VGet(pos.x + r * cos(Math::DegToRad(i)), pos.y + h, pos.z + r * sin(Math::DegToRad(i))), GetColor(255, 0, 0));
		if (flag) {
			if ((i % 90) == 0) {
				DrawLine3D(VGet(pos.x + r * cos(Math::DegToRad(i)), pos.y, pos.z + r * sin(Math::DegToRad(i))), VGet(pos.x + r * cos(Math::DegToRad(i)), pos.y + h, pos.z + r * sin(Math::DegToRad(i))), GetColor(255, 0, 0));
			}
		}
	}
	return true;
}