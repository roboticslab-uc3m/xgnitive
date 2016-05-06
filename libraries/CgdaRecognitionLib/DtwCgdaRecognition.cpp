#include "DtwCgdaRecognition.hpp"

namespace teo
{

/***********************************************************************/

bool DtwCgdaRecognition::setGeneralized(std::vector<std::vector<double >> &v1)
{
    if( v1.size() == 0 )
    {
        CD_ERROR("Empty vector!\n");
        return false;
    }
    generalizedT=v1;
    return true;
}

/***********************************************************************/

bool DtwCgdaRecognition::compare(std::vector<std::vector<double>> &v2, double& discrepancy)
{

    if( v2[0].size() != generalizedT[0].size())
    {
        CD_ERROR("DTW ERROR=>Input vectors MUST have the same number of features!\n")
        return false;
    }

    //dtw Aquila dst
    Aquila::Dtw dtw;
    discrepancy = dtw.getDistance(v2, generalizedT);
    CD_INFO("DTW distance (neighbors): %f\n", discrepancy)

    //Uncomment for dedugging the path
//    Aquila::DtwPathType path;
//    path= dtw.getPath();
//    for (auto it = path.begin(); it != path.end(); ++it)
//    {
//        CD_INFO("EL SIGUIENTE PUNTO (attemp,generalized) ES: (%zu, %zu) \n", it->x, it->y);
//        //CD_INFO("EL SIGUIENTE PUNTO Y ES: %zu \n", it->y);
//    }


    return true;
}

/***********************************************************************/

}  // namespace teo
