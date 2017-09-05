//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef CGDA_PAINT_IET_H_
#define CGDA_PAINT_IET_H_

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

#define DEFAULT_DELAY_S 0.010

//using namespace ObpenRAVE;

namespace teo
{

class CgdaPaintFitnessFunction : public EvaluateOp {

  public:

    void setPsqFeatures( vector<int>* _psqFeatures){
        psqFeatures = _psqFeatures;
    }

    void setPRpcClient( yarp::os::RpcClient* pRpcClient){
        this->pRpcClient = pRpcClient;
    }


    FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    void individualExecution(vector<double> results); //TE
    vector<int>* psqFeatures;

    yarp::dev::IPositionControl *mentalPositionControl;
    yarp::dev::IPositionControl *realPositionControl;
    yarp::os::RpcClient* pRpcClient;

};

typedef boost::shared_ptr<CgdaPaintFitnessFunction> CgdaPaintFitnessFunctionP;

}  // namespace teo

#endif /* CGDA_EXECUTION_IET_H_ */

