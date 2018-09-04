//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef CGDA_WAX_IET_H_
#define CGDA_WAX_IET_H_

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ecf/ECF.h>
#include <openrave-core.h>
#include <vector>
#include <sstream>
#include <string>
#include <valarray>     // std::valarray

#include "DtwCgdaRecognition.hpp"

using namespace OpenRAVE;

namespace teo
{

class CgdaWaxFitnessFunction : public EvaluateOp {

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
    vector<double>* pFresults;
    unsigned int* pIter;
    Transform T_base_object;
};

typedef boost::shared_ptr<CgdaWaxFitnessFunction> CgdaWaxFitnessFunctionP;

}  // namespace teo

#endif /* CGDA_EXECUTION_IET_H_ */

