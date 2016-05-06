
#include "gtest/gtest.h"

#include "ColorDebug.hpp"
#include "DtwCdgaRecognition.hpp"

namespace teo
{

/**
 * @brief Tests ...
 */
class TestCdgaRecognitionLib : public testing::Test // -- inherit the Test class (gtest.h)
{

public:

    virtual void SetUp() {
        // -- code here will execute just before each test ensues
        cdgaRecognition = new DtwCdgaRecognition;
    }

    virtual void TearDown()
    {
        // -- code here will be called just after each test completes
        // -- ok to through exceptions from here if need be
        delete cdgaRecognition;
        cdgaRecognition = 0;
    }

protected:

    /** CdgaRecognition object. */
    CdgaRecognition* cdgaRecognition;
};


TEST_F( TestCdgaRecognitionLib, TestCdgaRecognitionLib_DTW_setGeneralized)
{
    std::vector<std::vector<double>> generalizedRandom;

    //-- test Generalized
    generalizedRandom.push_back({27,58,48,35,25});
    generalizedRandom.push_back({24,8,3453,48,89});
    generalizedRandom.push_back({24,58,22,58,47});
    bool ok = cdgaRecognition->setGeneralized( generalizedRandom );

    ASSERT_TRUE( ok );
}

TEST_F( TestCdgaRecognitionLib, TestCdgaRecognitionLib_DTW_compare)
{

    std::vector<std::vector<double>> generalizedRandom;
    std::vector<std::vector<double>> attempVectRandom;

    //-- test Generalized
    //Each push_back adds a new row.
    generalizedRandom.push_back({27,58,48,35,25});
    generalizedRandom.push_back({24,8,3453,48,89});
    generalizedRandom.push_back({24,58,22,58,47});
    cdgaRecognition->setGeneralized( generalizedRandom );

    //-- test compare
    attempVectRandom.push_back({23,54, 345, 2, 18794});
    attempVectRandom.push_back({4,58, 324, 12, 7847});

    double discrepancy;
    bool ok = cdgaRecognition->compare(attempVectRandom,discrepancy);

    ASSERT_TRUE( ok );
}

TEST_F( TestCdgaRecognitionLib, TestCdgaRecognitionLib_DTW_zero)
{
    std::vector<std::vector<double>> generalizedForZeroDiscrepancy;
    std::vector<std::vector<double>> attempVectforZeroDiscrepancy;

    //-- test Generalized
    generalizedForZeroDiscrepancy.push_back({27,58,48,35,25});
    generalizedForZeroDiscrepancy.push_back({24,8,3453,12,43});

    cdgaRecognition->setGeneralized( generalizedForZeroDiscrepancy );

    //-- test compare
    attempVectforZeroDiscrepancy.push_back({27,58,48,35,25});
    attempVectforZeroDiscrepancy.push_back({24,8,3453,12,43});

    double discrepancy;
    cdgaRecognition->compare(attempVectforZeroDiscrepancy,discrepancy);

    ASSERT_EQ( discrepancy, 0 );
}

TEST_F( TestCdgaRecognitionLib, TestCdgaRecognitionLib_DTW_simple)
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

    cdgaRecognition->setGeneralized( generalizedForSimpleDiscrepancy );

    //-- test compare
    attempVectforSimpleDiscrepancy.push_back({0});
    attempVectforSimpleDiscrepancy.push_back({0});
    attempVectforSimpleDiscrepancy.push_back({1});
    attempVectforSimpleDiscrepancy.push_back({0});

    double discrepancy;
    cdgaRecognition->compare(attempVectforSimpleDiscrepancy,discrepancy);

    ASSERT_EQ( discrepancy, 1 );

}

}  // namespace teo

