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

void 
on_setup(
	__in void *context
	)
{
	std::cout << "SETUP: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
}

void 
on_start(
	__in void *context
	)
{
	std::cout << "START: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
}

void 
on_stop(
	__in void *context
	)
{
	std::cout << "STOP: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
}

void 
on_teardown(
	__in void *context
	)
{
	std::cout << "TEARDOWN: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
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
		luna_config config;
		config.insert(std::pair<luna_evt_t, std::pair<luna_evt_cb, void *>>(LUNA_EVT_SETUP, 
			std::pair<luna_evt_cb, void *>(on_setup, NULL)));
		config.insert(std::pair<luna_evt_t, std::pair<luna_evt_cb, void *>>(LUNA_EVT_START, 
			std::pair<luna_evt_cb, void *>(on_start, NULL)));
		config.insert(std::pair<luna_evt_t, std::pair<luna_evt_cb, void *>>(LUNA_EVT_STOP, 
			std::pair<luna_evt_cb, void *>(on_stop, NULL)));
		config.insert(std::pair<luna_evt_t, std::pair<luna_evt_cb, void *>>(LUNA_EVT_TEARDOWN, 
			std::pair<luna_evt_cb, void *>(on_teardown, NULL)));

		luna_display_config disp_config(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
		
		luna_input_config input_config;
		input_config.insert(std::pair<SDL_EventType, std::pair<luna_input_cb, void *>>(SDL_KEYDOWN, 
			std::pair<luna_input_cb, void *>(keydown_handler, NULL)));

		inst->start(config, disp_config, input_config);
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
