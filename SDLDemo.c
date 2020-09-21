#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "glad.h"

#define FAILURE -1
#define POS SDL_WINDOWPOS_UNDEFINED
#define WIDTH 640
#define HEIGHT 400
#define FLAG SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL

typedef struct _System {

	SDL_Window * window;
	SDL_GLContext context;
	SDL_DisplayMode mode;
	unsigned int width;
	unsigned int height;
	unsigned int flagSizeChanged;
	unsigned int flagFullscreen;
	float xdpi;
	float ydpi;

} System;

typedef struct _GUI {
	
	System system;

} GUI;

static GUI empty_gui();
static void open_gui(GUI * pgui);
static void close_gui(GUI * pgui);
static void run_gui(GUI * pgui);
static unsigned int event_app(GUI * pgui);
static void render_app(GUI * pgui, float delta);
static unsigned int event_system(GUI * pgui, SDL_Event event);

int main () {

	GUI gui = empty_gui();
	open_gui(&gui);
	run_gui(&gui);
	close_gui(&gui);

	return 0;
}

static GUI empty_gui() {

	GUI gui;
	memset(&gui, 0, sizeof(GUI));
	return gui;
}

static void open_gui(GUI * pgui) {

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {

		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		exit(FAILURE);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	pgui->system.window = SDL_CreateWindow("SDL Demo", POS, POS, WIDTH, HEIGHT, FLAG);

	if(pgui->system.window == 0) {

		SDL_Quit();

		SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
		exit(FAILURE);
	}

	pgui->system.context = SDL_GL_CreateContext(pgui->system.window);

	if(pgui->system.context == 0) {

		SDL_DestroyWindow(pgui->system.window);
		SDL_Quit();

		SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
		exit(FAILURE);
	}

	if(SDL_GL_MakeCurrent(pgui->system.window, pgui->system.context) != 0) {

		SDL_GL_DeleteContext(pgui->system.context);
		SDL_DestroyWindow(pgui->system.window);
		SDL_Quit();

		SDL_Log("SDL_GL_MakeCurrent failed: %s", SDL_GetError());
		exit(FAILURE);
	}

	if(SDL_GL_SetSwapInterval(1) != 0) {

		SDL_GL_MakeCurrent(pgui->system.window, 0);
		SDL_GL_DeleteContext(pgui->system.context);
		SDL_DestroyWindow(pgui->system.window);
		SDL_Quit();

		SDL_Log("SDL_GL_SetSwapInterval failed: %s", SDL_GetError());
		exit(FAILURE);
	}

	if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {

		SDL_GL_MakeCurrent(pgui->system.window, 0);
		SDL_GL_DeleteContext(pgui->system.context);
		SDL_DestroyWindow(pgui->system.window);
		SDL_Quit();

		SDL_Log("gladLoadGLLoader failed: %s", SDL_GetError());
		exit(FAILURE);
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	SDL_GetDesktopDisplayMode(0, &pgui->system.mode);
	if(pgui->system.mode.refresh_rate == 0) {

		SDL_GL_MakeCurrent(pgui->system.window, 0);
		SDL_GL_DeleteContext(pgui->system.context);
		SDL_DestroyWindow(pgui->system.window);
		SDL_Quit();

		SDL_Log("SDL_GetDesktopDisplayMode unable to determine refresh_rate: %s", SDL_GetError());
		exit(FAILURE);
	}

	if( SDL_GetDisplayDPI(0, 0, &pgui->system.xdpi, &pgui->system.ydpi) != 0 ) {

		SDL_Log("SDL_GetDisplayDPI unable to determine pdi: %s", SDL_GetError());
		exit(FAILURE);
	}

	pgui->system.width = WIDTH;
	pgui->system.height = HEIGHT;
}

static void close_gui(GUI * pgui) {

	SDL_GL_MakeCurrent(pgui->system.window, 0);
	SDL_GL_DeleteContext(pgui->system.context);
	SDL_DestroyWindow(pgui->system.window);
	SDL_Quit();
}

static void run_gui(GUI * pgui) {

	float delta = 1.0f / pgui->system.mode.refresh_rate;

	while(event_app(pgui)) {

		render_app(pgui, delta);
		SDL_GL_SwapWindow(pgui->system.window);
	}
}

static unsigned int event_app(GUI * pgui) {

	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));

	while(SDL_PollEvent(&event)){

		if(!event_system(pgui, event)) return 0;
	}

	return 1;
}

static unsigned int event_system(GUI * pgui, SDL_Event event) {

	switch(event.type) {

		case SDL_QUIT: {

			return 0;
		} break;

		case SDL_KEYDOWN: {

			if(event.key.keysym.sym == SDLK_F11) {

				pgui->system.flagFullscreen = !pgui->system.flagFullscreen;

				if(pgui->system.flagFullscreen) {

					SDL_SetWindowFullscreen(pgui->system.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					SDL_GetWindowSize(pgui->system.window, &pgui->system.width, &pgui->system.height);
					glViewport(0, 0, pgui->system.width, pgui->system.height);
					pgui->system.flagSizeChanged = 1;
				} else {

					SDL_SetWindowFullscreen(pgui->system.window, 0);
					SDL_GetWindowSize(pgui->system.window, &pgui->system.width, &pgui->system.height);
					glViewport(0, 0, pgui->system.width, pgui->system.height);
					pgui->system.flagSizeChanged = 1;
				}
			}

			else if(event.key.keysym.sym == SDLK_ESCAPE) {

				SDL_SetWindowFullscreen(pgui->system.window, 0);
			}
		} break;

		case SDL_WINDOWEVENT: {

			switch(event.window.event) {

				case SDL_WINDOWEVENT_RESIZED: {

			        const unsigned int width = event.window.data1;
			        const unsigned int height = event.window.data2;
			        pgui->system.width = width;
			        pgui->system.height = height;
			        glViewport(0, 0, width, height);
			        pgui->system.flagSizeChanged = 1;
			    } break;
			}
		} break;
	}

	return 1;
}

static void render_app(GUI * pgui, float delta) {

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

