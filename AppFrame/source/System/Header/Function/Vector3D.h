#pragma once
#include "dxlib.h"
#include <math.h>
class Vector3D
{
	//スカラー積？ スカラー減算?
public:
	float x, y, z;

	Vector3D()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	};

	Vector3D(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	};

	// DxLib の VECTOR 型 から Vector3D 型 へ変換する
	Vector3D(VECTOR vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	};

	void Set(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}

	void Set(VECTOR vec) {
		Set(vec.x, vec.y, vec.z);
	}

	inline Vector3D operator=(const VECTOR& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return (*this);
	}

	//和
	Vector3D operator+(const Vector3D& vec)const
	{
		Vector3D tmp;
		tmp.x = x + vec.x;
		tmp.y = y + vec.y;
		tmp.z = z + vec.z;
		return tmp;
	}

	//差
	Vector3D operator-(const Vector3D& vec)const
	{
		Vector3D tmp;
		tmp.x = x - vec.x;
		tmp.y = y - vec.y;
		tmp.z = z - vec.z;
		return tmp;
	}

	//内積
	float operator*(const Vector3D& vec)const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	float Dot(const Vector3D& left, const Vector3D right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	// +=
	Vector3D operator+=(const Vector3D& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return (*this);
	}

	// -=
	Vector3D operator-=(const Vector3D& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return (*this);
	}

	// *= スカラ－積
	Vector3D operator*=(const int scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return (*this);
	}

	Vector3D operator*=(const float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return (*this);
	}

	// /= スカラ－減算
	Vector3D operator/=(const int scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		return (*this);
	}

	Vector3D operator/=(const float scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		return (*this);
	}

	//ベクトルの長さ
	float Len() const
	{
		return sqrt(x * x + y * y + z * z);
	};

	//ベクトルの長さ 2乗
	float Sqlen() const
	{
		return x * x + y * y + z * z;
	}

	// 二つのベクトル（始点と終点）からの長さを得る
	static float Sqlen(const Vector3D& start, const Vector3D& end)
	{
		float len = 0.0f;
		len += (start.x - end.x) * (start.x - end.x);
		len += (start.y - end.y) * (start.y - end.y);
		len += (start.z - end.z) * (start.z - end.z);
		return len;
	}

	//単位ベクトルを返す
	//長さがなかった場合0ベクトルが返る
	Vector3D Normalize() const
	{
		Vector3D tmp;
		float len = this->Len();
		if (len > 0)
		{
			tmp.x = x / len;
			tmp.y = y / len;
			tmp.z = z / len;
		}
		return tmp;
	}

	// Vector3D → DxLibのVECTOR 形式に変換
	VECTOR toVECTOR() const
	{
		VECTOR tmp = VGet(0.0f,0.0f,0.0f);
		tmp.x = x;
		tmp.y = y;
		tmp.z = z;
		return tmp;
	}
};

inline Vector3D operator*(const float scale, const Vector3D& vec)
{
	Vector3D tmp(vec);
	tmp *= scale;
	return tmp;
}

inline Vector3D operator*(const Vector3D& vec, const float scale)
{
	Vector3D tmp(vec);
	tmp *= scale;
	return tmp;
}

//スカラー減算
inline Vector3D operator/(const Vector3D& vec, const float scale)
{
	Vector3D tmp(vec);
	tmp /= scale;
	return tmp;
}

// 外積
inline Vector3D operator%(const Vector3D& left, const Vector3D& right)
{
	Vector3D tmp;
	tmp.x = left.y * right.z - left.z * right.y;
	tmp.y = left.z * right.x - left.x * right.z;
	tmp.z = left.x * right.y - left.y * right.x;
	return tmp;
};