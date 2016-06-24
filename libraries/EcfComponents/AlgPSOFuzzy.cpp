#include "AlgPSOFuzzy.hpp"

#include <iostream>
#include <fstream>


PSOFuzzy::PSOFuzzy()
{
	// define algorithm name
	name_ = "PSOFuzzy";

	areGenotypesAdded_ = false;

	// create selection operators needed
	// in this case, SelBestOp
	selBestOp = static_cast<SelectionOperatorP> (new SelBestOp);
}


// register algorithm parameters
void PSOFuzzy::registerParameters(StateP state)
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


bool PSOFuzzy::initialize(StateP state)
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


bool PSOFuzzy::advanceGeneration(StateP state, DemeP deme)
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
//       c) PSO-FUZZY/Evaluate

    //PSO-Fuzzy Constants
    float b=10; //Emphasis operator. Bigger means giving more importance to best fit sol.
    float a=0.6; //Constant proporcionality. Bigger means bigger threshod. Max=1;
    float omega=0.0005;
    int M=5; //Granule life reward particle addition
    int Num_max_granules = 100; //Numero máximo de granulos anterior 5
    double Threshold = 0;
    double fitness_mean= 0;

    //Update Granules life.
    for(int k=0;k<Granules.size();k++){     
        Granules[k][1] -= 1;
        //std::cout<<" LA VIDA UPDATED DE EL GRANULO ES:::  "<< Granules[k][1]<<std::endl;
    }

    /****************PSO-FUZZY-THRESHOLD*************************/
    for( uint i = 0; i < deme->getSize(); i++ ) {
        IndividualP particle = deme->at(i);
        fitness_mean+= particle->fitness->getValue();
    }

    fitness_mean=fitness_mean/deme->getSize();

    IndividualP bestParticle = selBestOp->select( *deme );
    if(bestParticle->fitness->getValue()!=0){
        Threshold=a*(fitness_mean/(bestParticle->fitness->getValue())); // fitness/bestfitness (min=1)
    }
    else{
        Threshold=100000;
    }

    std::cout<<"======================================EL THRESHOLD ES=========================================================================================: "<<Threshold<<std::endl;

    /****************PSO-FUZZY-THRESHOLD_END*************************/

    for( uint i = 0; i < deme->getSize(); i++ ) { // for each particle


        IndividualP particle = deme->at(i); //Read "i" particle
        std::cout<<particle->toString()<<std::endl;

		// the whole point of this section is to compare fitness and pbest
        FloatingPointP flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(3));
        double &particlePbestFitness = flp->realValue[0];
        double fitness = particle->fitness->getValue();

        //There is a problem with the particlePbestFitness initialization in this algorithm. The following lines take care of this.
        //TODO: Find a way to do this in the ¿initialize fuction?.
        if(state->getGenerationNo()==1){
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

        /*************INIT-FIRST-GRANULE**************************************/
        //TODO: Find a way to do this in the ¿initialize fuction?.
        if(state->getGenerationNo()==1 && i==0){ //Init First Granule
            //Initialize the first granule to the first particle
            FloatingPointP flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(0));
            std::vector< double > &positions = flp->realValue;

            //aux vector to pushback Granule vector into Granules vector of vector
            std::vector<double> aux_vect;
            aux_vect.push_back (0);
            aux_vect.push_back (1);

            evaluate(particle); //Real fitness of the first particle
            aux_vect.push_back(particle->fitness->getValue());

            for(int k=0;k<positions.size();k++){
                aux_vect.push_back(positions[k]);
            }

            Granules.push_back (aux_vect);
            std::cout<<"FIRST GRANULE INITIALIZED. Its life is:  "<<Granules[0][1]<<std::endl;

        }
        /*********************************************************************/

        /************PSO-FUZZY************************************************/
        else{
            std::vector<double> similarity; //similarity (mu mean) between particle and granule
            double max_similarity = -10000;
            int Granule_index;
            for(int k=0;k<Granules.size();k++){ //Compare with all Granules
                similarity.push_back(0); //Allocate similarity vector
                //std::cout<<"Granules fitness for lambda es -->"<<Granules[k][2]<<std::endl;
                //double lambda=omega*1/pow(exp(-Granules[k][2]/230),b); //Gaussian variance no normalized. The bigger this value, the bigger the granule.
                double lambda=omega*1/pow(exp(-Granules[k][2]/230),b); //Gaussian variance no normalized. The bigger this value, the bigger the granule.
                //lambda=1;
                //std::cout<<"LAMBDA ES::::::::::::> "<<lambda<<std::endl;

                for(int l=0;l<positions.size();l++){
                   similarity [k] += (exp(-pow((positions[l]-Granules[k][l+3]),2)/pow(lambda,2))); //similarity value (Gaussian)
                   //std::cout<<"La posicion de la particula es   "<<positions[l]<<"  la posición del Granulo es "<< Granules[k][l+3]<<std::endl;

                }

                similarity[k]= similarity[k]/(positions.size());


                //std::cout<<"Similarity is:::"<<similarity[k]<<std::endl;
                //std::cout<<"Granule life is =  "<<Granules[k][1]<<std::endl;

                if (similarity[k]>max_similarity){ //Update best granule (most similar)
                    max_similarity=similarity[k];
                    Granule_index=k;
                }
            }

            //std::cout<<"LA SIMILARIDAD ES   "<<max_similarity<<std::endl;

            if (max_similarity>Threshold){
                particle->fitness->setValue(Granules[Granule_index][2]);
                Granules[Granule_index][1]+=M;
                //std::cout<<"Particle added to Granule:  "<<Granule_index<<" With Fitness " <<Granules[Granule_index][2]<<"  The life of this Granule is: "<<Granules[Granule_index][1]<<std::endl;
            }

            else{
                //Generating new granule and evaluating the particle
                FloatingPointP flp = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (particle->getGenotype(0));
                std::vector< double > &positions = flp->realValue;

                //aux vector to pushback "Granule vector" into "Granules vector of vector"
                std::vector<double> aux_vect;
                aux_vect.push_back (i); //Index of the leader particle for this granule (Maybe this is not useful)
                aux_vect.push_back (1); //Life of the Granule

                evaluate(particle); //Real fitness of the first particle
                aux_vect.push_back(particle->fitness->getValue());

                for(int l=0;l<positions.size();l++){
                    aux_vect.push_back(positions[l]);
                }

                Granules.push_back (aux_vect);
                //std::cout<<" New granule added "<<std::endl;
            }

            /************************************************************************/

        }

        /*****************************Update Granules life table*****************/
        if(Granules.size()>Num_max_granules){   //If we have more granules than the max number
            //std::cout<<"El número de Granulos es: "<<Granules.size();
            while (Granules.size()>Num_max_granules){   //Reduce the number to the max
                int WGIndex=0;
                for(int m=1;m<Granules.size();m++){ //Find the worst granule (less live) in the "table"
                    if(Granules[m][1]<Granules[WGIndex][1]){
                        WGIndex=m;
                    }
                }
                //Delete the worst Granule (rows)

                Granules.erase( Granules.begin() + WGIndex );
                //std::cout<<" El granulo numero " << WGIndex << " Con life "<<Granules[WGIndex][1]<<" Ha sido borrado "<<std::endl;

                //std::cout<<"El número de Granulos tras reducir es: "<<Granules.size();
            }

        }

        /************************************************************************/
    }
    //std::cout<<std::endl<<"THE NUMBER OF THIS GENERATION IS:"<<state->getGenerationNo() <<std::endl;
    std::cout<<std::endl<<"THE NUMBER OF EVALUATIONS IS:"<<state->getEvaluations() <<std::endl;
    std::cout<<std::endl<<"THE TIME TAKEN TO DO THIS IS:"<<state->getElapsedTime() <<std::endl;

    //*******************FILE OUTPUT FOR DEBUGGING***********************************************//
    //IndividualP bestParticle = selBestOp->select( *deme );

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
