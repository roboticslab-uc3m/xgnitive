#ifndef __DTW_ACTION_RECOGNITION_HPP__
#define __DTW_ACTION_RECOGNITION_HPP__

#include "ActionRecognition.hpp"

namespace teo
{

/**
 * @ingroup ActionRecognitionLib
 *
 * @brief This class receive two trajectories vector defined with vectors and compare them with DTW. It returns a "discrepancy value".
 *
 */
class DtwActionRecognition : public ActionRecognition
{

public:

    /**
     * With this fuction we just define the generalized trajectory, which
     * at the end is the one we want to achieve
     */
    virtual bool setGeneralized(std::vector<double> &v1);

    /**
     * Compare the vector "v2" with the generalized trajectory
     */
    virtual bool compare(std::vector<double> &v2, double& discrepancy);


private:

};

}  // namespace teo

#endif // __DTW_ACTION_RECOGNITION_HPP__

