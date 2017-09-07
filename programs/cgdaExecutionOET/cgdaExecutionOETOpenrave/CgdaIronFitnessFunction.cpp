// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaIronFitnessFunction.hpp"

#define NTPOINTS 9
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

    //std::vector<std::vector<double>> target;
    double target[NTPOINTS][NFEATURES] = {
        {0.272805, -0.500201, 0.012808, -2.705315, -1.320802, 5.775318},
        {0.272620, -0.502092, 0.012907, -2.733236, -1.338366, 5.918067},
        {0.266961, -0.508060, -0.000334, -2.957992, -2.483874, 8.253265},
        {0.251811, -0.514240, -0.034490, -0.639683, -4.632288, 15.243059},
        {0.238970, -0.514431, -0.067436, 2.434867, 2.338527, -6.968969},
        {0.275419, -0.523437, -0.045262, 5.214250, 4.780977, -30.054635},
        {0.319814, -0.541889, -0.003238, -5.554522, 0.992935, -11.918344},
        {0.334939, -0.561616, 0.015075, -2.510248, -1.748680, -1.851854},
        {0.340740, -0.560490, 0.019368, -2.510248, -1.748680, -1.851854}
    };

    //This is sqFeatures

    //target.push_back(feature1T1,feature2T2,feature3T3...);

    //*****************************************OBSERVATION STEP*****************************************************//

    std::vector<double> observation;
    observation.clear();

    //READ FROM PsqFeatures the last NFEATURES points wich is the actual state of the environment
    for(int i=0;i<psqFeatures->size();i++)
    {
        observation.push_back(psqFeatures->operator [](i));
    }

    //***************************************LOCALIZATION STEP******************************************************//

    //Serch the timeStep where we are (LOCALIZATION STEP)
    int timeStep=0;
    double diff=1000000;

    //Position
    for(int i=observation[0]; i<NTPOINTS;i++){ //Note that going back in the trajectory has no sense. observation [0] is the last timeStep of the trajectory.
        double aux_dist=0;
        for(int j=0;j<NFEATURES;j++){
            double aux_elem=0;
            if(j<3){ //POS
                aux_elem=observation[j+1]-target[i][j];
                aux_elem=aux_elem*aux_elem;
                aux_dist=aux_dist+aux_elem;
                std::cout<<" TARGET IS " << target[i][j]<<" OBSERVATION IS "<<observation[j]<<" DIS OBTAINED IS " <<aux_dist<<std::endl;
            }

            else{ //FORCE
                if(j==5){
                    aux_elem=(observation[j+1]-(target[i][j]-5)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_dist=aux_dist+aux_elem;
                    std::cout<<" TARGET IS " << target[i][j]<<" OBSERVATION IS "<<observation[j]<<" DIS OBTAINED IS " <<aux_dist<<std::endl;

                }
                else{
                    aux_elem=(observation[j+1]-(target[i][j]+2)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_dist=aux_dist+aux_elem;
                    std::cout<<" TARGET IS " << target[i][j]<<" OBSERVATION IS "<<observation[j]<<" DIS OBTAINED IS " <<aux_dist<<std::endl;
                }

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

    //********************************SIMULATED EXECUTION STEP******************************************************//

    //Set new positions of the robot using dEncRaw
    std::vector<double> dEncRaw(6);  // NUM_MOTORS
    dEncRaw[0] = genPoints[0];  // simple
    dEncRaw[1] = -genPoints[1];  // simple
    dEncRaw[3] = genPoints[2];  // simple

    dEncRaw[4] = 70; //TODO:This must be changed for IRON

    //std::cout<<" LO QUE ME VOY A MOVER ES :::::: "<<genPoints[0]<<" "<<genPoints[1]<<" "<<genPoints[2]<<std::endl;

    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw.data());


    //********************************SECOND OBSERVATION STEP*******************************************************//

    observation.clear();


    //POSITION
    yarp::os::Time::delay(DEFAULT_DELAY_S);
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
    //printf("El parámetro de posicion es %s\n", pos->toString().c_str());
    //std::cout<<"LA POSICIÓN A LA QUE ME MOVÍ ES ESTA:::::"<<std::endl;
    for(size_t i=0; i<pos->size(); i++)
    {
        observation.push_back(pos->get(i).asDouble());
        //printf("%f",observation[i+3]);
        std::cout<<observation[i]<<std::endl;
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
    for(size_t i=0; i<(b->size()-3); i++)
    {
        observation.push_back( b->get(i).asDouble() );
        //std(observation[i]);
    }

    //std::cout<<observation<<std::endl;

    //********************************FITNESS CALCULATION STEP******************************************************//

    //The fit is the L2 norm between the current features, and the t+1 feature environment.
    double fit=0;
    printf("EL TIME STEP ES :: %d \n",timeStep);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

    for(int i=0;i<NFEATURES;i++){
        double aux_elem;
        if(i<3){ //POSITION
            aux_elem=observation[i+1]-target[timeStep+1][i];
            aux_elem=aux_elem*aux_elem;
            fit=fit+aux_elem;
        }

        else{ //FORCE
            if(i==5){
                aux_elem=(observation[i+1]-(target[timeStep+1][i]-5)); //the best way we have right now to delete some noise
                aux_elem=aux_elem/100000;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
            }
            else{
                aux_elem=(observation[i+1]-(target[timeStep+1][i]+2)); //the best way we have right now to delete some noise
                aux_elem=aux_elem/100000;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
            }
        }
    }

    fit=sqrt(fit);

    //std::cout<<" TARGET X "<< target[timeStep+1][0]<<" OBERVACIÓN "<<observation[0]<<std::endl;
    //std::cout<<" TARGET Y "<< target[timeStep+1][1]<<" OBERVACIÓN "<<observation[1]<<std::endl;
    //std::cout<<" TARGET Z "<< target[timeStep+1][2]<<" OBERVACIÓN "<<observation[2]<<std::endl;




    //featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

    cout << std::endl << " fit: " << fit << " "<<std::endl;

    std::cout<<" "<<std::endl;
    std::cout<<" "<<std::endl;
    std::cout<<" "<<std::endl;
    std::cout<<" "<<std::endl;

    //GOING BACK TO INIT POSITION
    std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw2.data());

    return fit;
}

/************************************************************************/

void CgdaIronFitnessFunction::individualExecution(vector<double> results){

    //GOAL: Execute the obtained best values to obtain the STATS executed trajectory

    //std::vector<std::vector<double>> target;
    double target[NTPOINTS][NFEATURES] = {
        {0.272805, -0.500201, 0.012808, -2.705315, -1.320802, 5.775318},
        {0.272620, -0.502092, 0.012907, -2.733236, -1.338366, 5.918067},
        {0.266961, -0.508060, -0.000334, -2.957992, -2.483874, 8.253265},
        {0.251811, -0.514240, -0.034490, -0.639683, -4.632288, 15.243059},
        {0.238970, -0.514431, -0.067436, 2.434867, 2.338527, -6.968969},
        {0.275419, -0.523437, -0.045262, 5.214250, 4.780977, -30.054635},
        {0.319814, -0.541889, -0.003238, -5.554522, 0.992935, -11.918344},
        {0.334939, -0.561616, 0.015075, -2.510248, -1.748680, -1.851854},
        {0.340740, -0.560490, 0.019368, -2.510248, -1.748680, -1.851854}
    };

    //*****************************************OBSERVATION STEP*****************************************************//

    std::vector<double> observation;

    //READ FROM PsqFeatures the last NFEATURES points wich is the actual state of the environment
    for(int i=0;i<psqFeatures->size();i++)
    {
        observation.push_back(psqFeatures->operator [](i));
    }



    //***************************************LOCALIZATION STEP******************************************************//

    //Serch the timeStep where we are (LOCALIZATION STEP)
    int timeStep=0;
    double diff=1000000;

    //Position
    for(int i=observation[0]; i<NTPOINTS;i++){
        double aux_dist=0;
        for(int j=0;j<NFEATURES;j++){
            double aux_elem=0;
            if(j<3){ //POS
                aux_elem=observation[j+1]-target[i][j];
                aux_elem=aux_elem*aux_elem;
                aux_dist=aux_dist+aux_elem;
            }

            else{ //FORCE
                if(j==5){
                    aux_elem=(observation[j+1]-(target[i][j]-5)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_dist=aux_dist+aux_elem;
                }
                else{
                    aux_elem=(observation[j+1]-(target[i][j]+2)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_dist=aux_dist+aux_elem;
                }

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

    //********************************EXECUTION STEP****************************************************************//

    std::vector<double> dEncRaw(6);  // NUM_MOTORS

    std::cout<<"Los resultados obtenidos son: "<<std::endl;
    std::cout<<"0: "<<results[0]<<std::endl;
    std::cout<<"1: "<<results[1]<<std::endl;
    std::cout<<"2: "<<results[2]<<std::endl;


    dEncRaw[0] = results[0];  // simple
    dEncRaw[1] = -results[1];  // simple
    dEncRaw[3] = results[2];  // simple

    dEncRaw[4] = 70;

    mentalPositionControl->positionMove(dEncRaw.data());

    //********************************FINAL OBSERVATION STEP********************************************************//

    //Obtain the actual state of the feature environment.

    observation.clear();
    observation.push_back(timeStep);

    //POSITION
    yarp::os::Time::delay(DEFAULT_DELAY_S);
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
    printf("El parámetro del sensor de fuerza es %s\n", b->toString().c_str());
    for(size_t i=0; i<(b->size()-3); i++)
    {
        observation.push_back( b->get(i).asDouble() );
        std::cout<<observation[i]<<std::endl;
    }


    //***************************************LOCALIZATION STEP******************************************************//

    //Serch the timeStep where we are (LOCALIZATION STEP)
    timeStep=0;
    diff=1000000;

    //Position
    for(int i=observation[0]; i<NTPOINTS;i++){
        double aux_dist=0;
        for(int j=0;j<NFEATURES;j++){
            double aux_elem=0;
            if(j<3){ //POS
                aux_elem=observation[j+1]-target[i][j];
                aux_elem=aux_elem*aux_elem;
                aux_dist=aux_dist+aux_elem;
            }

            else{ //FORCE
                if(j==5){
                    aux_elem=(observation[j+1]-(target[i][j]-5)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_dist=aux_dist+aux_elem;
                }
                else{
                    aux_elem=(observation[j+1]-(target[i][j]+2)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_dist=aux_dist+aux_elem;
                }

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

    //********************************FITNESS CALCULATION STEP******************************************************//

    //The fit is the L2 norm between the current features, and the t+1 feature environment.
    double fit=0;
    printf("EL TIME STEP ES :: %d \n",timeStep);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

    for(int i=0;i<NFEATURES;i++){
        double aux_elem;
        if(i<3){ //POSITION
            aux_elem=observation[i+1]-target[timeStep][i];
            aux_elem=aux_elem*aux_elem;
            fit=fit+aux_elem;
        }

        else{ //FORCE
            if(i==5){
                aux_elem=(observation[i+1]-(target[timeStep][i]-5)); //the best way we have right now to delete some noise
                aux_elem=aux_elem/100000;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
            }
            else{
                aux_elem=(observation[i+1]-(target[timeStep][i]+2)); //the best way we have right now to delete some noise
                aux_elem=aux_elem/100000;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
            }
        }
    }

    fit=sqrt(fit);

    std::cout<<" TARGET X "<< target[timeStep][0]<<" OBERVACIÓN "<<observation[0]<<std::endl;
    std::cout<<" TARGET Y "<< target[timeStep][1]<<" OBERVACIÓN "<<observation[1]<<std::endl;
    std::cout<<" TARGET Z "<< target[timeStep][2]<<" OBERVACIÓN "<<observation[2]<<std::endl;

    std::cout<<" FITNESS "<<fit<<std::endl;


    //********************************MEMORY UPDATE STEP *******************************************************************//

    std::ofstream myfile1;

    std::cout<<"Saving results..."<<std::endl;
    myfile1.open("memoryOET.txt", std::ios_base::out );

    std::ofstream myfile2;
    myfile2.open("Trajectory.txt", std::ios_base::app);
    if (myfile1.is_open() && myfile2.is_open()){
        for(int i=0;i<observation.size();i++)
        {
            myfile1<<observation[i]<< " ";
            myfile2<<observation[i]<< " ";
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
    myfile1.close();

    myfile2<<"\n";
    myfile2.close();

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
