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

namespace cm {
	namespace camera {
		struct camera_t {
			float x;
			float y;
		};
		
		static camera_t current_camera = {
			.x = 0.0,
			.y = 0.0
		};

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
			al_translate_transform(&t, -current_camera.x + cfg::screen_width/2, -current_camera.y+cfg::screen_height/2);
			al_scale_transform(&t, 1.0, -1.0);
			al_translate_transform(&t, 0, cfg::screen_height);
			al_use_transform(&t);
		}
		
		void apply_identity() {
			ALLEGRO_TRANSFORM t;
			al_identity_transform(&t);
			al_scale_transform(&t, 1.0, -1.0);
			al_translate_transform(&t, 0, cfg::screen_height);
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
		
		const rect_t visible_area() {
			return rect_t{
				.x = current_camera.x - cfg::screen_width/2,
				.y = current_camera.y - cfg::screen_height/2,
				.w = (float)cfg::screen_width,
				.h = (float)cfg::screen_height
			};
		}
	}
}