#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "logger.hpp"

namespace gren
{

    struct Mesh
    {
        GLuint VBO, VAO, EBO;
        uint size;
    };

    struct Vertex
    {
        float position[3];
        float normal[3];
        float texCoord[2];
        float tangent[3];
        float bitangent[3];
    };

    static Mesh loadFromFileOBJ(const std::string &filename)
    {
        gren::Logger::debug("Loading OBJ file: " + filename);
        Mesh mesh;
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);

        // read line by line using std::ifstream DO NOT ADD ANY COMMENTS AFTER THIS

        std::ifstream file(filename);
        if (!file.is_open())
        {
            gren::Logger::error("Failed to open OBJ file: " + filename);
            throw std::runtime_error("Failed to open OBJ file: " + filename);
        }

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::string line;
        while (std::getline(file, line))
        {
            gren::Logger::debug("Processing line: " + line);
            if (line.empty() || line[0] == '#')
                continue; // skip empty lines and comments
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            if (prefix == "v")
            {
                float x, y, z;
                iss >> x >> y >> z;
                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);
            }
            else if (prefix == "vn")
            {
                float x, y, z;
                iss >> x >> y >> z;
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
            else if (prefix == "vt")
            {
                float u, v;
                iss >> u >> v;
                texCoords.push_back(u);
                texCoords.push_back(v);
            }
            else if (prefix == "f")
            {
                gren::Logger::debug("Processing face: " + line);
                auto processVertex = [&](unsigned int v, unsigned int vt, unsigned int vn)
                {
                    // OBJ indices are 1-based, convert to 0-based
                    Vertex vertex;
                    vertex.position[0] = positions[(v - 1) * 3];
                    vertex.position[1] = positions[(v - 1) * 3 + 1];
                    vertex.position[2] = positions[(v - 1) * 3 + 2];

                    if (!normals.empty())
                    {
                        vertex.normal[0] = normals[(vn - 1) * 3];
                        vertex.normal[1] = normals[(vn - 1) * 3 + 1];
                        vertex.normal[2] = normals[(vn - 1) * 3 + 2];
                    }
                    else
                    {
                        vertex.normal[0] = 0.0f;
                        vertex.normal[1] = 0.0f;
                        vertex.normal[2] = 0.0f;
                    }

                    if (!texCoords.empty())
                    {
                        vertex.texCoord[0] = texCoords[(vt - 1) * 2];
                        vertex.texCoord[1] = texCoords[(vt - 1) * 2 + 1];
                    }
                    else
                    {
                        vertex.texCoord[0] = 0.0f;
                        vertex.texCoord[1] = 0.0f;
                    }

                    // Add vertex to the vertices vector
                    vertices.push_back(vertex);
                    indices.push_back(indices.size());
                };

                unsigned int v1, v2, v3, v4, vt1, vt2, vt3, vt4, vn1, vn2, vn3, vn4;
                char slash; // to consume the slashes in the face definitions
                iss >> v1 >> slash >> vt1 >> slash >> vn1;
                iss >> v2 >> slash >> vt2 >> slash >> vn2;
                iss >> v3 >> slash >> vt3 >> slash >> vn3;

                processVertex(v1, vt1, vn1);
                processVertex(v2, vt2, vn2);
                processVertex(v3, vt3, vn3);

                // Handle quads by triangulating
                if (iss >> v4 >> slash >> vt4 >> slash >> vn4)
                {
                    processVertex(v1, vt1, vn1);
                    processVertex(v3, vt3, vn3);
                    processVertex(v4, vt4, vn4);
                }
            }
        }

        // Calculate tangents and bitangents for each vertex
        std::vector<float[3]> temp_tangents(vertices.size());
        std::vector<float[3]> temp_bitangents(vertices.size());

        for(size_t i = 0; i < indices.size(); i+=3)
        {
            Vertex &v0 = vertices[indices[i]];
            Vertex &v1 = vertices[indices[i+1]];
            Vertex &v2 = vertices[indices[i+2]];

            float deltaPos1[3] = { v1.position[0] - v0.position[0], v1.position[1] - v0.position[1], v1.position[2] - v0.position[2] };
            float deltaPos2[3] = { v2.position[0] - v0.position[0], v2.position[1] - v0.position[1], v2.position[2] - v0.position[2] };
            float deltaUV1[2] = { v1.texCoord[0] - v0.texCoord[0], v1.texCoord[1] - v0.texCoord[1] };
            float deltaUV2[2] = { v2.texCoord[0] - v0.texCoord[0], v2.texCoord[1] - v0.texCoord[1] };

            float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

            float tangent[3], bitangent[3];

            tangent[0] = f * (deltaUV2[1] * deltaPos1[0] - deltaUV1[1] * deltaPos2[0]);
            tangent[1] = f * (deltaUV2[1] * deltaPos1[1] - deltaUV1[1] * deltaPos2[1]);
            tangent[2] = f * (deltaUV2[1] * deltaPos1[2] - deltaUV1[1] * deltaPos2[2]);

            bitangent[0] = f * (-deltaUV2[0] * deltaPos1[0] + deltaUV1[0] * deltaPos2[0]);
            bitangent[1] = f * (-deltaUV2[0] * deltaPos1[1] + deltaUV1[0] * deltaPos2[1]);
            bitangent[2] = f * (-deltaUV2[0] * deltaPos1[2] + deltaUV1[0] * deltaPos2[2]);

            for(int j=0; j<3; ++j) {
                temp_tangents[indices[i]][j] += tangent[j];
                temp_tangents[indices[i+1]][j] += tangent[j];
                temp_tangents[indices[i+2]][j] += tangent[j];
                temp_bitangents[indices[i]][j] += bitangent[j];
                temp_bitangents[indices[i+1]][j] += bitangent[j];
                temp_bitangents[indices[i+2]][j] += bitangent[j];
            }
        }

        for(size_t i = 0; i < vertices.size(); ++i) {
            Vertex& v = vertices[i];
            
            // Gram-Schmidt orthogonalize
            float n_dot_t = v.normal[0] * temp_tangents[i][0] + v.normal[1] * temp_tangents[i][1] + v.normal[2] * temp_tangents[i][2];
            
            float t_res[3];
            t_res[0] = temp_tangents[i][0] - n_dot_t * v.normal[0];
            t_res[1] = temp_tangents[i][1] - n_dot_t * v.normal[1];
            t_res[2] = temp_tangents[i][2] - n_dot_t * v.normal[2];

            float t_len = sqrt(t_res[0]*t_res[0] + t_res[1]*t_res[1] + t_res[2]*t_res[2]);
            v.tangent[0] = t_res[0] / t_len;
            v.tangent[1] = t_res[1] / t_len;
            v.tangent[2] = t_res[2] / t_len;

            // Bitangent can be recalculated from N and T
            v.bitangent[0] = v.normal[1] * v.tangent[2] - v.normal[2] * v.tangent[1];
            v.bitangent[1] = v.normal[2] * v.tangent[0] - v.normal[0] * v.tangent[2];
            v.bitangent[2] = v.normal[0] * v.tangent[1] - v.normal[1] * v.tangent[0];
        }

        mesh.size = indices.size();

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


        constexpr GLsizei stride = sizeof(Vertex);
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        // TexCoords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);
        // Tangent
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, tangent));
        glEnableVertexAttribArray(3);
        // Bitangent
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, bitangent));
        glEnableVertexAttribArray(4);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        gren::Logger::debug("Finished creating mesh from OBJ file: " + filename);
        return mesh;
    }

}
