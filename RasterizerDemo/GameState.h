#pragma once
#include "EnumState.h"
#include "GUISprite.h"
#include "GUIText.h"
#include <string>
#include "Input.h"

class GameState
{
public:
	static EnumState activeState;
	GameState(std::string title);
	virtual ~GameState();
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void handleEvents() = 0;

	///xxx///
	virtual void Init(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height) = 0;
	virtual void SpritePass() = 0;
	/*virtual void Release() = 0;*/
};