// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef __EV_MONO_HPP__
#define __EV_MONO_HPP__

#include "CgdaPaintFitnessFunction.hpp"
#include "CgdaWaxFitnessFunction.hpp"
#include "CgdaConstrainedPaintFitnessFunction.hpp"
#include "CgdaConstrainedWaxFitnessFunction.hpp"


#include "AlgPSOInheritance.hpp"
#include "AlgPSOFuzzy.hpp"
#include "AlgConstrainedSST.hpp"

#define DEFAULT_FILE_NAME "evMono_ecf_params.xml"

namespace teo
{

class CgdaExecutionIET  {
  private:
    RobotBasePtr probot;
    EnvironmentBasePtr penv;
    ControllerBasePtr pcontrol;

  public:
    bool init();
};

} //namespace TEO
#endif  // __EV_MONO_HPP__

