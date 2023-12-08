/*****************************************************************//**
 * \file   FadeSound.h
 * \brief  音楽のフェード
 *
 * \author 齋藤 瑚羽
 * \date   August 2023
 *********************************************************************/
#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include "dxlib.h"
class FadeSound
{
public:
	 FadeSound();
	~FadeSound();
	void SetFadeBgm(int* bgm, int old_gauge, int set_count,int hang);
	void FadeBgmProcess();

protected:
	struct SOUND {
		int* change;
		int  old;
		int  now;
		int  count;
		int  max_count;
		bool  use;
		int  hang;
	};
public:
	std::vector<SOUND> change_volume;
};



