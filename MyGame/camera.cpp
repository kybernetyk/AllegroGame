//
//  camera.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#include "camera.h"
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
		al_build_transform(&t, -current_camera.x, -current_camera.y, 1.0, 1.0, 0.0);
		al_use_transform(&t);
	}
	
	const float x() {
		return current_camera.x;
	}
	const float y() {
		return current_camera.y;
	}


}