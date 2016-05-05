#include "DtwActionRecognition.hpp"


namespace teo
{

/***********************************************************************/

bool DtwActionRecognition::setGeneralized(std::vector<std::vector<double >> &v1)
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

bool DtwActionRecognition::compare(std::vector<std::vector<double>> &v2, double& discrepancy)
{
    //"from" and "to" are of the type std::vector< std::vector< double > > which is a vector of vectors.
    //Aquila::DtwDataType from, to;

    if( v2[0].size() != generalizedT[0].size())
    {
        CD_ERROR("DTW ERROR=>Input vectors MUST have the same number of features!\n")
        return false;
    }

    //Store input vectors in the two DtwDatype variables
    //from=v2;
    //to=generalizedT;

    //dtw Aquila dst
    Aquila::Dtw dtw;
    discrepancy = dtw.getDistance(v2, generalizedT);
    CD_INFO("DTW distance (neighbors): %f\n", discrepancy)
    //drawDtwPath(dtw, 1, 1);


    return true;
}

/***********************************************************************/

}  // namespace teo
