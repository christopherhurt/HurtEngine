#include "mesh.h"

Mesh::Mesh(string * objFile) {
	vector<Vec3> positionsList;
	vector<Vec2> texCoordsList;
	vector<Vec3> normalsList;
	vector<unsigned int> indicesList;
	vector<unsigned int> texCoordsIndices;
	vector<unsigned int> normalsIndices;
	
	ifstream file(*objFile);
	string line;
	bool texCoordsFound = false;
	while (getline(file, line)) {
		if (line.find("vt") == 0) {
			strtok(const_cast<char *>(line.c_str()), " ");
			float texCoordU = (float)atof(strtok(NULL, " "));
			float texCoordV = (float)atof(strtok(NULL, " "));
			texCoordsList.push_back(Vec2(texCoordU, texCoordV));
			texCoordsFound = true;
		}
		else if (line.find("vn") == 0) {
			strtok(const_cast<char *>(line.c_str()), " ");
			float normalX = (float)atof(strtok(NULL, " "));
			float normalY = (float)atof(strtok(NULL, " "));
			float normalZ = (float)atof(strtok(NULL, " "));
			normalsList.push_back(Vec3(normalX, normalY, normalZ));
		}
		else if (line.find("v") == 0) {
			strtok(const_cast<char *>(line.c_str()), " ");
			float posX = (float)atof(strtok(NULL, " "));
			float posY = (float)atof(strtok(NULL, " "));
			float posZ = (float)atof(strtok(NULL, " "));
			positionsList.push_back(Vec3(posX, posY, posZ));
		}
		else if (line.find("f") == 0) {
			strtok(const_cast<char *>(line.c_str()), " ");
			for (int i = 0; i < 3; i++) {
				indicesList.push_back((unsigned int)atof(strtok(NULL, " /")) - 1);

				if (texCoordsFound) {
					texCoordsIndices.push_back((unsigned int)atof(strtok(NULL, " /")) - 1);
					normalsIndices.push_back((unsigned int)atof(strtok(NULL, " /")) - 1);
				}
				else {
					normalsIndices.push_back((unsigned int)atof(strtok(NULL, " /")) - 1);
				}
			}
		}
	}

	if (texCoordsList.size() != 0 && texCoordsList.size() != positionsList.size()) {
		cerr << "Invalid number of texture coordinates in " << *objFile << endl;
		exit(-1);
	}
	if (normalsList.size() != positionsList.size()) {
		cerr << "Invalid number of normal vectors in " << *objFile << endl;
		exit(-1);
	}

	unsigned int numPositions = positionsList.size() * 3;
	unsigned int numTexCoords = texCoordsList.size() * 2;
	unsigned int numNormals = normalsList.size() * 3;
	unsigned int numIndices = indicesList.size();

	float * positions = new float[numPositions];
	float * texCoords = new float[numTexCoords];
	float * normals = new float[numNormals];
	unsigned int * indices = new unsigned int[numIndices];

	for (int i = 0; i < indicesList.size(); i++) {
		unsigned int index = indicesList[i];
		Vec3 pos = positionsList[index];
		positions[index * 3] = pos.x;
		positions[index * 3 + 1] = pos.y;
		positions[index * 3 + 2] = pos.z;

		if (texCoordsFound) {
			unsigned int texCoordsIndex = texCoordsIndices[i];
			Vec2 texCoord = texCoordsList[texCoordsIndex];
			texCoords[index * 2] = texCoord.x;
			texCoords[index * 2 + 1] = texCoord.y;
		}

		unsigned int normalsIndex = normalsIndices[i];
		Vec3 normal = normalsList[normalsIndex];
		normals[index * 3] = normal.x;
		normals[index * 3 + 1] = normal.y;
		normals[index * 3 + 2] = normal.z;

		indices[i] = index;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vertVbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertVbo);
	glBufferData(GL_ARRAY_BUFFER, numPositions * sizeof(float), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &texVbo);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glBufferData(GL_ARRAY_BUFFER, numTexCoords * sizeof(float), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &normVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normVbo);
	glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &indexEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] positions;
	delete[] texCoords;
	delete[] normals;
	delete[] indices;

	this->numIndices = numIndices;
}

void Mesh::bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEbo);
}

unsigned int Mesh::getNumIndices() {
	return numIndices;
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &indexEbo);
	glDeleteBuffers(1, &vertVbo);
	glDeleteBuffers(1, &texVbo);
	glDeleteBuffers(1, &normVbo);
	glDeleteVertexArrays(1, &vao);
}