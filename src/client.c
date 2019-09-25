#include <engine/color.h>
#include <engine/list.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <engine/ui/button.h>
#include <engine/ui/switch.h>
#include <engine/ui/textbox.h>
#include <engine/settings.h>
#include <engine/camera.h>
#include <engine/graphics/shader.h>
#include <engine/tilemap.h>
#include <engine/ui/progress_bar.h>
#include <SDL.h>
#include <stdlib.h>
#include "config.h"
#include "player.h"

// TODO: animated bar, dropdown

int main(int argc, const char* argv[]) {

	if(!render_init(1024, 768, "SimpleGame")) {
		log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	util_init();

	render_clear_color(COLOR_WHITE);

	Rect screen = util_rect(0, 0, settings_get_int("window_width"),
			settings_get_int("window_height"));

	Point mouse;

	Camera *camera = camera_create();
	camera_move(camera, 4, 3, -3);
	//camera_lookat(camera, 0, 0, 0);

	Tilemap *t = tilemap_create(50, 50, 16, TILE_AIR);
	tilemap_set(t, 11, 10, TILE_WALL);
	tilemap_set_rect_wall(t, util_rect(0, 0, 50, 50), TILE_WALL);

	char aFpsBuf[64];
	double delta = util_delta_time();
	double fps = util_fps();
	double averageFps = util_fps();
	double fpsTime = 0;
	int fpsTimesAdded = 1;

	Player *p = player_create(100, 500, 100, 100);
	p->rect.x = 500;
	p->rect.y = 400;

	Point prevPos = util_point(0, 0);
	Point curPos = util_point(0, 0);

	while(1) {
		// TODO: wrap this
		util_update_keyboard();
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				goto cleanup;
			}
		}

		// Update
		util_update();
		delta = util_delta_time();
		double deltaS = delta / 1000;
		double deltaX = 1000 / delta;

		fpsTime += delta;
		if(fpsTime > 500) {
			averageFps = fps / fpsTimesAdded;
			fps = util_fps();
			fpsTimesAdded = 1;
			fpsTime = 0;
		}
		else {
			fps += util_fps();
			fpsTimesAdded++;
		}

		int directionX = 0;
		int directionY = 0;
		int directionZ = 0;
		if(util_is_keypress(SDL_SCANCODE_D))
			directionZ = 1;
		else if(util_is_keypress(SDL_SCANCODE_A))
			directionZ = -1;
		if(util_is_keypress(SDL_SCANCODE_W))
			directionX = 1;
		else if(util_is_keypress(SDL_SCANCODE_S))
			directionX = -1;
		if(util_is_keypress(SDL_SCANCODE_SPACE))
			directionY = -1;
		else if(util_is_keypress(SDL_SCANCODE_LSHIFT))
			directionY = 1;
		float offx = directionX * 20.f * deltaS;
		float offy = directionY * 20.f * deltaS;
		float offz = directionZ * 20.f * deltaS;
		log_info("delta: %f, %f, %f\n", deltaS, deltaX, delta);
		if(directionX || directionY || directionZ) {
			log_info("called\n");
			camera_move(camera, offx, offy, offz);
		}
		util_str_format(aFpsBuf, sizeof(aFpsBuf), "FPS: %.02f", averageFps);

		mouse = util_mouse_pos();
		Point coords;

		// Handle player movement.
		//player_update(p, t, NULL);


		shader_update_camera(camera);

		// Render
		render_clear();

		//render_tilemap(t);
		//render_use_camera(0);
		//render_text_color_s(COLOR_GOLD);
		//render_text(28, STYLE_REGULAR, aFpsBuf, 20, 20);
		render_voxel(0, 0, 0, 5);

		//player_render(p);

		render_present();
		//SDL_Delay(1);
	}

cleanup:
	util_quit();
	settings_save("settings.ini");
	settings_quit();
	render_quit();
	return EXIT_SUCCESS;
}
