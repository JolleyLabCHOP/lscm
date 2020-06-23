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
#include <sstream>
#include <vector>

using namespace MeshLib;

int main(int argc, char * argv[])
{
	// Parse the fixed vertex definitions if any
	// They are give in format --fixed-vertex coordX,coordY,coordZ or -v coordX,coordY,coordZ
	int currentArgument = 1;
	std::string inFilePath("");
	std::string outFilePath("");
	std::vector<FixedVertexDefinition> fixedVertices;
	while (currentArgument < argc) {
		if (!strcmp(argv[currentArgument], "--fixed-vertex") || !strcmp(argv[currentArgument], "-v")) {
			currentArgument++;
			std::string fixedVertexArg(argv[currentArgument]);
			double fixedVertexPos[3] = {0.0, 0.0, 0.0};
			for (int i=0; i<3; i++) {
				size_t commaPos = fixedVertexArg.find(",");
				if (i<2 && commaPos == std::string::npos) {
					std::cerr << "Invalid fixed vertex argument: " << argv[currentArgument] << std::endl;
					break;
				}
				std::stringstream ss;
				ss << fixedVertexArg.substr(0, commaPos);
				if (ss.bad()) {
					std::cerr << "Cannot parse fixed vertex argument: " << argv[currentArgument] << std::endl;
					break;
				}
				ss >> fixedVertexPos[i];
				fixedVertexArg = fixedVertexArg.substr(commaPos+1);
			}
			FixedVertexDefinition currentFixedVertex(fixedVertexPos[0], fixedVertexPos[1], fixedVertexPos[2]);
			fixedVertices.push_back(currentFixedVertex);
		}
		else if (!strcmp(argv[currentArgument], "--fixed-coords") || !strcmp(argv[currentArgument], "-c")) {
			for (int i=0; i<fixedVertices.size(); ++i) {
				double fixedTexturePos[2] = {0.0, 0.0};
				std::stringstream ss;
				ss << argv[currentArgument+1] << " " << argv[currentArgument+2];
				ss >> fixedTexturePos[0] >> fixedTexturePos[1];
				//fixedVertices[i].set_fixed_points(atof(argv[currentArgument+1]), atof(argv[currentArgument+2]));
				fixedVertices[i].set_fixed_points(fixedTexturePos[0], fixedTexturePos[1]);
				currentArgument += 2;
			}
		}
		else if (inFilePath.empty()) {
			inFilePath = std::string(argv[currentArgument]);
		}
		else {
			outFilePath = std::string(argv[currentArgument]);
		}
		currentArgument++;
	}

	std::cout << "--> Reading mesh..." << std::endl;
	Mesh mesh;
	mesh.read_obj(inFilePath.c_str());

	if (fixedVertices.size() > 0) {
		std::cout << "--> Applying fixed vertices..." << std::endl;
		mesh.apply_fixed_vertices(fixedVertices);
	}

	FormTrait traits(&mesh);

	std::cout << "--> Computing conformal map..." << std::endl;
	LSCM lscm(&mesh);
	lscm.project();

	std::cout << "--> Writing mesh..." << std::endl;
	mesh.write_obj(outFilePath.c_str());
	return 0;
}
