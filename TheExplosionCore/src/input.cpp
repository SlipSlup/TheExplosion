
#include "input.hpp"

namespace TheExplosion {

	bool Input::m_pressed_keys[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};

	bool Input::isKeyPressed(const KeyCode key_code) { return m_pressed_keys[static_cast<size_t>(key_code)]; }
	
	void Input::PressKey(const KeyCode key_code) { m_pressed_keys[static_cast<size_t>(key_code)] = true; }

	void Input::ReleaseKey(const KeyCode key_code) { m_pressed_keys[static_cast<size_t>(key_code)] = false; }

}