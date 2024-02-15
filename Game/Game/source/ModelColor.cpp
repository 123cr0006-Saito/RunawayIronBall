#include "ModelColor.h"

ModelColor::ModelColor()
{
	_modelHandle = -1;
	_flickerEmissiveColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	_flickerTextureHandle = -1;
}

ModelColor::~ModelColor()
{
}

void ModelColor::Init(int modelHandle)
{
	_modelHandle = modelHandle;

	for (int i = 0; i < MV1GetMaterialNum(_modelHandle); i++)
	{
		MATERIAL* material = new MATERIAL();
		material->index = i;
		material->textureHandle = MV1GetTextureGraphHandle(_modelHandle, i);
		material->color = MV1GetMaterialEmiColor(_modelHandle, i);

		_defaultMaterial.push_back(material);
	}

	_flickerEmissiveColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	_flickerTextureHandle = ResourceServer::LoadGraph("res/Character/cg_player_girl/white.png");
}

void ModelColor::SetTexture(int textureHandle)
{
}

void ModelColor::SetEmissiveColor(COLOR_F color)
{
}

void ModelColor::ChangeFlickerTexture(bool b)
{
	for (auto itr = _defaultMaterial.begin(); itr != _defaultMaterial.end(); ++itr)
	{
		int nextTextureHandle;
		COLOR_F nextColor;
		if (b) {
			nextTextureHandle = _flickerTextureHandle;
			nextColor = _flickerEmissiveColor;
		}
		else {
			nextTextureHandle = (*itr)->textureHandle;
			nextColor = (*itr)->color;
		}
		MV1SetMaterialEmiColor(_modelHandle, (*itr)->index, nextColor);
		MV1SetTextureGraphHandle(_modelHandle, (*itr)->index, nextTextureHandle, FALSE);
	}
}
