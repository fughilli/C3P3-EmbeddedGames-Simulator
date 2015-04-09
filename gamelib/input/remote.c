/*
 * remote.c
 *
 *  Created on: Jan 22, 2015
 *      Author: Kevin
 */

#include "remote.h"
#include <string.h>
//#include "../../gamelib/gamelib.h"

#define REMOTE_PORT_A_BITMASK 0b00011100
#define REMOTE_PORT_B_BITMASK 0b11000100

void Remote_init() {
    ;
}

void Remote_read(Remote_buttons_t* rb) {
    if(rb)
        memcpy(rb, &sim_hooked_buttons, sizeof(Remote_buttons_t));
}
