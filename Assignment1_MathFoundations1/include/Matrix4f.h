// High level design note
// Our matrix should match the behavior of the glm library.
#ifndef MATRIX4F_H
#define MATRIX4F_H

#include <cmath>
#include <string>

// We need to Vector4f header in order to multiply a matrix
// by a vector.
#include "Vector4f.h"

// Matrix 4f represents 4x4 matrices in Math
struct Matrix4f{
private:
    float n[4][4];  // Store each value of the matrix

public:
    Matrix4f() = default;

    Matrix4f(float n00, float n10, float n20, float n30,
             float n01, float n11, float n21, float n31,
             float n02, float n12, float n22, float n32,
             float n03, float n13, float n23, float n33) {
        n[0][0] = n00; n[1][0] = n10; n[2][0] = n20; n[3][0] = n30;
        n[0][1] = n01; n[1][1] = n11; n[2][1] = n21; n[3][1] = n31;
        n[0][2] = n02; n[1][2] = n12; n[2][2] = n22; n[3][2] = n32;
        n[0][3] = n03; n[1][3] = n13; n[2][3] = n23; n[3][3] = n33;
    }

    // Matrix constructor from four vectors.
    // Note: 'd' will almost always be 0,0,0,1
    Matrix4f(const Vector4f& a, const Vector4f& b, const Vector4f& c, const Vector4f& d) {
      n[0][0] = a.x; n[1][0] = b.x; n[2][0] = c.x; n[3][0] = d.x;
      n[0][1] = a.y; n[1][1] = b.y; n[2][1] = c.y; n[3][1] = d.y;
      n[0][2] = a.z; n[1][2] = b.z; n[2][2] = c.z; n[3][2] = d.z;
      n[0][3] = a.w; n[1][3] = b.w; n[2][3] = c.w; n[3][3] = d.w;
    }

    // Makes the matrix an identity matrix
    void identity() {
        n[0][0] = 1.0f; n[1][0] = 0.0f; n[2][0] = 0.0f; n[3][0] = 0.0f;
        n[0][1] = 0.0f; n[1][1] = 1.0f; n[2][1] = 0.0f; n[3][1] = 0.0f;
        n[0][2] = 0.0f; n[1][2] = 0.0f; n[2][2] = 1.0f; n[3][2] = 0.0f;
        n[0][3] = 0.0f; n[1][3] = 0.0f; n[2][3] = 0.0f; n[3][3] = 1.0f;
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    float& operator ()(int i, int j) {
      return (n[j][i]);
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    const float& operator ()(int i, int j) const {
      return (n[j][i]);
    }

    // Return a single vector from the matrix.
    Vector4f& operator [](int j) {
      return (*reinterpret_cast<Vector4f *>(n[j]));
    }

    // Return a single vector from the matrix.
    const Vector4f& operator [](int j) const {
      return (*reinterpret_cast<const Vector4f *>(n[j]));
    }

    // Make a matrix rotate about various axis
    Matrix4f MakeRotationX(float t);

    Matrix4f MakeRotationY(float t);

    Matrix4f MakeRotationZ(float t);

    Matrix4f MakeScale(float sx, float sy, float sz);
};

// Matrix multiply by a vector
Vector4f operator *(const Matrix4f& M, const Vector4f& v) {
  float xp = M(0,0) * v[0] + M(0, 1) * v[1] + M(0, 2) * v[2] + M(0, 3) * v[3];
  float yp = M(1,0) * v[0] + M(1, 1) * v[1] + M(1, 2) * v[2] + M(1, 3) * v[3];
  float zp = M(2,0) * v[0] + M(2, 1) * v[1] + M(2, 2) * v[2] + M(2, 3) * v[3];
  float wp = M(3,0) * v[0] + M(3, 1) * v[1] + M(3, 2) * v[2] + M(3, 3) * v[3];

  return Vector4f(xp, yp, zp, wp);
}

// Matrix Multiplication
Matrix4f operator *(const Matrix4f& A, const Matrix4f& B) {
  return Matrix4f(A * B[0], A * B[1], A * B[2], A * B[3]);
}

// Make a matrix rotate about various axis
Matrix4f Matrix4f::MakeRotationX(float t) {
    Matrix4f rotationX = Matrix4f(1,  0,        0,       0,
                                  0,  cos(t),  -sin(t),  0,
                                  0,  sin(t),   cos(t),  0,
                                  0,  0,        0,       1);
    return rotationX * *this;
}

Matrix4f Matrix4f::MakeRotationY(float t) {
    Matrix4f rotationY = Matrix4f( cos(t),  0,  sin(t),  0,
                                   0,       1,  0,       0,
                                  -sin(t),  0,  cos(t),  0,
                                   0,       0,  0,       1);
    return rotationY * *this;
}

Matrix4f Matrix4f::MakeRotationZ(float t) {
    Matrix4f rotationZ = Matrix4f(cos(t),  -sin(t),  0,  0,
                                  sin(t),   cos(t),  0,  0,
                                  0,        0,       1,  0,
                                  0,        0,       0,  1);
    return rotationZ * *this;
}

Matrix4f Matrix4f::MakeScale(float sx,float sy, float sz) {
    Matrix4f scaling = Matrix4f(sx,  0,   0,   0,
                                0,   sy,  0,   0,
                                0,   0,   sz,  0,
                                0,   0,   0,   1);
    return scaling * *this;
}

#endif
