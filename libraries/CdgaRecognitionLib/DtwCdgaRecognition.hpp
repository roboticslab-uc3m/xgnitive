#ifndef __DTW_CDGA_RECOGNITION_HPP__
#define __DTW_CDGA_RECOGNITION_HPP__


#include <aquila/global.h>
#include <aquila/functions.h>
#include <aquila/ml/Dtw.h>

#include "CdgaRecognition.hpp"

namespace teo
{

/**
 * @ingroup CdgaRecognitionLib
 *
 * @brief This class receive two trajectories vector defined with vectors and compare them with DTW. It returns a "discrepancy value".
 *
 */
class DtwCdgaRecognition : public CdgaRecognition
{

public:

    /**
     * With this fuction we just define the generalized trajectory, which
     * at the end is the one we want to achieve
     */
    virtual bool setGeneralized(std::vector<std::vector<double>> &v1);

    /**
     * Compare the vector "v2" with the generalized trajectory
     */
    virtual bool compare(std::vector<std::vector<double>> &v2, double& discrepancy);


private:

};

}  // namespace teo

#endif // __DTW_CDGA_RECOGNITION_HPP__

