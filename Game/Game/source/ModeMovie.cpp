//----------------------------------------------------------------------
// @filename ModeMovie.h
// ＠date: 2024/03/16
// ＠author: saito ko
// @explanation
// ゲームクリア時にエンディングムービーを再生するクラス
//----------------------------------------------------------------------
#include "ModeMovie.h"
#include "ModeTitle.h"
#include "ModeFade.h"
bool ModeMovie::Initialize(){
	_movie = ResourceServer::Load("Ending", "res/Movie/Ending.mp4");
	SeekMovieToGraph(_movie, 0);
	PlayMovieToGraph(_movie);
	return true;
};

bool ModeMovie::Terminate(){
	DeleteGraph(_movie);
	return true;
};

bool ModeMovie::Process(){
	if (!GetMovieStateToGraph(_movie) && !ModeServer::GetInstance()->Search("Fade")) {
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "Title");
		ModeServer::GetInstance()->Add(NEW ModeFade(2000, true), 10, "Fade");
	}
	return true;
};

bool ModeMovie::Render() {
	DrawGraph(0, 0, _movie, true);
	return true;
};