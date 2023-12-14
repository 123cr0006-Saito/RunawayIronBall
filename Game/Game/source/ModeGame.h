#include "appframe.h"

#include "Camera.h"
#include "Player.h"
#include "Chain.h"

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

	Chain* _chain;
}; 
