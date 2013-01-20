//
//  input_manager.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 02/06/2012.
//
//

#include "input_manager.h"
#include <string.h>

namespace cm {
	namespace input_manager {
		static float _mouse_x = 0.0;
		static float _mouse_y = 0.0;
		static bool _left_button = false;
		static bool _right_button = false;
		static bool _pressed_keys[255];

		void init() {
			memset(_pressed_keys, false, sizeof(_pressed_keys)*sizeof(false));
		}
		void set_mouse(float x, float y) {
			_mouse_x = x;
			_mouse_y = y;
		}
		
		float mouse_x() {
			return _mouse_x;
		}

		float mouse_y() {
			return _mouse_y;
		}
		
		void set_left_button(bool b) {
			_left_button = b;
		}
		
		void set_right_button(bool b) {
			_right_button = b;
		}

		bool left_button() {
			return _left_button;
		}

		bool right_button() {
			return _right_button;
		}
		
		bool key_pressed(int keycode) {
			return _pressed_keys[keycode];
		}
		
		void set_key_pressed(int keycode, bool state) {
			_pressed_keys[keycode] = state;
		}


	}
}