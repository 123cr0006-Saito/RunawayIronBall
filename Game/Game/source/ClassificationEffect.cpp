#include "ClassificationEffect.h"
ClassificationEffect* ClassificationEffect::_instance = nullptr;
ClassificationEffect::ClassificationEffect() {
	_instance = this;
	// ファイルパス
	std::string filePath = "Data/FrameData/DirectionList.csv";
	//ファイル読み込み
	CFile csvFile(filePath);
	//ファイルが開いていたら実行
	if (csvFile.Success()) {
		int c = 0;
		const char* p = (const char*)csvFile.Data();
		int size = csvFile.Size(); 
		std::pair<std::string, int> param;
		c += GetString(&p[c], &param.first); // エフェクト名を取得
		c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &param.second); // keyを取得
		c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする

		// データを追加
		_commandList[param.second] = param.first;
	}
};

ClassificationEffect::~ClassificationEffect() {
	_commandList.clear();
	_instance = nullptr;
};

void ClassificationEffect::SetClassification(CommandParam param) {
	if (param.first == Play_Vibration_X) {
		// バイブレーション
	}
	else if (param.first == Play_Vibration_Y) {
		// バイブレーション
	}
	else if (param.first == Play_SE) {
		// SE
	}
	else if (param.first == Play_CameraVibration) {
		// カメラバイブレーション
	}
	else if (param.first == Play_Effect_PC) {
		// エフェクト
	}
	else if (param.first == Play_Effect_IC) {
		// エフェクト
	}
	else if (param.first == Play_Effect_IU) {
		// エフェクト
	}
	else if (param.first == Play_Effekseer_PC) {
		// エフェクシア
	}
	else if (param.first == Play_Effekseer_IC) {
		// エフェクシア
	}
	else if (param.first == Play_Effekseer_IU) {
		// エフェクシア
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "エフェクトコマンドが見つかりませんでした。", "エラー", MB_OK);
#endif
	}
};