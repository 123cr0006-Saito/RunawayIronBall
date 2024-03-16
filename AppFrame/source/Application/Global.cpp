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
	// 非同期読み込み設定
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
	// VOICE
	// Plime
	_soundServer->Add("PL_Ah", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Ah.wav"));
	_soundServer->Add("PL_Angry", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Angry.wav"));
	_soundServer->Add("PL_Avoidance01", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Avoidance_01.wav"));
	_soundServer->Add("PL_Avoidance02", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Avoidance_02.wav"));
	_soundServer->Add("PL_Avoidance03", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Avoidance_03.wav"));
	_soundServer->Add("PL_Damage01", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Damage_01.wav"));
	_soundServer->Add("PL_Damage02", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Damage_02.wav"));
	_soundServer->Add("PL_Dead", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Dead_Enemy.wav"));
	_soundServer->Add("PL_GameClear", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Game_Clear.wav"));
	_soundServer->Add("PL_GameOver", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Game_Over.wav"));
	_soundServer->Add("PL_Heal", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Heal.wav"));
	_soundServer->Add("PL_HealthAlert", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Health_Alert.wav"));
	_soundServer->Add("PL_Laugh", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Laugh.wav"));
	_soundServer->Add("PL_LevelUp01", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Level_Up_01.wav"));
	_soundServer->Add("PL_LevelUp02", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Level_Up_02.wav"));
	_soundServer->Add("PL_LostStamina", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Lost_Stamina.wav"));
	_soundServer->Add("PL_PullAttack01", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Pull_Attack.wav"));
	_soundServer->Add("PL_PullAttack02", new SoundItemVOICE("Sound/VOICE/IB/SE_IBVO_Pull_Attack.wav"));
	_soundServer->Add("PL_Question", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Question.wav"));
	_soundServer->Add("PL_SideSwing01", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Side_Swing_01.wav"));
	_soundServer->Add("PL_SideSwing02", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Side_Swing_02.wav"));
	_soundServer->Add("PL_Smile", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Smile.wav"));
	_soundServer->Add("PL_StageClear", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Stage_Clear.wav"));
	_soundServer->Add("PL_Tired", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Tired.wav"));
	_soundServer->Add("PL_Rotation", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_To_Rotation.wav"));
	_soundServer->Add("PL_VerticalSwing01", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Vertical_Swing_01.wav"));
	_soundServer->Add("PL_VerticalSwing02", new SoundItemVOICE("Sound/VOICE/Plim/SE_PLVO_Vertical_Swing_02.wav"));
	// IB
	_soundServer->Add("IB_Excited", new SoundItemVOICE("Sound/VOICE/IB/SE_IBVO_Excited.wav"));
	_soundServer->Add("IB_Gee", new SoundItemVOICE("Sound/VOICE/IB/SE_IBVO_Gee.wav"));
	_soundServer->Add("IB_Reply", new SoundItemVOICE("Sound/VOICE/IB/SE_IBVO_Reply.wav"));
	_soundServer->Add("IB_Shocking", new SoundItemVOICE("Sound/VOICE/IB/SE_IBVO_Shocking.wav"));
	_soundServer->Add("IB_Tired", new SoundItemVOICE("Sound/VOICE/IB/SE_IBVO_Tired.wav"));
	_soundServer->Add("IB_Title", new SoundItemVOICE("Sound/VOICE/IB/SE_IBVO_Title.wav"));
	// 非同期読み込み終了
	SetUseASyncLoadFlag(FALSE);
	return true;
}