#pragma once
#include "BoardPolygon.h"
#include <vector>
namespace PlaneEffect {
	class PlaneEffectManeger
	{
	public:
		PlaneEffectManeger();
		~PlaneEffectManeger();

		void LoadVertical(BoardPolygon* effect);
		virtual bool Update();
		virtual bool Render();
		static PlaneEffectManeger* _instance;
		static PlaneEffectManeger* GetInstance() { return _instance; };
	protected:
		std::vector<BoardPolygon*> _vertical;
	};
};
