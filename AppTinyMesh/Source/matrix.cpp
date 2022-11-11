#include "matrix.h"
#include <cmath>
#include <iostream>

int Matrix::Cols() const
{
    return cols;
}

int& Matrix::Cols()
{
    return cols;
}

int Matrix::Rows() const
{
    return rows;
}

int& Matrix::Rows()
{
    return rows;
}

const std::vector<double>& Matrix::Entries() const
{
    return entries;
}

std::vector<double>& Matrix::Entries()
{
    return entries;
}

Matrix::Matrix(int n)
{
    cols = n;
    rows = n;
    entries.resize(n*n);
}

Matrix::Matrix(int n, int m)
{
    cols = n;
    rows = m;
    entries.resize(n*m);
}

Matrix Matrix::getRotationX(double a) {
    Matrix m(3);

    m.Entries().assign({
        1.0, 0.0, 0.0,
        0.0, Math::ToZero(std::cos(a)), Math::ToZero(-std::sin(a)),
        0.0, Math::ToZero(std::sin(a)), Math::ToZero(std::cos(a))
    });

    return m;
}

Matrix Matrix::getRotationY(double a) {
    Matrix m(3);

    m.Entries().assign({
        Math::ToZero(std::cos(a)), 0.0, Math::ToZero(std::sin(a)),
        0.0, 1.0, 0.0,
        Math::ToZero(-std::sin(a)), 0.0, Math::ToZero(std::cos(a))
    });

    return m;
}

Matrix Matrix::getRotationZ(double a) {
    Matrix m(3);

    m.Entries().assign({
        1.0, 0.0, 0.0,
        0.0, Math::ToZero(std::cos(a)), Math::ToZero(-std::sin(a)),
        0.0, Math::ToZero(std::sin(a)), Math::ToZero(std::cos(a))
    });

    return m;
}

Matrix operator+(const Matrix& a, const Matrix& b)
{
    if (a.Cols() != b.Cols() || a.Rows() != b.Rows())
        return a;

    Matrix c(a.Cols(), a.Rows());

    std::vector<double> ae = a.Entries();
    std::vector<double> be = b.Entries();
    std::vector<double> ce = c.Entries();

    for (std::size_t i = 0; i < ae.size(); ++i)
    {
        ce[i] = ae[i] + be[i];
    }

    return c;
}

Matrix operator*(double n, const Matrix& a)
{
    Matrix b(a.Cols(), a.Rows());

    std::vector<double> ae = a.Entries();
    std::vector<double> be = b.Entries();

    for (std::size_t i = 0; i < ae.size(); ++i)
    {
        be[i] = ae[i] * n;
    }

    return b;
}

Vector operator*(const Matrix& a, const Vector& b)
{
    if (a.Cols() != 3 || a.Rows() != 3)
           return b;

    std::vector<double> ae = a.Entries();
    Vector v(0.0, 0.0, 0.0);

    for (int i = 0; i < a.Rows(); ++i)
    {
        v[i] = Math::ToZero(b[0] * ae[i * a.Cols()] +
               b[1] * ae[i * a.Cols() + 1] +
               b[2] * ae[i * a.Cols() + 2]);
    }

    return v;
}

Matrix operator*(const Matrix& a, const Matrix& b)
{
    if (a.Cols() != b.Rows())
        return a;

    Matrix c(a.Rows(), b.Cols());

    std::vector<double> ae = a.Entries();
    std::vector<double> be = b.Entries();
    std::vector<double> ce = c.Entries();

    for (int i = 0; i < a.Rows(); ++i)
    {
        for (int j = 0; j < b.Cols(); ++j)
        {
            double v = 0;
            for (int k = 0; k < a.Cols(); ++k)
            {
                v += a.Entries()[i * a.Rows() + k] * b.Entries()[b.Cols() + k];
            }
            ce[i * a.Rows() + j] = v;
        }
    }

    return c;
}
