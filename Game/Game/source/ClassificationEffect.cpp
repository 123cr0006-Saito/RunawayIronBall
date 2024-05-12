//----------------------------------------------------------------------
// @filename ClassificationEffect.cpp
// @date: 2024/02/28
// @author: saito ko
// @explanation
// FrameDataからのコマンドを受け取り、それに応じたエフェクトやSEを再生するクラス
//----------------------------------------------------------------------
#include "ClassificationEffect.h"
ClassificationEffect* ClassificationEffect::_instance = nullptr;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return なし
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
ClassificationEffect::~ClassificationEffect() {
	_commandList.clear();
	_instance = nullptr;
	delete vib ; vib = nullptr;
	delete _sVib; _sVib = nullptr;
};
//----------------------------------------------------------------------
// @brief 毎フレーム行う処理
// @return なし
//----------------------------------------------------------------------
void ClassificationEffect::Process(){
	_sVib->UpdateScreenVibration();
};
//----------------------------------------------------------------------
// @brief コマンドに応じた処理を行う
// @param param コマンドパラメータ
// @return なし
//----------------------------------------------------------------------
void ClassificationEffect::SetClassification(CommandParam param) {
	// コマンドに応じた処理を行う
	// コントローラー振動
	if (param.first == Play_Vibration) {
		if(global.GetVibration()) vib->SetVibrationController(param.second,1000);
	}
	// SE再生
	else if (param.first == Play_SE) {
		int voiceNum = static_cast<int>(param.second);
		std::string voiceName = _commandList[voiceNum].first;
		int randomMax = _commandList[voiceNum].second;
		if (randomMax != 0) {
			int randomNum = rand() % randomMax + 1;
			voiceName += std::to_string(randomNum);
		}
		global._soundServer->DirectPlay(voiceName);
	}
	// 画面振動(x軸)
	else if (param.first == Play_CameraVibration_X) {
		ScreenVibration::GetInstance()->SetVibrationX(param.second, 20);
	}
	// 画面振動(y軸)
	else if (param.first == Play_CameraVibration_Y) {
		ScreenVibration::GetInstance()->SetVibrationY(param.second, 20);
	}
	// 砂煙エフェクト
	else if (param.first == Play_Effect_Dust) {
		int effectName = static_cast<int>(param.second);
		ResourceServer::Mult handle = ResourceServer::SearchMult(_commandList[effectName].first.c_str());
		if (handle.AllNum != 0) {
			VECTOR pos = Player::GetInstance()->GetIBPos();
			float animSpeed = 1.0f / 60.0f * 1000;
			BoardPolygonDust* dust = NEW BoardPolygonDust(pos, _commandList[effectName].second, handle.handle, handle.AllNum, animSpeed);
			EffectManeger::GetInstance()->LoadEffect(dust);
		}
	}
	// エフェクシア (プレイヤー中心)
	else if (param.first == Play_Effekseer_PC) {	
		VECTOR* vec = Player::GetInstance()->GetPositionPtr();
		VECTOR* dir = Player::GetInstance()->GetForwardDirPtr();
		float height = Player::GetInstance()->GetCollision().up_pos.y / 2.0f; // 高さの半分を割り出す
		CreateEffeckseer(param.second, vec,height,*dir);
	}
	// エフェクシア(鉄球中心)
	else if (param.first == Play_Effekseer_IC) {
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		CreateEffeckseer(param.second, pos);
	}
	// エフェクシア (鉄球足元)
	else if (param.first == Play_Effekseer_IU) {
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		float height = Player::GetInstance()->GetIBBodyCollision().r * -1.0f; // 高さの半分を割り出す 今回は高さぶん下げるので -1を掛ける IBbodyCollisionの半径を取得
		CreateEffeckseer(param.second, pos,height);
	}
	// エフェクシア (回転攻撃)
	else if (param.first == Play_Effekseer_Rotation) {
		int effectName = static_cast<int>(param.second);
		int handle = ResourceServer::SearchSingle(_commandList[effectName].first.c_str(), ResourceServer::TYPE::Efk);
		if (handle != -1) {
			VECTOR* pos = Player::GetInstance()->GetPositionPtr();
			VECTOR* dir = Player::GetInstance()->GetForwardDirPtr();
			float height = Player::GetInstance()->GetCollision().up_pos.y; // 高さの半分を割り出す
			EffekseerRotation* effect = NEW EffekseerRotation(handle, pos, _commandList[effectName].second, dir, height);
			EffectManeger::GetInstance()->LoadEffect(effect);
		}
	}
	// エラー
	else {
#ifdef _DEBUG
		MessageBox(NULL, "エフェクトコマンドが見つかりませんでした。", "エラー", MB_OK);
#endif
	}
};
//----------------------------------------------------------------------
// @brief エフェクトの生成
// @param param 検索するエフェクトの番号
// @param pos 座標
// @param height 基準座標から+y軸方向にずらす高さ
// @param rotation 回転値
// @return なし
//----------------------------------------------------------------------
void ClassificationEffect::CreateEffeckseer(float param, VECTOR* pos, float height , VECTOR rotation ) {
	int effectName = static_cast<int>(param);
	int handle = ResourceServer::SearchSingle(_commandList[effectName].first.c_str(), ResourceServer::TYPE::Efk);
	EffekseerBase* effekseer = NEW EffekseerPosSynchro(handle, pos, _commandList[effectName].second, rotation,height);
	EffectManeger::GetInstance()->LoadEffect(effekseer);
};