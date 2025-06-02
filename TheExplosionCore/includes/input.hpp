
#pragma once

#include "keys.hpp"

namespace TheExplosion {

	class Input {

	public:

		static bool isKeyPressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);

	private:

		static bool m_pressed_keys[];

	};

}