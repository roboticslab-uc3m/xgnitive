//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef CGDA_PAINT_IET_H_
#define CGDA_PAINT_IET_H_

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ecf/ECF.h>
#include <openrave-core.h>
#include <vector>
#include <sstream>
#include <string>
//#include <valarray>     // std::valarray

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "DtwCgdaRecognition.hpp"

using namespace OpenRAVE;

namespace teo
{

class CgdaPaintFitnessFunction : public EvaluateOp {

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
    void setPsqPainted( vector<int>* _psqPainted){
        psqPainted = _psqPainted;

    }
    void setPRpcClient( yarp::os::RpcClient* pRpcClient){
        _pRpcClient = pRpcClient;
    }

//    void setIter(unsigned int* _piter){
//        pIter = _piter;
//    }

    FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    void individualExecution(vector<double> results); //TE
    RobotBasePtr probot;
    EnvironmentBasePtr penv;
    ControllerBasePtr pcontrol;
    KinBodyPtr _objPtr;
    KinBodyPtr _wall;
    vector<double>* pFresults;
    //unsigned int* pIter;
    Transform T_base_object;
    vector<int>* psqPainted;

    yarp::dev::IPositionControl *iPositionControl;
    yarp::os::RpcClient* _pRpcClient;

};

typedef boost::shared_ptr<CgdaPaintFitnessFunction> CgdaPaintFitnessFunctionP;

}  // namespace teo

#endif /* CGDA_EXECUTION_IET_H_ */

