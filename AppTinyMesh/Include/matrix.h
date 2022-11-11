#pragma once

#include "mathematics.h"
#include <vector>

class Matrix
{
protected:
    int cols, rows;
    std::vector<double> entries;

public:
    Matrix() {}
    explicit Matrix(int);
    explicit Matrix(int, int);

    ~Matrix() {}

    // Fields access
    int Cols() const;
    int& Cols();
    int Rows() const;
    int& Rows();
    const std::vector<double>& Entries() const;
    std::vector<double>& Entries();

    static Matrix getRotationX(double);
    static Matrix getRotationY(double);
    static Matrix getRotationZ(double);

    // Comparison
    friend bool operator==(const Matrix&, const Matrix&);
    friend bool operator!=(const Matrix&, const Matrix&);

    static Matrix Transpose(Matrix&);
    static Matrix Inverse(Matrix&); // TODO

    // Arithmetic operators
    friend Matrix operator+(const Matrix&, const Matrix&);
    friend Matrix operator*(double, const Matrix&);
    friend Vector operator*(const Matrix&, const Vector&);
    friend Matrix operator*(const Matrix&, const Matrix&);
};

inline bool operator==(const Matrix& a, const Matrix& b)
{
    if (a.Cols() != b.Cols() || a.Rows() != b.Rows())
        return false;

    std::vector<double> ae = a.Entries();
    std::vector<double> be = b.Entries();

    for (std::size_t i = 0; i < ae.size(); ++i)
    {
        if (ae[i] != be[i])
            return false;
    }

    return true;
}

inline bool operator!=(const Matrix& a, const Matrix& b)
{
    // TODO change
    return !(a==b);
}

inline Matrix Matrix::Transpose(Matrix& a)
{
    Matrix m = Matrix();

    m.Entries().resize(a.Entries().size());
    m.Cols() = a.Rows();
    m.Rows() = a.Cols();

    for (int i = 0; i < a.Rows(); ++i)
    {
        for (int j = 0; j < a.Cols(); ++j)
        {
            m.Entries()[j * m.Cols() + m.Rows()];
        }
    }

    return m;
}

inline Matrix Matrix::Inverse(Matrix& a)
{
    Matrix m = Matrix();

    m.Entries().resize(a.Entries().size());
    m.Cols() = a.Cols();
    m.Rows() = a.Rows();

    return m;
}
