// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef __EV_MONO_HPP__
#define __EV_MONO_HPP__

//#include "CgdaPaintFitnessFunction.hpp"
//#include "CgdaWaxFitnessFunction.hpp"
//#include "CgdaConstrainedPaintFitnessFunction.hpp"
//#include "CgdaConstrainedWaxFitnessFunction.hpp"
#include "CgdaIronFitnessFunction.hpp"


//#include "AlgPSOInheritance.hpp"
//#include "AlgPSOFuzzy.hpp"
//#include "AlgConstrainedSST.hpp"

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include <sys/time.h>


#define DEFAULT_FILE_NAME "evMono_ecf_params.xml"

namespace teo
{

class CgdaExecutionIET  {
  public:
    bool init();

    // YARP
    yarp::os::Network yarp;
    yarp::os::Port port;
    int portNum;
    yarp::dev::PolyDriver mentalDevice;
    yarp::dev::PolyDriver realDevice;
    yarp::os::RpcClient rpcClient;
    yarp::os::RpcClient rpcClientCart;


};

} //namespace TEO
#endif  // __EV_MONO_HPP__

