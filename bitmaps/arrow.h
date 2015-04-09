/*
 * arrow.h
 *
 *  Created on: Jan 16, 2015
 *      Author: Kevin
 */

#ifndef ARROW_H_
#define ARROW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ARROW_UP (arrow[0])
#define ARROW_RIGHT (arrow[1])
#define ARROW_DOWN (arrow[2])
#define ARROW_LEFT (arrow[3])

extern const uint8_t arrow[4][8];

#ifdef __cplusplus
}
#endif

#endif /* ARROW_H_ */
