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

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "DtwCgdaRecognition.hpp"

//using namespace OpenRAVE;

//This file comes from CgdaPaintFitnessFunction.hpp from the package evMono6.

#define NTPOINTS 17
#define NSQUARES 64
#define DEFAULT_DELAY_S 0.010

namespace teo
{

class CgdaPaintFitnessFunction : public EvaluateOp {

  public:
    void setPRpcClient( yarp::os::RpcClient* pRpcClient){
        this->pRpcClient = pRpcClient;
    }

    FitnessP evaluate(IndividualP individual);
    void registerParameters(StateP);
    bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    std::vector<double> trajectoryExecution(vector<double> result_trajectory);


    yarp::dev::IPositionControl *mentalPositionControl;
    yarp::os::RpcClient* pRpcClient;
};

typedef boost::shared_ptr<CgdaPaintFitnessFunction> CgdaPaintFitnessFunctionP;

}  // namespace teo

#endif /* FUNCTIONMINEVALOP_H_ */
