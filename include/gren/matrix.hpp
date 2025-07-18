#pragma once

#include <cmath>
#include <gren/vector.h>

namespace gren
{

    struct Matrix
    {
        float m_data[16];

        Matrix()
        {
            for (int i = 0; i < 16; ++i)
            {
                m_data[i] = (i % 5 == 0) ? 1.0f : 0.0f; // Identity matrix
            }
        }

        Matrix(const float *data)
        {
            for (int i = 0; i < 16; ++i)
            {
                m_data[i] = data[i];
            }
        }

        float &operator[](int index)
        {
            return m_data[index];
        }

        const float &operator[](int index) const
        {
            return m_data[index];
        }

        operator const float *() const
        {
            return &m_data[0];
        }

        static Matrix getPerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
        {
            Matrix projection;
            float tanHalfFov = tanf(fov / 2.0f);

            projection[0] = 1.0f / (aspectRatio * tanHalfFov);
            projection[5] = 1.0f / tanHalfFov;
            projection[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
            projection[11] = -1.0f;
            projection[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

            return projection;
        }

        static Matrix getViewMatrix(const Vector &position, const Vector &target, const Vector &up)
        {
            Matrix view;
            Vector zAxis = (position - target).normalize();
            Vector xAxis = up.cross(zAxis).normalize();
            Vector yAxis = zAxis.cross(xAxis);

            float data[16];

            data[0] = xAxis.x;
            data[4] = yAxis.x;
            data[8] = zAxis.x;
            data[12] = 0.0f;
            data[1] = xAxis.y;
            data[5] = yAxis.y;
            data[9] = zAxis.y;
            data[13] = 0.0f;
            data[2] = xAxis.z;
            data[6] = yAxis.z;
            data[10] = zAxis.z;
            data[14] = 0.0f;
            data[3] = -xAxis.dot(position);
            data[7] = -yAxis.dot(position);
            data[11] = -zAxis.dot(position);
            data[15] = 1.0f;

            view = Matrix(data);

            return view;
        }

        static Matrix getModelMatrix(const Vector &position, const Vector &rotation, const Vector &scale)
        {
            Matrix model;
            float cosX = cosf(rotation.x);
            float sinX = sinf(rotation.x);
            float cosY = cosf(rotation.y);
            float sinY = sinf(rotation.y);
            float cosZ = cosf(rotation.z);
            float sinZ = sinf(rotation.z);

            model[0] = scale.x * (cosY * cosZ);
            model[1] = scale.x * (cosY * sinZ);
            model[2] = -scale.x * sinY;
            model[3] = 0.0f;

            model[4] = scale.y * (sinX * sinY * cosZ - cosX * sinZ);
            model[5] = scale.y * (sinX * sinY * sinZ + cosX * cosZ);
            model[6] = scale.y * (sinX * cosY);
            model[7] = 0.0f;

            model[8] = scale.z * (cosX * sinY * cosZ + sinX * sinZ);
            model[9] = scale.z * (cosX * sinY * sinZ - sinX * cosZ);
            model[10] = scale.z * (cosX * cosY);
            model[11] = 0.0f;

            model[12] = position.x;
            model[13] = position.y;
            model[14] = position.z;
            model[15] = 1.0f;

            return model;
        }

        static Matrix getOrthographicMatrix(float left, float right, float bottom, float top, float nearPlane, float farPlane)
        {
            Matrix ortho;
            ortho[0] = 2.0f / (right - left);
            ortho[5] = 2.0f / (top - bottom);
            ortho[10] = -2.0f / (farPlane - nearPlane);
            ortho[12] = -(right + left) / (right - left);
            ortho[13] = -(top + bottom) / (top - bottom);
            ortho[14] = -(farPlane + nearPlane) / (farPlane - nearPlane);
            ortho[15] = 1.0f;

            return ortho;
        }
    };
}
