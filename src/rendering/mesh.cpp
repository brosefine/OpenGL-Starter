# include <rendering/mesh.h>
#include <iostream>

Mesh::Mesh(std::vector<glm::vec3> pos, std::vector<glm::vec2> uv, std::vector<unsigned int> indxs)
	: indices_(indxs){
	if (pos.size() != uv.size()) {
		std::cerr << "[Error][Mesh] Number of position vectors must match number of uv coordinates." << std::endl;
		return;
	}
	for (int i = 0; i < pos.size(); ++i) {
		vertices_.push_back({ pos.at(i), uv.at(i) });
	}

	createMesh();
}

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indxs)
	: vertices_(verts), indices_(indxs) {

	createMesh();
}

void Mesh::draw(int drawMode) const {
	glBindVertexArray(VAO_);
	glDrawElements(drawMode, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::createMesh() {
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);

	glBindVertexArray(VAO_);
	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);
	// elements (=indices)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
	// vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvCoord));

	glBindVertexArray(0);
}