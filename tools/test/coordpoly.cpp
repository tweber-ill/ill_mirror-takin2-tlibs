/**
 * tests calculation of coordination polyhedra
 * @author Tobias Weber <tobias.weber@tum.de>
 * @license GPLv2 or GPLv3
 */

// gcc -DNO_LAPACK -o coordpoly coordpoly.cpp ../log/log.cpp ../file/x3d.cpp -std=c++11 -lstdc++ -lm

#include <iostream>
#include <fstream>
#include "../math/geo.h"
#include "../file/x3d.h"

using T = double;
using t_vec = tl::ublas::vector<T>;

int main()
{
	std::ifstream ifstr("/home/tweber/tmp/coord.dat");
	std::vector<t_vec> vecVertices;
	t_vec vecCentral = tl::zero_v<t_vec>(3);

	while(1)
	{
		t_vec vec(3);
		ifstr >> vec[0] >> vec[1] >> vec[2];
		if(!ifstr)
			break;

		vecVertices.push_back(vec);
	}


	tl::X3d x3d;
	for(const t_vec& vec : vecVertices)
	{
		tl::X3dTrafo *pTrafo = new tl::X3dTrafo();
		pTrafo->SetTrans(vec - vecCentral);

		tl::X3dSphere *pSphere = new tl::X3dSphere(0.025);
		pSphere->SetColor(tl::make_vec({1., 0., 0.}));
		pTrafo->AddChild(pSphere);

		x3d.GetScene().AddChild(pTrafo);
	}


	std::vector<std::vector<t_vec>> vecPolys = tl::verts_to_polyhedron<t_vec, std::vector, T>(vecVertices);

	for(const auto& vecPoly : vecPolys)
	{
		tl::X3dPolygon *pPoly = new tl::X3dPolygon();
		//tl::X3dLines *pPoly = new tl::X3dLines();
		pPoly->SetColor(tl::make_vec({1., 1., 0.}));

		for(const t_vec& vec : vecPoly)
			pPoly->AddVertex(vec - vecCentral);

		x3d.GetScene().AddChild(pPoly);
	}



	x3d.Save("/home/tweber/tmp/coord.x3d");

	return 0;
}
