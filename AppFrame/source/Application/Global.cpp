#include "Global.h"

Global global;

Global::Global() {
	_timer = new Timer();
	_soundServer = new SoundServer();
	_timer->Start();
	exit_count = false;
	_stageNum = 1;
	_allExp = 0;
	_oldExp = 0;
	IsVibration = 1; 
};

Global::~Global() {
	delete _timer;
	delete _soundServer;
};

bool Global::Init() {
	_stageNum = 1;
	_allExp = 0;
	_oldExp = 0;
	return true;
};

bool Global::ResourceLoad() {
	// Effekseer
	ResourceServer::Load("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
	ResourceServer::Load("Rotation", "res/Effekseer/FX_3D_Rotate_2/FX_3D_Rotate.efkefc");
	ResourceServer::Load("SlashR", "res/Effekseer/Slash/SlashRight.efkefc");
	ResourceServer::Load("SlashL", "res/Effekseer/Slash/SlashLeft.efkefc");
	ResourceServer::Load("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
	ResourceServer::Load("FX_3D_Level_Up", "res/Effekseer/FX_3D_Level_Up/FX_3D_Level_Up.efkefc");
	SetUseASyncLoadFlag(true);
	// Model
	// StageModel
	ResourceServer::Load("SkySpehe", "res/SkySphere/Skyspehre.mv1");
	ResourceServer::Load("Tile", "res/Tile/Stage_Base_01.mv1");
	ResourceServer::Load("Mountain", "res/Mountain/Stage_mountain_02.mv1");
	// CharacterModel
	ResourceServer::Load("Player", "res/Character/cg_player_girl/Cg_Player_Girl.mv1");
	ResourceServer::Load("IronBall", "res/Character/Cg_Iron_Ball/Cg_Iron_Ball.mv1");
	ResourceServer::Load("Chain", "res/Chain/Cg_Chain.mv1");
	ResourceServer::Load("LoadingPlayer", "res/Character/Loading/Cg_Player_Girl.mv1");
	ResourceServer::Load("LoadingIronBall", "res/Character/Loading/Cg_Iron_Ball.mv1");
	ResourceServer::Load("LoadingChain", "res/Character/Loading/Cg_Chain.mv1");
	ResourceServer::Load("CryStar_Glass", "res/Enemy/Cg_Enemy_Crystar_Glass/Cg_Crystar_Glass.mv1");
	ResourceServer::Load("CryStar_Rock", "res/Enemy/Cg_Enemy_Crystar_Rock/Cg_Crystar_Rock.mv1");
	ResourceServer::Load("CryStar_Iron", "res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Crystar_Iron.mv1");
	ResourceServer::Load("Slablock_Glass", "res/Enemy/Cg_Enemy_Slablock_Glass/Cg_Slablock_Glass.mv1");
	ResourceServer::Load("Slablock_Rock", "res/Enemy/Cg_Enemy_Slablock_Rock/Cg_Slablock_Rock.mv1");
	ResourceServer::Load("Slablock_Iron", "res/Enemy/Cg_Enemy_Slablock_Iron/Cg_Slablock_Iron.mv1");
	ResourceServer::Load("Chainguard", "res/Enemy/Cg_Enemy_ChainGuard/Cg_Enemy_ChainGuard.mv1");
	ResourceServer::Load("CrystarRoof_Glass", "res/Enemy/Cg_Enemy_Crystar_Glass/Cg_Crystar_Roof_Glass.mv1");
	ResourceServer::Load("CrystarRoof_Rock", "res/Enemy/Cg_Enemy_Crystar_Rock/Cg_Enemy_Roof_Crystar_Rock.mv1");
	ResourceServer::Load("CrystarRoof_Glass", "res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Crystar_Roof_Iron.mv1");
	ResourceServer::Load("CrystarRoof_Iron", "res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Helmet_Enemy_Crystar_Iron.mv1");
	// Object
	ResourceServer::Load("Tower_Under", "res/Building/CG_OBJ_Tower/Tower_Under.mv1");
	ResourceServer::Load("Tower_Top", "res/Building/CG_OBJ_Tower/Tower_Top.mv1");
	ResourceServer::Load("CG_OBJ_Asphalt", "res/Building/CG_OBJ_Asphalt/CG_OBJ_Asphalt.mv1");
	ResourceServer::Load("CG_OBJ_Asphalt_Cross", "res/Building/CG_OBJ_Asphalt_Cross/CG_OBJ_Asphalt_Cross.mv1");
	ResourceServer::Load("CG_OBJ_Dirt", "res/Building/CG_OBJ_Dirt/CG_OBJ_Dirt.mv1");
	ResourceServer::Load("CG_OBJ_Keep_Out", "res/Building/CG_OBJ_Keep_Out/CG_OBJ_Keep_Out.mv1");
	ResourceServer::Load("CG_OBJ_Lantern", "res/Building/CG_OBJ_Lantern/CG_OBJ_Lantern.mv1");
	ResourceServer::Load("CG_OBJ_Long_House", "res/Building/CG_OBJ_Long_House/CG_OBJ_Long_House.mv1");
	ResourceServer::Load("CG_OBJ_Moss", "res/Building/CG_OBJ_Moss/CG_OBJ_Moss.mv1");
	ResourceServer::Load("CG_OBJ_Moss_01", "res/Building/CG_OBJ_Moss_01/CG_OBJ_Moss_01.mv1");
	ResourceServer::Load("CG_OBJ_Moss_02", "res/Building/CG_OBJ_Moss_02/CG_OBJ_Moss_02.mv1");
	ResourceServer::Load("CG_OBJ_Moss_03", "res/Building/CG_OBJ_Moss_03/CG_OBJ_Moss_03.mv1");
	ResourceServer::Load("CG_OBJ_Moss_04", "res/Building/CG_OBJ_Moss_04/CG_OBJ_Moss_04.mv1");
	ResourceServer::Load("CG_OBJ_Moss_05", "res/Building/CG_OBJ_Moss_05/CG_OBJ_Moss_05.mv1");
	ResourceServer::Load("CG_OBJ_Moss_06", "res/Building/CG_OBJ_Moss_06/CG_OBJ_Moss_06.mv1");
	ResourceServer::Load("CG_OBJ_Moss_07", "res/Building/CG_OBJ_Moss_07/CG_OBJ_Moss_07.mv1");
	ResourceServer::Load("CG_OBJ_Pole", "res/Building/CG_OBJ_Pole/CG_OBJ_Pole.mv1");
	ResourceServer::Load("CG_OBJ_Traffic_Light", "res/Building/CG_OBJ_Traffic_Light/CG_OBJ_Traffic_Light.mv1");
	ResourceServer::Load("CG_OBJ_Wood", "res/Building/CG_OBJ_Wood/CG_OBJ_Wood.mv1");
	ResourceServer::Load("House_Glass", "res/Building/House_Glass/House_Glass.mv1");
	ResourceServer::Load("House_Rock", "res/Building/House_Rock/House_Rock.mv1");
	ResourceServer::Load("House_Iron", "res/Building/House_Iron/House_Iron.mv1");
	ResourceServer::Load("Board", "res/ModeTitle/board.mv1");
	// Item
	ResourceServer::Load("Heart", "res/Item/Cg_Object_Heart/Cg_Object_Heart.mv1");
	// UI
	ResourceServer::Load("Stamina01", ("res/UI/Stamina/UI_Stamina_01.png"));
	ResourceServer::Load("Stamina02", ("res/UI/Stamina/UI_Stamina_02.png"));
	ResourceServer::Load("Stamina03", ("res/UI/Stamina/UI_Stamina_03.png"));
	ResourceServer::Load("Stamina04", ("res/UI/Stamina/UI_Stamina_04.png"));
	ResourceServer::Load("Stamina04", ("res/UI/Gauge/UI_EXP_Gauge_Black.png"));
	ResourceServer::Load("Stamina04", ("res/UI/Gauge/UI_EXP_Gauge_Red.png"));
	ResourceServer::Load("StageHandle1", "res/StageName/Stage1.png");
	ResourceServer::Load("StageHandle2", "res/StageName/Stage2.png");
	ResourceServer::Load("StageHandle3", "res/StageName/Stage3.png");
	ResourceServer::LoadMultGraph("Heart", "res/UI/Heart/UI_Heart", ".png", 3);
	ResourceServer::LoadMultGraph("UILevel", "res/UI/Level/UI_Level", ".png", 10);
	ResourceServer::LoadMultGraph("SuppressionGauge", "res/UI/SuppressionGauge/SuppressionGauge", ".png", 3);
	ResourceServer::LoadMultGraph("TimeNum", "res/UI/Time/Ui_Time", ".png", 10);
	ResourceServer::Load("TimeColon", "res/UI/Time/UI_Time_Colon.png");
	ResourceServer::Load("TimeFrame", "res/UI/Time/UI_TIME_Gauge.png");
	ResourceServer::Load("UIChain", "res/UI/Chain/Kusari_Straight.png");
	// Title
	ResourceServer::Load("T_BackGround", "res/ModeTitle/title_base.png");
	ResourceServer::Load("T_Title","res/ModeTitle/UI_Title.png");
	ResourceServer::Load("T_Start", "res/ModeTitle/UI_Start.png");
	ResourceServer::Load("T_Option", "res/ModeTitle/UI_Option.png");
	ResourceServer::Load("T_Quit", "res/ModeTitle/UI_Quit.png");
	ResourceServer::Load("T_Logo", "res/ModeTitle/UI_Logo.png");
	// Pause
	ResourceServer::Load("back", "res/ModePause/UI_Menu.png");
	ResourceServer::Load("check", "res/ModePause/UI_Menu_Check.png");
	ResourceServer::Load("checkBox", "res/ModePause/UI_Menu_Check_Box.png");
	ResourceServer::Load("volumBar", "res/ModePause/UI_Menu_Pink_Gauge.png");
	ResourceServer::Load("volumBarBack", "res/ModePause/UI_Menu_Black_Gauge.png");
	ResourceServer::Load("se", "res/ModePause/UI_Menu_SE.png");
	ResourceServer::Load("bgm", "res/ModePause/UI_Menu_BGM.png");
	ResourceServer::Load("vibration", "res/ModePause/UI_Menu_Controll.png");
	ResourceServer::Load("gide", "res/ModePause/UI_Menu_How_To.png");
	ResourceServer::Load("return", "res/ModePause/UI_Menu_Back.png");
	ResourceServer::Load("InstructionFrame", "res/ModePause/Operation/Frame.png");
	ResourceServer::Load("Operation", "res/ModePause/Operation/UI_Menu_Operation.png");
	ResourceServer::Load("Skip", "res/ModeScenario/Skip/UI_Skip.png");
	ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5);
	ResourceServer::LoadMultGraph("OperateItem", "res/ModePause/Operation/UI_Operation", ".png", 5);
	// GameOver
	ResourceServer::Load("Logo", "res/ModeGameOver/UI_Game_Over.png");
	ResourceServer::Load("Retry", "res/ModeGameOver/UI_Game_Over_Retry.png");
	ResourceServer::Load("Give", "res/ModeGameOver/UI_Game_Over_Title_Back.png");
	// GameClear
	ResourceServer::Load("skysphere", "res/ModeScenario/BackGround/skysphere.png");
	ResourceServer::Load("Time", "res/ModeResult/Time/UI_Valuation_Time_Logo.png");
	ResourceServer::Load("Colon", "res/ModeResult/Time/UI_Valuation_Time_Colon.png");
	ResourceServer::LoadMultGraph("Valuation", "res/ModeResult/Rank/UI_Valuation", ".png",4);
	ResourceServer::LoadMultGraph("C_Time", "res/ModeResult/Time/UI_Valuation_Time", ".png", 10);
	// Effect
	ResourceServer::LoadDivGraph("Dust", "res/HitEffect_Blue//FX_Dust_2D.png", 44, 20, 3, 1000, 1000);
	ResourceServer::LoadMultGraph("HitEffect_Blue", "res/Effect/HitEffect_Blue/HitEffect_Blue", ".png", 30);
	ResourceServer::LoadMultGraph("HitEffect_Red", "res/Effect/HitEffect_Red/HitEffect_Red", ".png", 30);
	// Gate
	ResourceServer::LoadDivGraph("Gate", "res/Gate/FX_Hole_2D00_sheet.png", 43, 16, 3, 1200, 1200);
	// Tutorial
	ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5);
	// Texture
	ResourceServer::Load("GirlTexWhite", "res/Character/cg_player_girl/FlickerTexture.png");
	// Ending
	ResourceServer::Load("Ending","res/Movie/Ending.mp4");

	SetUseASyncLoadFlag(false);
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
	_soundServer->Add("Stage04", new SoundItemBGM("Sound/BGM/BGM_Stage_Boss.wav"));
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