#pragma once

#include "grn/texture.h"
#include "grn/mesh.h"
#include "grn/vector.h"
#include "grn/matrix.h"
#include "grn/shader.h"
#include "grn/logger.h"

namespace grn
{

    class Framebuffer
    {
    public:
        Framebuffer(const Vector2i &size) : m_ID(0), m_size(size) {}
        virtual ~Framebuffer()
        {
            if (m_ID != 0)
            {
                glDeleteFramebuffers(1, &m_ID);
            }
        }

        virtual void initialize() = 0;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual bool isComplete() const = 0;

        Vector2i getSize() const
        {
            return m_size;
        }

        GLuint getID() const { return m_ID; }

    protected:
        GLuint m_ID;
        Vector2i m_size;
    };

    class FramebufferDeferred : public Framebuffer
    {
    public:
        Texture position, color, tangent, bitangent, normal, depth;
        FramebufferDeferred(const Vector2i &size) : Framebuffer(size)
        {
            initialize();
        }

        void initialize()
        {
            GLenum err;
            if ((err = glGetError()) != GL_NO_ERROR)
            {
                Logger::error("OpenGL error before framebuffer: " + std::to_string(err));
            }

            glGenFramebuffers(1, &m_ID);
            glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

            position.createColorTexture(m_size, GL_RGBA16, GL_UNSIGNED_BYTE);
            position.bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position.getID(), 0);

            color.createColorTexture(m_size, GL_RGBA16F, GL_FLOAT);
            color.bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, color.getID(), 0);

            tangent.createColorTexture(m_size, GL_RGBA16F, GL_FLOAT);
            tangent.bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tangent.getID(), 0);

            bitangent.createColorTexture(m_size, GL_RGBA16F, GL_FLOAT);
            bitangent.bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, bitangent.getID(), 0);

            normal.createColorTexture(m_size, GL_RGBA16F, GL_FLOAT);
            normal.bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, normal.getID(), 0);

            depth.createDepthTexture(m_size);
            depth.bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth.getID(), 0);

            GLenum drawBuffers[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
            glDrawBuffers(5, drawBuffers);

            GLuint renderBuffer;
            glGenRenderbuffers(1, &renderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, m_size.x, m_size.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

            if (!isComplete())
            {
                Logger::error("Framebuffer is not complete!");
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            if ((err = glGetError()) != GL_NO_ERROR)
            {
                Logger::error("OpenGL error after framebuffer: " + std::to_string(err));
            }
        }

        void bind() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        }
        void unbind() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        bool isComplete() const
        {
            return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        }

    private:
        GLuint m_ID;
    };

    struct RenderParameter
    {
        Vector3f *position;
        Vector3f *rotation;
        Vector3f *scale;
        Camera *camera;
        Texture *diffuse;
        Texture *normal;
        Texture *height;
        Shader *shader;
        Mesh *mesh;

        RenderParameter()
            : position(nullptr), rotation(nullptr), scale(nullptr), camera(nullptr),
              diffuse(nullptr), normal(nullptr), height(nullptr),
              shader(nullptr), mesh(nullptr) {}

        RenderParameter(Vector3f &pos,
                        Vector3f &rot,
                        Vector3f &scl,
                        Camera &cam,
                        Texture &tex,
                        Texture &nrm,
                        Texture &hgt,
                        Shader &shd,
                        Mesh &msh)
            : position(&pos), rotation(&rot), scale(&scl), camera(&cam), diffuse(&tex), normal(&nrm), height(&hgt), shader(&shd), mesh(&msh) {}
    };
    // abstract class for rendering
    class Renderer
    {
    public:
        virtual void setup() = 0;
        virtual void render(const RenderParameter &params) = 0;
        virtual void cleanup() = 0;
    };

    class EntityRenderer : public Renderer
    {
    public:
        EntityRenderer(Framebuffer &framebuffer) : Renderer()
        {
            m_framebuffer = &framebuffer;
        }
        void setup() override
        {
            m_framebuffer->bind();
            glViewport(0, 0, m_framebuffer->getSize().x, m_framebuffer->getSize().y);
            glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
        }
        void render(const Entity &entity, const Camera &camera, const Shader &shader)
        {
            RenderParameter params;
            params.camera = const_cast<Camera *>(&camera);
            params.shader = const_cast<Shader *>(&shader);
            params.position = const_cast<Vector3f *>(&entity.position);
            params.rotation = const_cast<Vector3f *>(&entity.rotation);
            params.scale = const_cast<Vector3f *>(&entity.scale);

            if (!entity.hasComponent<MeshComponent>())
                return;

            for (auto &component : entity.components)
            {
                if (auto meshComponent = dynamic_cast<MeshComponent *>(component))
                {
                    params.mesh = const_cast<Mesh *>(&meshComponent->getMesh());
                }
                else if (auto materialComponent = dynamic_cast<MaterialComponent *>(component))
                {
                    params.diffuse = const_cast<Texture *>(&materialComponent->getDiffuse());
                    params.normal = const_cast<Texture *>(&materialComponent->getNormal());
                    params.height = const_cast<Texture *>(&materialComponent->getHeight());
                }
            }
            render(params);
        }
        void render(const RenderParameter &params) override
        {
            Matrix model = Matrix::getModelMatrix(*params.position, *params.rotation, *params.scale);

            glUseProgram(params.shader->getProgram());
            glUniformMatrix4fv(glGetUniformLocation(params.shader->getProgram(), "model"), 1, GL_FALSE, model);

            glUniformMatrix4fv(glGetUniformLocation(params.shader->getProgram(), "view"), 1, GL_FALSE, params.camera->getViewMatrix());
            glUniformMatrix4fv(glGetUniformLocation(params.shader->getProgram(), "projection"), 1, GL_FALSE, params.camera->getProjectionMatrix());

            glUniform3fv(glGetUniformLocation(params.shader->getProgram(), "viewPos"), 1, -params.camera->position);

            // Bind texture and mesh
            glBindVertexArray(params.mesh->VAO);

            glActiveTexture(GL_TEXTURE0);
            params.diffuse->bind();
            glUniform1i(glGetUniformLocation(params.shader->getProgram(), "diffuseMap"), 0);

            glActiveTexture(GL_TEXTURE1);
            params.normal->bind();
            glUniform1i(glGetUniformLocation(params.shader->getProgram(), "normalMap"), 1);

            glActiveTexture(GL_TEXTURE2);
            params.height->bind();
            glUniform1i(glGetUniformLocation(params.shader->getProgram(), "heightMap"), 2);

            // Set up vertex attributes
            glDrawElements(GL_TRIANGLES, params.mesh->size, GL_UNSIGNED_INT, 0);

            params.diffuse->unbind();
            params.normal->unbind();
            params.height->unbind();
        }
        void cleanup() override
        {
            glUseProgram(0);
            m_framebuffer->unbind();
        }

    private:
        Framebuffer *m_framebuffer;
    };
} // namespace grn
