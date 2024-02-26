
#include "appframe.h"

class ModeTitle : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	//項目を選択する処理
	void SelectGameStart();//ゲームを開始する項目の処理
	void SelectOption();//オプション画面に変移する項目の処理
	void SelectGameEnd();//ゲームを終了する項目の処理

	void UpdateSelectItems();//選択されている項目の更新
	void  UpdateSelectToGameStart();//ゲームを開始する項目の更新
	void UpdateCrackedScreen();//画面が割れる処理
	void DrawTitleItems();//タイトルの描画
	void DrawCrackedScreen();//画面が割れ描画


protected:
	XInput* _input;
	int _modeCount;//今選択されている項目の番号
	std::unordered_map<std::string, int> _handleMap;//タイトルで使用する画像ハンドルのマップ
	bool _IsGameStart;//ゲームを開始したかどうか
	//-----------------
	//タイトルが割れる処理の変数
	int _modelHandle;//割れるモデルのハンドル
	int _frameSize;//割れるモデルのフレーム数
	int _currentTime;//現在の時間
	bool _IsBreak;//割れる処理が始まったかどうか
	VECTOR* _MoveVec;//移動ベクトル
	VECTOR* _rotVec;//回転ベクトル radian値

}; 
