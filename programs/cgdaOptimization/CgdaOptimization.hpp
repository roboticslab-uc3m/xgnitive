// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef FUNCTIONMINEVALOP_H_
#define FUNCTIONMINEVALOP_H_

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <valarray>     // std::valarray

#include <ecf/ECF.h>

#include <openrave-core.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include "DtwCgdaRecognition.hpp"

using namespace OpenRAVE;

//This file comes from FunctionMinEvalOp.hpp from the package evMono6.

namespace teo
{

class FunctionMinEvalOp : public EvaluateOp {

  public:
    void setPRobot(const RobotBasePtr& _probot) {
        probot = _probot;
    }
    void setPenv(const EnvironmentBasePtr& _penv){
        penv = _penv;
    }
    void setPcontrol(const ControllerBasePtr& _pcontrol){
        pcontrol = _pcontrol;
    }

  public:
    FitnessP evaluate(IndividualP individual);
    void registerParameters(StateP);
    bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    RobotBasePtr probot;
    EnvironmentBasePtr penv;
    ControllerBasePtr pcontrol;

    KinBodyPtr _objPtr;
    KinBodyPtr _wall;

    Transform T_base_object;
};

typedef boost::shared_ptr<FunctionMinEvalOp> FunctionMinEvalOpP;

}  // namespace teo

#endif /* FUNCTIONMINEVALOP_H_ */
