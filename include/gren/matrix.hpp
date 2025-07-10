#pragma once

#include <cmath>
namespace gren
{

    class Matrix
    {
    public:
        Matrix();
        Matrix(const float *data);

        float &operator[](int index);

        const float &operator[](int index) const;

        const float *data() const;

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

        static Matrix getViewMatrix(const float *position, const float *target, const float *up)
        {
            Matrix view;
            float zAxis[3] = {
                position[0] - target[0],
                position[1] - target[1],
                position[2] - target[2]};
            float length = sqrtf(zAxis[0] * zAxis[0] + zAxis[1] * zAxis[1] + zAxis[2] * zAxis[2]);
            zAxis[0] /= length;
            zAxis[1] /= length;
            zAxis[2] /= length;

            float xAxis[3];
            xAxis[0] = up[1] * zAxis[2] - up[2] * zAxis[1];
            xAxis[1] = up[2] * zAxis[0] - up[0] * zAxis[2];
            xAxis[2] = up[0] * zAxis[1] - up[1] * zAxis[0];

            length = sqrtf(xAxis[0] * xAxis[0] + xAxis[1] * xAxis[1] + xAxis[2] * xAxis[2]);
            xAxis[0] /= length;
            xAxis[1] /= length;
            xAxis[2] /= length;

            float yAxis[3];
            yAxis[0] = zAxis[1] * xAxis[2] - zAxis[2] * xAxis[1];
            yAxis[1] = zAxis[2] * xAxis[0] - zAxis[0] * xAxis[2];
            yAxis[2] = zAxis[0] * xAxis[1] - zAxis[1] * xAxis[0];

            float data[16];

            data[0] = xAxis[0];
            data[4] = yAxis[0];
            data[8] = -zAxis[0];
            data[12] = 0.0f;
            data[1] = xAxis[1];
            data[5] = yAxis[1];
            data[9] = -zAxis[1];
            data[13] = 0.0f;
            data[2] = xAxis[2];
            data[6] = yAxis[2];
            data[10] = -zAxis[2];
            data[14] = 0.0f;
            data[3] = -(xAxis[0] * position[0] + xAxis[1] * position[1] + xAxis[2] * position[2]);
            data[7] = -(yAxis[0] * position[0] + yAxis[1] * position[1] + yAxis[2] * position[2]);
            data[11] = zAxis[0] * position[0] + zAxis[1] * position[1] + zAxis[2] * position[2];
            data[15] = 1.0f;

            view = Matrix(data);

            return view;
        }

        static Matrix getModelMatrix(const float *position, const float *rotation, const float *scale)
        {
            Matrix model;
            float cosX = cosf(rotation[0]);
            float sinX = sinf(rotation[0]);
            float cosY = cosf(rotation[1]);
            float sinY = sinf(rotation[1]);
            float cosZ = cosf(rotation[2]);
            float sinZ = sinf(rotation[2]);

            model[0] = scale[0] * (cosY * cosZ);
            model[1] = scale[0] * (cosY * sinZ);
            model[2] = -scale[0] * sinY;
            model[3] = 0.0f;

            model[4] = scale[1] * (sinX * sinY * cosZ - cosX * sinZ);
            model[5] = scale[1] * (sinX * sinY * sinZ + cosX * cosZ);
            model[6] = scale[1] * (sinX * cosY);
            model[7] = 0.0f;

            model[8] = scale[2] * (cosX * sinY * cosZ + sinX * sinZ);
            model[9] = scale[2] * (cosX * sinY * sinZ - sinX * cosZ);
            model[10] = scale[2] * (cosX * cosY);
            model[11] = 0.0f;

            model[12] = position[0];
            model[13] = position[1];
            model[14] = position[2];
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

    private:
        float m_data[16];
    };
}
