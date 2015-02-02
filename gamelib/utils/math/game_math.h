#ifndef _GAME_MATH_H_
#define _GAME_MATH_H_

namespace GameMath
{

template <class T>
void clamp(T& val, T minval, T maxval)
{
    if(val < minval)
        val = minval;
    if(val > maxval)
        val = maxval;
}

}

#endif // _GAME_MATH_H_

