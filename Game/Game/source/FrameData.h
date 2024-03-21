#pragma once
#include "appframe.h"
#include <map>
#include <string>
#include <vector>

#include "MotionList.h"

typedef std::pair<int, float> CommandParam;

class FrameData
{
public:
	//コマンド
	//プレイヤー　0番台
    //エネミー　   100番台
    //エフェクト　1000番台

	FrameData();
	~FrameData();

	bool LoadData(std::string, std::vector<std::pair<int, std::string>> frameData);

	bool LoadData(std::string kindName, const std::vector<MotionNamePair>& motionList);

	void Process(int state, int animTime);

	std::vector<CommandParam> GetCommandData() { return _nextCommandList; };

	static std::map<std::string,std::map<int, std::multimap<int, CommandParam>>> _kindFrameData;//キャラクターの種類別に持つデータ

protected:
	//map key _state : multimap key _animTime : _commandParam
	std::map<int, std::multimap<int, CommandParam>>_frameData;//それぞれが持つフレームデータ
	std::vector<CommandParam> _nextCommandList;//次のフレームで実行するコマンドリスト
};

