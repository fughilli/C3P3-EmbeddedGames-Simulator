#ifndef _DISP_3D_H_
#define _DISP_3D_H_

#ifdef __cplusplus

#include "../utils/VectorLib/Matrix.h"
#include "../utils/VectorLib/Vector.h"
#include "stdint.h"

class Display3D
{
public:
    static Matrix4x4 glFrustum(fp_type left, fp_type right,
                               fp_type bottom, fp_type top,
                               fp_type nearVal, fp_type farVal);
    static Matrix4x4 glOrtho(fp_type left, fp_type right,
                             fp_type bottom, fp_type top,
                             fp_type nearVal, fp_type farVal);
    static bool glProject(const Matrix4x4& projMat, const Vector3d& other, Vector3d& ret, fp_type& w);
    static Matrix4x4 matFromTransforms(const Quaternion& rot, const Vector3d& trans, fp_type scale);
    static bool drawVertexBufferF(void* workspace, const uint16_t wspclen,
                                 const Vector3d* vbuf, uint16_t vbuflen,
                                 const uint16_t* tlist, uint16_t tlistlen,
                                 const Matrix4x4& projMat);
    static bool drawVertexBufferI(void* workspace, const uint16_t wspclen,
                                 const int16_t* vbuf, uint16_t vbuflen,
                                 const uint16_t* tlist, uint16_t tlistlen,
                                 const Matrix4x4& projMat);
};

#endif

#endif // _DISP_3D_H_

