#pragma once
#include "grn/vector.h"
#include <vector>
#include <grn/matrix.h>
#include <grn/texture.h>
#include <grn/mesh.h>

namespace grn
{

    class Component
    {
    public:
        virtual ~Component() = default;
        virtual void update(float deltaTime) = 0; // Update method for components
    };

    class MaterialComponent : public Component
    {
    public:
        MaterialComponent(const Texture &diffuse, const Texture &normal, const Texture &height)
            : m_diffuse(diffuse), m_normal(normal), m_height(height) {}
        void update(float deltaTime) override
        {
        }

        const Texture &getDiffuse() const { return m_diffuse; }
        const Texture &getNormal() const { return m_normal; }
        const Texture &getHeight() const { return m_height; }

    private:
        Texture m_diffuse;
        Texture m_normal;
        Texture m_height;
    };

    class MeshComponent : public Component
    {
    public:
        MeshComponent(const Mesh &mesh) : m_mesh(&mesh) {}
        void update(float deltaTime) override
        {
        }
        const Mesh &getMesh() const { return *m_mesh; }
        void setMesh(const Mesh &mesh) { m_mesh = &mesh; }

    private:
        const Mesh *m_mesh;
    };

    class Entity
    {
    public:
        Vector3f position;
        Vector3f rotation;
        Vector3f scale;

        Entity *parent;
        std::vector<Entity *> children;
        std::vector<Component *> components;

        Entity(const Vector3f &pos = Vector3f(0.0f, 0.0f, 0.0f),
               const Vector3f &rot = Vector3f(0.0f, 0.0f, 0.0f),
               const Vector3f &scl = Vector3f(1.0f, 1.0f, 1.0f))
            : position(pos), rotation(rot), scale(scl), parent(nullptr) {}
        Entity(const Entity &other)
            : position(other.position), rotation(other.rotation), scale(other.scale), parent(other.parent) {}

        void addChild(Entity *child)
        {
            if (child)
            {
                child->parent = this;
                children.push_back(child);
            }
        }
        void removeChild(Entity *child)
        {
            if (child)
            {
                auto it = std::find(children.begin(), children.end(), child);
                if (it != children.end())
                {
                    children.erase(it);
                    child->parent = nullptr;
                }
            }
        }
        void addComponent(Component *component)
        {
            if (component)
            {
                components.push_back(component);
            }
        }
        void removeComponent(Component *component)
        {
            if (component)
            {
                auto it = std::find(components.begin(), components.end(), component);
                if (it != components.end())
                {
                    components.erase(it);
                }
            }
        }
        template <typename T>
        T *getComponent()
        {
            for (auto &component : components)
            {
                if (T *casted = dynamic_cast<T *>(component))
                {
                    return casted;
                }
            }
            return nullptr;
        }
        template <typename T>
        bool hasComponent(T * = nullptr) const
        {
            for (const auto &component : components)
            {
                if (dynamic_cast<const T *>(component))
                {
                    return true;
                }
            }
            return false;
        }

        void update(float deltaTime)
        {
            for (auto &component : components)
            {
                if (component)
                {
                    component->update(deltaTime);
                }
            }
            for (auto &child : children)
            {
                if (child)
                {
                    child->update(deltaTime);
                }
            }
        }
    };

    class Camera : public Entity
    {
    public:
        Camera(const Vector3f &pos = Vector3f(0.0f, 0.0f, 0.0f),
               const Vector3f &rot = Vector3f(0.0f, 0.0f, 0.0f),
               const Vector3f &scl = Vector3f(1.0f, 1.0f, 1.0f))
            : Entity(pos, rot, scl) {}
        Camera(const Camera &other)
            : Entity(other) {}
        Matrix getViewMatrix() const
        {
            // Create the inverse transformation matrix for the camera
            // First create rotation matrices for each axis
            float cosX = cosf(-rotation.x);
            float sinX = sinf(-rotation.x);
            float cosY = cosf(-rotation.y);
            float sinY = sinf(-rotation.y);
            float cosZ = cosf(-rotation.z);
            float sinZ = sinf(-rotation.z);

            Matrix view;

            // Apply inverse rotation (transpose of rotation matrix)
            view[0] = cosY * cosZ;
            view[1] = sinX * sinY * cosZ + cosX * sinZ;
            view[2] = -cosX * sinY * cosZ + sinX * sinZ;
            view[3] = 0.0f;

            view[4] = -cosY * sinZ;
            view[5] = -sinX * sinY * sinZ + cosX * cosZ;
            view[6] = cosX * sinY * sinZ + sinX * cosZ;
            view[7] = 0.0f;

            view[8] = sinY;
            view[9] = -sinX * cosY;
            view[10] = cosX * cosY;
            view[11] = 0.0f;

            // Apply inverse translation
            view[12] = -(view[0] * position.x + view[4] * position.y + view[8] * position.z);
            view[13] = -(view[1] * position.x + view[5] * position.y + view[9] * position.z);
            view[14] = -(view[2] * position.x + view[6] * position.y + view[10] * position.z);
            view[15] = 1.0f;

            return view;
        }

        Matrix getProjectionMatrix()
        {
            return m_projectionMatrix;
        }

        void setProjectionMatrix(Matrix projection)
        {
            m_projectionMatrix = projection;
        }

    private:
        Matrix m_projectionMatrix;
    };
}
