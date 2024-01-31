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
	protected:
		std::vector<BoardPolygon*> _vertical;
	};
};
