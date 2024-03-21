#pragma once
class Suppression
{
public:
	Suppression() ;
	~Suppression();
	void AddSuppression(int suppression);
	void ClearSuppression();
	void SubSuppression(int suppression);
	bool GetIsRatio();
	int GetNowSuppression() { return _nowSuppression; }
	int GetMaxSuppression() { return _maxSuppression; }

	static Suppression* _instance;
	static Suppression* GetInstance() { return _instance; };

protected:
	
	int _nowSuppression, _maxSuppression;
};

