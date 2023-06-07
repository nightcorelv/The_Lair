#include "Input.h"


std::unordered_map<Input::Key, Input::State> Input::keyState;

bool Input::GetKeyDown(const Key key)
{
	return keyState[key].isDown;
}

bool Input::GetKeyPressed(const Key key)
{
	if (keyState[key].pressedTrigged && !keyState[key].pressed)
	{
		keyState[key].pressed = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::GetKeyUp(const Key key)
{
	return !keyState[key].isDown;
}

void Input::SetKeyDown(const Key key)
{
	if (!keyState[key].pressedTrigged)
	{
		keyState[key].pressedTrigged = true;
	}
	keyState[key].isDown = true;
}

void Input::SetKeyDown(const unsigned long long key)
{
	SetKeyDown((Input::Key)key);
}

void Input::SetKeyUp(const Key key)
{
	if (keyState[key].pressedTrigged)
	{
		keyState[key].pressed = false;
		keyState[key].pressedTrigged = false;
	}
	keyState[key].isDown = false;
}

void Input::SetKeyUp(const unsigned long long key)
{
	SetKeyUp((Input::Key)key);
}

void Input::Reset()
{

}

