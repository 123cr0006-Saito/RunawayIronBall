//----------------------------------------------------------------------
// @filename ModeFadeComeBack.cpp
// @date: 2024/03/02
// @author: saito ko
// @explanation
// ステージ遷移時にフェードイン、フェードアウトを行うためのクラス
// ModeFadeと違う点は常にフェードインしてからフェードアウトし、
// 指定のモードを削除しながら遷移する
//----------------------------------------------------------------------
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
	bool _IsProcessSkip;// フェード時プロセスをスキップするかどうか
	int _fadeEnd, _fadeStart;// フェードの開始と終了のアルファ値
	int _changeLayer;// レイヤーを変更する場合
	std::string _changeModeName;// 削除するモード名
	ModeBase* _deleteMode;// 削除するモード
};

