
#include "gtest/gtest.h"

#include "ColorDebug.hpp"
#include "DtwActionRecognition.hpp"
#include <iostream>

namespace teo
{

/**
 * @brief Tests ...
 */
class TestActionRecognitionLib : public testing::Test // -- inherit the Test class (gtest.h)
{

public:

    virtual void SetUp() {
        // -- code here will execute just before the test ensues
        actionRecognition = new DtwActionRecognition;
    }

    virtual void TearDown()
    {
        // -- code here will be called just after the test completes
        // -- ok to through exceptions from here if need be
        delete actionRecognition;
        actionRecognition = 0;
    }

protected:

    /** ActionRecognition object. */
    ActionRecognition* actionRecognition;
};


TEST_F( TestActionRecognitionLib, TestActionRecognitionLib_DTW) // -- we call the class that we want to do the test and we assign it a name
{
    std::vector<std::vector<double>> generalizedRandom, generalizedForZeroDiscrepancy, generalizedForSimpleDiscrepancy;
    std::vector<std::vector<double>> attempVectRandom, attempVectforZeroDiscrepancy, attempVectforSimpleDiscrepancy;
    double discrepancy;
    bool okzero=true;
    bool oksimple=true;

    //--Random Test-------------------------------------//
    //-- test Generalized
    generalizedRandom.push_back({27,58,48,35,25});
    generalizedRandom.push_back({24,8,3453,48,89});
    generalizedRandom.push_back({24,58,22,58,47});
    bool okg = actionRecognition->setGeneralized( generalizedRandom );

    //-- test compare
    attempVectRandom.push_back({23,54, 345, 2, 18794});
    attempVectRandom.push_back({4,58, 324, 12, 7847});
    bool okc = actionRecognition->compare(attempVectRandom,discrepancy);

    //--Zero Test----------------------------------------//
    //-- test Generalized
    generalizedForZeroDiscrepancy.push_back({27,58,48,35,25});
    generalizedForZeroDiscrepancy.push_back({24,8,3453,12,43});
    actionRecognition->setGeneralized( generalizedForZeroDiscrepancy );

    //-- test compare
    attempVectforZeroDiscrepancy.push_back({27,58,48,35,25});
    attempVectforZeroDiscrepancy.push_back({24,8,3453,12,43});
    actionRecognition->compare(attempVectforZeroDiscrepancy,discrepancy);

    if (discrepancy==0.0)
    {
        okzero=true;
    }
    else
    {
        CD_WARNING("ZERO TEST NO PASSED\n");
        okzero=false;
    }


    //--Simple Test-------------------------------------//
    //-- test Generalized
    generalizedForSimpleDiscrepancy.push_back({1});
    generalizedForSimpleDiscrepancy.push_back({0});
    generalizedForSimpleDiscrepancy.push_back({1});
    //double tam=generalizedForSimpleDiscrepancy.size();
    //CD_INFO("EL TAMAÑO ES: %f",tam);
    //std::cout<<generalizedForSimpleDiscrepancy[0][0]<<std::endl;
    actionRecognition->setGeneralized( generalizedForSimpleDiscrepancy );

    //-- test compare
    attempVectforSimpleDiscrepancy.push_back({2});
    attempVectforSimpleDiscrepancy.push_back({1});
    attempVectforSimpleDiscrepancy.push_back({0});
    actionRecognition->compare(attempVectforSimpleDiscrepancy,discrepancy);

    /*
     * The Matrix cost using euclidean distance is the following one
     *
     *  0    1
     *  |
     *  0 -- 1
     *
     * The optimal path is the one in the figure, which cost is one.
     */


    if (discrepancy==1)
    {
        oksimple=true;
    }
    else
    {
        CD_WARNING("ZERO TEST NO PASSED\n");
        oksimple=false;
    }

    CD_INFO("LA DISCREPANCIA ES %f \n",discrepancy);


    ASSERT_TRUE( okg & okc & okzero & oksimple);
}

}  // namespace teo

