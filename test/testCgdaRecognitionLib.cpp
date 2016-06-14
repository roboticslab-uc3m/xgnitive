
#include "gtest/gtest.h"

#include "ColorDebug.hpp"
#include "DtwCgdaRecognition.hpp"

namespace teo
{

/**
 * @brief Tests ...
 */
class TestCgdaRecognitionLib : public testing::Test // -- inherit the Test class (gtest.h)
{

public:

    virtual void SetUp() {
        // -- code here will execute just before each test ensues
        cgdaRecognition = new DtwCgdaRecognition;
    }

    virtual void TearDown()
    {
        // -- code here will be called just after each test completes
        // -- ok to through exceptions from here if need be
        delete cgdaRecognition;
        cgdaRecognition = 0;
    }

protected:

    /** CgdaRecognition object. */
    CgdaRecognition* cgdaRecognition;
};


TEST_F( TestCgdaRecognitionLib, TestCgdaRecognitionLib_DTW_setGeneralized)
{
    std::vector<std::vector<double>> generalizedRandom;

    //-- test Generalized
    generalizedRandom.push_back({27,58,48,35,25});
    generalizedRandom.push_back({24,8,3453,48,89});
    generalizedRandom.push_back({24,58,22,58,47});
    bool ok = cgdaRecognition->setGeneralized( generalizedRandom );

    ASSERT_TRUE( ok );
}

TEST_F( TestCgdaRecognitionLib, TestCgdaRecognitionLib_DTW_compare)
{

    std::vector<std::vector<double>> generalizedRandom;
    std::vector<std::vector<double>> attempVectRandom;

    //-- test Generalized
    //Each push_back adds a new row.
    //The val in a push back is a column.
    //generalizedRandom.push_back({0,10,40,80,100});
    //generalizedRandom.push_back({24,8,3453,48,89});
    //generalizedRandom.push_back({24,58,22,58,47});
    generalizedRandom.push_back({0});
    generalizedRandom.push_back({10});
    generalizedRandom.push_back({40});
    generalizedRandom.push_back({80});
    generalizedRandom.push_back({100});
    cgdaRecognition->setGeneralized( generalizedRandom );

    //-- test compare
    attempVectRandom.push_back({0});
    attempVectRandom.push_back({12.25});
    attempVectRandom.push_back({37.5});
    attempVectRandom.push_back({75});
    attempVectRandom.push_back({100});
    //attempVectRandom.push_back({0,13, 43, 2, 18794});
    //attempVectRandom.push_back({4,58, 324, 12, 7847});

    double discrepancy;
    bool ok = cgdaRecognition->compare(attempVectRandom,discrepancy);

    ASSERT_TRUE( ok );
}

TEST_F( TestCgdaRecognitionLib, TestCgdaRecognitionLib_DTW_zero)
{
    std::vector<std::vector<double>> generalizedForZeroDiscrepancy;
    std::vector<std::vector<double>> attempVectforZeroDiscrepancy;

    //-- test Generalized
    generalizedForZeroDiscrepancy.push_back({27,58,48,35,25});
    generalizedForZeroDiscrepancy.push_back({24,8,3453,12,43});

    cgdaRecognition->setGeneralized( generalizedForZeroDiscrepancy );

    //-- test compare
    attempVectforZeroDiscrepancy.push_back({27,58,48,35,25});
    attempVectforZeroDiscrepancy.push_back({24,8,3453,12,43});

    double discrepancy;
    cgdaRecognition->compare(attempVectforZeroDiscrepancy,discrepancy);

    ASSERT_EQ( discrepancy, 0 );
}

TEST_F( TestCgdaRecognitionLib, TestCgdaRecognitionLib_DTW_simple)
{

    /*
     * The Matrix cost using euclidean distance is the following one.
     *  ------------>Generalized
     *  |  0    0    1
     *  |
     *  |  0    1    1
     *  |     \
     *  |  1    0    0
     *  |          \
     *  |  0    1    1
     *  "
     * Attemp
     *
     * The optimal path is the one in the figure, wich corresponds with the lower cost
     * of aligning both signals, in this case is one.
     */

    std::vector<std::vector<double>> generalizedForSimpleDiscrepancy;
    std::vector<std::vector<double>> attempVectforSimpleDiscrepancy;

    //-- test Generalized
    generalizedForSimpleDiscrepancy.push_back({0});
    generalizedForSimpleDiscrepancy.push_back({1});
    generalizedForSimpleDiscrepancy.push_back({1});

    cgdaRecognition->setGeneralized( generalizedForSimpleDiscrepancy );

    //-- test compare
    attempVectforSimpleDiscrepancy.push_back({0});
    attempVectforSimpleDiscrepancy.push_back({0});
    attempVectforSimpleDiscrepancy.push_back({1});
    attempVectforSimpleDiscrepancy.push_back({0});

    double discrepancy;
    cgdaRecognition->compare(attempVectforSimpleDiscrepancy,discrepancy);

    ASSERT_EQ( discrepancy, 1 );

}

}  // namespace teo

