#include "DtwActionRecognition.hpp"
#include <iostream>


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
    generalizedT=v1;
    return true;
}

/***********************************************************************/

bool DtwActionRecognition::compare(std::vector<double> &v2, double& discrepancy)
{
    //"from" and "to" are of the type std::vector< std::vector< double > > which is a vector of vectors.
    Aquila::DtwDataType from, to;

    if( v2.size() != generalizedT.size())
    {
        CD_ERROR("DTW ERROR=>The two feature trajectories must have same dimensions!")
        return false;
    }

    //Store input vectors in the two DtwDatype variables
    from.push_back(v2);
    to.push_back(generalizedT);

    //dtw Aquila dst
    Aquila::Dtw dtw;
    discrepancy = dtw.getDistance(from, to);
    std::cout << "DTW distance (neighbors): " << discrepancy << "\n";
    //drawDtwPath(dtw, 1, 1);


    return true;
}

/***********************************************************************/

}  // namespace teo
