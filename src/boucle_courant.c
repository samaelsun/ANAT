#include "boucle_courant.h"
#include "main.h"

float current_read;
float current_calibration_offset = 0.0;
float current_ref = 0;
pi_float_t pi_boucle_courant;
unsigned char current_loop_enable = 0;

void boucle_courant_init(void)
    {
    pi_boucle_courant.T = 0.001;
    pi_boucle_courant.kp = 170;
    pi_boucle_courant.ki = 217000;
    pi_boucle_courant.sat_pos = 510;
    pi_boucle_courant.sat_neg = -510;
	pi_boucle_courant.sat_pos_int = 0.1;
	pi_boucle_courant.sat_neg_int = -0.1;
    pi_boucle_courant.integrale = 0;

	//pi_boucle_courant.kp = 100.0;
	//pi_boucle_courant.ki = 100.0;
	//pi_boucle_courant.sat_pos_int = 510;
	//pi_boucle_courant.sat_neg_int = -510;

    
    }


void process_boucle_courant(void)
    {
    static unsigned int i;
	
	float temp;
    pi_boucle_courant.mesure = current_read;
    pi_boucle_courant.reference = current_ref;
    
	/*
	i++;
	if(i>=1000)
		{
		printf("Current rx : [%i] ; Current res [%i]\r\n",(int16_t)(current_read*1000.0),(int16_t)(current_ref*1000.0));
		i=0;		
		}
	*/
	
	
	if(current_loop_enable)
		{
		do_pi_float(&pi_boucle_courant);
    
		temp = (pi_boucle_courant.commande);

		if(temp > 510.0) temp = 510.0;
		if(temp < -510.0) temp = -510.0;
    
		pwm_set((signed int)temp);
		// debug with manual pwm input from current ref
		//pwm_set((signed int)((float)(current_ref*500.0)));
		}
	else
		{
		pwm_set(0);
		}
    }
