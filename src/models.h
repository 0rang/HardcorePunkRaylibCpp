#ifndef MODELS_H
#define MODELS_H
#include "raylib.h"
//Model Objects
extern Model playerModel;
extern Model smallWallModel;
extern Model beerModel;
//Init Models
void ModelsInit();
//Cleanup Models
void ModelsCleanup();
#endif