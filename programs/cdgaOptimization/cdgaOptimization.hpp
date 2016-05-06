// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CDGA_OPTIMIZATION_HPP__
#define __CDGA_OPTIMIZATION_HPP__

#include <yarp/os/all.h>

namespace teo
{

/**
 * @ingroup cdgaOptimization
 *
 * @brief cdgaOptimization
 *
 */
class cdgaOptimization : public yarp::os::RFModule {
    public:
        bool configure(yarp::os::ResourceFinder &rf);

    protected:

        virtual bool interruptModule();
        virtual double getPeriod();
        virtual bool updateModule();

};

}  // namespace teo

#endif  // __CDGA_OPTIMIZATION_HPP__
