#pragma once
#include "Refactor.h"
namespace game_framework
{
	#pragma region - CLayer -
	class CLayer
	{
	public:
		CLayer();
		~CLayer();
		void SetLayer(int);
		int GetLayer();
	private:
		int layer;
	};
	#pragma endregion

}