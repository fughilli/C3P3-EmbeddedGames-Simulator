#ifndef _DISP_3D_H_
#define _DISP_3D_H_

#include "../utils/VectorLib/Matrix.h"
#include "../utils/VectorLib/Vector.h"

class Display3D
{
public:
    static Matrix4x4 glFrustum(fp_type left, fp_type right, fp_type bottom, fp_type top, fp_type nearVal, fp_type farVal);
    static Matrix4x4 glOrtho(fp_type left, fp_type right, fp_type bottom, fp_type top, fp_type nearVal, fp_type farVal);
    static bool glProject(const Matrix4x4& projMat, const Vector3d& other, Vector3d& ret, fp_type& w);
    static Matrix4x4 matFromTransforms(const Quaternion& rot, const Vector3d& trans, fp_type scale);
};

#endif // _DISP_3D_H_

