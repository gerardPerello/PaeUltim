//
// Created by Nazar on 14/06/2020.
//

#include "algorithms.h"
#include "dyn_app_motors.h"
#include "dyn_app_sensor.h"
#include "dyn_instr.h"
#include <stdio.h>


int findWall(){

    //En aquests punters anirem guardant les distàncies que llegeixen els sensors
    uint8_t dLeft, dCenter, dRight;
    uint8_t *distances[3] = {&dCenter, &dLeft, &dRight};


    //Si ja ens trobem amb una paret davant, hem acabat i retornem 0 com s'especifica en algorithms.h
    if(dCenter <= security_distance){
        return 0;
    }

    //valdrà 0, 1 o 2 depenent de si estem anant cap al centre, a l'esquerra o cap a la dreta respectivament.
    //Inicialment, val 0 perquè comencem movent-nos endavant
    int estado = 0;

    //ens movem endavant
    robotMoveContinuous(basicSpeed);

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

            estado = closest;

            switch (closest) {
                //Si és el frontal, ens movem endavant.
                case 0:
                    robotMoveContinuous(basicSpeed);
                    break;

                //Si és el de l'esquerra, ens movem cap a l'esquerra.
                case 1:
                    moveSideContinuous(basicSpeed, 1);
                    break;

                    //Si és el de la dreta, ens movem cap a la dreta.
                case 2:
                    moveSideContinuous(basicSpeed, 0);

            }

        }



    }


}