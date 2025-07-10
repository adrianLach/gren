#include <GL/glew.h>
#include <cmath>
#include <numeric>

namespace gren
{

    struct Mesh {
        GLuint VBO, VAO, EBO;
    };

    static gren::Mesh generateTerrain(uint32_t width, uint32_t height) {
        return gren::Mesh {
            .VBO = 0,
            .VAO = 0,
            .EBO = 0
        };
    }
}
