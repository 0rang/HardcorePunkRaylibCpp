#include "models.h"
#include "raylib.h"
#include "raymath.h"
Model playerModel;
Model smallWallModel;
Model beerModel;
void ModelsInit() {
    playerModel = LoadModel(ASSETS_PATH"Board.gltf");
    playerModel.transform = MatrixMultiply(MatrixScale(0.19992423057556152f, 0.030000029131770134f, 1.0f), playerModel.transform);
    beerModel = LoadModel(ASSETS_PATH"BeerBottleText.gltf");
    smallWallModel = LoadModel(ASSETS_PATH"SmallWallText.gltf");
}
void ModelsCleanup() {
    UnloadModel(playerModel);
    UnloadModel(smallWallModel);
}