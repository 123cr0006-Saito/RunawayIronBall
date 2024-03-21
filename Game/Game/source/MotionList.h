#pragma once
#include <vector>
#include <string>
#include <map>

typedef std::pair<std::string, int> MotionNamePair;
typedef std::string CHARA_NAME;

class MotionList
{
public:
	MotionList();
	~MotionList();

	static void Load(CHARA_NAME charaName, std::string fileName);
	static void DeleteAll();

	static std::vector<MotionNamePair>* GetMotionList(CHARA_NAME charaName);
	

private:
	static std::map<CHARA_NAME, std::vector<MotionNamePair>> _allCharaMotionList;
};