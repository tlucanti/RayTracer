/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 17:51:45 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/08 14:21:09 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <stdexcept>
# include <string_view>
# include <string>
# include <cstring>
# include "defs.h"

namespace tlucanti {
	class Exception : public std::exception
	/*
		This is a dummy class for basic exception messages

		all classes in this library using this exception as base class

		you can override this file by your own exception class
	*/
	{
	public:
		Exception(const std::string_view &parent, const std::string_view &message) noexcept
				: _message("[FAIL] " + std::string(parent) + ": " + std::string(message)) {}
		Exception(const std::string_view &parent, const std::string_view &message,
			int error) noexcept
				: _message("[FAIL] " + std::string(parent) + ": " + strerror(error) + ": " + std::string(message)) {}
		Exception(const std::string_view &parent, const std::string_view &message,
			const std::string_view &error) noexcept
				: _message("[FAIL] " + std::string(parent) + ": " + std::string(error) + ": " + std::string(message)) {}
		__WUR inline const char *what() const noexcept override
		{ return _message.c_str(); }
		~Exception() noexcept override = default;

	private:
		const std::string _message;
	};
}

#endif /* EXCEPTION_HPP */
