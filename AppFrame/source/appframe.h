#pragma once

#pragma comment(lib,"AppFrame.lib")

#include "DxLib.h"

// メモリリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define NEW new
#endif

//Application
#include "Application/ApplicationBase.h"
#include "Application/UtilMacro.h"

//CFile
#include "CFile/CFile.h"

//System
// Input
#include "System/Header/Input/XInput.h"
//Sound
#include "System/Header/Sound/SoundItem.h"
#include "System/Header/Sound/SoundServer.h"
//Function
#include "System/Header/Function/Easing.h"
#include "System/Header/Function/mydraw.h"
#include "System/Header/Function/mymath.h"
#include "System/Header/Function/3DDraw.h"
#include "System/Header/Function/Vector3D.h"
#include "System/Header/Function/DrawGauge.h"
#include "System/Header/Function/Vibration.h"
//Collision
#include "System/Header/Collision/2DCollision.h"
#include "System/Header/Collision/3DCollision.h"
#include "System/Header/Collision/MyStructure.h"

//Global
#include "Application/Global.h"
#include "System/Header/Resource/ResourceServer.h"