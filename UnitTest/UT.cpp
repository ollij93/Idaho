// Includes...
#include "stdafx.h"
#include "CppUnitTest.h"

// Project Includes...
#include "Core/Math.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
    TEST_CLASS(MatrixTestClass)
    {
    public:
        TEST_METHOD(MatrixOperatorTests)
        {
            Matrix3x3 xMatrix1;
            Matrix3x3 xMatrix2(1, 2, 3, 4, 5, 6, 7, 8, 9);
            Matrix3x3 xMatrix3(xMatrix2);

            // Test Constructors
            Assert::AreEqual(Matrix3x3::Identity(), xMatrix1, L"Matrix: Default constructor does not create identity matrix.", LINE_INFO());
            for (int i = 0; i < 9; i++) {
                Assert::AreEqual(float(i + 1), xMatrix2[i/3][i%3], L"Matrix: Error with float constructor or [] operators.", LINE_INFO());
            }
            Assert::AreEqual(xMatrix2, xMatrix3, L"Matrix: Error with copy constructor.", LINE_INFO());

            // Test * operators
            Assert::AreEqual(xMatrix2, Matrix3x3::Identity()*xMatrix2, L"Matrix: Identity matrix pre-multiply is not no-op.", LINE_INFO());
            Assert::AreEqual(xMatrix2, xMatrix2*Matrix3x3::Identity(), L"Matrix: Identity matrix post-multiply is not no-op.", LINE_INFO());
            Assert::AreEqual(Matrix3x3(30, 36, 42, 66, 81, 96, 102, 126, 150), xMatrix2*xMatrix2, L"Matrix: Multiplication error.", LINE_INFO());

            Assert::AreEqual(Vector3<float>(1.f, 2.f, 3.f), Matrix3x3::Identity()*Vector3<float>(1.f, 2.f, 3.f), L"Matrix: Identity pre-multiply vector is not no-op.", LINE_INFO());
            Assert::AreEqual(Vector3<float>(14.f, 32.f, 50.f), xMatrix2 * Vector3<float>(1.f, 2.f, 3.f), L"Matrix: Vector multiplication error.", LINE_INFO());

            // Test [] operators
            for (int i = 0; i < 9; i++) {
                xMatrix1[i/3][i%3] = float(rand())/float(RAND_MAX);
            }
            Assert::AreEqual(xMatrix1.xx, xMatrix1[0][0], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.xy, xMatrix1[0][1], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.xz, xMatrix1[0][2], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.yx, xMatrix1[1][0], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.yy, xMatrix1[1][1], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.yz, xMatrix1[1][2], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.zx, xMatrix1[2][0], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.zy, xMatrix1[2][1], L"Matrix: Error with [] operators.", LINE_INFO());
            Assert::AreEqual(xMatrix1.zz, xMatrix1[2][2], L"Matrix: Error with [] operators.", LINE_INFO());
        }

        TEST_METHOD(MatrixFunctionTests)
        {
            // Test RotateLocalX()
            // Test RotateWorldY()
        }

    };
}

namespace Microsoft {
    namespace VisualStudio {
        namespace CppUnitTestFramework {
            template<> std::wstring ToString<Matrix3x3>(const Matrix3x3& xMatrix)
            {
                std::wostringstream ss;
                ss << "{ ";
                ss << xMatrix.xx << ", ";
                ss << xMatrix.xy << ", ";
                ss << xMatrix.xz << "," << std::endl;
                ss << xMatrix.yx << ", ";
                ss << xMatrix.yy << ", ";
                ss << xMatrix.yz << "," << std::endl;
                ss << xMatrix.zx << ", ";
                ss << xMatrix.zy << ", ";
                ss << xMatrix.zz << " }";
                return ss.str();
            }

            template<> std::wstring ToString<Vector3<float>>(const Vector3<float>& xVector)
            {
                std::wostringstream ss;
                ss << "{ ";
                ss << xVector.x << ", ";
                ss << xVector.y << ", ";
                ss << xVector.z << " }";
                return ss.str();
            }
        }
    }
}
