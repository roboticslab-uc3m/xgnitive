//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef CGDA_IRON_IET_H_
#define CGDA_IRON_IET_H_

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ecf/ECF.h>
//#include <openrave-core.h>
#include <vector>
#include <sstream>
#include <string>
//#include <valarray>     // std::valarray

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "DtwCgdaRecognition.hpp"

#define DEFAULT_DELAY_S 0.01

#define NTPOINTS 9
#define NFEATURES 4
#define NSQUARES 16

//using namespace ObpenRAVE;

namespace teo
{

class CgdaIronFitnessFunction : public EvaluateOp {

  public:

    void setResults( vector<double>* _presults){
        pFresults = _presults;

    }

    void setIter(unsigned int* _piter){
        pIter = _piter;
    }

    void setPRpcClient( yarp::os::RpcClient* pRpcClient){
        this->pRpcClient = pRpcClient;
    }

    void setPForcePort( yarp::os::BufferedPort<yarp::os::Bottle>* pForcePort){
        this->pForcePort = pForcePort;
    }

    void setPRpcClientWorld( yarp::os::RpcClient* pRpcClientWorld){
        this->pRpcClientWorld = pRpcClientWorld;
    }

    void setPRpcClientCart( yarp::os::RpcClient* pRpcClientCart){
        this->pRpcClientCart = pRpcClientCart;
    }


    FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    void individualExecution(vector<double> results); //TE
    vector<double>* pFresults;
    unsigned int* pIter;

    yarp::dev::IPositionControl *mentalPositionControl;
    yarp::dev::IPositionControl *realPositionControl;
    yarp::os::RpcClient* pRpcClient;
    yarp::os::BufferedPort<yarp::os::Bottle>* pForcePort;

    yarp::os::RpcClient* pRpcClientWorld;
    yarp::os::RpcClient* pRpcClientCart;

};

typedef boost::shared_ptr<CgdaIronFitnessFunction> CgdaIronFitnessFunctionP;

}  // namespace teo

#endif /* CGDA_IRON_IET_H_ */

