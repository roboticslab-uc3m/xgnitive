// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef __EV_MONO_HPP__
#define __EV_MONO_HPP__

#include "CgdaPaintFitnessFunction.hpp"
//The following cost functions are not yet implemented in OET
//#include "CgdaWaxFitnessFunction.hpp"
//#include "CgdaConstrainedPaintFitnessFunction.hpp"
//#include "CgdaConstrainedWaxFitnessFunction.hpp"


#include "AlgPSOInheritance.hpp"
#include "AlgPSOFuzzy.hpp"
#include "AlgConstrainedSST.hpp"

#define DEFAULT_FILE_NAME "evMono_ecf_params.xml"

namespace teo
{

class CgdaExecutionOET
{

public:
    bool init(int argc, char **argv);

private:
    RobotBasePtr probot;
    EnvironmentBasePtr penv;
    ControllerBasePtr pcontrol;
    vector<int> sqPainted;
};

} //namespace TEO

#endif  // __EV_MONO_HPP__

