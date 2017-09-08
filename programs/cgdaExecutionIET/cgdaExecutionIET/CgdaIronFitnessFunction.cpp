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

    //********************************SIMULATED EXECUTION STEP******************************************************//

    for(int t=0;t<=*pIter;t++) { //Execute the t first points
        //Set new positions of the robot using dEncRaw
        std::vector<double> dEncRaw(6);  // NUM_MOTORS
        if(t<*pIter){
            dEncRaw[0] = (pFresults->operator [](t*3+0));  // simple
            dEncRaw[1] = -(pFresults->operator [](t*3+1));  // simple
            dEncRaw[3] = (pFresults->operator [](t*3+2));  // simple
        }
        else if(t==*pIter){
            dEncRaw[0] = genPoints[0];  // simple
            dEncRaw[1] = -genPoints[1];  // simple
            dEncRaw[3] = genPoints[2];  // simple
        }
        else{cerr << "ERROR IN pIter or t" << std::endl;}
        dEncRaw[4] = 0;
        //Actually move the robot
        mentalPositionControl->positionMove(dEncRaw.data());
    }



    //std::cout<<" LO QUE ME VOY A MOVER ES :::::: "<<genPoints[0]<<" "<<genPoints[1]<<" "<<genPoints[2]<<std::endl;


    //********************************OBSERVATION STEP*******************************************************//

    std::vector<double> observation;

    //POSITION
    //yarp::os::Time::delay(DEFAULT_DELAY_S);
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
        //std::cout<<observation[i]<<std::endl;
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
    printf("EL TIME STEP ES :: %d \n",*pIter);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

    for(int i=0;i<NFEATURES;i++){
        double aux_elem;
        if(i<3){ //POSITION
            aux_elem=observation[i]-target[*pIter][i];
            aux_elem=aux_elem*aux_elem;
            fit=fit+aux_elem;
        }

        else{ //FORCE
            if(i==5){
                aux_elem=(observation[i]-(target[*pIter][i]-5)); //the best way we have right now to delete some noise
                aux_elem=aux_elem/100000;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
            }
            else{
                aux_elem=(observation[i]-(target[*pIter][i]+2)); //the best way we have right now to delete some noise
                aux_elem=aux_elem/100000;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
            }
        }
    }

    fit=sqrt(fit);

    std::cout<<" TARGET X "<< target[*pIter][0]<<" OBERVACIÓN "<<observation[0]<<std::endl;
    std::cout<<" TARGET Y "<< target[*pIter][1]<<" OBERVACIÓN "<<observation[1]<<std::endl;
    std::cout<<" TARGET Z "<< target[*pIter][2]<<" OBERVACIÓN "<<observation[2]<<std::endl;

    std::cout<<" FIT "<<fit<<std::endl;




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

void CgdaIronFitnessFunction::individualExecution(vector<double> result_trajectory){

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

    //Obtain the actual state of the feature environment.

    double fit=0;
    std::vector<double> observation;
    observation.clear();

    for(int t=0;t<=NTPOINTS;t++) {

        //********************************FINAL OBSERVATION STEP********************************************************//

        //std::cout<<"Time interval"<<t<<std::endl;

        std::vector<double> dEncRaw(6);  // NUM_MOTORS
        dEncRaw[0] = result_trajectory[3*t+0];  // simple
        dEncRaw[1] = -result_trajectory[3*t+1];  // simple
        dEncRaw[3] = result_trajectory[3*t+2];  // simple

        dEncRaw[4] = 0;

        //Actually move the robot
        mentalPositionControl->positionMove(dEncRaw.data());

        //POSITION
        //yarp::os::Time::delay(DEFAULT_DELAY_S);
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
            //std::cout<<observation[i+4]<<std::endl;
        }

        //********************************FITNESS CALCULATION STEP******************************************************//

        //The fit is the L2 norm between the current features, and the t+1 feature environment.
        double aux_fit=0;
        //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

        for(int i=0;i<NFEATURES;i++){
            double aux_elem;
            if(i<3){ //POSITION
                aux_elem=observation[i+NFEATURES*t]-target[t][i];
                aux_elem=aux_elem*aux_elem;
                aux_fit=aux_fit+aux_elem;
            }

            else{ //FORCE
                if(i==5){
                    aux_elem=(observation[i+NFEATURES*t]-(target[t][i]-5)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_fit=aux_fit+aux_elem;
                }
                else{
                    aux_elem=(observation[i+NFEATURES*t]-(target[t][i]+2)); //the best way we have right now to delete some noise
                    aux_elem=aux_elem/100000;
                    aux_elem=aux_elem*aux_elem;
                    aux_fit=aux_fit+aux_elem;
                }
            }
        }
        fit=fit+aux_fit;
    }


    fit=sqrt(fit);

    //std::cout<<" TARGET X "<< target[*+1][0]<<" OBERVACIÓN "<<observation[1]<<std::endl;
    //std::cout<<" TARGET Y "<< target[timeStep+1][1]<<" OBERVACIÓN "<<observation[2]<<std::endl;
    //std::cout<<" TARGET Z "<< target[timeStep+1][2]<<" OBERVACIÓN "<<observation[3]<<std::endl;

    std::cout<<" FITNESS "<<fit<<std::endl;


    //********************************MEMORY UPDATE STEP *******************************************************************//

    std::ofstream myfile1;

    std::cout<<"Saving results..."<<std::endl;
    myfile1.open("memoryOET.txt", std::ios_base::out );

    std::ofstream myfile2;
    myfile2.open("Trajectory.txt", std::ios_base::app);

    std::cout<<" THE SIZE OF OBSERVATION IS "<<observation.size()<<std::endl;
    if (myfile1.is_open() && myfile2.is_open()){
        for(int i=0;i<observation.size();i++)
        {
            //std::cout<<" LA OBSERVACIÓN ES "<<observation[i]<<std::endl;
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
