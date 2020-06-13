/*
 * joystick.c
 *
 *  Created on: 25 mar. 2020
 *      Author: Hristo
 */

#include "joystick.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "../dyn_test/b_queue.h"
#include "../dyn/dyn_app_motors.h"

uint8_t Estado_actual = Ninguno, Anterior = Ninguno;

void Set_estado_anterior(uint8_t valor){
	Anterior = valor;
}

void Set_estado_actual(uint8_t valor){
	Estado_actual = valor;
}

void Get_estado(uint8_t* estado, uint8_t* anterior ){
	*estado = Estado_actual;
	*anterior = Anterior;
}

/**
 * Handler to exit the current thread under the appropiate signal
 */
static void handler(int signum)
{
	pthread_exit(NULL);
}

/**
 * Thread to emulate the joystick
 */
void* joystick_emu(void *vargp) {
	uint8_t tecla, estado;

	// Add SIGTERM handler to kill the current thread
	signal(SIGTERM, handler);
    uint8_t tmp;
    uint16_t speed;
    bool direction;
    bool endless;

	while(1){
		scanf("%c", &tecla);

//		printf("Tecla: %c, 0x%02X\n", tecla, tecla);
//		fflush(stdout);
		switch(tecla){
		case 0x0A:
		case 0x0D:
			continue; //omitir el retorno, yendo directamente a la siguiente iteracion del while.
		case 'i':
			estado = Up;
            //Posem el robot en mode moviment continu.
            robotMoveContinuous(100);

			break;
		case 'j':
			estado = Left;
            //Volem anar cap endavant i a la l'esquerra
            moveSideContinuous(100,1);
            dyn_readTurnSpeed(ID_MOTOR_LEFT, &speed, &direction);
			break;
		case 'k':
			estado = Center;
            //Cridem a la funció per a aturar el robot
            robotStop();

            break;
		case 'l':
			estado = Right;
            //Volem anar enrere i a la dreta
            moveSideContinuous(100,0);
			break;
		case 'm':
			estado = Down;
            //Provem a passar-li una velocitat negativa, per a veure si es posa en la direcció contrària.
            robotMoveContinuous(100);
			break;
		case 'a':
			estado = Sw1;
			//Volem que el robot es mogui en sentit antihorari sobre si mateix
            robotSpinContinuous(100);
            dyn_readTurnSpeed(ID_MOTOR_LEFT, &speed, &direction);
			break;
		case 's':
			estado = Sw2;
			//Volem que el robot giri en sentit horari sobre si mateix
            robotSpinContinuous(-100);
            dyn_readTurnSpeed(ID_MOTOR_LEFT, &speed, &direction);
			break;
		case 'q':
			estado = Quit;
			break;
		default:
			estado = Ninguno;
		}
		Anterior = Otro;//Me aseguro de que anterior sea diferente de estado}
		Estado_actual = estado;
//		printf("Estado actual = %d\n", Estado_actual);
//		fflush(stdout);
	}
}

