#ifndef __CGDA_RECOGNITION_HPP__
#define __CGDA_RECOGNITION_HPP__

#include <vector>

#include "ColorDebug.hpp"

namespace teo
{

/**
 *
 * @ingroup xgnitive_libraries
 * \defgroup CgdaRecognitionLib
 * @brief Contains teo::CgdaRecognition base class and derived.
 */

/**
 * @ingroup CgdaRecognitionLib
 *
 * @brief This class receives two trajectories vector defined with vectors and compare them. It returns a "discrepancy value".
 * 
 */
class CgdaRecognition
{

public:

    /**
     * With this fuction we just define the generalized trajectory, which
     * at the end is the one we want to achieve
     */
    virtual bool setGeneralized(std::vector< std::vector< double > > &v1) = 0;

    /**
     * Compare the vector "v2" with the generalized trajectory
     */
    virtual bool compare(std::vector< std::vector< double > > &v2, double& discrepancy) = 0;


protected:
    std::vector< std::vector< double > > generalizedT;

};

}  // namespace teo

#endif // __CGDA_RECOGNITION_HPP__

