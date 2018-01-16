// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaPaintFitnessFunction.hpp"

#define NTPOINTS 17
#define NSQUARES 64

namespace teo
{

/************************************************************************/

void CgdaPaintFitnessFunction::registerParameters(StateP state) {
	state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaPaintFitnessFunction::initialize(StateP state) {

    voidP sptr = state->getRegistry()->getEntry("function"); // get parameter value

	return true;
}

/************************************************************************/

FitnessP CgdaPaintFitnessFunction::evaluate(IndividualP individual) {
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

double CgdaPaintFitnessFunction::getCustomFitness(vector <double> genPoints){

    double Const_target[NTPOINTS]={0, 6.25, 12.5, 18.75, 25, 31.25, 37.5
                       , 43.75, 50, 56.25, 62.5, 68.75, 75, 81.25, 87.5, 93.75, 100};

    yarp::os::Bottle cmd3,res3;
    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);

    double percentage;
    int timeStep=0;
    double Npaint=0;

    //printf("sqFeatures check %d \n", psqFeatures->operator [](0));

    for(int i=0;i<psqFeatures->size();i++)
    {
        Npaint += psqFeatures->operator [](i);
    }

    //Percentage of the wall painted before evolution
    printf("EL NPAINT ES::::::: %f \n",Npaint);
    percentage=(Npaint/NSQUARES)*100;
    printf("EL PERCENTAGE ES::::::: %f \n",percentage);

    //Serch the timeStep where we are (LOCALIZATION STEP)
    double diff=1000000;
    for(int i=0;i<NTPOINTS;i++){ //We give priority to the elements at the last positions
        double diff_aux;
        //printf("Percentage: %d \n",percentage);
        //printf("Target: %d \n", Const_target[i]);
        diff_aux=percentage-Const_target[i];
        diff_aux=diff_aux*diff_aux;
        diff_aux=sqrt(diff_aux);


        if(diff_aux<diff){
            timeStep=i;
            diff=diff_aux;
            //printf("i -> %d", i);
            //printf("Time Step %d \n",timeStep);

        }
    }


    //Set new positions of the robot using dEncRaw
    std::vector<double> dEncRaw(6);  // NUM_MOTORS
    dEncRaw[0] = genPoints[0];  // simple
    dEncRaw[1] = -genPoints[1];  // simple
    dEncRaw[3] = genPoints[2];  // simple

    dEncRaw[4] = 45;

    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw.data());

    //Calculate new percentage

    Npaint=0;
    yarp::os::Time::delay(DEFAULT_DELAY_S);
    yarp::os::Bottle cmd2,res2;
    cmd2.addString("get");
    pRpcClient->write(cmd2,res2);
    for(int i=0;i<res2.size();i++)
    {
        if ( res2.get(i).asInt() || psqFeatures->operator [](i) )  // logic OR;
            Npaint ++;
    }




    //Fitness of the actual point= percentage of the wall painted

    percentage=(Npaint/NSQUARES)*100;

    //The fit is the L2 norm between the current features, and the t+1 feature environment.
    double fit;
    printf("EL TIME STEP ES EL SIGUIENTE:: %d \n",timeStep);
    printf("El percentage es:: %f", percentage);
    printf("El Consta target es:: %f \n", Const_target[timeStep+1]);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.
    fit=percentage-Const_target[timeStep+1];
    fit=fit*fit;
    //printf("Diff_aux %d \n",diff_aux );
    fit=sqrt(fit);
    //featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

    cout << std::endl << " fit: " << fit << " ";

    std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw2.data());

    return fit;
}

/************************************************************************/

void CgdaPaintFitnessFunction::individualExecution(vector<double> results){

    //Move robot
    std::vector<double> dEncRaw(6);  // NUM_MOTORS

    dEncRaw[0] = results[0];  // simple
    dEncRaw[1] = -results[1];  // simple
    dEncRaw[3] = results[2];  // simple

    dEncRaw[4] = 45;

    mentalPositionControl->positionMove(dEncRaw.data());

    yarp::os::Time::delay(DEFAULT_DELAY_S);
    yarp::os::Bottle cmd,res;
    cmd.addString("get");
    pRpcClient->write(cmd,res);
    printf("got: %s\n",res.toString().c_str());
    for(int i=0;i<res.size();i++)
    {
        //std::cout << "past: ";
        //std::cout << psqFeatures->operator [](i);
        //std::cout << " present: ";
        psqFeatures->operator [](i) |= res.get(i).asInt();  // logic OR
        //std::cout << psqFeatures->operator [](i);
        //std::cout << std::endl;
    }

    std::ofstream myfile1;
    myfile1.open("memoryOET.txt", std::ios_base::out );
    if (myfile1.is_open()){
        for(int i=0;i<res.size();i++)
        {
            //myfile1<<"1 ";
            //myfile1<< psqFeatures->operator[](i) << " ";
            myfile1<< psqFeatures->operator [](i);
            myfile1<< " ";
            //std::cout<<psqFeatures->operator[](i) << " ";
            std::cout<< psqFeatures->operator [](i);
            std::cout<< " ";
        }
        std::cout<<std::endl;
        //myfile1<<bestInd[0]->fitness->getValue()<<std::endl; //Fitness
    }
    myfile1.close();

    std::ofstream myfile2;
    myfile2.open("trajectoryOET.txt", std::ios_base::app);
    if (myfile2.is_open()){
        for(int i=0;i<res.size();i++)
        {
            myfile2<< results[0];
            myfile2<< " ";
            myfile2<< results[0];
            myfile2<< " ";
            myfile2<< results[0];
            myfile2<<"\n";
        }
        std::cout<<std::endl;
        //myfile1<<bestInd[0]->fitness->getValue()<<std::endl; //Fitness
    }
    myfile1.close();



    //-- Move the mental robot to 0 so env can be reset
    std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    mentalPositionControl->positionMove(dEncRaw2.data());
    //-- Reset the mental
    yarp::os::Bottle cmd3,res3;
    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);

}

/************************************************************************/

} // namespace teo
