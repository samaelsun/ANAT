#ifndef BOUCLE_COURANT_H_
#define BOUCLE_COURANT_H_

#include "main.h"

extern float current_read;
extern float current_calibration_offset;
extern float current_ref;
extern pi_float_t pi_boucle_courant;
extern unsigned char current_loop_enable;

void boucle_courant_init(void);
void process_boucle_courant(void);

#endif /*BOUCLE_COURANT_H_*/
