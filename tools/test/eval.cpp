/**
 * tlibs test file
 * @author Tobias Weber <tobias.weber@tum.de>
 * @license GPLv2 or GPLv3
 *
 * ----------------------------------------------------------------------------
 * tlibs -- a physical-mathematical C++ template library
 * Copyright (C) 2017-2021  Tobias WEBER (Institut Laue-Langevin (ILL),
 *                          Grenoble, France).
 * Copyright (C) 2015-2017  Tobias WEBER (Technische Universitaet Muenchen
 *                          (TUM), Garching, Germany).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 */

// gcc -o eval eval.cpp ../log/log.cpp -std=c++11 -lstdc++ -lm

#include "../string/eval.h"
#include "../string/eval_impl.h"
#include <iostream>
#include <tuple>
#include <vector>

int main()
{
	std::vector<std::tuple<std::string, double>> vecExprs
	{
		std::make_tuple("exp(sin(-pi^2 + 1))", std::exp(std::sin(-M_PI*M_PI + 1.))),
		std::make_tuple("12 - 34*5 + pi", 12.-34.*5. + M_PI),
		std::make_tuple("1e4 + sqrt(1e3)", 1e4 + std::sqrt(1e3)),
		std::make_tuple("sin(sqrt(4^pi) - 5) + cos(34)^2", std::sin(std::sqrt(std::pow(4.,M_PI)) - 5.) + std::cos(34.)*std::cos(34.) ),
	};

	for(const auto& tup : vecExprs)
	{
		auto result = tl::eval_expr<std::string, double>(std::get<0>(tup));
		std::cout << "Expression: " << std::get<0>(tup);
		std::cout << ", parsing OK: " << result.first;
		std::cout << ", value: " << result.second;
		std::cout << ", check: " << std::get<1>(tup) << std::endl;
	}

	auto result2 = tl::eval_expr<std::wstring, float>(L"2*pi * 100 / 10");
	std::wcout << L"OK: " << result2.first;
	std::wcout << L", value: " << result2.second << std::endl;

	auto result3 = tl::eval_expr<std::string, int>("mod(abs(5-4-3-2-1), 3)");
	std::cout << "OK: " << result3.first;
	std::cout << ", value: " << result3.second << std::endl;

	return 0;
}
