#include <gren/matrix.hpp>

namespace gren
{
    Matrix::Matrix()
    {
        for (int i = 0; i < 16; ++i)
        {
            m_data[i] = (i % 5 == 0) ? 1.0f : 0.0f; // Identity matrix
        }
    }

    Matrix::Matrix(const float *data)
    {
        for (int i = 0; i < 16; ++i)
        {
            m_data[i] = data[i];
        }
    }

    float &Matrix::operator[](int index)
    {
        return m_data[index];
    }

    const float &Matrix::operator[](int index) const
    {
        return m_data[index];
    }

    const float *Matrix::data() const
    {
        return m_data;
    }
}