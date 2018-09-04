//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors:raulfdzbis, jgvictores,  smorante

#ifndef Constrained_SST_h
#define Constrained_SST_h

#include "ECF_base.h"
#include "ECF_macro.h"

#include "SelRandomOp.h"
#include "SelWorstOp.h"
#include "floatingpoint/FloatingPoint.h"

#include "Algorithm.h"

/**
 *
 * @ingroup xgnitive_libraries
 * \defgroup ECFComponents
 * @brief Contains xgnitive algorithms added to ECF.
 */

/**
 * \defgroup algConstrainedSST
 * @ingroup ECFComponents
 *
 *
 * @brief Steady State Tournament using spatial and velocity constraints.
 *
 * @section algConstrainedSST_legal Legal
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
 * This file can be edited at algConstrainedSST
 *
 */

//*
// * \brief Steady state algorithm with tournament elimination operator
// * \ingroup algorithms serial
// *
// * This algorithm is genotype independent (it can be used with any Genotype).
// *
// * The algorithm flow:
// *
// \verbatim
//  single generation {
//   repeat(deme size times) {
//    randomly add <nTournament_> individuals to the tournament;
//    select the worst one in the tournament;
//    randomly select two parents from remaining ones in the tournament;
//    replace the worst with crossover child;
//    perform mutation on child;
//   }
//  }
// \endverbatim
// *

class ConstrainedSST : public Algorithm
{
public:
	ConstrainedSST();
	bool advanceGeneration(StateP state, DemeP deme);
	bool initialize(StateP state);
	void registerParameters(StateP state);

protected:
	uint nTournament_;	//!< tournament size
	SelectionOperatorP selRandomOp, selWorstOp;

};
typedef boost::shared_ptr<ConstrainedSST> ConstrainedSSTP;

#endif // Constrained_SST_h

