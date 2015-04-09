/*
 * watchmen_bw.h
 *
 *  Created on: Jan 4, 2015
 *      Author: Kevin
 */

#ifndef WATCHMEN_BW_H_
#define WATCHMEN_BW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CONST_BITMAP

#ifdef CONST_BITMAP

extern const uint8_t watchmen_bitmap[];

#else

extern uint8_t watchmen_bitmap[];

#endif

#ifdef __cplusplus
}
#endif

#endif /* WATCHMEN_BW_H_ */
