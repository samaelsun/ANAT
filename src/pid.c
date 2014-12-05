#include "pid.h"
#include "main.h"

void do_pi_float(pi_float_t* pi)
    {
    static float erreur;
    static float commande;

    erreur = (pi->reference - pi->mesure);

    // Integrale par trapèze
    pi->integrale += (((erreur + pi->last_erreur)/2)*pi->T);
    
    pi->last_erreur = erreur;

    commande = erreur * pi->kp + pi->integrale * pi->ki;

    // Saturation de la sortie
    if (commande > pi->sat_pos)
	pi->commande = pi->sat_pos;
    else
	if (commande < pi->sat_neg)
	    pi->commande = pi->sat_neg;
	else
	    pi->commande = commande;

    if (pi->integrale > pi->sat_pos_int)
	pi->integrale = pi->sat_pos_int;
    if (pi->integrale < pi->sat_neg_int)
	pi->integrale = pi->sat_neg_int;

    }

