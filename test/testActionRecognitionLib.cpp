
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
        // -- code here will execute just before each test ensues
        actionRecognition = new DtwActionRecognition;
    }

    virtual void TearDown()
    {
        // -- code here will be called just after each test completes
        // -- ok to through exceptions from here if need be
        delete actionRecognition;
        actionRecognition = 0;
    }

protected:

    /** ActionRecognition object. */
    ActionRecognition* actionRecognition;
};


TEST_F( TestActionRecognitionLib, TestActionRecognitionLib_DTW_setGeneralized)
{
    std::vector<std::vector<double>> generalizedRandom;

    //-- test Generalized
    generalizedRandom.push_back({27,58,48,35,25});
    generalizedRandom.push_back({24,8,3453,48,89});
    generalizedRandom.push_back({24,58,22,58,47});
    bool ok = actionRecognition->setGeneralized( generalizedRandom );

    ASSERT_TRUE( ok );
}

TEST_F( TestActionRecognitionLib, TestActionRecognitionLib_DTW_compare)
{

    std::vector<std::vector<double>> generalizedRandom;
    std::vector<std::vector<double>> attempVectRandom;

    //-- test Generalized
    generalizedRandom.push_back({27,58,48,35,25});
    generalizedRandom.push_back({24,8,3453,48,89});
    generalizedRandom.push_back({24,58,22,58,47});
    actionRecognition->setGeneralized( generalizedRandom );

    //-- test compare
    attempVectRandom.push_back({23,54, 345, 2, 18794});
    attempVectRandom.push_back({4,58, 324, 12, 7847});

    double discrepancy;
    bool ok = actionRecognition->compare(attempVectRandom,discrepancy);

    ASSERT_TRUE( ok );
}

TEST_F( TestActionRecognitionLib, TestActionRecognitionLib_DTW_zero)
{
    std::vector<std::vector<double>> generalizedForZeroDiscrepancy;
    std::vector<std::vector<double>> attempVectforZeroDiscrepancy;

    //-- test Generalized
    generalizedForZeroDiscrepancy.push_back({27,58,48,35,25});
    generalizedForZeroDiscrepancy.push_back({24,8,3453,12,43});
    actionRecognition->setGeneralized( generalizedForZeroDiscrepancy );

    //-- test compare
    attempVectforZeroDiscrepancy.push_back({27,58,48,35,25});
    attempVectforZeroDiscrepancy.push_back({24,8,3453,12,43});

    double discrepancy;
    actionRecognition->compare(attempVectforZeroDiscrepancy,discrepancy);

    ASSERT_EQ( discrepancy, 0 );
}

TEST_F( TestActionRecognitionLib, TestActionRecognitionLib_DTW_simple)
{

    /*
     * The Matrix cost using euclidean distance is the following one
     *
     *  0    1
     *  |
     *  0 -- 1
     *
     * The optimal path is the one in the figure, which cost is one.
     */

    std::vector<std::vector<double>> generalizedForSimpleDiscrepancy;
    std::vector<std::vector<double>> attempVectforSimpleDiscrepancy;

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

    double discrepancy;
    actionRecognition->compare(attempVectforSimpleDiscrepancy,discrepancy);

    ASSERT_EQ( discrepancy, 1 );

}

}  // namespace teo

