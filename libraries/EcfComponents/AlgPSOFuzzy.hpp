//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors:raulfdzbis, jgvictores,  smorante

#ifndef Particle_Swarm_Optimization_fuzzy_h
#define Particle_Swarm_Optimization_fuzzy_h

#include "ECF_base.h"
#include "ECF_macro.h"

#include "Algorithm.h"
#include "floatingpoint/FloatingPoint.h"

#include <cmath>

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
class PSOFuzzy : public Algorithm
{
public:
	PSOFuzzy();

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
    //PSO+Fuzzy. The first coordinate (rows) are the granules. Each the columns are the properties.
    //In a granule (i): G[i][0]=Index of the leader particle, Gk[i][1]=Life of the granule, Gk[i][2]=Fitness Granule. The rest are characteristics.
    std::vector< std::vector<double> > Granules;
};
typedef boost::shared_ptr<PSOFuzzy> PSOFuzzyP;

#endif // Particle_Swarm_Optimization_h

