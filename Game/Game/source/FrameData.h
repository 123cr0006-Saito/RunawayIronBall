#pragma once
#include "appframe.h"
#include <map>
#include <string>
#include <vector>

class FrameData
{
public:
	typedef std::pair<int, float> CommandParam;
	//コマンド
	//プレイヤー　0番台
    //エネミー　   100番台
    //エフェクト　1000番台

	FrameData();
	~FrameData();
	bool LoadData(std::string, std::vector<std::pair<int, std::string>> frameData);

	void Process(int state, int animTime);
	
	static std::map<std::string,std::map<int, std::unordered_multimap<int, CommandParam>>> _kindFrameData;//キャラクターの種類別に持つデータ

protected:
	//map key _state : multimap key _animTime : _commandParam
	std::map<int, std::unordered_multimap<int, CommandParam>>_frameData;//それぞれが持つフレームデータ
	std::vector<CommandParam> _nextCommandList;//次のフレームで実行するコマンドリスト
};

