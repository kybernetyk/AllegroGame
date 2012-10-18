//
//  camera.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#include "camera.h"
#include "sys_config.h"
#include <allegro5/allegro.h>

namespace camera {
	struct camera_t {
		float x;
		float y;
	};
	
	static camera_t current_camera = {.x = 0.0,
									  .y = 0.0};
	void translate_to(float x, float y) {
		current_camera.x = x;
		current_camera.y = y;
	}
	void translate_by(float x, float y) {
		current_camera.x += x;
		current_camera.y += y;
	}

	void apply() {
		ALLEGRO_TRANSFORM t;
		al_identity_transform(&t);
	
		//al_build_transform(&t, -current_camera.x, -current_camera.y, 1.0, 1.0, 0.0);
		//al_translate_transform(&t, 0, -current_camera.y);
		al_translate_transform(&t, -current_camera.x + SCREEN_W/2, -current_camera.y+SCREEN_H/2);

		al_scale_transform(&t, 1.0, -1.0);
		al_translate_transform(&t, 0, SCREEN_H);
		al_use_transform(&t);
	}
	
	void reset() {
		current_camera.x = current_camera.y = 0.0;
	}
	
	const float x() {
		return current_camera.x;
	}
	const float y() {
		return current_camera.y;
	}


}