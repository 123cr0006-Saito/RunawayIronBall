#pragma once
#include "BuildingBase.h"
#include "TowerParts.h"

class Tower : public BuildingBase
{
	typedef BuildingBase base;

public:
	Tower();
	~Tower();

	//bool Init(int modelHandle, VECTOR startPos) override;
	bool Init(std::array<int, 3> modelHandle, VECTOR startPos);
	bool Process() override;
	bool Render() override;

	void BlastOffProcess();

	void SetFalling(VECTOR vDir);

	void UpdateCollision();

	// デバッグ情報の表示
	bool DrawDebugInfo() override;

protected:
	
	struct TOWER_PARTS_INFO {
		// この要素が使用中かどうか
		bool use;

		bool blast;
		VECTOR blastDir;

		// パーツのモデルハンドル
		int modelHandle;
		// パーツの位置
		VECTOR pos;
		// 各軸における1フレームの回転値
		VECTOR vRot;
	};

	// パーツの情報
	std::vector<TowerParts*> _towerParts;



	
	bool _isFalling;
	int _prevFallCnt;
	int _fallCnt;


	int _bottomIndex;
	VECTOR _endPos;

	float _r;
	float _up;
};