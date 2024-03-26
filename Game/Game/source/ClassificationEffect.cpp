#include "ClassificationEffect.h"
ClassificationEffect* ClassificationEffect::_instance = nullptr;
ClassificationEffect::ClassificationEffect() {
	_instance = this;
	vib = NEW Vibration(DX_INPUT_PAD1);
	_sVib = NEW ScreenVibration();
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
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &param.second); // パラメーターを取得
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
	delete vib ; vib = nullptr;
	delete _sVib; _sVib = nullptr;
};

void ClassificationEffect::SetClassification(CommandParam param) {
	if (param.first == Play_Vibration) {
		vib->SetVibrationController(param.second,1000);
	}
	else if (param.first == Play_SE) {
		// SE
		int voiceNum = static_cast<int>(param.second);
		std::string voiceName = _commandList[voiceNum].first;
		int randomMax = _commandList[voiceNum].second;
		if (randomMax != 0) {
			int randomNum = rand() % randomMax + 1;
			voiceName += std::to_string(randomNum);
		}
		global._soundServer->DirectPlay(voiceName);
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
			BoardPolygonDust* dust = NEW BoardPolygonDust(pos, _commandList[effectName].second, handle.handle, handle.AllNum, animSpeed);
			EffectManeger::GetInstance()->LoadEffect(dust);
		}
	}
	else if (param.first == Play_Effekseer_PC) {
		// エフェクシア プレイヤー中心
		VECTOR* vec = Player::GetInstance()->GetPositionPtr();
		VECTOR* dir = Player::GetInstance()->GetForwardDirPtr();
		float height = Player::GetInstance()->GetCollision().up_pos.y / 2.0f; // 高さの半分を割り出す
		CreateEffeckseer(param.second, vec,height,*dir);
	}
	else if (param.first == Play_Effekseer_IC) {
		// エフェクシア 鉄球中心
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		CreateEffeckseer(param.second, pos);
	}
	else if (param.first == Play_Effekseer_IU) {
		// エフェクシア 鉄球足元
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		float height = Player::GetInstance()->GetIBBodyCollision().r * -1.0f; // 高さの半分を割り出す 今回は高さぶん下げるので -1を掛ける IBbodyCollisionの半径を取得
		CreateEffeckseer(param.second, pos,height);
	}
	else if (param.first == Play_Effekseer_Rotation) {
		int effectName = static_cast<int>(param.second);
		int handle = ResourceServer::SearchSingle(_commandList[effectName].first.c_str(), ResourceServer::TYPE::Efk);
		if (handle != -1) {
			VECTOR* pos = Player::GetInstance()->GetPositionPtr();
			VECTOR* dir = Player::GetInstance()->GetForwardDirPtr();
			float height = Player::GetInstance()->GetCollision().up_pos.y / 2.0f; // 高さの半分を割り出す
			EffekseerRotation* effect = NEW EffekseerRotation(handle, pos, 1, dir, height);
			EffectManeger::GetInstance()->LoadEffect(effect);
		}
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "エフェクトコマンドが見つかりませんでした。", "エラー", MB_OK);
#endif
	}
};

void ClassificationEffect::CreateEffeckseer(float param, VECTOR* pos, float height , VECTOR rotation ) {
	int effectName = static_cast<int>(param);
	int handle = ResourceServer::SearchSingle(_commandList[effectName].first.c_str(), ResourceServer::TYPE::Efk);
	EffekseerBase* effekseer = NEW EffekseerPosSynchro(handle, pos, _commandList[effectName].second, rotation,height);
	EffectManeger::GetInstance()->LoadEffect(effekseer);
};