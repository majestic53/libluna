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

#include <cstdarg>
#include "../include/luna.h"

namespace LUNA {

	#define EXCEPTION_MALFORMED "Malformed exception"

	_luna_exception::_luna_exception(
		__in const std::string &message,
		__in_opt const std::string &function,
		__in_opt const std::string &file,
		__in_opt size_t line
		) :
			std::runtime_error(message),
			m_file(file),
			m_function(function),
			m_line(line)
	{
		return;
	}

	_luna_exception::_luna_exception(
		__in const _luna_exception &other
		) :
			std::runtime_error(other),
			m_file(other.m_file),
			m_function(other.m_function),
			m_line(other.m_line)
	{
		return;
	}

	_luna_exception::~_luna_exception(void)
	{
		return;
	}

	_luna_exception &
	_luna_exception::operator=(
		__in const _luna_exception &other
		)
	{

		if(this != &other) {
			std::runtime_error::operator=(other);
			m_file = other.m_file;
			m_function = other.m_function;
			m_line = other.m_line;
		}

		return *this;
	}

	std::string 
	_luna_exception::file(void)
	{
		return m_file;
	}

	std::string 
	_luna_exception::function(void)
	{
		return m_function;
	}

	void 
	_luna_exception::generate(
		__in const std::string &message,
		__in const std::string &function,
		__in const std::string &file,
		__in size_t line,
		__in const char *format,
		...
		)
	{
		int length = 0;
		va_list arguments;
		std::string buffer;
		std::stringstream stream;

		if(!message.empty()) {
			stream << message;
		}

		if(format) {
			va_start(arguments, format);
			length = vsnprintf(NULL, 0, format, arguments);
			va_end(arguments);

			if(length < 0) {
				buffer = EXCEPTION_MALFORMED;
			} else {
				va_start(arguments, format);
				buffer.resize(++length);

				length = vsnprintf((char *) &buffer[0], length, 
					format, arguments);
				if(length < 0) {
					buffer = EXCEPTION_MALFORMED;
				}

				va_end(arguments);
			}
		}

		if(!buffer.empty()) {

			if(!message.empty()) {
				stream << ":";
			}

			stream << buffer;
		}

		throw luna_exception(stream.str(), function, file, line);
	}

	size_t 
	_luna_exception::line(void)
	{
		return m_line;
	}

	std::string 
	_luna_exception::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		UNREFERENCE_PARAM(verbose);

		result << what();
#ifndef NDEBUG
		result << " (";

		if(!m_function.empty()) {
			result << m_function << ":";
		}

		if(!m_file.empty()) {
			result << m_file << ":";
		}

		result << m_line << ")";
#endif // NDEBUG

		return result.str();
	}
}
