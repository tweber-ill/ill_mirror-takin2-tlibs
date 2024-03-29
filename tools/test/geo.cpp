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

// gcc -DNO_LAPACK -o geo geo.cpp ../log/log.cpp -std=c++11 -lstdc++ -lm

#include <iostream>
#include "../math/geo.h"

using T = double;
using t_vec = typename tl::Plane<T>::t_vec;

int main()
{
	tl::Plane<T> plane1(
		tl::make_vec({1., 1., 0.}),	// vec0
		tl::make_vec({0., 1.5, 1.}),	// dir0
		tl::make_vec({1., 0., 1.})	// dir1
		);

	tl::Plane<T> plane2(
		tl::make_vec({0., 2., 0.}),	// vec0
		tl::make_vec({1., 0., 0.}),	// dir0
		tl::make_vec({0., 0., -1.})	// dir1
		);

	tl::Plane<T> plane3(
		tl::make_vec({1., 1., 3.}),	// vec0
		tl::make_vec({1., 1., 0.}),	// dir0
		tl::make_vec({0., 2., 0.5})	// dir1
		);

	t_vec pt;
	plane1.intersect(plane2, plane3, pt);
	std::cout << pt << std::endl;


	// alternative method
	tl::Line<T> line;
	plane1.intersect(plane2, line);

	T t;
	line.intersect(plane3, t);
	std::cout << line(t) << std::endl;
}
