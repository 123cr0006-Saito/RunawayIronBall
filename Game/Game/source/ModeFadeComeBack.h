#pragma once
#include "ModeFade.h"

class ModeFadeComeBack : public ModeFade
{
public:
	ModeFadeComeBack(int Time, std::string mode,int layer,bool IsProcessSkip = false); // ���C���[��ύX����ꍇ
	ModeFadeComeBack(int Time, ModeBase* mode, bool IsProcessSkip = false);  // ���[�h��delete����ꍇ
	ModeFadeComeBack(int Time, ModeBase* mode, std::string modeName, int layer ,bool IsProcessSkip = false);  //���C���[��ύX�� ���[�h��delete����ꍇ
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

