#include "Tower.h"

Tower::Tower()
{
}

Tower::~Tower()
{
}

bool Tower::Init(std::array<int, 3> modelHandle, VECTOR startPos)
{
	for (int i = 0; i < 3; i++) {
		if (modelHandle[i] == -1) {
			return false;
		}

		TOWER_PARTS_INFO* pInfo = new TOWER_PARTS_INFO();
		pInfo->use = true;
		pInfo->modelHandle = modelHandle[i];
		if (i == 0) {
			pInfo->pos = startPos;
		}
		else {
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m =  MV1GetFrameLocalWorldMatrix(_partsInfo[i - 1]->modelHandle, 3);
			pInfo->pos = VTransform(vOrigin, m);
		}
		MV1SetPosition(pInfo->modelHandle, pInfo->pos);

		pInfo->vRot = VGet(0.0f, 0.0f, 0.0f);
		_partsInfo.push_back(pInfo);
	}


	return true;
}

bool Tower::Process()
{
	return true;
}

bool Tower::Render()
{
	bool successAll = true;
	for (auto itr = _partsInfo.begin(); itr != _partsInfo.end(); ++itr) {
		int success = MV1DrawModel((*itr)->modelHandle);
		if (success == -1) {
			successAll = false;
		}
	}
	return successAll;
}

bool Tower::DrawDebugInfo()
{
	if (!base::DrawDebugInfo()) { return false; }
	return true;
}
