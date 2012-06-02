//
//  camera.h
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#ifndef __MyGame__camera__
#define __MyGame__camera__
namespace camera {
	void translate_to(float x, float y);
	void translate_by(float x, float y);
	void apply();
	void reset();
	
	const float x();
	const float y();
}


#endif /* defined(__MyGame__camera__) */
