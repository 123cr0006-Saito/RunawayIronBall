
#include "appframe.h"

class ModeTitle : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SelectGameStart();//ゲームを開始する項目の処理
	void SelectOption();//オプション画面に変移する項目の処理
	void SelectGameEnd();//ゲームを終了する項目の処理
protected:
	XInput* _input;
	int _modeCount;//今選択されている項目の番号
}; 
