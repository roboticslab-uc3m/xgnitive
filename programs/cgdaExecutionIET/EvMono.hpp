// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __EV_MONO_HPP__
#define __EV_MONO_HPP__

#include "CgdaExecutionIET.hpp"

#define DEFAULT_FILE_NAME "evMono_ecf_params.xml"



class EvMono  {
  private:
    RobotBasePtr probot;
    EnvironmentBasePtr penv;
    ControllerBasePtr pcontrol;

  public:
    bool init();
};

#endif  // __EV_MONO_HPP__

