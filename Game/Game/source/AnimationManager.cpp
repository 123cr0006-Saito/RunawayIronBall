#include "AnimationManager.h"

AnimationManager::AnimationManager(int modelHandle)
{
	_modelHandle = modelHandle;
	_animNo = -1;
	_playTime = 0.0f;
}

AnimationManager::~AnimationManager()
{
}

// ANIMATION_INFO型のアニメーション情報の初期設定を行う
void AnimationManager::SetupAnimationInfo(int statusNo, int animIndex, int loopTimes)
{
	ANIMATION_INFO info;
	info.animIndex = animIndex;
	info.loopTimes = loopTimes;

	_animInfo[statusNo] = info;
}

// アニメーションアイテムを追加する
void AnimationManager::AddAnimationItem(int statusNo)
{
	auto itr = _animInfo.find(statusNo);
	// アニメーション情報が存在する場合
	if (itr != _animInfo.end())
	{
		AnimationItem* anim = new AnimationItem();

		ANIMATION_INFO info = itr->second;
		int attachIndex = MV1AttachAnim(_modelHandle, info.animIndex, -1, FALSE);
		float totalTime = MV1GetAttachAnimTotalTime(_modelHandle, attachIndex);
		int loopTimes = info.loopTimes;

		anim->Setup(attachIndex, totalTime, loopTimes);
		_anim.push_back(anim);
	}
}

void AnimationManager::Process(int StatusNo)
{
	// 再生するアニメーションが変わった場合
	if (_animNo != StatusNo) {
		// アタッチされているアニメーションに閉じ時間を設定する
		for (auto itrItem = _anim.begin(); itrItem != _anim.end(); ++itrItem)
		{
			if ((*itrItem)->_closeTime == 0.0f) {
				// 閉じ時間を設定する
				(*itrItem)->_closeTotalTime = 6.0f;
				(*itrItem)->_closeTime = (*itrItem)->_closeTotalTime;
			}
		}

		// アニメーション番号を更新する
		_animNo = StatusNo;
		// アニメーションアイテムを追加する
		AddAnimationItem(StatusNo);
	}

	for (auto itrItem = _anim.begin(); itrItem != _anim.end(); )
	{
		if ((*itrItem)->_closeTime == 0.0f) {
			// 再生時間を進める
			(*itrItem)->_playTime += 1.0f;
			
			// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
			if ((*itrItem)->_playTime >= (*itrItem)->_totalTime) {
				if ((*itrItem)->_loopCnt > 1 || (*itrItem)->_loopCnt == 0) {
					if ((*itrItem)->_loopCnt > 1) {
						(*itrItem)->_loopCnt--;
					}
					// 再生時間を0に戻す
					(*itrItem)->_playTime = 0.0f;
				}
				else {
					(*itrItem)->_playTime = (*itrItem)->_totalTime;
				}
			}
		}
		else {
			// 閉じ時間を減らす
			(*itrItem)->_closeTime -= 1.f;

			// 閉じ時間が終わったか？
			if ((*itrItem)->_closeTime <= 0.f) {
				// アニメーションをデタッチする
				MV1DetachAnim(_modelHandle, (*itrItem)->_attachIndex);
				// このアニメーションを削除
				delete (*itrItem);
				itrItem = _anim.erase(itrItem);
				continue;
			}
			// ブレンド率を変更する
			MV1SetAttachAnimBlendRate(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_closeTime / (*itrItem)->_closeTotalTime);
		}
		// 再生時間をセットする
		MV1SetAttachAnimTime(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_playTime);
		// 次のアニメーションアイテムへ
		++itrItem;
	}
}
