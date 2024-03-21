#pragma once
#include "ModeFade.h"

class ModeFadeComeBack : public ModeFade
{
public:
	ModeFadeComeBack(int Time, std::string mode,int layer,bool IsProcessSkip = false); // レイヤーを変更する場合
	ModeFadeComeBack(int Time, ModeBase* mode, bool IsProcessSkip = false);  // モードをdeleteする場合
	ModeFadeComeBack(int Time, ModeBase* mode, std::string modeName, int layer ,bool IsProcessSkip = false);  //レイヤーを変更し モードをdeleteする場合
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process()override;
	virtual bool Render()override;
protected:
	bool _IsProcessSkip;
	int _fadeEnd, _fadeStart;
	int _changeLayer;
	std::string _changeModeName;
	ModeBase* _deleteMode;
};

