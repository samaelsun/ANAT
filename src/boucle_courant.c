#include "boucle_courant.h"
#include "main.h"

float current_read;
float current_ref = 0;
pi_float_t pi_boucle_courant;


void boucle_courant_init(void)
    {
    pi_boucle_courant.T = 0.001;
    pi_boucle_courant.kp = 170;
    pi_boucle_courant.ki = 217000;
    pi_boucle_courant.sat_pos = 510;
    pi_boucle_courant.sat_neg = -510;
    pi_boucle_courant.sat_pos_int = .01;
    pi_boucle_courant.sat_neg_int = -.01;
    pi_boucle_courant.integrale = 0;
    
    }


void process_boucle_courant(void)
    {
    float temp;
    pi_boucle_courant.mesure = current_read;
    pi_boucle_courant.reference = current_ref;
    
    do_pi_float(&pi_boucle_courant);
    
    temp = (pi_boucle_courant.commande);

    if(temp > 510.0) temp = 510.0;
    if(temp < -510.0) temp = -510.0;
    
    pwm_set((signed int)temp);
    }
