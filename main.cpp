// Least squares conformal mapping
//
// Author   : Mi, Liang (Arizona State University)
// Email    : icemiliang@gmail.com
// Date     : June 13th 2018

#include <cmath>
#include "Mesh.h"
#include "FormTrait.h"
#include "LSCM.h"
#include <iostream>
#include <vector>

using namespace MeshLib;

int main(int argc, char * argv[]) {

	// Parse the fixed vertex definitions if any
	// They are give in format --fixed-vertex coordX coordY coordZ
	std::vector<FixedVertexDefinition> fixedVertices;
	for (int i=3; i+5<argc; ) {
		if (strcmp(argv[i], "--fixed-vertex")) {
			i++;
			continue;
		}

		FixedVertexDefinition currentFixedVertex(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]), atof(argv[i+4]), atof(argv[i+5]));
		fixedVertices.push_back(currentFixedVertex);
		i += 6;
	}

	std::cout << "--> Reading mesh..." << std::endl;
	Mesh mesh;
	mesh.read_obj(argv[1]);

	if (fixedVertices.size() > 0) {
		std::cout << "--> Applying fixed vertices..." << std::endl;
		mesh.apply_fixed_vertices(fixedVertices);
	}

	FormTrait traits(&mesh);

	std::cout << "--> Computing conformal map..." << std::endl;
	LSCM lscm(&mesh);
    lscm.project();

	std::cout << "--> Writing mesh..." << std::endl;
	mesh.write_obj(argv[2]);
	return 0;
}
