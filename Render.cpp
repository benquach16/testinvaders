#include "Render.h"

constexpr uint16_t bufSize = 128;
const char text1[16] = "Current Score:";
const char text2[16] = "Current Lives:";
const char text3[32] = "You lost! Final score: ";
char deathMsg[bufSize];
char scoreMsg[bufSize];
char livesMsg[bufSize];

namespace RenderQueue
{
	Array<vec2> positions[static_cast<int>(Engine::Sprite::Count)];
	Array<vec2>& get(Engine::Sprite idx) {
		return positions[static_cast<int>(idx)];
	}
	
	void Add(Renderable data) {
		positions[static_cast<int>(data.sprite)].push(data.position);
	}
	
	void RenderUI(Engine& engine, int score, int health)
	{
		constexpr uint16_t text2len = 16 + 2; // 2 digits for extra lives
		sprintf_s(scoreMsg, bufSize, "%s%d", text1,score);
		engine.drawText(
			scoreMsg, 
			0, 
			0);
		sprintf_s(livesMsg, bufSize, "%s%d", text2,health);
		engine.drawText(
			livesMsg, 
			(Engine::CanvasWidth - (text2len) * Engine::FontWidth), 
			0);
	}

	void RenderDeath(Engine& engine, int score)
	{
		constexpr uint16_t text3len = 22 + 4; // 4 digits for score
		sprintf_s(deathMsg, bufSize, "%s%d", text3,score);
		engine.drawText(
			deathMsg, 
			(Engine::CanvasWidth - (text3len) * Engine::FontWidth)/2, 
			Engine::CanvasHeight/2);	
	}

	void Render(Engine& engine)
	{
		//static_assert(static_cast<int>(Engine::Sprite::Player) == 0);
		//#pragma unroll
		for(int i = static_cast<int>(Engine::Sprite::Player); i < static_cast<int>(Engine::Sprite::Count); ++i) {
			for(uint32_t j = 0; j < RenderQueue::positions[i].getMaxIdx(); ++j) {
				vec2 data = RenderQueue::positions[i][j];
				engine.drawSprite(static_cast<Engine::Sprite>(i), data.x, data.y);
			}
			RenderQueue::positions[i].clear();
		}
	}
}; //RenderQueue
