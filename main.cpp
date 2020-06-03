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
#include <fstream>


using namespace MeshLib;

int main(int argc, char * argv[]) {

	if (argv[3]) {

		
		int totalVertices = 0;
		for (int i = 3; strcmp(argv[i], "--fixed-vertex") == 0; i += 4)
		{
			totalVertices++;
			if (argv[i + 4] == NULL) break; 
		}
		
		char* originalName = argv[1];
		FILE* file = fopen(originalName, "r");
		FILE* file2 = fopen("Copy.obj", "w+");

		if (file == NULL) return 1;
		
		// delete preset fixed vertices from human.obj, run it, add them back in using the command prompt
		// Try with another model
		// Include the 0.0 0.0 parameters after the fixed vertex

		// 112.954 68.5313 4.34287
		// 112.47 129.683 -3.1017

		char line[255];
		bool foundMatches[1024];
		int lineNumber = 0;
		char seps[] = "\n ";
		int numberOfVertices = 0;
		char* token = strtok(line, seps);

		
		
		do {
			
			fgets(line, sizeof(line), file);
			//std::cout << line << std::endl;
			if (line[0] != 'v') {
				fprintf(file2, line);
			}
			
		} while (line[0] != 'v');
		
		token = strtok(line, seps);

		while (strcmp(token, "v") == 0 && numberOfVertices != totalVertices) {
			
			const char* float1 = strtok(NULL, seps);
			const char* float2 = strtok(NULL, seps);
			const char* float3 = strtok(NULL, seps);
			int i = 3;
			int argumentNumber = 0;

			while (argv[i]) {

				argumentNumber++;
				
				if ((strcmp(argv[i + 1], float1) == 0) && (strcmp(argv[i + 2], float2) == 0) && (strcmp(argv[i + 3], float3) == 0))
				{
					//std::cout << "found match" << std::endl;
					fprintf(file2, "v %s %s %s fix 0.0 0.0\n", float1, float2, float3);
					foundMatches[argumentNumber - 1] = true;
					numberOfVertices++;
					break;
				}
				else 
				{
					i += 4;
				}
			}

			if (argv[i] == NULL) fprintf(file2, "v %s %s %s\n", float1, float2, float3);


			lineNumber++;
			fgets(line, sizeof(line), file);
			token = strtok(line, seps);
		}
		
		// If inputs not matched in current obj, add them before copying rest of file
		for (int i = 0; i < totalVertices; i++) {
			if (foundMatches[i] != 1) {
				fprintf(file2, "v %s %s %s fix 0.0 0.0\n", argv[(i + (3*(i+1))) + 1], argv[(i + (3 * (i + 1))) + 2], argv[(i + (3 * (i + 1))) + 3]);
			}
		}

		// Print rest of line that was left parsed after breaking the first while loop
		while (token != NULL) {
			fprintf(file2, "%s ", token);
			token = strtok(NULL, seps);
		}
		
		fprintf(file2, "\n");

		// Copying rest of file
		while (fgets(line, sizeof(line), file) != NULL) {
			fprintf(file2, line);
		}
		
		fclose(file);
		fclose(file2);
		remove(originalName);
		rename("Copy.obj", originalName);
		//std::cout << lineNumber << std::endl;



	}

	std::cout << "--> Reading mesh..." << std::endl;
	Mesh mesh;
	mesh.read_obj(argv[1]);

	FormTrait traits(&mesh);

	std::cout << "--> Computing conformal map..." << std::endl;
	LSCM lscm(&mesh);
    lscm.project();

	std::cout << "--> Writing mesh..." << std::endl;
	mesh.write_obj(argv[2]);
	return 0;
}
