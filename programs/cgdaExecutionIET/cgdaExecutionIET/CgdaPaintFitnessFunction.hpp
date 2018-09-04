//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef CGDA_PAINT_IET_H_
#define CGDA_PAINT_IET_H_

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ecf/ECF.h>
#include <vector>
#include <sstream>
#include <string>
#include <valarray>     // std::valarray

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "DtwCgdaRecognition.hpp"

#define NTPOINTS 17
#define NSQUARES 16
#define DEFAULT_DELAY_S 0.010

namespace teo
{

class CgdaPaintFitnessFunction : public EvaluateOp {

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

    FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    std::vector<double> trajectoryExecution(vector<double> result_trajectory); //TE
    vector<double>* pFresults;
    unsigned int* pIter;

    yarp::dev::IPositionControl *mentalPositionControl;
    //yarp::dev::IPositionControl *realPositionControl;
    yarp::os::RpcClient* pRpcClient;
};

typedef boost::shared_ptr<CgdaPaintFitnessFunction> CgdaPaintFitnessFunctionP;

}  // namespace teo

#endif /* CGDA_EXECUTION_IET_H_ */

