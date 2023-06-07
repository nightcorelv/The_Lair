#pragma once

#include <unordered_map>

struct Input final
{
	enum class Key
	{
		Enter = 0x0D,
		Esc = 0x1B,
		Space = 0x20,

		Up_Arrow = 0x26,
		Down_Arrow = 0x28,
		Right_Arrow = 0x27,
		Left_Arrow = 0x25,

		L_Mouse = 0x01,
		R_Mouse = 0x02,

		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

	};

	static bool GetKeyDown(const Key key);
	static bool GetKeyPressed(const Key key);
	static bool GetKeyUp(const Key key);

	static void SetKeyDown(const Key key);
	static void SetKeyDown(const unsigned long long key);

	static void SetKeyUp(const Key key);
	static void SetKeyUp(const unsigned long long key);

	static void Reset();

private:

	struct State final
	{
		bool pressedTrigged = false;
		bool pressed = false;
		bool isDown = false;
	};
	static std::unordered_map<Key, State> keyState;

};