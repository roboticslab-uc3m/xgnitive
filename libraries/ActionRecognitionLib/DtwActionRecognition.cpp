#include "DtwActionRecognition.hpp"

namespace teo
{

/***********************************************************************/

bool DtwActionRecognition::setGeneralized(std::vector<double> &v1)
{
    if( v1.size() == 0 )
    {
        CD_ERROR("Empty vector!\n");
        return false;
    }

    return true;
}

/***********************************************************************/

bool DtwActionRecognition::compare(std::vector<double> &v2, double& discrepancy)
{
    return true;
}

/***********************************************************************/

}  // namespace teo
