/******************************************************************************/
/*Programmeur : Mathieu Cossette                                              */
/*Fichier     : Protocole.c                                                   */
/*                                                                            */
/*Fonction #1 : void protocol_init(void)                                      */
/*Description : Sert a l'initialisation des variables                         */
/*                                                                            */
/*Fonction #2 : inline void protocol_timer_ms (void)                          */
/*Description : Sert a decrementer les variable timer                         */
/*                                                                            */
/*Fonctipon#3 : inline void process_protocol_alloc_id(void)                   */
/*Description : Sert pour l'attribution d'adresse dynamique aux               */
/*              aux differents modules du robot ANAT.                         */
/*                                                                            */
/*Version     : 1                                                             */
/*Date        : 21/06/2007                                                    */
/******************************************************************************/

#include <avr/io.h>
#include "protocol.h"
#include "main.h"

volatile unsigned int protocol_start_delay_ms;
volatile unsigned char protocol_transmit_delay_ms;
volatile unsigned int protocol_allocation_timeout_ms;

volatile unsigned char prot_tx_timeout_ms;
volatile unsigned char protocol_id;
volatile unsigned char protocol_last_id;
volatile unsigned char protocol_rx_char_timemout_ms;
volatile unsigned char protocol_tx_char_timemout_ms;
volatile unsigned char prot_tx_complete = 0;
volatile unsigned char protocol_reply_data[PROTOCOL_LONG_TX_SIZE];
volatile unsigned char prot_reply_flag = 0;

volatile unsigned char protocol_long_version = 0;

volatile signed char current_ref_rx = 0;
volatile unsigned char xsum_rx;

volatile unsigned char protocol_tx_size_var = 0;

/******************************************************************************/
/******************************************************************************/
//Fonction d'intialisation du protocole
void protocol_init(void)
    {
    protocol_id = '1';
    }
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
//Fonction timer
void protocol_timer_ms(void)
    {
    if (protocol_start_delay_ms)
	protocol_start_delay_ms--;
    if (protocol_transmit_delay_ms)
	protocol_transmit_delay_ms--;
    if (protocol_allocation_timeout_ms)
	protocol_allocation_timeout_ms--;

    //Si protocol_rx_char_timemout_ms retourne une valeur, on le décrémente.
    if (protocol_rx_char_timemout_ms)
	protocol_rx_char_timemout_ms--;
    //Si protocol_tx_char_timemout_ms retourne une valeur, on le décrémente.
    if (protocol_tx_char_timemout_ms)
	protocol_tx_char_timemout_ms--;
    }
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
//Attribution des adresses dynamiques aux cartes
unsigned char process_protocol_alloc_id(void)
    {
    unsigned char rx_char;
    static unsigned char state_tx = 0;
    static unsigned char state_rx = 0;
    static unsigned char last_flag;
    //static unsigned char enable_in_latch;
    static unsigned char get_last_address;

    switch (state_tx)
	{
    case 0:
	ENABLE_OUT_INIT;
	ENABLE_OUT_CLEAR;

	DDRA &= ~(1 << PA2);
	PORTA |= (1 << PA2);

	protocol_start_delay_ms = 100;
	last_flag = 0;
	get_last_address = 0;
	protocol_id = '1';
	protocol_last_id = '1';
	state_tx = 1;
	//rs485_putchar('A');
	break;
    case 1:
	//rs485_putchar('B');
	if (!protocol_start_delay_ms)
	    {
	    //rs485_putchar('C');
	    if (ENABLE_IN_READ)
		{
		//rs485_putchar('D');
		rs485_putchar(protocol_id);
		protocol_transmit_delay_ms = 50;
		state_tx = 2;
		}
	    }
	break;
    case 2:
	//rs485_putchar('E');
	if (!protocol_transmit_delay_ms)
	    {
	    //rs485_putchar('F');
	    ENABLE_OUT_SET;
	    last_flag = 1;
	    state_tx = 100;
	    }
	break;
    case 100:
	// Do nothing
	break;
	}

    switch (state_rx)
	{
    case 0:
	protocol_allocation_timeout_ms = 1000;
	state_rx = 1;
	break;
    case 1:
	if (!protocol_allocation_timeout_ms)
	    {
	    if (last_flag)
		{
		protocol_last_id = protocol_id;

		rs485_putchar('@');
		rs485_putchar(protocol_last_id);
		state_rx = 100;
		}
	    else
		{
		state_rx = 2;
		}

	    }
	else
	    {
	    state_rx = 2;
	    }
	break;
    case 2:
	if (rs485_char_ready())
	    {
	    rx_char = rs485_getchar();

	    if (get_last_address)
		{
		protocol_last_id = rx_char;
		state_rx = 100;
		}
	    else
		{
		if (rx_char == '@')
		    {
		    get_last_address = 1;
		    }
		else
		    {
		    last_flag = 0;
		    if ((ENABLE_IN_READ) == 0)
			{
			protocol_id = rx_char + 1;
			}
		    state_rx = 1;
		    }
		}
	    }
	else
	    {
	    state_rx = 1;
	    }
	break;

    case 100:
	return 1;
	break;
	}
    return 0;
    }

/******************************************************************************/
/******************************************************************************/
//Processus de réception
void process_protocol_rx(void)
    {
    static unsigned char state = 0;
    static unsigned char rx_char_count = 0;
    //static signed char pwm_value = 0;
    unsigned char data;

    //Sur la variable d'état
    switch (state)
	{
    //Cas de l'attente du caractère de départ
    case PROT_RX_WAIT_FOR_START_CHAR:
	{
	//Si il ya des données dans le buffer
	if (rs485_char_ready())
	    {
	    data = rs485_getchar();
	    //Si le caractère reçu est le caractère de départ
	    if (data == PROT_RX_START_LONG_VERISON)
		{
		//On initialise le "timer" de réception
		protocol_rx_char_timemout_ms = PROT_RX_CHAR_TIMEOUT_MS;
		//On met le compteur de caractère à 0
		rx_char_count = 0;
		//on met la variable d'état sur le processus de réception
		protocol_long_version = 1;
		protocol_tx_size_var = PROTOCOL_LONG_TX_SIZE;
		state = PROT_RX_PROCESS;
		}
	    else if (data == PROT_RX_START_SHORT_VERSION)
		{
		//On initialise le "timer" de réception
		protocol_rx_char_timemout_ms = PROT_RX_CHAR_TIMEOUT_MS;
		//On met le compteur de caractère à 0
		rx_char_count = 0;
		protocol_long_version = 0;
		protocol_tx_size_var = PROTOCOL_SHORT_TX_SIZE;
		state = PROT_RX_PROCESS;
		}
	    else if (data == PROT_RX_START_RESET_ENCODER)
		{
		//On initialise le "timer" de réception
		protocol_rx_char_timemout_ms = PROT_RX_CHAR_TIMEOUT_MS;
		//On met le compteur de caractère à 0
		rx_char_count = 0;
		protocol_long_version = 0;
		protocol_tx_size_var = PROTOCOL_SHORT_TX_SIZE;
		state = PROT_RX_PROCESS;
		reset_encoder();
		}
	    }
	}
	break;

	//Cas du processus de réception
    case PROT_RX_PROCESS:
	{
	//Si le "timer" de réception est à 0
	if (protocol_rx_char_timemout_ms == 0)
	    {
	    //On met la variable d'état à la fin de la réception
	    state = PROT_RX_END;
	    //On met le "timer" de réception = au temps d'une transmission*la taille d'une transmission*l'identificateur du nombre de module+1
	    protocol_rx_char_timemout_ms = (PROT_RX_CHAR_TIMEOUT_MS * PROTOCOL_RX_SIZE
		    * ((protocol_last_id - 48) + 1));
	    }
	//Sinon si une donnée est prête à être reçu
	else if (rs485_char_ready())
	    {
	    //Si le compteur de caractère / la dimension d'une réponse = l'identificateur de la carte
	    if (rx_char_count == ((protocol_id - '1') * PROTOCOL_RX_SIZE))
		{
		//On met les donnée reçu en mémoire
		current_ref_rx = rs485_getchar();
		}
	    else if (rx_char_count == (((protocol_id - '1') * PROTOCOL_RX_SIZE) + 1))
		{
		//On met les donnée reçu en mémoire
		xsum_rx = rs485_getchar();
		}
	    else
		{
		// On effectue la prise du caractère sans la mettre en mémoire
		rs485_getchar();
		}
	    //On incrémente le compteur de caractère reçu
	    rx_char_count++;
	    }
	//Si le compteur de caractère reçu = au nombre de module * 
	if (rx_char_count == (((protocol_last_id - '0')) * PROTOCOL_RX_SIZE))
	    {
	    //On met le flag de réponse à 1
	    prot_reply_flag = 1;
	    //On met la variable d'état à la fin de la réception
	    state = PROT_RX_END;
	    //On met le "timer" de réception = au temps d'une transmission*la taille d'une transmission*l'identificateur du nombre de module
	    protocol_rx_char_timemout_ms = (PROT_RX_CHAR_TIMEOUT_MS * protocol_tx_size_var
		    * ((protocol_last_id - 48) + 1));

	    // Comme current_ref_rx peut aller de -127 a +126, diviser par 100 donne des amperes
	    // Donc un courant de -1.27A a +1.26A

	    if ((unsigned char) current_ref_rx == xsum_rx)
		{
		current_ref = (float) current_ref_rx / 100.0;
		}
	    }
	}
	break;

	//Cas de la fin de la réception
    case PROT_RX_END:
	{
	//Si la variable de transmission complète =1 ou si le "timer" de réception =0
	if (prot_tx_complete || (protocol_rx_char_timemout_ms == 0))
	    {
	    //On met la variable de transmission complèté à 1
	    prot_tx_complete = 0;
	    //On met la variable d'état sur l'attente du caractère de début de transmission
	    state = PROT_RX_WAIT_FOR_START_CHAR;
	    }
	}
	break;
	}
    }
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
//Processus qui gère le protocole lié à la transmission
void process_protocol_tx(void)
    {
    //Static = Le spécificateur de classe de stockage static, 
    //utilisé avec une variable locale, permet de préserver la dernière valeur 
    //entre les appels successifs à cette fonction.
    //Une variable statique agit comme une variable locale mais a une durée 
    //de vie d'une variable externe.
    static unsigned char state = 0;
    static unsigned char reply_count;
    static signed long encoder_data;
    unsigned char i;
    signed int current_tx;
    //sur la variable d'état
    switch (state)
	{

    //Cas où l'on attend une réponse du flag de transmission
    case PROT_TX_WAIT_FOR_REPLY_FLAG:
	{
	//On disable le flag
	if (prot_reply_flag)
	    {
	    prot_reply_flag = 0;
	    //on met la variable d'état à l'initialisation de transmission
	    state = PROT_TX_INIT_TRANSMISSION;
	    }

	}
	break;

	//Cas de l'initialisation de la transmission 
    case PROT_TX_INIT_TRANSMISSION:
	{
	//remet le compteur de réponse à 0
	reply_count = 0;
	//Initialise le "timer" de transmission
	protocol_tx_char_timemout_ms = PROT_TX_CHAR_TIMEOUT_MS;
	//Met la variable d'état à l'état de vérification des condition entourant la transmission
	state = PROT_TX_PROCESS_CONDTIONS;
	}
	break;

	//Cas de vérification des condition entourant la transmission
    case PROT_TX_PROCESS_CONDTIONS:
	{

	//Si le "timer de transmission est à 0
	if (protocol_tx_char_timemout_ms == 0)
	    {
	    //Place la variable sur fin de la transmission
	    state = PROT_TX_END;
	    }
	// si le compteur de réponse = l'identificateur * la taille d'une réponse
	else if (reply_count == ((protocol_id - '0' - 1) * protocol_tx_size_var))
	    {
	    //On met l'état sur transmission
	    state = PROT_TX_TRANSMIT;
	    }
	//Si une donnée est prète à être reçu
	else if (rs485_char_ready())
	    {
	    //On initialise le "timer" de transmission
	    protocol_tx_char_timemout_ms = PROT_TX_CHAR_TIMEOUT_MS;
	    //On prend la donnée sur le port 485
	    rs485_getchar();
	    //Le compteur de réponse est incrémenté
	    reply_count++;
	    }
	}
	break;

	//Cas de transmission d'une donnée
    case PROT_TX_TRANSMIT:
	{
	if (protocol_long_version)
	    {
	    // limit switch
	    protocol_reply_data[0] = 0x00;

	    if (LSTA_READ)
		protocol_reply_data[0] |= 0x01;
	    if (RSTA_READ)
		protocol_reply_data[0] |= 0x02;

	    // encoder
	    encoder_data = get_encoder();
	    protocol_reply_data[1] = ((encoder_data) & 0xFF);
	    protocol_reply_data[2] = ((encoder_data >> 8) & 0xFF);
	    // current
	    current_tx = (signed int) (current_read * 10000);

	    protocol_reply_data[3] = ((current_tx) & 0xFF);
	    protocol_reply_data[4] = ((current_tx >> 8) & 0xFF);
	    // xsum
	    protocol_reply_data[PROTOCOL_LONG_TX_SIZE - 1] = 0;
	    for (i = 0; i < PROTOCOL_LONG_TX_SIZE - 1; i++)
		{
		//On calcul le xsum
		protocol_reply_data[PROTOCOL_LONG_TX_SIZE - 1] += protocol_reply_data[i];
		}
	    //De 0 à la taille de la réponse
	    for (i = 0; i < PROTOCOL_LONG_TX_SIZE; i++)
		{
		//On envois les "caractères" de réponse
		rs485_putchar(protocol_reply_data[i]);
		}

	    }
	else
	    {

	    // encoder
	    encoder_data = get_encoder();
	    protocol_reply_data[0] = ((encoder_data) & 0xFF);
	    protocol_reply_data[1] = ((encoder_data >> 8) & 0xFF);

	    // xsum
	    protocol_reply_data[PROTOCOL_SHORT_TX_SIZE - 1] = 0;
	    for (i = 0; i < PROTOCOL_SHORT_TX_SIZE - 1; i++)
		{
		//On calcul le xsum
		protocol_reply_data[PROTOCOL_SHORT_TX_SIZE - 1] += protocol_reply_data[i];
		}

	    //De 0 à la taille de la réponse
	    for (i = 0; i < PROTOCOL_SHORT_TX_SIZE; i++)
		{
		//On envois les "caractères" de réponse
		rs485_putchar(protocol_reply_data[i]);
		}
	    }

	//On incrémente le compteur de réponse
	reply_count += PROTOCOL_SHORT_TX_SIZE;
	//On place la variable d'état sur l'atttente de fin de transmission
	state = PROT_TX_WAIT_END_OF_ALL_TRANSMIT;
	}
	break;

	//Cas de l'attente de fin de transmission
    case PROT_TX_WAIT_END_OF_ALL_TRANSMIT:
	{

	//Si le "timer" de transmission est à 0 on met la variable d'état à fin de transmission
	if (protocol_tx_char_timemout_ms == 0)
	    {
	    state = PROT_TX_END;
	    }
	//Si le compteur de réponse = l'identificateur du nombre de module + 1 * taille d'une transmission
	else if (reply_count == (((protocol_last_id - '0') + 1) * protocol_tx_size_var))
	    {
	    //on met la variable d'état à fin de transmission
	    state = PROT_TX_END;
	    }
	//Si une donnée est prête à être reçu
	else if (rs485_char_ready())
	    {
	    //on prend une donnée sur le port 485
	    rs485_getchar();
	    //On incrémente le compteur de réponse
	    reply_count++;
	    }

	}
	break;

	//Cas de la fin d'une transmission
    case PROT_TX_END:
	{
	//Si le buffer de transmission est vide
	if (tx_buffer_empty())
	    {
	    //On met la variable d'état sur l'attente d'une requète de réponse
	    state = PROT_TX_WAIT_FOR_REPLY_FLAG;
	    //On met la variable de transmission complèté à 1
	    prot_tx_complete = 1;
	    }
	}
	break;
	}
    }
/******************************************************************************/
/******************************************************************************/

///******************************************************************************/
///******************************************************************************/
////Fonction de reception des commande PWM
//inline void process_protocol_rx(void)
//{
////Variable de la réception
//static unsigned char rx_state = 0;
////Valeur reçu pour le pwm
//static unsigned char pwm_value = 0;
////Nombre de caractere recu
//static unsigned char rx_count=0;
////ENABLE_OUT_CLEAR;
//
//while(rx_state != PROT_RX_END)
//{
//switch(rx_state)
//	{
//	/**********************************************************************/
//	//Initialisation du timer de réception
//	case PROT_RX_INIT:
//		{
//		//Initialisation du timer de la séquence de réception
//		prot_timeout_ms = PROT_RX_TIMEOUT_MS;
//		rx_state = PROT_RX_WAIT_FOR_START;
//		} break;
//	/**********************************************************************/
//	
//	/**********************************************************************/
//	//Attente du caractère de début de transmission
//	case PROT_RX_WAIT_FOR_START:
//		{
//		//Si le timer à terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
////			rs485_putchar(PROT_ERROR_ID);
//			rx_state=PROT_RX_END;
//			}
//		//Si le timer n'a pas fini son décompte
//		else
//			{
//			//S'il y a eu un caractère reçu
//			if(rs485_char_ready())
//				{
//				//Et, si ce caractère est le caractères de départ de la 
//				//transmission
//				if(rs485_getchar() == PROT_RX_START_RX)
//					{
//					rx_state = PROT_RX_GET_ID;
//					}
//				}
//			}
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	//Attent de réception de notre adresse
//	case PROT_RX_GET_ID:
//		{
//		//Si le timer à terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
////			rs485_putchar(PROT_ERROR_ID);
//			rx_state=PROT_RX_END;
//			}
//		//Si le timer n'a pas fini son décompte
//		else
//			{
//			//Si un caractère est reçu
//			if(rs485_char_ready())
//				{
//				//Si ce caractère est notre adresse
//				if(rs485_getchar() == protocol_id)
//					{
//					rx_state = PROT_RX_GET_PWM;
//					}
//				}
//			}
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	//Acquisition de la valeur du PWM
//	case PROT_RX_GET_PWM:
//		{
//		//Si le timer a terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
////			rs485_putchar(PROT_ERROR_ID);
//			rx_state = PROT_RX_END;
//			}
//		//S'il y a un caractère reçu
//		else
//			{
//			//Si un caractère est reçu
//			if(rs485_char_ready())
//				{
////Programme de test//////////////////////////
//				if(rs485_getchar()=='!')
//					{
//					//On place le caractère dans pwm_value
//					pwm_value = 0;//rs485_getchar();
//					rx_state = PROT_RX_PWM_SET;
//					}
//				else
//					{
//					//On place le caractère dans pwm_value
//					pwm_value = 68;//rs485_getchar();
//					rx_state = PROT_RX_PWM_SET;
//					}
//				}
//			}
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	case PROT_RX_PWM_SET:
//		{
//		//Si le timer a terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
////			rs485_putchar(PROT_ERROR_ID);
//			rx_state=PROT_RX_END;
//			}
//		//Si le timer n'a pas terminer son décompte
//		else
//			{
//			//Si un caractère est reçu
//			if(rs485_char_ready())
//				{
//				//Si le caractère correspond 
//				//au caractère de fin de réception
//				if(rs485_getchar()==PROT_RX_END_ID)
//					{
//					//On applique le pwm
//					pwm_set(pwm_value);
//					rx_state = PROT_RX_END;
//					}
//				//Si le caractère ne correspond pas
//				//au caractère de fin de réception
//				else
//					{
//					rx_state = PROT_RX_PWM_SET;
//					}
//				}
//
//			}
//		} break;
//	/**********************************************************************/
//	}
//}
//}
///******************************************************************************/
///******************************************************************************/
///******************************************************************************/
///******************************************************************************/
////Fonction qui commande la transmission de la donnée des encodeurs
//inline void process_protocol_tx(void)	
//{
////Variable de la réception
//static unsigned char tx_state = 0;
//
//tx_state=0;
////Rester dans la machine d'etat tant que la fin 
////de la transmission nèest pas atteinte
//while(tx_state != PROT_TX_END)
//{
//switch(tx_state)
//	{
//	/**********************************************************************/
//	case PROT_TX_INIT:
//		{
//		//Initialisation du timer de transmission
//		prot_timeout_ms=PROT_TX_TIMEOUT_MS;
//		tx_state = PROT_TX_WAIT_FOR_START;
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	case PROT_TX_WAIT_FOR_START:
//		{
//		//Si le timer a terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
//			rs485_putchar(PROT_ERROR_ID);
//			tx_state=PROT_TX_END;
//			}
//		//Si le timer n'a pas terminer son décompte
//		else
//			{
//			//Si un caractère est reçu
//			if (rs485_char_ready())
//				{
//				//Si le caractere est le caractere de demarrage de transmission
//				if(rs485_getchar()=='T')
//					{
//					tx_state=PROT_TX_CHECK_FIRST_ID;
//					}
//				}
//			}
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	case PROT_TX_CHECK_FIRST_ID:
//		{
//		//Si le timer à terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
//			//on envoi le caractere d'erreur
//			rs485_putchar(PROT_ERROR_ID);
//			tx_state=PROT_TX_END;
//			}
//		else
//			{
//			//Si l'adresse de la carte correspond a la premiere carte
//			if(protocol_id=='1')
//				{
//				tx_state=PROT_TX_SEND_ID;
//				}
//			//Si l'adresse de la carte ne correspond pas a la premiere carte
//			else
//				{
//				tx_state=PROT_TX_WAIT_PREV_ID;
//				}
//			}
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	case PROT_TX_WAIT_PREV_ID:
//		{
//		//Si le timer à terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
//			//on envoi le caractere d'erreur
//			rs485_putchar(PROT_ERROR_ID);
//			tx_state=PROT_TX_END;
//			}
//		//Si le timer n'a pas fini son décompte
//		else
//			{
//			//Si un caractère est reçu
//			if (rs485_char_ready())
//				{
//				//Si le caractere l'adresse de la carte precedente
//				if(rs485_getchar()==protocol_id-1)
//					{
//					tx_state=PROT_TX_GET_PREV_ENCOD;
//					}
//				}
//			}
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	case PROT_TX_GET_PREV_ENCOD:
//		{
//		//Si le timer à terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
//			//on envoi le caractere d'erreur
//			rs485_putchar(PROT_ERROR_ID);
//			tx_state=PROT_TX_END;
//			}
//		//Si le timer n'a pas fini son décompte
//		else
//			{
//			//Si un caractère est reçu
//			if (rs485_char_ready())
//				{
//				rs485_getchar();
//				tx_state=PROT_TX_SEND_ID;
//				}
//			}
//		} break;
//	/**********************************************************************/
//
//	/**********************************************************************/
//	case PROT_TX_SEND_ID:
//		{
//		//Si le timer à terminer son décompte
//		if(prot_timeout_ms<=0)
//			{
//			//on envoi le caractere d'erreur
//			rs485_putchar(PROT_ERROR_ID);
//			tx_state=PROT_TX_END;
//			}
//		else
//			{	
//			rs485_putchar(protocol_id);
//			tx_state=PROT_TX_SEND_ENCODER;
//			}
//		} break;
//	/**********************************************************************/
//	
//	/**********************************************************************/
//	case PROT_TX_SEND_ENCODER:
//		{
//			rs485_putchar(get_encoder());
//			tx_state=PROT_TX_VERIF_LAST_ID;
//		} break;
//	/**********************************************************************/
//		
//	/**********************************************************************/
//	case PROT_TX_VERIF_LAST_ID:
//		{
//		//Si l'adresse de la carte est la derniere adresse
//		if(protocol_id==protocol_last_id)
//			{
//			rs485_putchar(PROT_TX_END_ID);				
//			tx_state=PROT_TX_END;	
//			}
//		else
//			{
//			tx_state=PROT_TX_END;
//			}
//		} break;
//	/**********************************************************************/
//	}
//}
//}
