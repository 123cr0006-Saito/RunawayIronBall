#include "Floor.h"
Floor::Floor(){

};

Floor::~Floor() {
	Delete();
};

void Floor::Delete() {
	// 中身があった場合削除
	if (_modelList.size() > 0) {
		for (auto&& delList : _nameList) {
			ResourceServer::MV1DeleteModelAll(delList);
		}
		_nameList.clear();
		_modelList.clear();
	}
};

void Floor::Create(myJson jsonFile, int stageNum) {
	
	// 中身があった場合削除
	Delete();

	// タイルの読み込み
	VECTOR pos, rotate, scale;
	std::string path = "res/Building/";
	std::vector<std::string> nameList = LoadName(stageNum);
	//タイルの種類だけループ
	for (auto&& name : nameList) {
		nlohmann::json loadObject = jsonFile._json.at(name);
		_nameList.push_back(name);
		std::string modelName = path + name + "/" + name + ".mv1";
		//タイルの数だけループ
		for (auto&& object : loadObject) {
			object.at("translate").at("x").get_to(pos.x);
			object.at("translate").at("y").get_to(pos.z);
			object.at("translate").at("z").get_to(pos.y);
			object.at("rotate").at("x").get_to(rotate.x);
			object.at("rotate").at("y").get_to(rotate.z);
			object.at("rotate").at("z").get_to(rotate.y);
			object.at("scale").at("x").get_to(scale.x);
			object.at("scale").at("y").get_to(scale.z);
			object.at("scale").at("z").get_to(scale.y);

			//座標修正
			pos.x *= -1;
			pos.y += 1;
			//degree->radian
			rotate.x = rotate.x * DX_PI_F / 180;
			rotate.y = (rotate.y + 180) * DX_PI_F / 180;
			rotate.z = rotate.z * DX_PI_F / 180;

			int model = ResourceServer::MV1LoadModel(name, modelName);
			MV1SetPosition(model, pos);
			MV1SetRotationXYZ(model, rotate);
			MV1SetScale(model, scale);
			_modelList.push_back(model);
		}
	}
};

std::vector<std::string> Floor::LoadName(int stageNum) {
	std::vector<std::string> nameList;
	std::string filePath = "Data/LoadStageName/Tile/Tile0"  + std::to_string(stageNum) + ".csv";
	// csvファイルを読み込む
	CFile file(filePath);
	// ファイルが開けた場合
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string objectName;
			c += GetString(&p[c], '\r\n', &objectName, size - c); // モデルの名前を取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			nameList.push_back(objectName);
		}
	}
	return nameList;
};

void Floor::Render() {
	for (auto&& list : _modelList) {
		MV1DrawModel(list);
	}
};