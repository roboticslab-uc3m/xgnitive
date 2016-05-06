// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CGDA_OPTIMIZATION_HPP__
#define __CGDA_OPTIMIZATION_HPP__

#include <yarp/os/all.h>

namespace teo
{

/**
 * @ingroup cgdaOptimization
 *
 * @brief CgdaOptimization
 *
 */
class CgdaOptimization : public yarp::os::RFModule {
    public:
        bool configure(yarp::os::ResourceFinder &rf);

    protected:

        virtual bool interruptModule();
        virtual double getPeriod();
        virtual bool updateModule();

};

}  // namespace teo

#endif  // __CGDA_OPTIMIZATION_HPP__
