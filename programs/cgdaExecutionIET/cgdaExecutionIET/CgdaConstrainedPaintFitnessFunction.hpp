//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#ifndef CGDA_CONSTRAINED_PAINT_IET_H_
#define CGDA_CONSTRAINED_PAINT_IET_H_

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

#include <kdl/chainfksolverpos_recursive.hpp>

#include "DtwCgdaRecognition.hpp"

#define GdL 3

namespace teo
{

class CgdaConstrainedPaintFitnessFunction : public EvaluateOp {

  public:
    void setPRobot(const OpenRAVE::RobotBasePtr& _probot) {
        probot = _probot;
    }
    void setPenv(const OpenRAVE::EnvironmentBasePtr& _penv){
        penv = _penv;
    }
    void setPcontrol(const OpenRAVE::ControllerBasePtr& _pcontrol){
        pcontrol = _pcontrol;
    }
    void setResults( vector<double>* _presults){
        pFresults = _presults;

    }

    void setIter(unsigned int* _piter){
        pIter = _piter;
    }

    void setEvaluations(int* _pconst_evaluations){
        const_evaluations=_pconst_evaluations;
    }

    void trajectoryExecution(int NumberPoints, vector<double> result_trajectory); //TE

  private:
    FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
    double getCustomFitness(vector<double> genPoints);
    OpenRAVE::RobotBasePtr probot;
    OpenRAVE::EnvironmentBasePtr penv;
    OpenRAVE::ControllerBasePtr pcontrol;
    OpenRAVE::KinBodyPtr _objPtr;
    OpenRAVE::KinBodyPtr _wall;
    vector<double>* pFresults;
    unsigned int* pIter;
    OpenRAVE::Transform T_base_object;
    int* const_evaluations;

    /** The chain. **/
    KDL::Chain chain;

};

typedef boost::shared_ptr<CgdaConstrainedPaintFitnessFunction> CgdaConstrainedPaintFitnessFunctionP;

}  // namespace teo

#endif /* CGDA_EXECUTION_IET_H_ */

