#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace gren
{

    struct Mesh
    {
        GLuint VBO, VAO, EBO;
        uint size;
    };

    static Mesh loadFromFileOBJ(const std::string &filename)
    {
        Mesh mesh;
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);

        // read line by line using std::ifstream DO NOT ADD ANY COMMENTS AFTER THIS

        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open OBJ file: " + filename);
        }

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        std::string line;
        while (std::getline(file, line))
        {
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
                auto processVertex = [&](unsigned int v, unsigned int vt, unsigned int vn)
                {
                    // OBJ indices are 1-based, convert to 0-based
                    vertices.push_back(positions[(v - 1) * 3]);
                    vertices.push_back(positions[(v - 1) * 3 + 1]);
                    vertices.push_back(positions[(v - 1) * 3 + 2]);

                    if (!normals.empty())
                    {
                        vertices.push_back(normals[(vn - 1) * 3]);
                        vertices.push_back(normals[(vn - 1) * 3 + 1]);
                        vertices.push_back(normals[(vn - 1) * 3 + 2]);
                    }

                    if (!texCoords.empty())
                    {
                        vertices.push_back(texCoords[(vt - 1) * 2]);
                        vertices.push_back(texCoords[(vt - 1) * 2 + 1]);
                    }

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

        mesh.size = indices.size();

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Set vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return mesh;
    }

}
