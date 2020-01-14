// Include our custom library
#include "PPM.h"
#include <iostream>

int main() {

    PPM myPPM("../textures/test.ppm");
    myPPM.darken();
    myPPM.savePPM("../textures/test_darken.ppm");
    std::cout << "Done saving." << std::endl;

    return 0;
}
