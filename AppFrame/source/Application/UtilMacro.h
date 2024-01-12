#include "dxlib.h"

#define	SAFE_DELETE(x)	if(x){delete x;x=NULL;}
#define S_Draw(y,value) DrawFormatString(0,y*15,GetColor(255,255,255),"%u",value);
#define D_Draw(y,value) DrawFormatString(0,y*15,GetColor(255,255,255),"%d",value);
#define F_Draw(y,value) DrawFormatString(0,y*15,GetColor(255,255,255),"%f",value);
#define V_Draw(y,value) DrawFormatString(0,y*15,GetColor(255,255,255),"%f;%f;%f",value.x,value.y,value.z)
