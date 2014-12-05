#ifndef _FILTRE_H_
#define _FILTRE_H_

typedef struct _filtre_2e_ordre_t
{
	float in;

	float out;
	float out_dot;
	float out_dot_dot;

	float T;

	float w0;
	float xsi;
	
	float w0_carre;
	float deux_xsi_w;
} filtre_2e_ordre_t;

void filtre_2e_ordre_init(filtre_2e_ordre_t* filtre);
void filtre_2e_ordre_process(filtre_2e_ordre_t* filtre);

#endif //_FILTRE_H_
