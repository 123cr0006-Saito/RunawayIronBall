#pragma once
#include "appframe.h"
#include "ObjectBase.h"

class TowerParts : public ObjectBase
{
public:
	friend class Tower;

	TowerParts();
	~TowerParts();

	// ����������
	void Init(int modelHandle, VECTOR startPos);
	// �X�V����
	void Process();
	// �`�揈��
	void Render();

	// �����蔻��̍X�V
	void UpdateCollision();



	void SetUse(bool use) {
		_use = use;
	}

	bool GetUse() {
		return _use;
	}

	VECTOR GetPos() {
		return _pos;
	}

	void SetUseCollision(bool useCollision) {
		_useCollision = useCollision;
	}

	void SetBlast(VECTOR vDir) {
		_blast = true;
		_blastDir = vDir;

		_isFalling = false;
	}

	void SetFalling(VECTOR endPos) {
		_isFalling = true;
		_fallCnt = 0;
		_fallStartPos = _pos;
		_fallEndPos = endPos;
	}

	bool GetIsFalling() { return _isFalling; }

	Sphere GetCollision() { return _sphereCollision; }

protected:
	// ������я���
	void BlastOffProcess();
	// ��������
	void FallProcess();


protected:
	bool _use;

	bool _useCollision;

	bool _blast;
	VECTOR _blastDir;
	int _blastCnt;

	bool _isFalling;
	int _fallCnt;
	VECTOR _fallStartPos;
	VECTOR _fallEndPos;


	int _modelHandle;
	VECTOR _pos;
	VECTOR _vRot;

	Sphere _sphereCollision;

	VECTOR _localCenterPos;


public:
	// �x�[�^�Ł@��
	static std::vector<TowerParts*> _blastTowerParts;
	static void InitBlastTowerParts() {
		_blastTowerParts.clear();
	}
	static void AddBlastTowerParts(TowerParts* towerParts) {
		_blastTowerParts.push_back(towerParts);
	}
	static void CheckFinishedBlastTowerParts() {
		for (auto itr = _blastTowerParts.begin(); itr != _blastTowerParts.end();) {
			if ((*itr)->GetUse() == false) {
				itr = _blastTowerParts.erase(itr);
			}
			else {
				++itr;
			}
		}
	}
};