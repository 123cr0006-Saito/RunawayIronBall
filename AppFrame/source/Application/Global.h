#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Function/Timer.h"
#include "../System/Header/Sound/SoundItem.h"
#include "../System/Header/Sound/SoundServer.h"
class Global
{
public:
//関数
	Global();
	~Global();
	bool Init();
	bool ResourceLoad();
	bool SoundLoad();
	void SetExit() { exit_count = true; }
	bool GetExit() { return exit_count; }

	void AddStageNum() { _stageNum++; }
	int GetStageNum() { return _stageNum; }
	void SetAllExp(int allExp) { _allExp = allExp; }
	void SetAllExpAdd(int exp) { _allExp += exp; }
	int GetAllExp() { return _allExp; }
	int GetOldExp() { return _oldExp; }
	void SetOldExp(int exp) { _oldExp = exp; }
	bool GetVibration() { return IsVibration; }
	void SetVibration() { IsVibration = 1 - IsVibration; }

//変数・クラス
	Timer* _timer;
	SoundServer* _soundServer;
	bool exit_count; //ゲーム終了フラグ
	int IsVibration; //振動の有無を格納します。0 :振動なし 1:振動あり
	int _stageNum; //ステージの番号を格納します。
	int _allExp; //全ての経験値を格納します。
	int _oldExp; //ステージが始まった時の経験値を格納します。
};
extern Global global;

