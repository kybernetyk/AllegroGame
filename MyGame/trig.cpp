//
//  types.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 20/01/2013.
//
//

#include "trig.h"

namespace cm {
	namespace trig {
		bool is_point_inside_rect(point_t p, rect_t &r) {
			return (p.x >= r.x &&
					p.x <= r.x + r.w &&
					p.y >= r.y &&
					p.y <= r.y + r.h);
		}
	}
}