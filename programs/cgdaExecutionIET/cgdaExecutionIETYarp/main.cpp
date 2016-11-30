// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * @ingroup xgnitive_programs
 * \defgroup cgdaExecutionIETYarp cgdaExecutionIETYarp
 *
 * @brief Executes the cgda system in teoSim or the real robot using Yarp. An Incrementally Evolved Trajectories (IET) strategy is also used in this case.
 *
 * @section cgdaExecutionIETYarp_legal Legal
 *
 * Copyright: 2016 (C) Universidad Carlos III de Madrid
 *
 * Author: raulfdzbis, jgvictores
 *
 * CopyPolicy: This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 3.0 or later
 *
 * <hr>
 *
 * This file can be edited at cgdaExecutionIETYarp
 *
 */

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaExecutionIET.hpp"

int main(int argc, char **argv) {

    teo::CgdaExecutionIET mod;
    return mod.init();
}

