#pragma once

// Includes...
#include <math.h>
#include <DirectXMath.h>

namespace Math {
    // Constants
    static const float PI = 3.141592654f;
    static const float Sqrt2 = 1.414213562f;
    static const float Sqrt3 = 1.732050808f;
    static const float InvSqrt2 = 0.707106781f;
    static const float InvSqrt3 = 0.577350269f;
}

template<typename T>
class Vector2 {
public:
    Vector2() : x((T)0), y((T)0) {}
    Vector2(T _x, T _y) : x(_x), y(_y) {}
    Vector2(const Vector2<T>& xVector) : x(xVector.x), y(xVector.y) {}
    ~Vector2() {}

    static inline const Vector2<T> ZeroVector() { return Vector2<T>(); }

    Vector2<T> operator +(const Vector2<T>& xVector) const { return Vector2<T>(x + xVector.x, y + xVector.y); }
    Vector2<T> operator -(const Vector2<T>& xVector) const { return Vector2<T>(x - xVector.x, y - xVector.y); }

    T x;
    T y;
};

template<typename T>
class Vector3 {
public:
    Vector3() : x((T)0), y((T)0), z((T)0) {}
    Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector3<T>& xVector) : x(xVector.x), y(xVector.y), z(xVector.z) {}
    ~Vector3() {}

    operator DirectX::XMVECTOR() const { return DirectX::XMVectorSet(x, y, z, 0); }

    static inline const Vector3<T> ZeroVector() { return Vector3<T>(); }

    void operator +=(Vector3<T> xVector) { x += xVector.x; y += xVector.y; z += xVector.z; }
    void operator -=(Vector3<T> xVector) { x -= xVector.x; y -= xVector.y; z -= xVector.z; }
    void operator *=(T val) { x *= val; y *= val; z *= val; }

    Vector3<T> operator +(const Vector3<T>& xVector) const { return Vector3<T>(x + xVector.x, y + xVector.y, z + xVector.z); }
    Vector3<T> operator -(const Vector3<T>& xVector) const { return Vector3<T>(x - xVector.x, y - xVector.y, z + xVector.z); }
    Vector3<T> operator *(T val) const { return Vector3<T>(x * val, y * val, z * val); }

    T operator*(Vector3<T> xVector) const { return (x * xVector.x + y * xVector.y + z * xVector.z); }
    Vector3<T>
    operator^(Vector3<T> xVector) const {
        Vector3<T> xResultVector;

        xResultVector.x = y * xVector.z - z * xVector.y;
        xResultVector.y = z * xVector.x - x * xVector.z;
        xResultVector.z = x * xVector.y - y * xVector.x;

        return xResultVector;
    }

    T LengthSqrd() const { return x*x + y*y + z*z; }

    T x;
    T y;
    T z;
};

class Matrix3x3 {
public:
    Matrix3x3()
      : xx(1.f), xy(0.f), xz(0.f),
        yx(0.f), yy(1.f), yz(0.f),
        zx(0.f), zy(0.f), zz(1.f)
    {
    }
    Matrix3x3(
        float _xx, float _xy, float _xz,
        float _yx, float _yy, float _yz,
        float _zx, float _zy, float _zz)
        : xx(_xx), xy(_xy), xz(_xx)
        , yx(_yx), yy(_yy), yz(_yz)
        , zx(_zx), zy(_zy), zz(_zz)
    {
    }
    ~Matrix3x3() {}

    static inline const Matrix3x3 Identity() { return Matrix3x3(); }

    Matrix3x3
    operator*(const Matrix3x3& xMatrix) const
    {
        Matrix3x3 xResult;
        xResult.xx = xx *xMatrix.xx + xy * xMatrix.yx + xz * xMatrix.zx;
        xResult.xy = xx *xMatrix.xy + xy * xMatrix.yy + xz * xMatrix.zy;
        xResult.xz = xx *xMatrix.xz + xy * xMatrix.yz + xz * xMatrix.zz;

        xResult.yx = yx *xMatrix.xx + yy * xMatrix.yx + yz * xMatrix.zx;
        xResult.yy = yx *xMatrix.xy + yy * xMatrix.yy + yz * xMatrix.yz;
        xResult.yz = yx *xMatrix.xz + yy * xMatrix.yz + yz * xMatrix.zz;

        xResult.zx = zx *xMatrix.xx + zy * xMatrix.yx + zz * xMatrix.zx;
        xResult.zy = zx *xMatrix.xy + zy * xMatrix.yy + zz * xMatrix.zy;
        xResult.zz = zx *xMatrix.xz + zy * xMatrix.yz + zz * xMatrix.zz;

        return xResult;
    }

    void
    RotateLocalX(float fDeltaTheta)
    {
        Matrix3x3 xRotateMat = { 1.f, 0.f, 0.f,
            0.f, cosf(fDeltaTheta), -sinf(fDeltaTheta),
            0.f, sinf(fDeltaTheta), cosf(fDeltaTheta) };
        Matrix3x3 xResult = xRotateMat * (*this);
        (*this) = xResult;
    }
    void
    RotateWorldY(float fDeltaPhi)
    {
        Matrix3x3 xRotateMat = { cosf(fDeltaPhi), 0.f, sinf(fDeltaPhi),
            0.f,  1.f, 0.f,
            -sinf(fDeltaPhi), 0.f, cosf(fDeltaPhi) };
        Matrix3x3 xResult = (*this) * xRotateMat;
        (*this) = xResult;
    }

    float xx, xy, xz;
    float yx, yy, yz;
    float zx, zy, zz;
};
