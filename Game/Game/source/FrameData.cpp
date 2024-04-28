//----------------------------------------------------------------------
// @filename FrameData.cpp
// @date: 2024/02/09
// @author: saito ko
// @explanation
// キャラクターのモーションのフレームで実行するコマンドを管理するクラス
//----------------------------------------------------------------------
#include "FrameData.h"
#include "ClassificationEffect.h"

std::map<std::string, std::map<int, std::multimap<int, CommandParam>>>  FrameData::_kindFrameData;//キャラクターの種類別に持つデータ
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
FrameData::FrameData(){};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
FrameData::~FrameData() {
	_frameData.clear();
	_kindFrameData.clear();
};
//----------------------------------------------------------------------
// @brief フレームデータの読み込み
// @param kindName キャラクターの種類
// @param motionList モーションの名前が入ったリスト
// @return 成功したかどうか
//----------------------------------------------------------------------
bool FrameData::LoadData(std::string kindName, const std::vector<MotionNamePair>& motionList)
{
	auto itr = _kindFrameData.find(kindName);
	if (itr == _kindFrameData.end()) {
		int i = 0;
		//パラメータを保存しておくリストを作成
		for (auto it = motionList.begin(); it != motionList.end(); ++it) {
			// ファイルパスの作成
			std::string filePath = "Data/FrameData/" + kindName + "/FD_" + it->first + ".csv";
			//ファイル読み込み
			CFile csvFile(filePath);
			//ファイルが開いた場合実行
			std::multimap<int, CommandParam> paramList;
			if (csvFile.Success()) {
				int c = 0;
				const char* p = (const char*)csvFile.Data();
				int size = csvFile.Size();
				int key = 0;
				CommandParam param;
				while (c < size) {
					c += GetDecNum(&p[c], &key);//アニメーションフレームを検索
					c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &param.first);//コマンドを検索
					c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &param.second);//パラメータを検索
					c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
					paramList.insert(std::make_pair(key, param));//リストにため込む
				}
				//終わったので本リストに代入
				_kindFrameData[kindName][i] = paramList;
				
			}
#ifdef _DEBUG
			else {
				std::string errar = filePath + "が開けませんでした。";
				MessageBox(NULL, errar.c_str(), "ファイルが開けません", MB_OK);
			}
#endif
			i++;
		}
	}
	_frameData = _kindFrameData[kindName];
	return true;
};
//----------------------------------------------------------------------
// @brief フレームデータの読み込み
// @param state 現在のアニメーションの状態
// @param animTime 現在のアニメーションのフレーム
// @return 成功したかどうか
//----------------------------------------------------------------------
void FrameData::Process(int state, int animTime) {
	//コンテナの初期化
	if (!_nextCommandList.empty()) {
		_nextCommandList.clear();
	}

	auto itr = _frameData[state].equal_range(animTime);
	for (auto& it = itr.first; it != itr.second ; ++it) {
		if (it->second.first < 1000) {
			//キャラ固有の処理
			_nextCommandList.push_back(it->second);
		}
		else {
			//エフェクト
			ClassificationEffect::GetInstance()->SetClassification(it->second);
		}
	}
};

