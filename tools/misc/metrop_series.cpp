/**
 * metropolis test
 * @author Tobias Weber <tobias.weber@tum.de>
 * @date 8-oct-16
 * @license GPLv2 or GPLv3
 */
// gcc -march=native -O3 -std=c++11 -DNO_JPEG -DNO_TIFF -o metrop_series metrop_series.cpp ../../log/log.cpp ../../math/rand.cpp -lstdc++ -lm -lpng

#include "../../math/mag.h"
#include "../../math/rand.h"
#include "../../math/units.h"
#include "../../gfx/gil.h"
#include "../../helper/array.h"
#include <iostream>
#include <fstream>

using t_real = double;
static const tl::t_energy_si<t_real> meV = tl::get_one_meV<t_real>();
static const tl::t_temperature_si<t_real> kelvin = tl::get_one_kelvin<t_real>();
static const t_real k = t_real(tl::get_kB<t_real>() / meV * kelvin);

int main()
{
	tl::init_rand();

	std::size_t iIter = 1e7;

	long iW = 256;
	long iH = 256;

	t_real J = 2.5;		// meV

	boost::multi_array<bool, 2> arr
		= tl::rand_array<bool, 2, boost::array, boost::multi_array>({iW, iH});
	std::ofstream ofstrE("E.dat");

	for(t_real T=200.; T>=0.; T-=10.)
	{
		t_real Etot = 0., S = 0.;
		tl::log_info("T = ", T, " K");
		tl::metrop<t_real, 2>({iW,iH}, iIter, J, k, T, arr, &Etot, &S);
		ofstrE << T << "\t" << Etot <<  "\t" << S << std::endl;

		using t_view = tl::gil::gray8_view_t;
		using t_pix = typename t_view::value_type;
		t_view view;
		std::vector<t_pix> vecPix;
		tl::create_imgview<t_view>(iW, iH, vecPix, view);

		for(long i=0; i<iH; ++i)
			for(long j=0; j<iW; ++j)
				*(view.row_begin(i)+j) = arr[i][j]*255;

		std::ostringstream ostrImg;
		ostrImg << "T" << T << ".png";
		tl::save_view(ostrImg.str().c_str(), &view);
	}

	return 0;
}
