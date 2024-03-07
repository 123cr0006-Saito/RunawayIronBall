#include "Light.h"
Light::Light(std::string fileName) {
	LoadLightColor(fileName);
};

Light::~Light() {
	DeleteLightHandle(_lightHandle);
};

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