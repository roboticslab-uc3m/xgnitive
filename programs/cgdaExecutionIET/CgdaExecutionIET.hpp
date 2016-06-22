
#ifndef FUNCTIONMINEVALOP_H_
#define FUNCTIONMINEVALOP_H_

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ecf/ECF.h>
#include <openrave-core.h>
#include <vector>

#include "DtwCgdaRecognition.hpp"
using namespace OpenRAVE;

class CgdaExecutionIET : public EvaluateOp {

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
    void setResults( vector<double>* _presults){
        pFresults = _presults;

    }

    void setIter(unsigned int* _piter){
        pIter = _piter;
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
    vector<double>* pFresults;
    unsigned int* pIter;
    Transform T_base_object;
};

typedef boost::shared_ptr<CgdaExecutionIET> CgdaExecutionIETP;

#endif /* FUNCTIONMINEVALOP_H_ */

