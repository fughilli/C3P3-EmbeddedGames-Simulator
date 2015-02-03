#include "disp3d.h"

// Credit: http://www.songho.ca/opengl/gl_projectionmatrix.html
Matrix4x4 Display3D::glFrustum(fp_type left, fp_type right, fp_type bottom, fp_type top, fp_type nearVal, fp_type farVal)
{
    Matrix4x4 ret;

    ret.vals.d.f00 = 2 * nearVal / (right - left);
    ret.vals.d.f02 = (right + left) / (right - left);

    ret.vals.d.f11 = 2 * nearVal / (top - bottom);
    ret.vals.d.f12 = (top + bottom) / (top - bottom);

    ret.vals.d.f22 = - (farVal + nearVal) / (farVal - nearVal);
    ret.vals.d.f23 = - 2 * farVal * nearVal / (farVal - nearVal);

    ret.vals.d.f32 = (fp_type)(-1);

    return ret;
}

Matrix4x4 Display3D::glOrtho(fp_type left, fp_type right, fp_type bottom, fp_type top, fp_type nearVal, fp_type farVal)
{
    Matrix4x4 ret;

    ret.vals.d.f00 = 2 / (right - left);
    ret.vals.d.f03 = - (right + left) / (right - left);

    ret.vals.d.f11 = 2 / (top - bottom);
    ret.vals.d.f13 = - (top + bottom) / (top - bottom);

    ret.vals.d.f22 = - 2 / (farVal - nearVal);
    ret.vals.d.f23 = - (farVal + nearVal) / (farVal - nearVal);

    ret.vals.d.f33 = (fp_type)(1);

    return ret;
}

bool Display3D::glProject(const Matrix4x4& projMat, const Vector3d& other, Vector3d& ret, fp_type& w)
{
    w = (projMat.vals.d.f30 * other.x) + (projMat.vals.d.f31 * other.y) + (projMat.vals.d.f32 * other.z) + (projMat.vals.d.f33);

    ret.x = (projMat.vals.d.f00 * other.x) + (projMat.vals.d.f01 * other.y) + (projMat.vals.d.f02 * other.z) + (projMat.vals.d.f03);
    ret.y = (projMat.vals.d.f10 * other.x) + (projMat.vals.d.f11 * other.y) + (projMat.vals.d.f12 * other.z) + (projMat.vals.d.f13);
    ret.z = (projMat.vals.d.f20 * other.x) + (projMat.vals.d.f21 * other.y) + (projMat.vals.d.f22 * other.z) + (projMat.vals.d.f23);

    ret.x /= w;
    ret.y /= w;
    ret.z /= w;

    return true;
}

Matrix4x4 Display3D::matFromTransforms(const Quaternion& rot, const Vector3d& trans, fp_type scale)
{
    Matrix4x4 ret = Matrix4x4(rot) * Matrix4x4(trans) * Matrix4x4(scale);
}
