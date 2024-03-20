#pragma once
#include "appframe.h"

class ModelColor
{
public:
	ModelColor();
	~ModelColor();

	void Init(int modelHandle);
	void SetTexture(int textureHandle);
	void SetEmissiveColor(COLOR_F color);

	void ChangeFlickerTexture(bool b);

private:
	int _modelHandle;

	struct MATERIAL
	{
		int index;
		int textureHandle;
		COLOR_F color;
	};
	std::vector<MATERIAL*> _defaultMaterial;

	int _flickerTextureHandle;
	COLOR_F _flickerEmissiveColor;
};