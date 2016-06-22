#include "AlgPSOInheritance.hpp"

#include <iostream>
#include <fstream>


PSOInheritance::PSOInheritance()
{
	// define algorithm name
	name_ = "PSOInheritance";

	areGenotypesAdded_ = false;

	// create selection operators needed
	// in this case, SelBestOp
	selBestOp = static_cast<SelectionOperatorP> (new SelBestOp);
}


// register algorithm parameters
void PSOInheritance::registerParameters(StateP state)
{
	registerParameter(state, "weightType", (voidP) new InertiaWeightType(CONSTANT), ECF::INT,
		"weight type update: 0 - constant, 1 - time dependant (based on max generations)");
	registerParameter(state, "weight", (voidP) new double(0.8), ECF::DOUBLE,
		"initial inertia weight (either constant or time dependant)");
	registerParameter(state, "maxVelocity", (voidP) new double(10.0), ECF::DOUBLE,
		"max particle velocity");
	registerParameter(state, "bounded", (voidP) new uint(0), ECF::UINT, 
		"should the algorithm respect the bounds defined in the genotype or not (default: 0)");
}


bool PSOInheritance::initialize(StateP state)
{
	// initialize all operators
	selBestOp->initialize(state);

	voidP weightType = getParameterValue(state, "weightType");
	m_weightType = *((InertiaWeightType*) weightType.get());

	voidP weight = getParameterValue(state, "weight");
	m_weight = *((double*) weight.get());

	voidP maxV = getParameterValue(state, "maxVelocity");
	m_maxV = *((double*) maxV.get());

	// test if inertia weight type is time variant and if so, check if max iterations specified
	if(m_weightType == TIME_VARIANT) {
		if(state->getRegistry()->isModified("term.maxgen")) {
			// read maxgen parameter
			m_maxIter = *(boost::static_pointer_cast<int>( state->getRegistry()->getEntry("term.maxgen") ));
		}
		else {
			ECF_LOG_ERROR(state, "Error: term.maxgen has to be specified in order to use time variant inertia eight in PSO algorithm");
			throw("");
		}
	}

	// algorithm accepts a single FloatingPoint Genotype
	FloatingPointP flp (new FloatingPoint::FloatingPoint);
	if(state->getGenotypes()[0]->getName() != flp->getName()) {
		ECF_LOG_ERROR(state, "Error: PSO algorithm accepts only a single FloatingPoint genotype!");
		throw ("");
	}

	voidP sptr = state->getGenotypes()[0]->getParameterValue(state, "dimension");
	uint numDimension = *((uint*) sptr.get());

	voidP bounded = getParameterValue(state, "bounded");
	bounded_ = *((bool*) bounded.get());

	sptr = state->getGenotypes()[0]->getParameterValue(state, "lbound");
	lbound_ = *((double*) sptr.get());

	sptr = state->getGenotypes()[0]->getParameterValue(state, "ubound");
	ubound_ = *((double*) sptr.get());

	// batch run check
	if(areGenotypesAdded_)
		return true;

	FloatingPointP flpoint[4];
	for(uint iGen = 1; iGen < 4; iGen++) {

		flpoint[iGen] = (FloatingPointP) new FloatingPoint::FloatingPoint;
		state->setGenotype(flpoint[iGen]);

		if(iGen == 3)
			flpoint[iGen]->setParameterValue(state, "dimension", (voidP) new uint(1));
		else
			flpoint[iGen]->setParameterValue(state, "dimension", (voidP) new uint(numDimension));

		// other parameters are proprietary (ignored by the algorithm)
		flpoint[iGen]->setParameterValue(state, "lbound", (voidP) new double(0));
		flpoint[iGen]->setParameterValue(state, "ubound", (voidP) new double(1));
	}
	ECF_LOG(state, 1, "PSO algorithm: added 3 FloatingPoint genotypes (particle velocity, best-so-far postition, best-so-far fitness value)");

	// mark adding of genotypes
	areGenotypesAdded_ = true;

	return true;
}


bool PSOInheritance::advanceGeneration(StateP state, DemeP deme)
{
//       a) For each particle:
//          1) If the fitness value is better than the best fitness value (pBest) in  history
//          2) Set current value as the new pBest
//          3) Put particle in the ranking array using the fitness value
//          End
//       b) For the p best particles in the ranking arrayºº
//          1) Find, in the particle neighborhood, the particle with the best fitness
//          2) Calculate particle velocity according to the velocity equation (1)
//          3) Apply the velocity constriction
//          4) Update particle position according to the position equation (2)
//          5) Apply the position constriction
//       c) Inherit // Evaluate
//          1) Get the fitness value via evaluation or inheritance.
//          End

	for( uint i = 0; i < deme->getSize(); i++ ) { // for each particle 
        IndividualP particle = deme->at(i);                                                                             //Read "i" particle

		// the whole point of this section is to compare fitness and pbest
        FloatingPointP flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(3));
        double &particlePbestFitness = flp->realValue[0];
        double fitness = particle->fitness->getValue();

        //There is a problem with the particlePbestFitness initialization in this algorithm. The following lines take care of this.
        //TODO: Find a way to do this in the ¿initialize fuction?.
        if(state->getGenerationNo()==1){
            //std::cout<<"INCIALIZADOOOOOOOOOOOOO!!!!!!!!!!!!!!!!!"<<std::endl;
            particlePbestFitness=fitness;
        }
        else{
            //std::cout<<"FITNESS"<<fitness<<std::endl;

            flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(0));
            std::vector< double > &positions = flp->realValue;

            flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(2));
            std::vector< double > &pbestx = flp->realValue;

            // set particle pbestx-es
            if( /*iter == 0 ||*/ fitness < particlePbestFitness ) { // minimize error
                particlePbestFitness = fitness; //Update particle personal fitness

                // set pbestx-es
                for( uint j = 0;j<pbestx.size();j++ ) {
                    pbestx[j] = positions[j];
                }
            }
        }

		// NOTE store best particle index?
        //std::cout<<"THE PBEST OF THIS PARTICLE IS!!!!!!!!!!!!:"<<particlePbestFitness<<std::endl;
	}

	// b)
	for( uint i = 0; i < deme->getSize(); i++ ) { // for each particle
		IndividualP particle = deme->at(i);

		IndividualP bestParticle = selBestOp->select( *deme );

		FloatingPointP flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(0));
		std::vector< double > &positions = flp->realValue;

		flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(1));
		std::vector< double > &velocities = flp->realValue;

		flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(2));
        std::vector< double > &pbestx = flp->realValue;

        double R1=rand()/(float)RAND_MAX, R2=rand()/(float)RAND_MAX, vf;
        int C1=2, C2=2;



        double weight_up;

		switch( m_weightType )
		{
			//time variant weight, linear from weight to 0.4
			case TIME_VARIANT:
			weight_up = ( m_weight - 0.4 ) * ( m_maxIter - state->getGenerationNo() ) / m_maxIter + 0.4;
			break;

			// constant inertia weight
			case CONSTANT:
            default:
			weight_up = m_weight;
			break;
		}
		// calculate particle velocity according to the velocity equation (1)
		flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (bestParticle->getGenotype(2));
		std::vector< double > &bestParticlesPbestx = flp->realValue;
		for( uint j = 0; j < velocities.size(); j++ ) {
			double velocity;

			velocity = weight_up * velocities[j] +
               2 * R1 * (pbestx[j] - positions[j]) +
               2 * R2 * (bestParticlesPbestx[j] - positions[j]);

			if( velocity > m_maxV ) velocity = m_maxV;
            if( velocity < -m_maxV) velocity = -m_maxV;
			velocities[j] = velocity;

            positions[j] += velocities[j];      //Updated positions with velocitites X(t+1)=X(t)+velocities(t);
			// TODO apply position constriction

			// check for bounds
            if(bounded_) {
				if(positions[j] < lbound_)
					positions[j] = lbound_;
				if(positions[j] > ubound_)
					positions[j] = ubound_;
			}

            //std::cout<<"LA VELOCIDAD ES::::"<<velocity<<std::endl;
		}

        int proportion=40;
        if(rand()%100>=proportion){
            //Initial PSO inheritance algorithm -> 100%inheritance no evaluations.
            //determine new particle fitness

            //Particle best personal fitness
//            flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(3));
//            double &particlePbestFitness = flp->realValue[0];

            //Best particle fitness
//            flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (bestParticle->getGenotype(3));
//            double &bestparticlePbestFitness = flp->realValue[0];

//            vf=(C1*R1*(particlePbestFitness-particle->fitness->getValue())+C2*R2*(bestparticlePbestFitness-particle->fitness->getValue()))
//                    /(1+C1*R1+C2*R2);

//            vf=vf+particle->fitness->getValue();



            evaluate( particle );

            //std::cout<< " Inherited: " << vf<< " -> Evaluated "<< particle->fitness->getValue() <<std::endl ;

        }
        else{
            //Particle best personal fitness
            flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(3));
            double &particlePbestFitness = flp->realValue[0];

            //Best particle fitness
            flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (bestParticle->getGenotype(3));
            double &bestparticlePbestFitness = flp->realValue[0];

            //std::cout<<std::endl<<"The EverPersonalBEST of this particle is:"<<particlePbestFitness<<std::endl;
            //std::cout<<std::endl<<"THE present LEADER(Allbes) of this particle fitness is:"<<bestparticlePbestFitness<<std::endl;

            //Inheritance based on flight formula
            std::cout<<"Particle Fitness"<<particle->fitness->getValue()<<" "<<std::endl;


            //Fitness inheritance
            vf=(C1*R1*(particlePbestFitness-particle->fitness->getValue())+C2*R2*(bestparticlePbestFitness-particle->fitness->getValue()))
                    /(1+C1*R1+C2*R2);
            particle->fitness->setValue(vf+particle->fitness->getValue());

            std::cout<< " Inherited: " << particle->fitness->getValue()<< " "<< std::endl ;
        }

	}

    //std::cout<<std::endl<<"THE NUMBER OF THIS GENERATION IS:"<<state->getGenerationNo() <<std::endl;
    std::cout<<std::endl<<"THE NUMBER OF EVALUATIONS ARE:"<<state->getEvaluations() <<std::endl;
    std::cout<<std::endl<<"THE TIME TAKEN TO DO THIS IS:"<<state->getElapsedTime() <<std::endl;

    //*******************FILE OUTPUT FOR DEBUGGING***********************************************//
    IndividualP bestParticle = selBestOp->select( *deme );

    FloatingPointP flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (bestParticle->getGenotype(3));

    double &bestparticlePbestFitness = flp->realValue[0];
    std::ofstream myfile1;
    myfile1.open("FitnessvsEvaluations.txt", std::ios_base::app);
    if (myfile1.is_open()){
        myfile1<<bestparticlePbestFitness<<" ";
        myfile1<<state->getEvaluations()<<std::endl;
    }
    //*******************************************************************************************//

	return true;
}
