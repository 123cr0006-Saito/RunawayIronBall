#pragma once
#include "PlaneEffectBase.h"
#include <vector>
namespace PlaneEffect {
	class PlaneEffectManeger
	{
	public:
		PlaneEffectManeger();
		~PlaneEffectManeger();

		void LoadVertical(PlaneEffectBase* effect);
		virtual bool Update();
		virtual bool Render();
		static PlaneEffectManeger* _instance;
		static PlaneEffectManeger* GetInstance() { return _instance; };
	protected:
		std::vector<PlaneEffectBase*> _vertical;
	};
};
