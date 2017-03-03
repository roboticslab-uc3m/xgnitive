// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef __EV_MONO_HPP__
#define __EV_MONO_HPP__

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "CgdaPaintFitnessFunction.hpp"
//The following cost functions are not yet implemented in OET
//#include "CgdaWaxFitnessFunction.hpp"
//#include "CgdaConstrainedPaintFitnessFunction.hpp"
//#include "CgdaConstrainedWaxFitnessFunction.hpp"


#include "AlgPSOInheritance.hpp"
#include "AlgPSOFuzzy.hpp"
#include "AlgConstrainedSST.hpp"

#include <sys/time.h>

#define DEFAULT_FILE_NAME "evMono_ecf_params.xml"

namespace teo
{

class CgdaExecutionOET
{

public:
    int init(int argc, char **argv);

private:
    // Openrave
    //RobotBasePtr probot;
    //EnvironmentBasePtr penv;
    //ControllerBasePtr pcontrol;
    vector<int> sqPainted;

    // YARP
    yarp::os::Network yarp;
    yarp::os::Port port;
    int portNum;
    yarp::dev::PolyDriver mentalDevice;
    yarp::dev::PolyDriver realDevice;
    yarp::dev::PolyDriver realSimDevice;
    yarp::os::RpcClient rpcClient;

};

} //namespace TEO

#endif  // __EV_MONO_HPP__

