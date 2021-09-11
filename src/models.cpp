#include "models.h"
#include "raylib.h"
#include "raymath.h"
Model floorModel;
Model playerModel;
Model smallWallModel;
Model beerModel;
Shader lshader;
void ModelsInit() {
    floorModel = LoadModel(ASSETS_PATH"PlaneText.gltf");
    playerModel = LoadModel(ASSETS_PATH"Board.gltf");
    playerModel.transform = MatrixMultiply(MatrixScale(0.19992423057556152f, 0.030000029131770134f, 1.0f), playerModel.transform);
    beerModel = LoadModel(ASSETS_PATH"BeerBottleText.gltf");
    smallWallModel = LoadModel(ASSETS_PATH"SmallWallText.gltf");
    lshader = LoadShader(ASSETS_PATH"getnormal.vs", ASSETS_PATH"veclighting.fs");
    smallWallModel.materials[0].shader = lshader;
    playerModel.materials[0].shader = lshader;
    beerModel.materials[0].shader = lshader;
}
void ModelsCleanup() {
    UnloadShader(lshader);
    UnloadModel(playerModel);
    UnloadModel(smallWallModel);
    UnloadModel(floorModel);
    UnloadModel(beerModel);
}