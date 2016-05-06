// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CgdaOptimization.hpp"

namespace teo
{

/************************************************************************/

bool CgdaOptimization::configure(yarp::os::ResourceFinder &rf) {

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("cgdaOptimization options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        //printf("\t--file (default: \"%s\")\n",fileName.c_str());
    }

    printf("--------------------------------------------------------------\n");

    return true;
}

/************************************************************************/
double CgdaOptimization::getPeriod() {
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

/************************************************************************/
bool CgdaOptimization::updateModule() {
    //printf("StateMachine in state [%d]. cgdaOptimization alive...\n", stateMachine.getMachineState());
    return true;
}

/************************************************************************/

bool CgdaOptimization::interruptModule() {
    return true;
}

/************************************************************************/

}  // namespace teo

