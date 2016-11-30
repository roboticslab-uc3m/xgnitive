//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors:raulfdzbis, jgvictores,  smorante

/**
 *
 * \defgroup algPSOInheritance
 * @ingroup ECFComponents
 *
 * @brief Particle Swarm Optimization Inheritance approach.
 *
 * @section algPSOInheritance_legal Legal
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
 * This file can be edited at algPSOInheritance
 *
 */

#ifndef Particle_Swarm_Optimization_inheritance_h
#define Particle_Swarm_Optimization_inheritance_h

#include "ECF_base.h"
#include "ECF_macro.h"

#include "Algorithm.h"
#include "floatingpoint/FloatingPoint.h"


/**
 * \brief Particle swarm optimization algorithm  with inheritance (see e.g. http://en.wikipedia.org/wiki/Particle_swarm_optimization)
 * \ingroup algorithms serial
 * 
 * PSO algorithm accepts only a single FloatingPoint genotype (vector of real values).
 * Additionally, it adds the following genotypes for algorithm implementation:
 * - FloatingPoint genotype as particle velocity
 * - FloatingPoint genotype as best-so-far position
 * - FloatingPoint genotype as best-so-far fitness value
 */
class PSOInheritance : public Algorithm
{
public:
	PSOInheritance();

	enum InertiaWeightType
	{
		CONSTANT,
		TIME_VARIANT
	};

	bool advanceGeneration(StateP state, DemeP deme);
	bool initialize(StateP state);
	void registerParameters(StateP state);

protected:
	SelectionOperatorP selBestOp;

	int m_maxIter;
	InertiaWeightType m_weightType;
	double m_weight;
	double m_maxV;
	bool areGenotypesAdded_;
	bool bounded_;	//!< constrained or not
	double lbound_, ubound_;
};
typedef boost::shared_ptr<PSOInheritance> PSOInheritanceP;

#endif // Particle_Swarm_Optimization_h

