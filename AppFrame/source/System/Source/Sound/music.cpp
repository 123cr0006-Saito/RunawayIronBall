/*****************************************************************//**
 * \file   music.cpp
 * \brief  ‰¹Šy
 *
 * \author âV“¡ Œè‰H
 * \date   August 2023
 *********************************************************************/
#include "../../Header/Sound/music.h"
#include "dxlib.h"
#include "../../Header/Function/Easing.h"
music::music() {
	for (int i = 0; i < 3; i++) {
		sound_volum[i] = 255;
	}
};

music::~music() {
	
};
									 
void music::SetSeVolume(int se) {
	ChangeVolumeSoundMem(sound_volum[0], se);
};

void music::SetBgmVolume(int bgm) {
	ChangeVolumeSoundMem(sound_volum[1], bgm);
};

void music::SetVoiceVolume(int voice) {
	ChangeVolumeSoundMem(sound_volum[2], voice);
};

void music::MakeSe(int se) {
	PlaySoundMem(se, DX_PLAYTYPE_BACK);
}

void music::MakeBgm(int bgm) {
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void music::MakeVoice(int voice) {
	PlaySoundMem(voice, DX_PLAYTYPE_BACK);
}

void music::StopSe(int Se) {
	StopSoundMem(Se);
}

void music::StopBgm(int Bgm) {
	StopSoundMem(Bgm);
}

