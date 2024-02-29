#include "ClassificationEffect.h"
ClassificationEffect* ClassificationEffect::_instance = nullptr;
ClassificationEffect::ClassificationEffect() {
	_instance = this;
	// ファイルパス
	std::string filePath = "Data/DirectionList/DirectionList.csv";
	//ファイル読み込み
	CFile csvFile(filePath);
	//ファイルが開いていたら実行
	if (csvFile.Success()) {
		int c = 0;
		const char* p = (const char*)csvFile.Data();
		int size = csvFile.Size();
		while (c < size) {
			int key = 0;
			std::pair<std::string, int> param;
			c += GetString(&p[c], &param.first); // エフェクト名を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &key); // keyを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &param.second); // keyを取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			// データを追加
			_commandList[key] = param;
		}
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "DirectionList が開けませんでした。", "エラー", MB_OK);
#endif
	}
};

ClassificationEffect::~ClassificationEffect() {
	_commandList.clear();
	_instance = nullptr;
};

void ClassificationEffect::SetClassification(CommandParam param) {
	if (param.first == Play_Vibration) {
		// バイブレーション
	}
	else if (param.first == Play_SE) {
		// SE
		int seName = static_cast<int>(param.second);
		global._soundServer->DirectPlay(_commandList[seName].first);
	}
	else if (param.first == Play_CameraVibration_X) {
		// カメラバイブレーション X
		ScreenVibration::GetInstance()->SetVibrationX(param.second, 20);
	}
	else if (param.first == Play_CameraVibration_Y) {
		// カメラバイブレーション Y
		ScreenVibration::GetInstance()->SetVibrationY(param.second, 20);
	}
	else if (param.first == Play_Effect_Dust) {
		// エフェクト
		int effectName = static_cast<int>(param.second);
		ResourceServer::Mult handle = ResourceServer::SearchMult(_commandList[effectName].first.c_str());
		if (handle.AllNum != 0) {
			VECTOR pos = Player::GetInstance()->GetIBPos();
			float animSpeed = 1.0f / 60.0f * 1000;
			BoardPolygonDust* dust = new BoardPolygonDust(pos, _commandList[effectName].second, handle.handle, handle.AllNum, animSpeed);
			EffectManeger::GetInstance()->LoadEffect(dust);
		}
	}
	else if (param.first == Play_Effekseer_PC) {
		// エフェクシア プレイヤー中心
		VECTOR vec = VGet(0,0,0);
		CreateEffeckseer(param.second, &vec);
	}
	else if (param.first == Play_Effekseer_IC) {
		// エフェクシア 鉄球中心
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		CreateEffeckseer(param.second, pos);
	}
	else if (param.first == Play_Effekseer_IU) {
		// エフェクシア 鉄球足元
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		CreateEffeckseer(param.second, pos);
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "エフェクトコマンドが見つかりませんでした。", "エラー", MB_OK);
#endif
	}
};

void ClassificationEffect::CreateEffeckseer(float param, VECTOR* pos) {
	int effectName = static_cast<int>(param);
	int handle = ResourceServer::SearchSingle(_commandList[effectName].first.c_str(), ResourceServer::TYPE::Efk);
	EffekseerBase* effekseer = new EffekseerPosSynchro(handle, pos, _commandList[effectName].second);
	EffectManeger::GetInstance()->LoadEffect(effekseer);
};