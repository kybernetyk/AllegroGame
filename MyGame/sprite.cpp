//
//  sprite_manager.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 02/06/2012.
//
//

#include "sprite.h"

namespace sprite {
	sprite_t create_sprite(std::string filename) {
		sprite_t ret;
		ret.bitmap = std::shared_ptr<ALLEGRO_BITMAP>(al_load_bitmap(filename.c_str()), al_destroy_bitmap);
		if (!ret.bitmap.get()) {
			throw std::runtime_error("couldn't load sprite file: " + filename);
		}
		return ret;
	}
	void draw_sprite(sprite_t &sprite, float x, float y, float rotation) {
		if (rotation == 0.0) {
			al_draw_bitmap(sprite.bitmap.get(), x, y, 0);
			return;
		}
		float cx = x + al_get_bitmap_width(sprite.bitmap.get())/2.0;
		float cy = y + al_get_bitmap_height(sprite.bitmap.get())/2.0;
		al_draw_rotated_bitmap(sprite.bitmap.get(),
							   cx, cy,
							   x, y,
							   rotation,
							   0);
	}
}