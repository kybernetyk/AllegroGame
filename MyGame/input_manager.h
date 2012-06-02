//
//  input_manager.h
//  MyGame
//
//  Created by Leon Szpilewski on 02/06/2012.
//
//

#ifndef __MyGame__input_manager__
#define __MyGame__input_manager__

namespace input_manager {
	void init();
	
	float mouse_x();
	float mouse_y();
	bool left_button();
	bool right_button();
	
	void set_mouse(float x, float y);
	void set_left_button(bool b);
	void set_right_button(bool b);
	
	bool key_pressed(int keycode);
	void set_key_pressed(int keycode, bool state);
}

#endif /* defined(__MyGame__input_manager__) */
