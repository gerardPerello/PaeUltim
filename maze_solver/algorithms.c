//
// Created by Nazar on 14/06/2020.
//

#include "algorithms.h"
#include "dyn_app_motors.h"
#include "dyn_app_sensor.h"
#include "dyn_instr.h"
#include <stdio.h>
#include <unistd.h>


int findWall(){

    //En aquests punters anirem guardant les distàncies que llegeixen els sensors
    uint8_t dLeft, dCenter, dRight;
    uint8_t *distances[3] = {&dCenter, &dLeft, &dRight};


    //valdrà 0, 1 o 2 depenent de si estem anant cap al centre, a l'esquerra o cap a la dreta respectivament.
    //Inicialment, val 0 perquè comencem movent-nos endavant
    int estado = -1;

    //Anirem repetint l'algoritme fins que ens trobem prou a prop d'una paret
    while(true){

        //Llegim les distancies dels tres sensors
        dyn_readDistanceLeft(ID_SENSOR, &dLeft);
        dyn_readDistanceCenter(ID_SENSOR, &dCenter);
        dyn_readDistanceRight(ID_SENSOR, &dRight);

        //Una variable per a saber quin és el sensor amb la paret més propera
        int closest = 0;

        //Recorrem els valors dels sensors
        for(int i = 0; i < 3; i++){

            //Si ja ens hem apropat suficient a una paret, retornem el sensor corresponent i aturem el robot
            if(*distances[i] <= security_distance){
                robotStop();
                return i;
            }

            //Per tal de trobar el sensor amb la paret més propera, actualitzem la variable per a cada sensor
            //si la distància llegida és menor a la distància dels sensors anteriors
            if( *distances[closest] > *distances[i]){
                closest = i;
            }

        }


        //Ara que sabem quin és el sensor amb la paret més propera, podem dir-li al robot què fer
        //depenent d'aquesta dada:

        if(estado != closest){

            printf("Cambiando de estado %i a estado %i\n", estado, closest);
            estado = closest;


            switch (closest) {
                //Si és el frontal, ens movem endavant.
                case 0:
                    robotMoveContinuous(basicSpeed);
                    break;

                //Si és el de l'esquerra, ens movem cap a l'esquerra.
                case 1:
                    robotSpinContinuous(basicSpeed);
                    break;

                    //Si és el de la dreta, ens movem cap a la dreta.
                case 2:
                    robotSpinContinuous(-basicSpeed);
                    break;

            }

        }



    }


}

void steerLeft(int increment, uint16_t *speedLeft, uint16_t *speedRight, uint8_t idLeft, uint8_t idRight){

    if(increment < 0){
        steerLeft(-increment, speedRight, speedLeft, idRight, idLeft);
    }

    if(increment == 0){

        if(*speedLeft > basicSpeed && *speedRight > basicSpeed){
            *speedLeft -= 10;
            *speedRight -= 10;

            dyn_setTurnSpeed(idLeft, *speedLeft, 0);
            dyn_setTurnSpeed(idRight, *speedRight, 0);
            return;
        }

        if(*speedLeft < basicSpeed && *speedRight < basicSpeed){
            *speedLeft += 5;
            *speedRight += 5;

            dyn_setTurnSpeed(idLeft, *speedLeft, 0);
            dyn_setTurnSpeed(idRight, *speedRight, 0);
            return;
        }

    }

    if(increment > 0){

        if(basicSpeed  >= increment + *speedRight){

            *speedRight += increment;
            dyn_setTurnSpeed(idRight, *speedRight, 0);
            return;

        }

        if(*speedLeft >= increment){

            *speedLeft -= increment;
            dyn_setTurnSpeed(idLeft, *speedLeft, 0);
            return;

        }

        if(*speedRight + increment <= maxSpeed){

            *speedRight += increment;
            dyn_setTurnSpeed(idRight, *speedRight, 0);
            return;
        }
    }


}

void followWall(){

    robotStop();
    uint16_t motorLeft = 0;
    uint16_t motorRight = 0;
    uint8_t sensorLeft = 0;
    uint8_t sensorFront = 0;

    while(true){

        dyn_readDistanceCenter(ID_SENSOR, &sensorFront);
        dyn_readDistanceLeft(ID_SENSOR, &sensorLeft);

        if(sensorFront == 255 && sensorLeft == 255){
            findWall();
        }

        else if(sensorLeft == 255){
            steerLeft(10, &motorLeft, &motorRight, ID_MOTOR_LEFT, ID_MOTOR_RIGHT);
        }

        else if(sensorFront == 255){
            steerLeft(2* (((int) sensorLeft) - security_distance), &motorLeft, &motorRight, ID_MOTOR_LEFT, ID_MOTOR_RIGHT);
        }

        else{

            steerLeft(((int)sensorLeft)/sensorFront, &motorLeft, &motorRight, ID_MOTOR_LEFT, ID_MOTOR_RIGHT );
        }


    }


}

void followWall2(){

    uint8_t sensorLeft = 0;
    uint8_t sensorFront = 0;
    uint16_t speed =0;

    while(true){

        dyn_readDistanceCenter(ID_SENSOR, &sensorFront);
        dyn_readDistanceLeft(ID_SENSOR, &sensorLeft);

        if(speed < 100 && sensorFront > 100){

            speed+= 10;
            dyn_setTurnSpeed(ID_MOTOR_LEFT, speed, 0);
            dyn_setTurnSpeed(ID_MOTOR_RIGHT, speed, 0);

        }else{

            if(sensorFront > 30){

                if(sensorLeft > 15){

                    dyn_setTurnSpeed(ID_MOTOR_RIGHT, speed + 5, 0);

                }else if (sensorLeft < 5){

                    dyn_setTurnSpeed(ID_MOTOR_RIGHT, speed - 5, 0);

                }else{

                    dyn_setTurnSpeed(ID_MOTOR_RIGHT, speed, 0);
                }

            }else{

                dyn_setTurnSpeed(ID_MOTOR_RIGHT, speed - 15, 0);

            }

        }
    }




}