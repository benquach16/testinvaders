#include "Render.h"

const char text1[16] = "Current Score:";
const char text2[16] = "Current Lives:";
const char text3[32] = "You lost! Final score: ";
char deathMsg[128];
char scoreMsg[128];
char livesMsg[128];

namespace RenderQueue
{
	Array<vec2> positions[static_cast<int>(Engine::Sprite::Count)];
	Array<vec2>& get(Engine::Sprite idx) {
		return positions[static_cast<int>(idx)];
	}	
	void Add(Engine::Sprite sprite, vec2 position) {
		positions[static_cast<int>(sprite)].push(position);
	}
	void RenderUI(Engine& engine, int score, int health)
	{
		constexpr uint16_t text2len = 18;
		sprintf_s(scoreMsg, 128, "%s%d", text1,score);
		engine.drawText(
			scoreMsg, 
			0, 
			0);
		sprintf_s(livesMsg, 128, "%s%d", text2,health);
		engine.drawText(
			livesMsg, 
			(Engine::CanvasWidth - (text2len) * Engine::FontWidth), 
			0);
	}

	void RenderDeath(Engine& engine, int score)
	{
		constexpr uint16_t text3len = 24;
		sprintf_s(deathMsg, 128, "%s%d", text3,score);
		engine.drawText(
			deathMsg, 
			(Engine::CanvasWidth - (text3len) * Engine::FontWidth)/2, 
			Engine::CanvasHeight/2);	
	}

	void Render(Engine& engine)
	{
		//static_assert(static_cast<int>(Engine::Sprite::Player) == 0);
		for(int i = static_cast<int>(Engine::Sprite::Player); i < static_cast<int>(Engine::Sprite::Count); ++i) {
			for(uint32_t j = 0; j < RenderQueue::positions[i].getMaxIdx(); ++j) {
				vec2 data = RenderQueue::positions[i][j];
				engine.drawSprite(static_cast<Engine::Sprite>(i), data.x, data.y);
			}
			RenderQueue::positions[i].clear();
		}
	}
}; //RenderQueue
