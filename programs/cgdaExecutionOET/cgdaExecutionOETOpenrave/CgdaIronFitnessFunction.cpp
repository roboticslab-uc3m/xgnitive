// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaIronFitnessFunction.hpp"

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

//***********************Define the generalized feature trajectory*****//

//std::vector<std::vector<double>> target;
double target[NTPOINTS][NFEATURES] = {
    {0.272805, -0.500201, 0.012808, 5.775318},
    {0.272620, -0.502092, 0.012907, 5.918067},
    {0.266961, -0.508060, -0.000334, 8.253265},
    {0.251811, -0.514240, -0.034490, 15.243059},
    {0.238970, -0.514431, -0.067436, -6.968969},
    {0.275419, -0.523437, -0.045262, -30.054635},
    {0.319814, -0.541889, -0.003238, -11.918344},
    {0.334939, -0.561616, 0.015075, -1.851854},
    {0.340740, -0.560490, 0.019368, -1.511597}
};

/********************************LOCALIZATION****************************/

int CgdaIronFitnessFunction::localization(std::vector<double> state){

    //State is : {"Last T", "Feature1", "Feature2"...}

    //Serch the timeStep where we are (LOCALIZATION STEP)
    int timeStep=0;
    double diff=1000000;

    //Position
    for(int i=state[0]; i<state[0]+3 && i<NTPOINTS;i++){ //Note that going back in the trajectory has no sense. memory [0] is the last timeStep of the trajectory.
        double aux_dist=0;
        for(int j=0;j<NFEATURES;j++){
            double aux_elem=0;
            if (j==3){
                aux_elem=(state[j+1]-target[i][j])/300;
                aux_elem=aux_elem*aux_elem;
                aux_dist=aux_dist+aux_elem;
                //std::cout<<" TARGET IS " << target[i][j]<<" memory IS "<<state[j+1]<<" DIS OBTAINED IS " <<aux_elem<<std::endl;
            }
            else{
                aux_elem=state[j+1]-target[i][j];
                aux_elem=aux_elem*aux_elem;
                aux_dist=aux_dist+aux_elem;
                //std::cout<<" TARGET IS " << target[i][j]<<" memory IS "<<state[j+1]<<" DIS OBTAINED IS " <<aux_elem<<std::endl;
            }
        }

        aux_dist=sqrt(aux_dist);
        //std::cout<<"LA DIST PARA EL TIME STEP "<<i<<" ES "<<aux_dist<<std::endl;

        if(aux_dist<diff){
            timeStep=i;
            diff=aux_dist;
            //printf("i -> %d", i);
            //printf("Time Step %d \n",timeStep);

        }
    }

    return timeStep;

}

/********************************OBSERVATION****************************/

std::vector<double> CgdaIronFitnessFunction::observation(){

    std::vector<double> observationData;
    observationData.clear();

    //POSITION
    //yarp::os::Time::delay(DEFAULT_DELAY_S);
    //yarp::os::Bottle cmd,res;
    //cmd.addString("stat");

    /*cmd.addString("world");
    cmd.addString("whereis");
    cmd.addString("tcp");
    cmd.addString("rightArm");*/
    //pRpcClientCart->write(cmd,res);
    yarp::os::Bottle* res = pRpcClientCart->read(true);
    printf("Got: %s\n",res->toString().c_str());

    for(size_t i=0; i<res->size(); i++)
    {
        observationData.push_back( res->get(i).asDouble() );
        //std(observationData[i]);
    }

    //FORCE
    yarp::os::Bottle* b = pForcePort->read(false);
    if(!b)
    {
        printf("No force yet\n");
        do{
            yarp::os::Time::delay(DEFAULT_DELAY_S);
            b = pForcePort->read(false);
            if(b){
                break;
            }
            printf("Waiting receiving force\n");

        }while(1);
    }
    //printf("El parámetro del sensor de fuerza es %s\n", b->toString().c_str());
    for(size_t i=0; i<b->size(); i++)
    {
        observationData.push_back( b->get(i).asDouble() );
        //std(observationData[i]);
    }

   // std::cout<<" THE OBSERVATION IS :::::::::: "<<observationData<<std::endl;

    return observationData;
}

double CgdaIronFitnessFunction::getCustomFitness(vector <double> genPoints){

    //This is sqFeatures


    //*****************************************RETRIEVAL STEP*****************************************************//
    std::vector<double> memory;
    memory.clear();

    //READ FROM PsqFeatures the last NFEATURES points wich is the actual state of the environment
    for(int i=0;i<psqFeatures->size();i++)
    {
        memory.push_back(psqFeatures->operator [](i));
        //std::cout<<psqFeatures->operator [](i)<<std::endl;

    }

    //***************************************LOCALIZATION STEP******************************************************//

    int timeStep;
    timeStep=localization(memory);

    //********************************SIMULATED EXECUTION STEP******************************************************//

    //Set new positions of the robot using dEncRaw
    std::vector<double> dEncRaw(6);  // NUM_MOTORS
    dEncRaw[0] = genPoints[0];  // simple
    dEncRaw[1] = -genPoints[1];  // simple
    dEncRaw[3] = genPoints[2];  // simple

    //-1.951544 -44.622798 -37.498539 79.687256 -0.406794 -14.854639
    //-2.441159 -44.947978 -37.58001 80.114206 -0.389922 -14.743856
    //(-2.399277 -45.104218 -37.491525 79.966313 0.378349 -14.809414) //for 1 position
//    dEncRaw[0] = -2.399277;  // simple
//    dEncRaw[1] = -45.104218;  // simple
//    dEncRaw[2] = -37.491525;
//    dEncRaw[3] = 79.966313;  // simple
//    dEncRaw[4] = 0.378349;  // simple
//    dEncRaw[5] = -14.809414;  // simple




    //dEncRaw[4] = 70; //TODO:This must be changed for IRON

    //std::cout<<" LO QUE ME VOY A MOVER ES :::::: "<<genPoints[0]<<" "<<genPoints[1]<<" "<<genPoints[2]<<std::endl;

    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw.data());

    //************************************** OBSERVATION STEP*******************************************************//
    std::vector<double> observationData, observationClean;
    observationData=observation();

    observationClean.push_back(observationData[0]); //X
    observationClean.push_back(observationData[1]); //Y
    observationClean.push_back(observationData[2]); //Z
    observationClean.push_back(observationData[7]); //Fz


    //********************************FITNESS CALCULATION STEP******************************************************//

    //The fit is the L2 norm between the current features, and the t+1 feature environment.
    double fit=0;
    printf("EL TIME STEP ES :: %d \n",timeStep);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

    for(int i=0;i<NFEATURES;i++){
        double aux_elem;
        if(i==3){
            aux_elem=(observationClean[i]-target[timeStep+1][i])/300;
            aux_elem=aux_elem*aux_elem;
            fit=fit+aux_elem;
            std::cout<<" TARGET "<< target[timeStep+1][i]<<" OBERVACIÓN "<<observationClean[i]<<" FIT "<<fit<<std::endl;

        }
        else{
            aux_elem=observationClean[i]-target[timeStep+1][i];
            std::cout<<aux_elem<<" "<<std::endl;
            aux_elem=aux_elem*aux_elem;
            fit=fit+aux_elem;
            //std::cout<<"Fit is at some time step: "<<fit<<std::endl;
            std::cout<<" TARGET "<< target[timeStep+1][i]<<" OBERVACIÓN "<<observationClean[i]<<" FIT "<<fit<<std::endl;

        }


    }

    fit=sqrt(fit);

//    std::cout<<" TARGET X "<< target[timeStep+1][0]<<" OBERVACIÓN "<<observationData[0]<<std::endl;
//    std::cout<<" TARGET Y "<< target[timeStep+1][1]<<" OBERVACIÓN "<<observationData[1]<<std::endl;
//    std::cout<<" TARGET Z "<< target[timeStep+1][2]<<" OBERVACIÓN "<<observationData[2]<<std::endl;

    std::cout<<" FIT "<<fit<<std::endl;




    //featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

    cout << std::endl << " fit: " << fit << " "<<std::endl;

    std::cout<<" "<<std::endl;
    std::cout<<" "<<std::endl;
    std::cout<<" "<<std::endl;
    std::cout<<" "<<std::endl;

    //GOING BACK TO INIT POSITION
    //std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    //Actually move the robot
    //mentalPositionControl->positionMove(dEncRaw2.data());

    return fit;
}

/************************************************************************/

void CgdaIronFitnessFunction::individualExecution(vector<double> results){

    //GOAL: Execute the obtained best values to obtain the STATS executed trajectory

    //Physical limits normalization approximation, using force senosr "limits"

//    for (int i=0;i<NTPOINTS;i++){
//        target[i][3]=target[i][3]/60;

//    }


    //*****************************************RETRIEVAL STEP*****************************************************//

    std::vector<double> memory;

    //READ FROM PsqFeatures the last NFEATURES points wich is the actual state of the environment
    for(int i=0;i<psqFeatures->size();i++)
    {
        memory.push_back(psqFeatures->operator [](i));
        //std::cout<<"LA FEATURE "<<i<<" IN THE LAST STEP VALUES" <<psqFeatures->operator [](i)<<std::endl;
    }



    //***************************************LOCALIZATION STEP******************************************************//

    std::vector<double> state;
    state=memory;

    int timeStep;
    timeStep=localization(state);

    //********************************EXECUTION STEP****************************************************************//

    std::vector<double> dEncRaw(6);  // NUM_MOTORS

//    std::cout<<"Los resultados obtenidos son: "<<std::endl;
//    std::cout<<"0: "<<results[0]<<std::endl;
//    std::cout<<"1: "<<results[1]<<std::endl;
//    std::cout<<"2: "<<results[2]<<std::endl;


    dEncRaw[0] = results[0];  // simple
    dEncRaw[1] = -results[1];  // simple
    dEncRaw[3] = results[2];  // simple

//    dEncRaw[0] = -2.399277;  // simple
//    dEncRaw[1] = -45.104218;  // simple
//    dEncRaw[2] = -37.491525;
//    dEncRaw[3] = 79.966313;  // simple
//    dEncRaw[4] = 0.378349;  // simple
//    dEncRaw[5] = -14.809414;  // simple


    //dEncRaw[4] = 70;

    mentalPositionControl->positionMove(dEncRaw.data());

    //********************************FINAL OBSERVATION STEP********************************************************//

    std::vector<double> observationData, observationClean;
    observationData=observation();

    observationClean.push_back(observationData[0]); //X
    observationClean.push_back(observationData[1]); //Y
    observationClean.push_back(observationData[2]); //Z
    observationClean.push_back(observationData[7]); //Fz

    //std::cout<<observationData<<std::endl;

    //***************************************CURRENT LOCALIZATION STEP******************************************************//

    state.clear();
    state.push_back(timeStep);
    for (int i=0; i<observationClean.size();i++){
        state.push_back(observationClean[i]);
    }

    timeStep=localization(state);
    std::cout<<"El timestep es:"<<timeStep<<std::endl;

    //********************************FITNESS CALCULATION STEP******************************************************//

    //The fit is the L2 norm between the current features, and the t+1 feature environment.
    double fit=0;
    printf("EL TIME STEP ES :: %d \n",timeStep);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.


    for(int i=0;i<NFEATURES;i++){
        double aux_elem;
        if(i==3){
            aux_elem=(observationClean[i]-target[timeStep+1][i])/300; //Force is the feature 7
            aux_elem=aux_elem*aux_elem;
            fit=fit+aux_elem;

        }
        else{
            aux_elem=observationClean[i]-target[timeStep+1][i];
            aux_elem=aux_elem*aux_elem;
            fit=fit+aux_elem;
            std::cout<<"Fit is at some time step: "<<fit<<std::endl;
        }


    }

    fit=sqrt(fit);

//    std::cout<< observationClean<<std::endl;

//    std::cout<<" TARGET X "<< target[timeStep+1][0]<<" OBERVACIÓN "<<observationClean[0]<<std::endl;
//    std::cout<<" TARGET Y "<< target[timeStep+1][1]<<" OBERVACIÓN "<<observationClean[1]<<std::endl;
//    std::cout<<" TARGET Z "<< target[timeStep+1][2]<<" OBERVACIÓN "<<observationClean[2]<<std::endl;
//    std::cout<<" TARGET Fz "<< target[timeStep+1][3]<<" OBERVACIÓN "<<observationClean[7]<<std::endl;

    std::cout<<" FITNESS "<<fit<<std::endl;


    //********************************MEMORY UPDATE STEP *******************************************************************//

    std::ofstream myfile1;

    std::cout<<"Saving results..."<<std::endl;
    myfile1.open("memoryOET.txt", std::ios_base::out );

    std::ofstream myfile2;
    myfile2.open("Trajectory.txt", std::ios_base::app);

    //std::cout<<" THE SIZE OF OBSERVATION IS "<<observationData.size()<<std::endl;
    if (myfile1.is_open() && myfile2.is_open()){
        myfile1<<timeStep<<" ";
        myfile2<<timeStep<<" ";
        for(int i=0;i<observationClean.size();i++)
        {
            //std::cout<<" LA OBSERVACIÓN ES "<<observation[i]<<std::endl;
            myfile1<<observationClean[i]<< " ";
            myfile2<<observationClean[i]<< " ";
            //myfile1<<"1 ";
            //myfile1<< psqFeatures->operator[](i) << " ";
            //P myfile1<< psqFeatures->operator [](i);
            //Pmyfile1<< " ";
            //std::cout<<psqFeatures->operator[](i) << " ";
            //Pstd::cout<< psqFeatures->operator [](i);
            //Pstd::cout<< " ";
        }
        std::cout<<std::endl;
    }
    myfile1<<fit<< " ";
    myfile2<<fit<<" ";
    myfile1.close();

    myfile2<<"\n";
    myfile2.close();

    //********************************RESET STEP*******************************************************************//
    //-- Move the mental robot to 0 so env can be reset
    //std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    //mentalPositionControl->positionMove(dEncRaw2.data());
    //-- Reset the wall
    //P:yarp::os::Bottle cmd3,res3;
    //P:cmd3.addString("reset");
    //P:pRpcClient->write(cmd3,res3);

}

/************************************************************************/

} // namespace teo
