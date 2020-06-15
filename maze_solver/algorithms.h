//
// Created by Nazar on 14/06/2020.
//

#include <stdint.h>

#ifndef P4_PC_JOYSTICK2_HABITACION_ALGORITHMS_H
#define P4_PC_JOYSTICK2_HABITACION_ALGORITHMS_H

#endif //P4_PC_JOYSTICK2_HABITACION_ALGORITHMS_H

//Velocitat a la que mourem el robot
static const int basicSpeed = 100;
static const int maxSpeed = 1023;
static const int turnSpeed = 10;
//Distància mínima de seguretat a les parets que mantindrem
static const int security_distance = 10;

//Al cridar aquesta funció, el robot hauria de trobar la paret més propera.
//retornarà 0, 1 o 2, depenent de si la paret trobada es troba davant, a l'esquerra o a la dreta del robot, respectivament.
int findWall();
void followWall();

