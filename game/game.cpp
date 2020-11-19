#include "game.h"

#include "../gamelib/utils/VectorLib/Matrix.h"
#include "../gamelib/utils/VectorLib/Vector.h"
#include "../gamelib/display/disp3d.h"

Matrix4x4 projMat;
Vector3d posvec;
Quaternion rotquat;

// Mesh data
// Every set of 3 integers is a vertex

// The first 3 integers in the mesh list is vertex 0
// The next 3 integers are vertex 1, and so on...
const int16_t mesh[] = { 0, -4, 14, 0, -14, 14, -9, -4, 9, -9, -14, 9, -14, -4,
                         0, -14, -14, 0, -9, -4, -9, -9, -14, -9, 0, -4, -14, 0, -14, -14, 9, -4,
                         -9, 9, -14, -9, 14, -4, 0, 14, -14, 0, 9, -4, 9, 9, -14, 9, 0, -31, 7,
                         -5, -31, 5, -7, -31, 0, -5, -31, -5, 0, -31, -7, 5, -31, -5, 7, -31, 0,
                         5, -31, 5, 0, -42, 3, -2, -42, 2, -3, -42, 0, -2, -42, -2, 0, -42, -3,
                         2, -42, -2, 3, -42, 0, 2, -42, 2, 0, -48, 4, -3, -48, 3, -4, -48, 0, -3,
                         -48, -3, 0, -48, -4, 3, -48, -3, 4, -48, 0, 3, -48, 3, 0, -51, 10, -7,
                         -51, 7, -10, -51, 0, -7, -51, -7, 0, -51, -10, 7, -51, -7, 10, -51, 0,
                         7, -51, 7, 0, 0, 9, -6, 0, 6, -9, 0, 0, -6, 0, -6, 0, 0, -9, 6, 0, -6,
                         9, 0, 0, 6, 0, 6,
                       };

// Triangle list
// Maps triangles into the mesh list

// The first 3 numbers here specify that vertices 1,3,2 are connected as a triangle
// Then, vertices 3,5,4 and so on...
const uint16_t tlist[] = { 1, 3, 2, 3, 5, 4, 5, 7, 6, 7, 9, 8, 9, 11, 10, 11,
                           13, 12, 15, 23, 16, 15, 1, 0, 13, 15, 14, 0, 48, 55, 16, 23, 31, 15, 13,
                           22, 11, 21, 22, 11, 9, 20, 7, 19, 20, 5, 18, 19, 3, 17, 18, 1, 16, 17,
                           31, 39, 32, 23, 22, 30, 21, 29, 30, 20, 28, 29, 20, 19, 27, 18, 26, 27,
                           17, 25, 26, 16, 24, 25, 39, 47, 40, 31, 30, 38, 29, 37, 38, 28, 36, 37,
                           27, 35, 36, 27, 26, 34, 25, 33, 34, 24, 32, 33, 41, 40, 46, 38, 46, 47,
                           38, 37, 45, 36, 44, 45, 35, 43, 44, 35, 34, 42, 33, 41, 42, 32, 40, 41,
                           48, 50, 52, 14, 55, 54, 12, 54, 53, 10, 53, 52, 8, 52, 51, 4, 6, 51, 4,
                           50, 49, 2, 49, 48, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 10, 11,
                           12, 1, 15, 16, 14, 15, 0, 12, 13, 14, 14, 0, 55, 24, 16, 31, 23, 15, 22,
                           13, 11, 22, 21, 11, 20, 9, 7, 20, 7, 5, 19, 5, 3, 18, 3, 1, 17, 24, 31,
                           32, 31, 23, 30, 22, 21, 30, 21, 20, 29, 28, 20, 27, 19, 18, 27, 18, 17,
                           26, 17, 16, 25, 32, 39, 40, 39, 31, 38, 30, 29, 38, 29, 28, 37, 28, 27,
                           36, 35, 27, 34, 26, 25, 34, 25, 24, 33, 43, 42, 41, 45, 44, 43, 41, 46,
                           45, 46, 40, 47, 45, 43, 41, 39, 38, 47, 46, 38, 45, 37, 36, 45, 36, 35,
                           44, 43, 35, 42, 34, 33, 42, 33, 32, 41, 54, 55, 48, 52, 53, 54, 50, 51,
                           52, 48, 49, 50, 52, 54, 48, 12, 14, 54, 10, 12, 53, 8, 10, 52, 6, 8, 51,
                           50, 4, 51, 2, 4, 49, 0, 2, 48,
                         };

uint8_t gworkspace[4096];

Remote_buttons_t rbs;

void Game_init()
{
    // Compute the aspect ratio of the screen
    fp_type aspect = screen.getWidth();
    aspect /= screen.getHeight();

    // Compute the projection matrix using glFrustum
    projMat = Display3D::glFrustum(-aspect, aspect, -1, 1, 5, 1000);

    // Add some pretransformations to the projection matrix
    // Scale by 0.2 and then translate by 50 units into the screen
    projMat = projMat * Matrix4x4(Vector3d::k * -60) * Matrix4x4(0.2);

    // Initialize the translation vector to <0,0,0> and the rotation
    // quaternion to identity (no rotation)
    posvec = Vector3d::zero;
    rotquat = Quaternion::identity;

    // Set up the display
    TVOut_EnableDoubleBuffer(true);
    TVOut_ClearBufferOnSwap(true);
}

void Game_loop()
{
    // Read the remote
    Remote_read(&rbs);

    if (rbs.buttons.left)
    {
        // If pressing left, right-multiply the
        // rotation quaternion by 0.05f radians around
        // the j axis (vertical)
        rotquat *= Vector3d::j.rotationAroundAxis(0.05f);
    }
    if (rbs.buttons.right)
    {
        // If pressing right, right-multiply the
        // rotation quaternion by -0.05f radians around
        // the j axis (vertical)
        rotquat *= Vector3d::j.rotationAroundAxis(-0.05f);
    }
    if (rbs.buttons.up)
    {
        // If pressing up, right-multiply the
        // rotation quaternion by 0.05f radians around
        // the i axis (horizontal)
        rotquat *= Vector3d::i.rotationAroundAxis(-0.05f);
    }
    if (rbs.buttons.down)
    {
        // If pressing down, right-multiply the
        // rotation quaternion by -0.05f radians around
        // the i axis (horizontal)
        rotquat *= Vector3d::i.rotationAroundAxis(0.05f);
    }
    if (rbs.buttons.a)
    {
        // If pressing a, add a displacement of
        // 1.0f along the k axis (out of the screen)
        // to the position vector
        posvec += Vector3d::k * 1.0f;
    }
    if (rbs.buttons.b)
    {
        // If pressing b, add a displacement of
        // -1.0f along the k axis (out of the screen)
        // to the position vector
        posvec += Vector3d::k * -1.0f;
    }

    // Normalize the rotation quaternion to prevent accumulating fp error
    rotquat = rotquat.unit();

    /* Draw the mesh. Arguments are:
     * drawVertexBufferI(
     *                   workspace,         <-- A buffer large enough to store
     *                                          intermediate geometry computations
     *                   workspace_size,    <-- the size of the workspace, in bytes
     *                   mesh,              <-- the vertex buffer (int16_t*)
     *                   mesh_size,         <-- the number of datapoints in the mesh
     *                                          buffer (has to be a multiple of 3,
     *                                          as there are 3 datapoints per vertex)
     *                   triangle_list,     <-- the triangle list (int16_t*)
     *                   triangle_list_size,<-- the number of indices in the triangle
     *                                          list (has to be a multiple of 3,
     *                                          as there are 3 vertex indices per
     *                                          triangle)
     *                   projection_matrix  <-- the projection matrix (Matrix4x4). Can
     *                  );                      contain right-multiplied transformations
     *                                          to apply before projection.
     */
    Display3D::drawVertexBufferI(gworkspace, 4096, mesh, 168, tlist, 324,
                                 // Matrix operations are performed right-to-left.
                                 // The first thing done here is to rotate the model,
                                 // then translate it, and finally project it.
                                 projMat * Matrix4x4(posvec) * Matrix4x4(rotquat));

    // Swap the double buffer and wait for the frame to draw to the screen.
    TVOut_Swap();
    TVOut_WaitFrames(1);
}

