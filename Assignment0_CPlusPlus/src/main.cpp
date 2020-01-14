// Include our custom library
#include "PPM.h"

int main() {
    PPM myPPM("../textures/test1.ppm");
    myPPM.darken();
    myPPM.savePPM("../textures/test_darken1.ppm");

    return 0;
}
