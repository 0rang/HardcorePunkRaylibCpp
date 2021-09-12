#ifndef MODELS_H
#define MODELS_H
#include "raylib.h"
//Model Objects
extern Model floorModel;
extern Model playerModel;
extern Model smallWallModel;
extern Model bigWallModel;
extern Model beerModel;
extern Model bspModel;
//Init Models
void ModelsInit();
//Cleanup Models
void ModelsCleanup();
#endif