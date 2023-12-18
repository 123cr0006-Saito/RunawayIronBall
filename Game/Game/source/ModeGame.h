#include "appframe.h"

#include "Camera.h"
#include "Player.h"
#include "Chain.h"
#include "SlaBlock.h"

class ModeGame : public ModeBase
{
	typedef ModeBase base;

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


protected:
	Camera* _camera;
	Player* _player;
	SlaBlock* _sla[10];
	Chain* _chain;
}; 
