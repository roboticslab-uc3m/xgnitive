// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * @ingroup xgnitive_programs
 * \defgroup cgdaExecution cgdaExecution
 *
 * @brief Executes the cgda system using a classic Full Trajectory Evolution strategy.
 *
 * @section cgdaExecution_legal Legal
 *
 * Copyright: 2016 (C) Universidad Carlos III de Madrid
 *
 * Author: jgvictores, raulfdzbis, smorante
 *
 * CopyPolicy: This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 3.0 or later
 *
 * <hr>
 *
 * This file can be edited at cgdaExecution
 *
 */

#include <yarp/os/all.h>

#include "CgdaExecution.hpp"

//using namespace yarp::os;

int main(int argc, char **argv) {

    //Just init CgdaExecution.
    teo::CgdaExecution mod;
    return mod.init();

    //    ResourceFinder rf;
    //    rf.setVerbose(true);
    //    rf.setDefaultContext("cgdaOptimization");
    //    rf.setDefaultConfigFile("cgdaOptimization.ini");
    //    rf.configure(argc, argv);

    //    teo::CgdaOptimization mod;
    //    if(rf.check("help")) {
    //        return mod.runModule(rf);
    //    }

    //    printf("Run \"%s --help\" for options.\n",argv[0]);
    //    printf("%s checking for yarp network... ",argv[0]);
    //    fflush(stdout);
    //    Network yarp;
    //    if (!yarp.checkNetwork()) {
    //        fprintf(stderr,"[fail]\n%s found no yarp network (try running \"yarpserver &\"), bye!\n",argv[0]);
    //        return 1;
    //    } else printf("[ok]\n");

    //    return mod.runModule(rf);
}
