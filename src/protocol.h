#ifndef PROTOCOL_H_
#define PROTOCOL_H_

//Declaration relative au Enable_Out
//#define ENABLE_OUT_DDR 		DDRA
//#define ENABLE_OUT_PORT 	PORTA
//#define ENABLE_OUT_PIN		(1<<PA6)
//#define ENABLE_OUT_INIT		ENABLE_OUT_DDR |= ENABLE_OUT_PIN
//#define ENABLE_OUT_SET		ENABLE_OUT_PORT |= ENABLE_OUT_PIN
//#define ENABLE_OUT_CLEAR	ENABLE_OUT_PORT &= (~ENABLE_OUT_PIN)

//Definition des differents temps d'attente
//lors de l'allocation d'adresse dynamique
#define PROT_INIT_TIMEOUT_MS 100
#define PROT_RX_ID_PLUS_2_TIMEOUT_MS 255
#define PROT_RX_CHAR_TIMEOUT	200 //Note pour Sylvain: Ce temps a ete change
									//pour les besoins de la cause
#define PROT_EN_OUT_SET_TIMEOUT_MS 10
#define PROT_SET_ID_TIMEOUT_MS 15


//Définition des paramêtres du protocole
#define PROT_TX_CHAR_TIMEOUT_MS	20
#define PROT_RX_CHAR_TIMEOUT_MS	20

#define PROT_RX_START_LONG_VERISON '='
#define PROT_RX_START_SHORT_VERSION '-'
#define PROT_RX_START_RESET_ENCODER 'r'

#define PROT_ADDR_END_ID '@'   //64 en ASCII
#define PROT_ERROR_ID '?'      //63 en ASCII

#define PROTOCOL_LONG_TX_SIZE 6
#define PROTOCOL_SHORT_TX_SIZE 3
#define PROTOCOL_RX_SIZE 2

//Définition des paramêtres lié à la transmission
#define PROT_TX_WAIT_FOR_REPLY_FLAG 0
#define PROT_TX_INIT_TRANSMISSION 10
#define PROT_TX_PROCESS_CONDTIONS 20
#define PROT_TX_TRANSMIT 30
#define PROT_TX_WAIT_END_OF_ALL_TRANSMIT 35
#define PROT_TX_END 40

//Définition des paramêtres lié à la réception
#define PROT_RX_WAIT_FOR_START_CHAR 0
#define PROT_RX_PROCESS 10
#define PROT_RX_END 20
#define PROT_RX_RESET_ENCODER 30

//Définition des variables reçu et transmise en réponse
//Ces paramêtres seront modifier en dehors de protocol.c et peut-être par le MCU
extern volatile unsigned char protocol_reply_data[PROTOCOL_LONG_TX_SIZE];
extern volatile unsigned char protocol_received_data[PROTOCOL_RX_SIZE];


//Definition des différents temps d'attente
//lors de la réception du PWM
//#define PROT_RX_TIMEOUT_MS	1000

//Definition des différents temps d'attente
//lors de la transmission de donnee des encodeurs
//#define PROT_TX_TIMEOUT_MS	2000


//Caracteres specifiques du protocole
//#define PROT_ADDR_END_ID '@'   //64 en ASCII
//#define PROT_ERROR_ID '?'      //63 en ASCII
//#define PROT_RX_START_RX '='
//#define PROT_RX_END_ID '*'     //42 en ASCII
//#define PROT_TX_END_ID '*'

//Definition de la valeur attribue aux differents etats
//de l'allocation d'adresse dynamique
//#define PROT_POWER_UP 0
//#define PROT_INIT 5
//#define PROT_FIRST_CARD_DETECT 10
//#define PROT_WAIT_10_MS 20
//#define PROT_ENABLE_OUT_SET 30
//#define PROT_WAIT_5_MS 40
//#define PROT_SEND_ADDR 50
//#define PROT_WAIT_ENABLE_IN_AND_RX_ADDR 60
//#define PROT_WAIT_50_MS 70
//#define PROT_RX_NEXT_ADDR 80
//#define PROT_WAIT_END_ID 90
//#define PROT_RX_LAST_ID 95
//#define PROT_GOTO_RUN_MODE 99

//Definition de la valeur attribue aux differents etats
//de la réception du PWM
//#define PROT_RX_INIT 0
//#define PROT_RX_WAIT_FOR_START 10
//#define PROT_RX_GET_ID 20
//#define PROT_RX_GET_PWM 30
//#define PROT_RX_PWM_SET 40
//#define PROT_RX_END 50

//Definition de la valeur attribue aux differents etats
//de la transmission des donnees des encodeur
//#define PROT_TX_INIT 0
//#define PROT_TX_WAIT_FOR_START 10
//#define PROT_TX_CHECK_FIRST_ID 20
//#define PROT_TX_WAIT_PREV_ID 30
//#define PROT_TX_GET_PREV_ENCOD 40
//#define PROT_TX_SEND_ID 50
//#define PROT_TX_SEND_ENCODER 60
//#define PROT_TX_VERIF_LAST_ID 70
//#define PROT_WAIT_SEND_LAST_ID 80
//#define PROT_TX_END 90

//definition des fonction
void protocol_init(void);
void protocol_timer_ms(void);
unsigned char process_protocol_alloc_id(void);
void process_protocol_rx(void);
void process_protocol_tx(void);
//unsigned char protocol_lid(void);

//#define PROT_RX_WAIT_FOR_START_CHAR 0
//#define PROT_RX_INIT 10
//#define PROT_RX_PROCESS 20
//#define PROT_RX_END 30

//extern volatile unsigned char protocol_reply_data[PROTOCOL_REPLY_SIZE];
//extern volatile unsigned char protocol_received_data[PROTOCOL_RECEIVE_SIZE];
#endif //_PROTOCOLE_H_

