#include "main.h"
#include "filtre.h"

void filtre_2e_ordre_init(filtre_2e_ordre_t* filtre)
{
filtre->w0_carre = filtre->w0 * filtre->w0;
filtre->deux_xsi_w = filtre->w0 * 2.0 * filtre->xsi;

filtre->out = 0;
filtre->out_dot = 0;
filtre->out_dot_dot = 0;
}

void filtre_2e_ordre_process(filtre_2e_ordre_t* filtre)
{
filtre->out_dot_dot = ((filtre->in - filtre->out) * filtre->w0_carre) - filtre->out_dot * filtre->deux_xsi_w;

filtre->out_dot += filtre->out_dot_dot * filtre->T;

filtre->out += filtre->out_dot * filtre->T;
}

