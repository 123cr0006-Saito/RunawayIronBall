#include "Light.h"
Light::Light(std::string fileName) {
	LoadLightColor(fileName);
};

Light::~Light() {
	DeleteLightHandle(_lightHandle);
};

bool Light::LoadLightColor(std::string fileName) {
	std::string filePath = "Data/LightData/" + fileName + ".csv";
	//�t�@�C���ǂݍ���
	CFile csvFile(filePath);
	//�t�@�C�����J�����ꍇ���s
	if (csvFile.Success()) {
		int c = 0;
		const char* p = (const char*)csvFile.Data();
		int size = csvFile.Size();
		int key = 0;
		VECTOR dir;
		COLOR_F color;
		{
		    c += GetFloatNum(&p[c], &dir.x);//�p�����[�^������
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &dir.y);//�p�����[�^������
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &dir.z);//�p�����[�^������
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &color.r);//�p�����[�^������
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &color.g);//�p�����[�^������
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &color.b);//�p�����[�^������
			color.a = 0.0f;//���l�Ȃ�
			_lightHandle = CreateDirLightHandle(dir);
			SetLightDifColorHandle(_lightHandle, color);
		}
	}
	return true;
};