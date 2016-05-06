// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "cdgaOptimization.hpp"

namespace teo
{

/************************************************************************/

bool cdgaOptimization::configure(yarp::os::ResourceFinder &rf) {

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("cdgaOptimization options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        //printf("\t--file (default: \"%s\")\n",fileName.c_str());
    }

    printf("--------------------------------------------------------------\n");

    return true;
}

/************************************************************************/
double cdgaOptimization::getPeriod() {
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

/************************************************************************/
bool cdgaOptimization::updateModule() {
    //printf("StateMachine in state [%d]. cdgaOptimization alive...\n", stateMachine.getMachineState());
    return true;
}

/************************************************************************/

bool cdgaOptimization::interruptModule() {
    return true;
}

/************************************************************************/

}  // namespace teo

