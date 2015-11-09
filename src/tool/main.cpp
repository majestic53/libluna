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

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE "Luna [" STRING_CAT(WINDOW_WIDTH) "x" STRING_CAT(WINDOW_HEIGHT) "]"

typedef struct {
	luna_ptr instance;
	std::vector<GLuint> shaders;
	GLuint program;
	GLuint vao, vbo;
} luna_test_context;

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
	__in SDL_Window *window,
	__in SDL_GLContext screen,
	__in void *context
	)
{
	luna_err_t result = LUNA_ERR_NONE;
	luna_test_context *cont = (luna_test_context *) context;

	//std::cout << "DRAW: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;

	if(!cont) {
		result = LUNA_ERR_INVALID_ARGUMENT;
		goto exit;
	}

	glUseProgram(cont->program);
	glBindVertexArray(cont->vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glUseProgram(0);

exit:
	return result;
}

luna_err_t 
on_setup(
	__in void *context
	)
{
	GLint attrib;
	luna_err_t result = LUNA_ERR_NONE;
	luna_test_context *cont = (luna_test_context *) context;

	GLfloat vertexData[] = {
		0.f, 0.8f, 0.f, -0.8f, -0.8f, 0.f, 0.8f, -0.8f, 0.f,
		};

	std::cout << "SETUP: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;

	if(!cont) {
		result = LUNA_ERR_INVALID_ARGUMENT;
		goto exit;
	}

	cont->shaders.push_back(cont->instance->add_shader("./test/shader/fragment.glsl", true, GL_FRAGMENT_SHADER));
	cont->shaders.push_back(cont->instance->add_shader("./test/shader/vertex.glsl", true, GL_VERTEX_SHADER));

	cont->program = cont->instance->add_shader_program(cont->shaders);

	// TODO: turn this into a singleton operation
	glGenVertexArrays(1, &cont->vao);
	glBindVertexArray(cont->vao);

	glGenBuffers(1, &cont->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cont->vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	attrib = cont->instance->shader_program_attribute(cont->program, "vert0");
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// ---

exit:
	return result;
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
	luna_err_t result = LUNA_ERR_NONE;
	luna_test_context *cont = (luna_test_context *) context;

	std::cout << "TEARDOWN: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;

	if(!cont) {
		result = LUNA_ERR_INVALID_ARGUMENT;
		goto exit;
	}

	// TODO: turn this into a singleton operation
	glDeleteBuffers(1, &cont->vbo);
	cont->vbo = SCALAR_INVALID(GLint);
	glDeleteVertexArrays(1, &cont->vao);
	cont->vao = SCALAR_INVALID(GLint);
	// ---

	cont->instance->remove_shader_program(cont->program);
	cont->program = 0;

	cont->instance->remove_shader(cont->shaders.at(0));
	cont->instance->remove_shader(cont->shaders.at(1));
	cont->shaders.clear();

exit:
	return result;
}

luna_err_t 
on_tick(
	__in SDL_Window *window,
	__in SDL_GLContext screen,
	__in uint32_t tick,
	__in void *context
	)
{
	//std::cout << "TICK: 0x" << SCALAR_AS_HEX(void *, context) << std::endl;
	return LUNA_ERR_NONE;
}

int 
main(void)
{
	int result = 0;
	luna_test_context cont = { 0 };

	std::cout << luna::version() << std::endl << std::endl;

	try {
		cont.instance = luna::acquire();
		cont.instance->initialize();

		luna_draw_config draw_config(on_draw, &cont);

		luna_event_config evt_config;
		evt_config.add(LUNA_EVT_SETUP, on_setup, &cont);
		evt_config.add(LUNA_EVT_START, on_start, &cont);
		evt_config.add(LUNA_EVT_STOP, on_stop, &cont);
		evt_config.add(LUNA_EVT_TEARDOWN, on_teardown, &cont);

		luna_display_config disp_config(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
		
		luna_input_config input_config;
		input_config.add(SDL_KEYDOWN, keydown_handler, &cont);
		input_config.add(SDL_WINDOWEVENT, window_handler, &cont);

		luna_tick_config tick_config(on_tick, &cont);

		cont.instance->start(draw_config, tick_config, evt_config, disp_config, input_config);
		cont.instance->uninitialize();
	} catch(luna_exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = SCALAR_INVALID(int);
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = SCALAR_INVALID(int);
	}

	return result;
}
