//----------------------------------------------------------------------
// @filename myJson.cpp
// ＠date: 2024/01/18
// ＠author: saito ko
// @explanation
// jsonファイルを読み込むクラス
//----------------------------------------------------------------------
#include "myJson.h"
myJson::myJson(std::string fileName) {
	//ファイルを開く
	std::ifstream file(fileName);
	if (!file) {
		//ファイルが読み込めなかった
		return;
	}
	//ファイルが読み込めたのでjsonクラスに移動
	file >> _json;
	//オブジェクトの数を格納
	_size = _json.size();
};

myJson::~myJson(){
	_json.clear();
};