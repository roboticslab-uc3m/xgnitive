//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors:raulfdzbis, jgvictores,  smorante

#include "AlgConstrainedSST.hpp"


ConstrainedSST::ConstrainedSST()
{
	// define algorithm name
	name_ = "ConstrainedSST";

	// create selection operators needed
	// in this case, SelRandomOp and SelWorstOp
	selRandomOp = static_cast<SelectionOperatorP> (new SelRandomOp);
	selWorstOp = static_cast<SelectionOperatorP> (new SelWorstOp);
}


void ConstrainedSST::registerParameters(StateP state)
{
	registerParameter(state, "tsize", (voidP) new uint(3), ECF::UINT,
		"tournament size (individuals selected randomly, worst one eliminated)");
}


bool ConstrainedSST::initialize(StateP state)
{
    //Initialize all operators
	selRandomOp->initialize(state);
	selWorstOp->initialize(state);

	// read parameter values 
	voidP tsizep = getParameterValue(state, "tsize");
	nTournament_ = *((uint*) tsizep.get());

	if(nTournament_ < 3) {
		ECF_LOG(state, 1, "Error: ConstrainedSST algorithm requires minimum tournament size of 3!");
        throw "";
	}

	return true;
}


bool ConstrainedSST::advanceGeneration(StateP state, DemeP deme)
{
    //! this algorithm: one _generation_ is 'deme->size()' _iterations_
	for(uint iIter = 0; iIter < deme->size(); iIter++) {

		ECF_LOG(state, 5, "Individuals in tournament: ");

		std::vector<IndividualP> tournament;
		for (uint i = 0; i < nTournament_; ++i) {
			// select a random individual for the tournament
			tournament.push_back(selRandomOp->select(*deme));
			ECF_LOG(state, 5, uint2str(i) + ": " + tournament[i]->toString());
		}

		// select the worst from the tournament
		IndividualP worst = selWorstOp->select(tournament);
		ECF_LOG(state, 5, "The worst from the tournament: " + worst->toString());

		// remove pointer to 'worst' individual from vector 'tournament'
		removeFrom(worst, tournament);

        //Lets limit the variation after mutation (velocity)
        FloatingPointP flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (tournament[0]->getGenotype(0));
        std::vector< double > &positions_0=flp->realValue;

        flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (tournament[1]->getGenotype(0));
        std::vector< double > &positions_1=flp->realValue;

		// crossover the first two (remaining) individuals in the tournament
		mate(tournament[0], tournament[1], worst);

		// perform mutation on new individual
		mutate(worst);

        flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (worst->getGenotype(0));
        std::vector< double > &positions_new=flp->realValue;

        //Calculate angular velocity between generations
        std::vector<double> velocity_0, velocity_1;
        //velocity.push_back(0);
        //velocity.push_back(0);
        for( uint j = 0; j < positions_0.size(); j++ ) {

            //velocity[0] += abs(positions_new[j]-positions_0[j]);
            //velocity[1] += abs(positions_new[j]-positions_1[j]);
            velocity_0.push_back(abs(positions_new[j]-positions_0[j]));
            velocity_1.push_back(abs(positions_new[j]-positions_1[j]));
            std::cout<<"Velocidad  - "<<velocity_0[j]<<std::endl;

        }

        //std::cout<<"VELOCIDAD:::"<< velocity[0]<<" "<< velocity[1]<<" " <<std::endl;

//        if(velocity[0]<120||velocity[1]<120){
//            // create new fitness
//            evaluate(worst);
//            ECF_LOG(state, 5, "New individual: " + worst->toString());
//        }

        if((velocity_0[0]<30 && velocity_0[1]<30 && velocity_0[2]<30) || (velocity_1[0]<30 && velocity_1[1]<30 && velocity_1[2]<30)){
            // create new fitness
            evaluate(worst);
            ECF_LOG(state, 5, "New individual: " + worst->toString());
        }
        else{
            std::cout<<"***************************************VELOCITY LIMITED********************************"<<std::endl;
            worst->fitness->setValue(10000);
        }

        //std::cout<<"FITNESS WORST"<<worst->fitness->getValue()<<std::endl;
	}
	return true;
}
