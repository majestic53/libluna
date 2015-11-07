/**
 * libluna
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * libluna is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libluna is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../lib/include/luna.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Luna [" STRING_CAT(WINDOW_WIDTH) "x" STRING_CAT(WINDOW_HEIGHT) "]"

luna_input_evt 
keydown_handler(
	__in const SDL_Event &event,
	__in void *context
	)
{
	luna_input_evt result = LUNA_INPUT_EVT_NONE;

	std::cout << "KEYDOWN: 0x" << SCALAR_AS_HEX(uint32_t, event.key.keysym.sym) << std::endl;

	if(event.key.keysym.sym == SDLK_ESCAPE) {
		result = LUNA_INPUT_EVT_QUIT;
	}

	return result;
}

luna_input_evt 
window_handler(
	__in const SDL_Event &event,
	__in void *context
	)
{
	luna_input_evt result = LUNA_INPUT_EVT_NONE;

	std::cout << "WIN_CHANGE: 0x" << SCALAR_AS_HEX(uint32_t, event.window.event) << std::endl;

	return result;
}

luna_err_t 
on_draw(
	__in void *context,
	__in SDL_GLContext &screen
	)
{
	std::cout << "DRAW: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
	return LUNA_ERR_NONE;
}

luna_err_t 
on_setup(
	__in void *context
	)
{
	std::cout << "SETUP: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
	return LUNA_ERR_NONE;
}

luna_err_t 
on_start(
	__in void *context
	)
{
	std::cout << "START: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
	return LUNA_ERR_NONE;
}

luna_err_t 
on_stop(
	__in void *context
	)
{
	std::cout << "STOP: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
	return LUNA_ERR_NONE;
}

luna_err_t 
on_teardown(
	__in void *context
	)
{
	std::cout << "TEARDOWN: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
	return LUNA_ERR_NONE;
}

luna_err_t 
on_tick(
	__in void *context,
	__in uint32_t tick
	)
{
	std::cout << "TICK: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
	return LUNA_ERR_NONE;
}

int 
main(void)
{
	int result = 0;
	luna_ptr inst = NULL;

	std::cout << luna::version() << std::endl << std::endl;

	try {
		inst = luna::acquire();
		inst->initialize();

		// TODO
		luna_draw_config draw_config(on_draw);

		luna_event_config evt_config;
		evt_config.add(LUNA_EVT_SETUP, on_setup);
		evt_config.add(LUNA_EVT_START, on_start);
		evt_config.add(LUNA_EVT_STOP, on_stop);
		evt_config.add(LUNA_EVT_TEARDOWN, on_teardown);

		luna_display_config disp_config(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
		
		luna_input_config input_config;
		input_config.add(SDL_KEYDOWN, keydown_handler);
		input_config.add(SDL_WINDOWEVENT, window_handler);

		luna_tick_config tick_config(on_tick);

		inst->start(draw_config, tick_config, evt_config, disp_config, input_config);
		// ---

		inst->uninitialize();
	} catch(luna_exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = SCALAR_INVALID(int);
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = SCALAR_INVALID(int);
	}

	return result;
}
