#include "disp3d.h"
#include "screen.h"

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

bool Display3D::drawVertexBufferF(void* workspace, const uint16_t wspclen, const Vector3d* vbuf, uint16_t vbuflen, const uint16_t* tlist, uint16_t tlistlen, const Matrix4x4& projMat)
{
    if(((vbuflen*(sizeof(Point_t) + sizeof(fp_type))) > wspclen) || (tlistlen % 3))
    {
        return false;
    }

    int32_t hsw = screen.getWidth()/2;
    int32_t hsh = screen.getHeight()/2;

    Point_t* screen_coords = (Point_t*)(workspace);
    fp_type* depths = (fp_type*)(((int)workspace) + (vbuflen * sizeof(Point_t)));

    Vector3d temp;

    for(uint32_t i = 0; i < vbuflen; i++)
    {
        glProject(projMat, vbuf[i], temp, depths[i]);

        screen_coords[i].x = hsw + (temp.x * hsw);
        screen_coords[i].y = hsh + (temp.y * hsh);
    }

    for(uint32_t i = 0; i < tlistlen; i += 3)
    {
        screen.line(&screen_coords[tlist[i]], &screen_coords[tlist[i+1]], WHITE);
        screen.line(&screen_coords[tlist[i+1]], &screen_coords[tlist[i+2]], WHITE);
        screen.line(&screen_coords[tlist[i+2]], &screen_coords[tlist[i]], WHITE);
    }
}

bool Display3D::drawVertexBufferI(void* workspace, const uint16_t wspclen, const int16_t* vbuf, uint16_t vbuflen, const uint16_t* tlist, uint16_t tlistlen, const Matrix4x4& projMat)
{
    if(((vbuflen*(sizeof(Point_t) + sizeof(fp_type))) > wspclen) || (tlistlen % 3))
    {
        return false;
    }

    int32_t hsw = screen.getWidth()/2;
    int32_t hsh = screen.getHeight()/2;

    Point_t* screen_coords = (Point_t*)(workspace);
    fp_type* depths = (fp_type*)(((int)workspace) + (vbuflen * sizeof(Point_t)));

    Vector3d temp, tempin;

    for(uint32_t i = 0; i < vbuflen; i++)
    {
        tempin = Vector3d(vbuf[i*3], vbuf[i*3+1], vbuf[i*3+2]);
        glProject(projMat, tempin, temp, depths[i]);

        screen_coords[i].x = hsw + (temp.x * hsw);
        screen_coords[i].y = hsh + (temp.y * hsh);
    }

    for(uint32_t i = 0; i < tlistlen; i += 3)
    {
        screen.line(&screen_coords[tlist[i]], &screen_coords[tlist[i+1]], WHITE);
        screen.line(&screen_coords[tlist[i+1]], &screen_coords[tlist[i+2]], WHITE);
        screen.line(&screen_coords[tlist[i+2]], &screen_coords[tlist[i]], WHITE);
    }
}
