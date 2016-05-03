#ifndef __ACTION_RECOGNITION_HPP__
#define __ACTION_RECOGNITION_HPP__

#include <vector>

#include "ColorDebug.hpp"

namespace teo
{

//! This class receive two trajectories vector defined with vectors and compare them. It returns a "discrepancy value"
class ActionRecognition
{

public:

    /**
     * With this fuction we just define the generalized trajectory, which
     * at the end is the one we want to achieve
     */
    virtual bool setGeneralized(std::vector<double> &v1) = 0;

    /**
     * Compare the vector "v2" with the generalized trajectory
     */
    virtual bool compare(std::vector<double> &v2, double& discrepancy) = 0;


private:
    std::vector<double> generalizedT;

};

}  // namespace teo

#endif // __ACTION_RECOGNITION_HPP__

