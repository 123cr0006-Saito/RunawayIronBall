/*****************************************************************//**
 * \file   music.h
 * \brief  ƒTƒEƒ“ƒh‚ÌÄ¶
 *
 * \author âV“¡ Œè‰H
 * \date   August 2023
 *********************************************************************/
#pragma once


class music
{
public:
	 music();
	 ~music();

	void SetSeVolume( int se);
	void SetBgmVolume( int bgm);
	void SetVoiceVolume( int voice);

	void MakeSe(int num);
	void MakeBgm(int num);
	void MakeVoice(int voice);

	void StopBgm(int Bgm);
	void StopSe(int Se);

protected:
	const int SeMax = 100;
	const int BgmMax = 15;
	const int VoiceMax = 100;
	int sound_volum[3];
};

