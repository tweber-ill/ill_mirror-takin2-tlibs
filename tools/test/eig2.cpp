/**
 * tlibs test file
 * @author Tobias Weber <tobias.weber@tum.de>
 * @license GPLv2 or GPLv3
 */

// gcc -I/usr/include/lapacke -o eig2 eig2.cpp ../log/log.cpp -lstdc++ -lm -llapacke -llapack -std=c++11

#define TLIBS_INC_HDR_IMPLS
#include "../math/linalg.h"
#include "../math/linalg2.h"

namespace ublas = boost::numeric::ublas;
using T = double;

int main()
{
	ublas::matrix<T> M = tl::make_mat({
		{1.1,-2.2,3.3},
		{-2.2,5.5,8.8},
		{3.3,8.8,-9.9}	});
	//ublas::matrix<T> M = tl::make_mat({{123.4,0},{0,567.8}});

	ublas::matrix<T> M_org = M;
	std::cout << M << std::endl;

	std::vector<ublas::vector<T>> evecs;
	std::vector<T> evals, evals_check;
	tl::eigenvec_sym<T>(M, evecs, evals);
	tl::eigenval_sym<T>(M, evals_check);
	for(int i=0; i<evals.size(); ++i)
		std::cout << "eval: " << evals[i] <<
		", eval_check: " << evals_check[i] <<
		", evec: " << (evecs[i]/ublas::norm_2(evecs[i])) <<
		", len: " << ublas::norm_2(evecs[i]) << std::endl;
	std::cout << std::endl;
	for(int i=0; i<evals.size(); ++i)
		std::cout <<
		"val*vec: " << evals[i]*evecs[i] <<
		"\nmat*vec:" << ublas::prod(M, evecs[i]) << std::endl;
	std::cout << std::endl;

	std::vector<ublas::vector<T>> evecs2;
	std::vector<T> evals2;
	tl::eigenvec_sym_simple(M, evecs2, evals2, true);
	for(int i=0; i<evals2.size(); ++i)
		std::cout << "eval: " << evals2[i] <<
		", evec: " << (evecs2[i]/ublas::norm_2(evecs2[i])) <<
		", len: " << ublas::norm_2(evecs2[i]) << std::endl;
	std::cout << std::endl;

	std::vector<ublas::vector<T>> evecs2_r, evecs2_i;
	std::vector<T> evals2_r, evals2_i;
	std::vector<T> evals2_r_check, evals2_i_check;
	tl::eigenvec(M, evecs2_r, evecs2_i, evals2_r, evals2_i, true);
	tl::eigenval(M, evals2_r_check, evals2_i_check);
	for(int i=0; i<evals2_r.size(); ++i)
		std::cout << "eval r: " << evals2_r[i] <<
		", eval r check: " << evals2_r_check[i] <<
		", evec r: " << evecs2_r[i] << std::endl;
	for(int i=0; i<evals2_i.size(); ++i)
		std::cout << "eval i: " << evals2_i[i] <<
		", eval i check: " << evals2_i_check[i] <<
		", evec i: " << evecs2_i[i] << std::endl;
	std::cout << std::endl;


	// ----------------------------------------------------------------

	std::vector<ublas::vector<std::complex<T>>> evecs_c, evecs_c_sel;
	std::vector<T> evals_c, evals_c_sel, evals_c_check;
	ublas::matrix<std::complex<T>> Mc = tl::make_mat<ublas::matrix<std::complex<T>>>({
		{std::complex<T>(1., 0.), std::complex<T>(3., 1.5),std::complex<T>(5., 2.)},
		{std::complex<T>(3., -1.5), std::complex<T>(2., 0.), std::complex<T>(2.2, -1.7)},
		{std::complex<T>(5., -2.), std::complex<T>(2.2, 1.7), std::complex<T>(4., 0.)},
	});
	std::cout << "hermitian: " << Mc << std::endl;

	tl::eigenvec_herm<T>(Mc, evecs_c, evals_c, true);
	tl::eigenvecsel_herm<T>(Mc, evecs_c_sel, evals_c_sel, true, -5., 5., 1e-4);
	tl::eigenval_herm<T>(Mc, evals_c_check);
	for(int i=0; i<evals_c.size(); ++i)
	{
		std::cout << "eval: " << evals_c[i] <<
		", eval_check: " << evals_c_check[i] <<
		", evec: " << evecs_c[i] << std::endl;
	}
	std::cout << std::endl;
	for(int i=0; i<evals_c.size(); ++i)
		std::cout <<
		"val*vec: " << evals_c[i]*evecs_c[i] <<
		"\nmat*vec:" << ublas::prod(Mc, evecs_c[i]) << std::endl;
	std::cout << std::endl;

	for(int i=0; i<evals_c_sel.size(); ++i)
	{
		std::cout << "eval_sel: " << evals_c_sel[i] <<
		", evec_sel: " << evecs_c_sel[i] << std::endl;
	}
	std::cout << std::endl;

	for(int i=0; i<evals_c_sel.size(); ++i)
		std::cout <<
			"val*vec_sel: " << evals_c_sel[i]*evecs_c_sel[i] <<
			"\nmat*vec_sel:" << ublas::prod(Mc, evecs_c_sel[i]) << std::endl;
	std::cout << std::endl;



	std::vector<ublas::vector<std::complex<T>>> evecs_c2;
	std::vector<std::complex<T>> evals_c2;
	std::vector<std::complex<T>> evals_c2_check;

	tl::eigenvec_cplx<T>(Mc, evecs_c2, evals_c2, true);
	tl::eigenval_cplx<T>(Mc, evals_c2_check);
	for(int i=0; i<evals_c2.size(); ++i)
		std::cout << "eval: " << evals_c2[i] <<
		", eval_check: " << evals_c2_check[i] <<
		", evec: " << evecs_c2[i] << std::endl;
	std::cout << std::endl;
	for(int i=0; i<evals_c.size(); ++i)
		std::cout <<
		"val*vec: " << evals_c2[i]*evecs_c2[i] <<
		"\nmat*vec:" << ublas::prod(Mc, evecs_c2[i]) << std::endl;
	std::cout << std::endl;


	return 0;
}
