// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __EV_MONO_HPP__
#define __EV_MONO_HPP__

#include "CgdaFitnessFunction.hpp"

//#include "AlgPSOInheritance.hpp"
//#include "AlgPSOFuzzy.hpp"

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include <sys/time.h>


#define DEFAULT_FILE_NAME "evMono_ecf_params.xml"

namespace teo
{

class CgdaExecution  {
  public:
    bool init();

    // YARP
    yarp::os::Network yarp;
    yarp::os::Port port;
    int portNum;
    yarp::dev::PolyDriver mentalDevice;
    yarp::dev::PolyDriver realDevice;
    yarp::os::RpcClient rpcClient;
    yarp::os::BufferedPort<yarp::os::Bottle> forcePort;
    yarp::os::RpcClient rpcClientWorld;
    yarp::os::RpcClient rpcClientCart;

};

}

#endif  // __EV_MONO_HPP__

