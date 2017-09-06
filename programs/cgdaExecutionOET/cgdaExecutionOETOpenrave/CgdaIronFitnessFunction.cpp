// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaIronFitnessFunction.hpp"

#define NTPOINTS 17
#define NFEATURES 6
#define NSQUARES 16

namespace teo
{

/************************************************************************/

void CgdaIronFitnessFunction::registerParameters(StateP state) {
	state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaIronFitnessFunction::initialize(StateP state) {

    voidP sptr = state->getRegistry()->getEntry("function"); // get parameter value

	return true;
}

/************************************************************************/

FitnessP CgdaIronFitnessFunction::evaluate(IndividualP individual) {
    // Evaluation creates a new fitness object using a smart pointer
	// in our case, we try to minimize the function value, so we use FitnessMin fitness (for minimization problems)
	FitnessP fitness (new FitnessMin);

	// we define FloatingPoint as the only genotype (in the configuration file)
	FloatingPoint::FloatingPoint* gen = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();

	// we implement the fitness function 'as is', without any translation
	// the number of variables is read from the genotype itself (size of 'realValue' vactor)

    double value =0;
    value= getCustomFitness(gen->realValue);
    fitness->setValue(value);
    return fitness;
}

/************************************************************************/

double CgdaIronFitnessFunction::getCustomFitness(vector <double> genPoints){

    //***********************Define the generalized feature trajectory**********************************************//

    std::vector<std::vector<double>> target;
    //double target[NFEATURES][NTPOINTS];



//    int a[3][4] = {
//       {0, 1, 2, 3} ,   /*  initializers for row indexed by 0 */
//       {4, 5, 6, 7} ,   /*  initializers for row indexed by 1 */
//       {8, 9, 10, 11}   /*  initializers for row indexed by 2 */
//    };

    //This is sqFeatures

    //target.push_back(feature1T1,feature2T2,feature3T3...);

    //*****************************************OBSERVATION STEP*****************************************************//

    std::vector<double> observation;

    //READ FROM PsqFeatures
    for(int i=0;i<psqFeatures->size();i++)
    {
        observation.push_back(psqFeatures->operator [](i));
    }

    //Obtain TEO state
    //P:yarp::os::Bottle cmd3,res3;
    //P:cmd3.addString("reset");
    //P:pRpcClient->write(cmd3,res3);

    //P:double percentage;
    //P:double Npaint=0;

    //P:for(int i=0;i<psqFeatures->size();i++)
    //P:{
    //P:    Npaint += psqFeatures->operator [](i);
    //P: }

    //Percentage of the wall painted before evolution
    //P:printf("EL NPAINT ES::::::: %f \n",Npaint);
    //P:percentage=(Npaint/NSQUARES)*100;
    //P:printf("EL PERCENTAGE ES::::::: %f \n",percentage);

    //***************************************LOCALIZATION STEP******************************************************//

    //Serch the timeStep where we are (LOCALIZATION STEP)
    int timeStep;
    double diff=1000000;
    for(int i=0;i<NFEATURES;i++){
        double aux_dist=0;
        double aux_elem;
        for(int j=0;j<NTPOINTS;j++){ //We give priority to the elements at the last positions
            aux_elem=observation[i]-target[i][j];
            aux_elem=aux_elem*aux_elem;
            aux_dist=aux_dist+aux_elem;
        }
        //printf("Percentage: %d \n",percentage);
        //printf("Target: %d \n", Const_target[i]);
        //P:diff_aux=percentage-Const_target[i];


        //P:diff_aux=diff_aux*diff_aux;
        //P: diff_aux=sqrt(diff_aux);

        aux_dist=sqrt(aux_dist);

        if(aux_dist<diff){
            timeStep=i;
            diff=aux_dist;
            //printf("i -> %d", i);
            //printf("Time Step %d \n",timeStep);

        }
    }

    //********************************SIMULATED EXECUTION STEP******************************************************//

    //Set new positions of the robot using dEncRaw
    std::vector<double> dEncRaw(6);  // NUM_MOTORS
    dEncRaw[0] = genPoints[0];  // simple
    dEncRaw[1] = -genPoints[1];  // simple
    dEncRaw[3] = genPoints[2];  // simple

    dEncRaw[4] = 45; //TODO:This must be changed for IRON

    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw.data());


    //********************************SECOND OBSERVATION STEP*******************************************************//

    observation.clear();

    //FORCE

    yarp::os::Bottle* b = pForcePort->read(false);
    if(!b)
    {
        printf("No force yet\n");
    }
    printf("El parámetro del sensor de fuerza es %s\n", b->toString().c_str());
    for(size_t i=0; i<b->size(); i++)
    {
        observation.push_back( b->get(i).asDouble() );
        //std(observation[i]);
    }

    //POSITION

    yarp::os::Bottle cmd,res;
    cmd.addString("world");
    cmd.addString("whereis");
    cmd.addString("tcp");
    cmd.addString("rightArm");
    pRpcClientWorld->write(cmd,res);
    /*printf("El parámetro de posicion es %s\n", res.toString().c_str());
    for(size_t i=0; i<res.size(); i++)
    {
        observation.push_back( res.get(i).asDouble() );
        //std(observation[i]);
    }*/
    yarp::os::Bottle* pos = res.get(0).asList();
    printf("El parámetro de posicion es %s\n", pos->toString().c_str());
    for(size_t i=0; i<pos->size(); i++)
    {
        observation.push_back( pos->get(i).asDouble() );
        //std(observation[i]);
    }

    //P:Npaint=0;
    //P:yarp::os::Time::delay(DEFAULT_DELAY_S);
    //P:yarp::os::Bottle cmd2,res2;
    //P:cmd2.addString("get");
    //P:pRpcClient->write(cmd2,res2);
    //P:for(int i=0;i<res2.size();i++)
    //P:{
    //P:    if ( res2.get(i).asInt() || psqFeatures->operator [](i) )  // logic OR;
    //P:        Npaint ++;
    //P:}



    //********************************FITNESS CALCULATION STEP******************************************************//

    //Fitness of the actual point= percentage of the wall painted

    //P:percentage=(Npaint/NSQUARES)*100;

    //The fit is the L2 norm between the current features, and the t+1 feature environment.
    double fit;
    printf("EL TIME STEP ES EL SIGUIENTE:: %d \n",timeStep);
    //P:printf("El percentage es:: %f", percentage);
    //P:printf("El Consta target es:: %f \n", Const_target[timeStep+1]);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

    for(int i=0;i<NFEATURES;i++){
        double aux_fit,aux_elem;
        aux_elem=observation[i]-target[timeStep][i];
        aux_fit=aux_elem*aux_elem;
        fit=fit+aux_fit;
    }

    fit=sqrt(fit);

    //featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

    cout << std::endl << " fit: " << fit << " ";

    //GOING BACK TO INIT POSITION
    std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw2.data());

    return fit;
}

/************************************************************************/

void CgdaIronFitnessFunction::individualExecution(vector<double> results){

    //GOAL: Execute the obtained values to obtain the executed trajectory

    //********************************EXECUTION STEP****************************************************************//

    std::vector<double> dEncRaw(6);  // NUM_MOTORS

    dEncRaw[0] = results[0];  // simple
    dEncRaw[1] = -results[1];  // simple
    dEncRaw[3] = results[2];  // simple

    dEncRaw[4] = 45;

    mentalPositionControl->positionMove(dEncRaw.data());

    //********************************FINAL OBSERVATION STEP********************************************************//

    //Obtain the actual state of the feature environment.

    std::vector<double> observation;

    //FORCE

    yarp::os::Bottle* b = pForcePort->read(false);
    if(!b)
    {
        printf("No force yet\n");
    }
    printf("El parámetro del sensor de fuerza es %s\n", b->toString().c_str());
    for(size_t i=0; i<b->size(); i++)
    {
        observation.push_back( b->get(i).asDouble() );
        //std(observation[i]);
    }

    //POSITION

    yarp::os::Bottle cmd,res;
    cmd.addString("world");
    cmd.addString("whereis");
    cmd.addString("tcp");
    cmd.addString("rightArm");
    pRpcClientWorld->write(cmd,res);
    /*printf("El parámetro de posicion es %s\n", res.toString().c_str());
    for(size_t i=0; i<res.size(); i++)
    {
        observation.push_back( res.get(i).asDouble() );
        //std(observation[i]);
    }*/
    yarp::os::Bottle* pos = res.get(0).asList();
    printf("El parámetro de posicion es %s\n", pos->toString().c_str());
    for(size_t i=0; i<pos->size(); i++)
    {
        observation.push_back( pos->get(i).asDouble() );
        //std(observation[i]);
    }


    //P:yarp::os::Time::delay(DEFAULT_DELAY_S);
    //yarp::os::Bottle cmd,res;
    //P:cmd.addString("get");
    //P:pRpcClient->write(cmd,res);
    //P:printf("got: %s\n",res.toString().c_str());
    //P:for(int i=0;i<res.size();i++)
    //P:{
        //std::cout << "past: ";
        //std::cout << psqFeatures->operator [](i);
        //std::cout << " present: ";
        //P:psqFeatures->operator [](i) |= res.get(i).asInt();  // logic OR
        //std::cout << psqFeatures->operator [](i);
        //std::cout << std::endl;
    //P:}

    //********************************MEMORY UPDATE STEP*******************************************************************//

    std::ofstream myfile1;
    myfile1.open("memoryOET.txt", std::ios_base::out );
    if (myfile1.is_open()){
        for(int i=0;i<observation.size();i++)
        {
            myfile1<<observation[i];
            //myfile1<<"1 ";
            //myfile1<< psqFeatures->operator[](i) << " ";
            //P myfile1<< psqFeatures->operator [](i);
            //Pmyfile1<< " ";
            //std::cout<<psqFeatures->operator[](i) << " ";
            //Pstd::cout<< psqFeatures->operator [](i);
            //Pstd::cout<< " ";
        }
        std::cout<<std::endl;
        //myfile1<<bestInd[0]->fitness->getValue()<<std::endl; //Fitness
    }
    myfile1.close();

    //********************************RESET STEP*******************************************************************//
    //-- Move the mental robot to 0 so env can be reset
    std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    mentalPositionControl->positionMove(dEncRaw2.data());
    //-- Reset the wall
    //P:yarp::os::Bottle cmd3,res3;
    //P:cmd3.addString("reset");
    //P:pRpcClient->write(cmd3,res3);

}

/************************************************************************/

} // namespace teo
