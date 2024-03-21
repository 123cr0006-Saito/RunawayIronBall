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
	// ”ñ“¯Šú“Ç‚Ýž‚ÝÝ’è
	SetUseASyncLoadFlag(TRUE);
	//BGM
	_soundServer->Add("Title", new SoundItemBGM("Sound/BGM/BGM_Title.wav"));
	_soundServer->Add("Stage01", new SoundItemBGM("Sound/BGM/BGM_Stage_01.wav"));
	_soundServer->Add("Stage02", new SoundItemBGM("Sound/BGM/BGM_Stage_02.wav"));
	_soundServer->Add("Stage03", new SoundItemBGM("Sound/BGM/BGM_Stage_03.wav"));
	_soundServer->Add("B_Scenario", new SoundItemBGM("Sound/BGM/BGM_Scenario_01.wav"));
	_soundServer->Add("Result", new SoundItemBGM("Sound/BGM/BGM_Result.wav"));
	//SE
	//Object
	_soundServer->Add("House_Glass_Break", new SoundItemSE("Sound/SE/Object/SE_HO_Glass_Break.wav"));
	_soundServer->Add("House_Glass_Hit", new SoundItemSE("Sound/SE/Object/SE_HO_Glass_Hit.wav"));
	_soundServer->Add("House_Iron_Break", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Break.wav"));
	_soundServer->Add("House_Iron_Hit", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Hit.wav"));
	_soundServer->Add("House_Rock_Break", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Break.wav"));
	_soundServer->Add("House_Rock_Hit", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Hit.wav"));
	_soundServer->Add("CG_OBJ_Pole_Break", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Break.wav"));
	_soundServer->Add("CG_OBJ_Pole_Hit", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Hit.wav"));
	_soundServer->Add("CG_OBJ_Traffic_Light_Break", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Break.wav"));
	_soundServer->Add("CG_OBJ_Traffic_Light_Hit", new SoundItemSE("Sound/SE/Object/SE_HO_Iron_Hit.wav"));
	_soundServer->Add("CG_OBJ_Long_House_Break", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Break.wav"));
	_soundServer->Add("CG_OBJ_Long_House_Hit", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Hit.wav"));
	_soundServer->Add("CG_OBJ_Lantern_Break", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Break.wav"));
	_soundServer->Add("CG_OBJ_Lantern_Hit", new SoundItemSE("Sound/SE/Object/SE_HO_Rock_Hit.wav"));
	//Hit
	_soundServer->Add("SE_Hit01", new SoundItemSE("Sound/SE/Player/SE_Hit_Combo_01.wav"));
	_soundServer->Add("SE_Hit02", new SoundItemSE("Sound/SE/Player/SE_Hit_Combo_01.wav"));
	_soundServer->Add("SE_Hit03", new SoundItemSE("Sound/SE/Player/SE_Hit_Combo_01.wav"));
	_soundServer->Add("SE_Hit_Tower", new SoundItemSE("Sound/SE/Player/SE_Hit_Combo_01.wav"));
	_soundServer->Add("SE_Hit_Rotate", new SoundItemSE("Sound/SE/Player/SE_Hit_Rotate.wav"));
	_soundServer->Add("SE_Avoidance", new SoundItemSE("Sound/SE/Player/SE_PL_Avoidance.wav"));
	_soundServer->Add("SE_Damage", new SoundItemSE("Sound/SE/Player/SE_PL_Damage.wav"));
	_soundServer->Add("SE_Heal", new SoundItemSE("Sound/SE/Player/SE_PL_Heal.wav"));
	_soundServer->Add("SE_LevelUP", new SoundItemSE("Sound/SE/Player/SE_PL_Level_Up.wav"));
	_soundServer->Add("SE_Rotation", new SoundItemSE("Sound/SE/Player/SE_PL_Rotation.wav"));
	_soundServer->Add("SE_Swing", new SoundItemSE("Sound/SE/Player/SE_PL_Side_Swing.wav"));
	//Select
	_soundServer->Add("SE_Open", new SoundItemSE("Sound/SE/Select/SE_Menu_Open.wav"));
	_soundServer->Add("SE_Press", new SoundItemSE("Sound/SE/Select/SE_Menu_Press.wav"));
	_soundServer->Add("SE_Select", new SoundItemSE("Sound/SE/Select/SE_Menu_Select.wav"));
	_soundServer->Add("SE_Break", new SoundItemSE("Sound/SE/Select/SE_Menu_Break.wav"));
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
	//Enemy
	_soundServer->Add("SE_SLV_Falling", new SoundItemSE("Sound/SE/Enemy/SE_SLB_Falling.wav"));
	_soundServer->Add("SE_SLV_Jump", new SoundItemSE("Sound/SE/Enemy/SE_SLB_Jump.wav"));
	SetUseASyncLoadFlag(FALSE);
	return true;
}