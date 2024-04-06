//----------------------------------------------------------------------
// @filename Floor.cpp
// ＠date: 2024/03/03
// ＠author: saito ko
// @explanation
// ゲームステージある床(タイル)を管理するクラス
//----------------------------------------------------------------------
#pragma once
#include "myJson.h"
#include "appframe.h"
class Floor
{
public :
	Floor();
	~Floor();
	void Delete();
	void Create(myJson jsonFile, int stageNum);
	std::vector<std::string> LoadName(int stageNum);
	void Render();
protected:
	std::vector<int> _modelList;//モデルのリスト
	std::vector<std::string> _nameList;//モデルの名前リスト
};

