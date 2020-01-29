#ifndef Vector4f_H
#define Vector4f_H

#include <cmath>

// Vector4f performs vector operations with 4-dimensions
// The purpose of this class is primarily for 3D graphics
// applications.
struct Vector4f {
    // Note: x, y, z, w are a convention
    // x, y, z, w could be position, but also any 4-component value.
    float x, y, z, w;

    // Default constructor
    Vector4f() = default;

    // The "Real" constructor we want to use.
    // This initializes the values x, y, z, w
    Vector4f(float a, float b, float c, float d) {
        this->x = a;
        this->y = b;
        this->z = c;
        this->w = d;
    }

    // Index operator, allowing us to access the individual
    // x, y, z, w components of our vector.
    float& operator[](int i) {
        return ((&x)[i]);
    }

    // Index operator, allowing us to access the individual
    // x, y, z, w components of our vector.
    const float& operator[](int i) const {
        return ((&x)[i]);
    }

    // Multiplication Operator
    // Multiply vector by a uniform-scalar.
    Vector4f& operator *=(float s) {
        this->x *= s;
        this->y *= s;
        this->z *= s;
        this->w *= s;

        return (*this);
    }

    // Division Operator
    Vector4f& operator /=(float s) {
        this->x /= s;
        this->y /= s;
        this->z /= s;
        this->w /= s;

        return (*this);
    }

    // Addition operator
    Vector4f& operator +=(const Vector4f& v) {
      this->x += v.x;
      this->y += v.y;
      this->z += v.z;
      this->w += v.w;

      return (*this);
    }

    // Subtraction operator
    Vector4f& operator -=(const Vector4f& v) {
      this->x -= v.x;
      this->y -= v.y;
      this->z -= v.z;
      this->w -= v.w;

      return (*this);
    }

};

// Compute the dot product of a Vector4f
inline float Dot(const Vector4f& a, const Vector4f& b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

// Multiplication of a vector by a scalar values
inline Vector4f operator *(const Vector4f& v, float s) {
  return Vector4f(v.x * s, v.y * s, v.z * s, v.w * s);
}

// Division of a vector by a scalar value.
inline Vector4f operator /(const Vector4f& v, float s) {
  return Vector4f(v.x / s, v.y / s, v.z / s, v.w / s);
}

// Negation of a vector
// Use Case: Sometimes it is handy to apply a force in an opposite direction
inline Vector4f operator -(const Vector4f& v) {
  return v * -1;
}

// Return the magnitude of a vector
inline float Magnitude(const Vector4f& v) {
  return sqrt(Dot(v, v));
}

// Add two vectors together
inline Vector4f operator +(const Vector4f& a, const Vector4f& b) {
  return Vector4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

// Subtract two vectors
inline Vector4f operator -(const Vector4f& a, const Vector4f& b) {
  return Vector4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

// Set a vectors magnitude to 1
// Note: This is NOT generating a normal vector
inline Vector4f Normalize(const Vector4f& v) {
  return v / Magnitude(v);
}

// Vector Projection
// Note: This is the vector projection of 'a' onto 'b'
inline Vector4f Project(const Vector4f& a, const Vector4f& b) {
  return a * Dot(a, Normalize(b));
}

// a x b (read: 'a crossed b')
// Produces a new vector perpendicular to a and b.
// (So long as a and b are not parallel which returns zero vector)
// Note: For a Vector4f, we can only compute a cross porduct to 
//       to vectors in 3-dimensions. Simply ignore w, and set to (0,0,0,1)
//       for this vector.
inline Vector4f CrossProduct(const Vector4f& a, const Vector4f& b) {
  float xp = (a.y * b.z) - (a.z * b.y);
  float yp = (a.z * b.x) - (a.x * b.z);
  float zp = (a.x * b.y) - (a.y * b.x);
  return Vector4f(xp, yp, zp, 1);
}

#endif
