/*
 * remote.h
 *
 *  Created on: Jan 22, 2015
 *      Author: Kevin
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

typedef union {
	struct {
		uint8_t b :1;
		uint8_t left :1;
		uint8_t up :1;
		uint8_t right :1;
		uint8_t down :1;
		uint8_t a :1;
		uint8_t :2;
	} buttons;

	uint8_t raw;
} Remote_buttons_t;

extern Remote_buttons_t sim_hooked_buttons;

void Remote_init();
void Remote_read(Remote_buttons_t* rb);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* REMOTE_H_ */
