// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef __EV_MONO_HPP__
#define __EV_MONO_HPP__

#include "CgdaConstrainedPaintFitnessFunction.hpp"

#include "AlgPSOInheritance.hpp"
#include "AlgPSOFuzzy.hpp"
#include "AlgConstrainedSST.hpp"

#define DEFAULT_FILE_NAME "evMono_ecf_params.xml"

namespace teo
{

class CgdaExecutionIET  {

public:
    bool init();

private:
    yarp::os::Network yarp; // connect to YARP network
    yarp::dev::PolyDriver dd; //create a YARP multi-use driver
    yarp::dev::IPositionControl *pos; //make a position controller object we call 'pos'
    yarp::dev::IEncoders *enc; //make an encoder controller object we call 'enc'
    yarp::dev::IVelocityControl *vel; //make a velocity controller object we call 'vel'
    yarp::dev::IControlMode *mode; //make a mode controller object we call 'mode'
};

} //namespace TEO
#endif  // __EV_MONO_HPP__

