#include "OpenGLMesh.h"
#include <glad.h>
FOpenGLMesh::FOpenGLMesh(const char*name)
{
	Rename(name);
	glGenVertexArrays(1, &Vao);
	glGenBuffers(1, &Vbo);
	glGenBuffers(1, &Ibo);
}

FOpenGLMesh::~FOpenGLMesh()
{
	glDeleteVertexArrays(1, &Vao);
	glDeleteBuffers(1, &Ibo);
	glDeleteBuffers(1, &Vbo);
}

void FOpenGLMesh::SetData(const std::vector<FVertex>& vertices, const std::vector<uint32_t>& indices,const FAABB& obb)
{
	OBB = obb;
	VertexNum = vertices.size();
	IndexNum = indices.size();

	glBindVertexArray(Vao);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, Position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, Tangent));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, Bitangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, Normal));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, BoneIds));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, BoneWeights));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, UV0));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, UV1));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, UV2));
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (GLvoid*)offsetof(FVertex, UV3));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}
