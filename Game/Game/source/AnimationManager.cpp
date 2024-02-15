#include "AnimationManager.h"

std::map<CHARA_NAME, ANIM_MAP> AnimationManager::_animMap;

AnimationManager::AnimationManager()
{
	_modelHandle = -1;
	_animNo = -1;
	_playTime = 0.0f;
	_charaAnimMapPtr = nullptr;
	_latestAnimItem = nullptr;
}

AnimationManager::~AnimationManager()
{
	for (auto itr = _animContainer.begin(); itr != _animContainer.end(); ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
	_animContainer.clear();
	_charaAnimMapPtr = nullptr;
}

// アニメーション情報用マップコンテナを追加する
//	引数: mapコンテナへのポインタ（各クラスの静的メンバ変数）
void AnimationManager::InitMap(std::map<int, ANIMATION_INFO>* animMap)
{
	if (this->_charaAnimMapPtr == nullptr) {
		this->_charaAnimMapPtr = animMap;
	}
}

void AnimationManager::InitMap(CHARA_NAME charaName, int modelHandle, std::string fileName)
{
	auto itr = _animMap.find(charaName);
	if (itr == _animMap.end()) {
		std::string filePath = "Data/MotionList/" + fileName;

		// csvファイルを読み込む
		CFile file(filePath);
		// ファイルが開けた場合
		if (file.Success()) {
			const char* data = (const char*)file.Data();
			int c = 0;
			int size = file.Size();
			int stateNo = 0;
			while (c < size) {
				std::string motionName;
				int loopTimes = 0;
				// モーション名を取得
				c += GetString(&data[c], &motionName);
				// カンマの位置を見つける
				c += FindString(&data[c], ',', &data[size]); c++;
				// ループ回数を取得
				c += GetDecNum(&data[c], &loopTimes);
				// 改行などスキップ
				c += SkipSpace(&data[c], &data[size]);

				ANIMATION_INFO info;
				info.animIndex = MV1GetAnimIndex(modelHandle, motionName.c_str());
				info.loopTimes = loopTimes;
				_animMap[charaName][stateNo] = info;
				stateNo++;
#ifdef _DEBUG
				if (info.animIndex == -1) {
					std::string message = "[" + motionName + "] アニメーションが見つかりませんでした";

					MessageBox(NULL, message.c_str(), "エラー", MB_OK);
				}
#endif // _DEBUG
			}
		}
#ifdef _DEBUG
		else {
			// ファイルが開けなかった場合
			MessageBox(NULL, "ファイルが開けませんでした", "エラー", MB_OK);
		}
#endif // _DEBUG
	}
	_charaAnimMapPtr = &_animMap[charaName];
	_modelHandle = modelHandle;
}

// ANIMATION_INFO型のアニメーション情報の初期設定を行う
void AnimationManager::SetupAnimationInfo(int statusNo, int animIndex, int loopTimes)
{
	// 引数statusNoに対応するアニメーション情報が存在するか調べる
	auto itr = (*_charaAnimMapPtr).find(statusNo);
	// アニメーション情報が存在しない場合のみ、アニメーション情報を追加する
	if (itr == (*_charaAnimMapPtr).end()) {
		ANIMATION_INFO info;
		info.animIndex = animIndex;
		info.loopTimes = loopTimes;
		(*_charaAnimMapPtr)[statusNo] = info;
	}

}

// アニメーションアイテムを追加する
void AnimationManager::AddAnimationItem(int statusNo)
{
	auto itr = (*_charaAnimMapPtr).find(statusNo);
	// アニメーション情報が存在する場合
	if (itr != (*_charaAnimMapPtr).end())
	{
		AnimationItem* anim = new AnimationItem();

		ANIMATION_INFO info = itr->second;
		int attachIndex = MV1AttachAnim(_modelHandle, info.animIndex, -1, FALSE);
		float totalTime = MV1GetAttachAnimTotalTime(_modelHandle, attachIndex);
		int loopTimes = info.loopTimes;

		anim->Setup(attachIndex, totalTime, loopTimes);
		_animContainer.push_back(anim);
		_latestAnimItem = anim;
	}
}

// アニメーションの再生処理
void AnimationManager::Process(int StatusNo)
{
	// 再生するアニメーションが変わった場合
	if (_animNo != StatusNo) {
		// アタッチされているアニメーションに閉じ時間を設定する
		for (auto itrItem = _animContainer.begin(); itrItem != _animContainer.end(); ++itrItem)
		{
			if ((*itrItem)->_closeTime == 0.0f) {
				// 閉じ時間を設定する
				(*itrItem)->_closeTotalTime = 4.0f;
				(*itrItem)->_closeTime = (*itrItem)->_closeTotalTime;
			}
		}

		// アニメーション番号を更新する
		_animNo = StatusNo;
		// アニメーションアイテムを追加する
		AddAnimationItem(StatusNo);
	}

	// 最後に追加されたアニメーションアイテムの再生時間を取得する
	_playTime = _latestAnimItem->_playTime;

	for (auto itrItem = _animContainer.begin(); itrItem != _animContainer.end(); )
	{
		// 再生時間をセットする
		MV1SetAttachAnimTime(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_playTime);

		/* 再生時間の更新処理 */
		// 閉じ時間が設定されていない場合
		if ((*itrItem)->_closeTime == 0.0f) {
			// 再生時間を進める
			(*itrItem)->_playTime += 1.0f;
			
			// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
			if ((*itrItem)->_playTime > (*itrItem)->_totalTime) {
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
				itrItem = _animContainer.erase(itrItem);
				continue;
			}
			// ブレンド率を変更する
			MV1SetAttachAnimBlendRate(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_closeTime / (*itrItem)->_closeTotalTime);
		}
		// 次のアニメーションアイテムへ
		++itrItem;
	}
}
