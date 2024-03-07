#include "Global.h"

Global global;

Global::Global() {
	_timer = new Timer();
	_soundServer = new SoundServer();
	_timer->Start();
	exit_count = false;
	Init();
};

Global::~Global() {
	delete _timer;
	delete _soundServer;
};

bool Global::Init() {
	SoundLoad();
	return true;
};

bool Global::SoundLoad() {
	// ”ñ“¯Šú“Ç‚İ‚İİ’è
	SetUseASyncLoadFlag(TRUE);
	//BGM
	_soundServer->Add("Title", new SoundItemBGM("Sound/BGM/Bgm_title.mp3"));
	_soundServer->Add("Stage03", new SoundItemBGM("Sound/BGM/Bgm_Stage.mp3"));
	//SE
	//Object
	_soundServer->Add("OBJ_GlassBreak", new SoundItemSE("Sound/SE/Object/SE_HO_Glass_Break.wav"));
	_soundServer->Add("OBJ_GlassHit", new SoundItemSE("Sound/SE/Object/SE_HO_Glass_Hit.wav"));
	_soundServer->Add("OBJ_IronBreak", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Break.wav"));
	_soundServer->Add("OBJ_IronHit", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Hit.wav"));
	_soundServer->Add("OBJ_RockBreak", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Break.wav"));
	_soundServer->Add("OBJ_RockHit", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Hit.wav"));
	//Player
	_soundServer->Add("PL_Avoidance", new SoundItemSE("Sound/SE/Player/SE_PL_Avoidance.wav"));
	_soundServer->Add("PL_Damage", new SoundItemSE("Sound/SE/Player/SE_PL_Damage.wav"));
	_soundServer->Add("PL_Flight", new SoundItemSE("Sound/SE/Player/SE_PL_Flight.wav"));
	_soundServer->Add("PL_Heal", new SoundItemSE("Sound/SE/Player/SE_PL_Heal.wav"));
	_soundServer->Add("PL_LevelUP", new SoundItemSE("Sound/SE/Player/SE_PL_Level_Up.wav"));
	_soundServer->Add("PL_Jump", new SoundItemSE("Sound/SE/Player/SE_PL_Long_Jump.wav"));
	_soundServer->Add("PL_Rotation", new SoundItemSE("Sound/SE/Player/SE_PL_Rotation.wav"));
	_soundServer->Add("PL_Swing", new SoundItemSE("Sound/SE/Player/SE_PL_Side_Swing.wav"));
	//Select
	_soundServer->Add("SE_Open", new SoundItemSE("Sound/SE/Select/SE_Menu_Open.wav"));
	_soundServer->Add("SE_Press", new SoundItemSE("Sound/SE/Select/SE_Menu_Press.wav"));
	_soundServer->Add("SE_Select", new SoundItemSE("Sound/SE/Select/SE_Menu_Select.wav"));
	// ”ñ“¯Šú“Ç‚İ‚İI—¹
	SetUseASyncLoadFlag(FALSE);
	return true;
}