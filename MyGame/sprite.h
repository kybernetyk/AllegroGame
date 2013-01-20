//
//  sprite_manager.h
//  MyGame
//
//  Created by Leon Szpilewski on 02/06/2012.
//
//

#ifndef __MyGame__sprite_manager__
#define __MyGame__sprite_manager__
#include <allegro5/allegro.h>
#include <string>
namespace cm {
	struct sprite_t {
		std::shared_ptr<ALLEGRO_BITMAP> bitmap;
	};

	namespace sprite {
		sprite_t create_sprite(std::string filename);
		void destroy_sprite(sprite_t &sprite);
		void draw_sprite(sprite_t &sprite, float x, float y, float rotation);
		void draw_sprite(sprite_t &sprite, float x, float y);
	}
}
#endif /* defined(__MyGame__sprite_manager__) */
