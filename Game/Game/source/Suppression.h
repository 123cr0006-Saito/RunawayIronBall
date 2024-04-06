//----------------------------------------------------------------------
// @filename Suppression.h
// ＠date: 2024/02/26
// ＠author: saito ko
// @explanation
// 敵や建物を破壊した時に減少していく制圧ゲージを管理するクラス
//----------------------------------------------------------------------
#pragma once
class Suppression
{
public:
	Suppression() ;
	~Suppression();
	void AddSuppression(int suppression);
	void ClearSuppression();
	void SubSuppression(int suppression);
	bool GetIsRatioBut20PercentLess();
	int GetNowSuppression() { return _nowSuppression; }
	int GetMaxSuppression() { return _maxSuppression; }

	static Suppression* _instance;
	static Suppression* GetInstance() { return _instance; };

protected:
	
	int _nowSuppression, _maxSuppression;// 現在の制圧値と最大の制圧値
};

