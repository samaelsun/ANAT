#ifndef PID_H_
#define PID_H_

#include <inttypes.h>

typedef struct _pi_float_t
{
	float mesure;
	float reference;
	float commande;
	
	// Trim Output
	float sat_pos;
	float sat_neg;
	
	// Trim Integral
	float sat_pos_int;
	float sat_neg_int;

	// Variable mémoire
	float integrale;
	float last_erreur;

	// Période d'échantillonage
	float T;
	
	// Constantes PI
	float kp;
	float ki;
} pi_float_t;

void do_pi_float(pi_float_t* pid);

#endif /*PID_H_*/
