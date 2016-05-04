
#include "gtest/gtest.h"

#include "ColorDebug.hpp"
#include "DtwActionRecognition.hpp"

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
    std::vector< double > generalized;
    std::vector< double > v2;
    double discrepancy;

    //-- test Generalized
    generalized={27,58,48,35,25};
    bool okg = actionRecognition->setGeneralized( generalized );

    //-- test compare
    v2={23,54, 345, 2, 18794};
    bool okc = actionRecognition->compare(v2,discrepancy);
    ASSERT_TRUE( okg & okc );
}

}  // namespace teo

