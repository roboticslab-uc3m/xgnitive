
#include "gtest/gtest.h"



#include "ColorDebug.hpp"

namespace teo
{

/**
 * @brief Tests ...
 */
class RecognitionLibTest : public testing::Test // -- inherit the Test class (gtest.h)
{

public:

    virtual void SetUp() {
    // -- code here will execute just before the test ensues
    }

    virtual void TearDown()
    {
    // -- code here will be called just after the test completes
    // -- ok to through exceptions from here if need be
    }

protected:

    /** CAN BUS device. */

};

/*
TEST_F( RecognitionLibTest, TechnosoftIposGetPresencewithReset) // -- we call the class that we want to do the test and we assign it a name
{
    int canId = 0;
    int ret = 0;

    ASSERT_EQ(buffer.id-canId , 0x700);
}
*/
}  // namespace teo

