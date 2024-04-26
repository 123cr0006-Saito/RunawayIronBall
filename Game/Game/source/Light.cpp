//----------------------------------------------------------------------
// @filename Light.cpp
// @date: 2024/03/01
// @author: saito ko
// @explanation
// ファイルからデータを読み込みライトを設定するクラス
//----------------------------------------------------------------------
#include "Light.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param fileName ライトのデータが入ったファイル名
//----------------------------------------------------------------------
Light::Light(std::string fileName) {
	LoadLightColor(fileName);
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
Light::~Light() {
	DeleteLightHandle(_lightHandle);
};
//----------------------------------------------------------------------
// @brief ライトの色を設定する
// @param fileName ライトのデータが入ったファイル名
// @return 成功しているどうか
//----------------------------------------------------------------------
bool Light::LoadLightColor(std::string fileName) {
	std::string filePath = "Data/LightData/" + fileName + ".csv";
	//ファイル読み込み
	CFile csvFile(filePath);
	//ファイルが開いた場合実行
	if (csvFile.Success()) {
		int c = 0;
		const char* p = (const char*)csvFile.Data();
		int size = csvFile.Size();
		int key = 0;
		VECTOR dir;
		COLOR_F color;
		{
		    c += GetFloatNum(&p[c], &dir.x);//パラメータを検索
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &dir.y);//パラメータを検索
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &dir.z);//パラメータを検索
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &color.r);//パラメータを検索
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &color.g);//パラメータを検索
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &color.b);//パラメータを検索
			color.a = 0.0f;//α値なし
			_lightHandle = CreateDirLightHandle(dir);
			SetLightDifColorHandle(_lightHandle, color);
		}
	}
	return true;
};