//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef CGDA_CONSTRAINED_PAINT_IET_H_
#define CGDA_CONSTRAINED_PAINT_IET_H_

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

#include <kdl/chainfksolverpos_recursive.hpp>

#include "DtwCgdaRecognition.hpp"

#define GdL 3

namespace teo
{

class CgdaConstrainedPaintFitnessFunction : public EvaluateOp {

  public:
    void setResults( vector<double>* _presults){
        presults = _presults;
    }

    void setIter(unsigned int* _piter){
        pIter = _piter;
    }

    void setEvaluations(int* _pconst_evaluations){
        const_evaluations=_pconst_evaluations;
    }

    void setPControl(yarp::dev::IPositionControl* _pos){
        pos=_pos;
    }

    void setEControl(yarp::dev::IEncoders* _enc){
        enc=_enc;
    }

    void trajectoryExecution(int NumberPoints, vector<double> result_trajectory); //TE

  private:
    FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    vector<double>* presults;
    unsigned int* pIter;
    int* const_evaluations;

    /** Teo Sim **/
    yarp::dev::IPositionControl* pos;
    yarp::dev::IEncoders *enc;

    /** The chain. **/
    KDL::Chain chain;

};

typedef boost::shared_ptr<CgdaConstrainedPaintFitnessFunction> CgdaConstrainedPaintFitnessFunctionP;

}  // namespace teo

#endif /* CGDA_EXECUTION_IET_H_ */

