#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <functional>
#include "scene_manager.h"
#include "loading_scene.h"
#include "input_manager.h"

const float FPS = 60.0f;


int main(int argc, char **argv) {
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *buffer = NULL;
	bool redraw = true;
	
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "can't init primitives addons\n");
		return -1;
	}
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();
	
	timer = al_create_timer(1.0 / FPS);
	if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
	
	//size of our window
	float windowWidth = 800;
	float windowHeight = 600;

	//size of our screen. will be scaled to match window size
	float screenWidth = 800;
	float screenHeight = 600;
	
	// initialization
//	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(windowWidth, windowHeight);
	buffer = al_create_bitmap(screenWidth, screenHeight);
	
	// calculate scaling factor
	float sx = windowWidth / screenWidth;
	float sy = windowHeight / screenHeight;
	float scale = std::min(sx, sy);
	
	// calculate how much the buffer should be scaled
	float scaleW = screenWidth * scale;
	float scaleH = screenHeight * scale;
	float scaleX = (windowWidth - scaleW) / 2;
	float scaleY = (windowHeight - scaleH) / 2;

	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}
	
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
	input_manager::init();
	
	scene_manager::set_scene(loading_scene::create());
	
	al_start_timer(timer);

	
	for (;;) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			scene_manager::tick(1.0/FPS);
		} else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
				  ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
			input_manager::set_mouse(ev.mouse.x, ev.mouse.y);
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (ev.mouse.button == 1)
				input_manager::set_left_button(true);
			if (ev.mouse.button == 2)
				input_manager::set_right_button(true);
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (ev.mouse.button == 1)
				input_manager::set_left_button(false);
			if (ev.mouse.button == 2)
				input_manager::set_right_button(false);
		} else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			input_manager::set_key_pressed(ev.keyboard.keycode, false);
		} else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			input_manager::set_key_pressed(ev.keyboard.keycode, true);
		}
		
		if(redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			
			// render a frame
			al_set_target_bitmap(buffer);
			scene_manager::draw(1.0/FPS);

			al_set_target_backbuffer(display);
			al_clear_to_color(al_map_rgb(0, 128, 0));
			al_draw_scaled_bitmap(buffer, 0, 0, screenWidth, screenHeight, scaleX, scaleY, scaleW, scaleH, 0);

			al_flip_display();
		}
	}
	scene_manager::destroy();

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	
	return 0;
}

