#pragma once

#include "Engine.h"
#include "vector.h"
#include "Array.h"

struct Renderable
{
	vec2 position;
	Engine::Sprite sprite;
	Renderable(vec2 _position = vec2(), Engine::Sprite _sprite = Engine::Sprite::Player) : position(_position), sprite(_sprite) {}
};

namespace RenderQueue
{
	// queue organized by type of sprite
	extern Array<vec2> positions[static_cast<int>(Engine::Sprite::Count)];
	
	Array<vec2>& get(Engine::Sprite idx);
	
	void Add(Renderable data);

	void RenderUI(Engine& engine, int score, int health);

	void RenderDeath(Engine& engine, int score);

	void Render(Engine& engine);
}; // RenderQueue