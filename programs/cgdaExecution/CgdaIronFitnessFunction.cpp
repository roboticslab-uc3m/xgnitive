// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CgdaIronFitnessFunction.hpp"


namespace teo
{

/************************************************************************/

double CgdaIronFitnessFunction::getCustomFitness(vector <double> genPoints){

    //Generalized
    double target_iron[NTPOINTS][NFEATURES] = {
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

    double fit=0;

    for(int t=0;t<NTPOINTS;t++) {

        double aux_fit=0;

        std::vector<double> dEncRaw(6);  // NUM_MOTORS
        dEncRaw[0] = genPoints[3*t+0];  // simple
        dEncRaw[1] = -genPoints[3*t+1];  // simple
        dEncRaw[3] = genPoints[3*t+2];  // simple

        //dEncRaw[4] = 45;

        //Actually move the robot
        mentalPositionControl->positionMove(dEncRaw.data());


        //********************************OBSERVATION STEP*******************************************************//

        std::vector<double> observationData, observationClean;
        observationData.clear();

        //POSITION
        yarp::os::Time::delay(DEFAULT_DELAY_S);
        //yarp::os::Bottle cmd,res;
        //cmd.addString("stat");

        /*cmd.addString("world");
        cmd.addString("whereis");
        cmd.addString("tcp");
        cmd.addString("rightArm");*/
        //pRpcClientCart->write(cmd,res);/usr/local/share/teo/contexts/kinematics/rightArmKinematics.ini
        /*yarp::os::Bottle* res = pRpcClientCart->read(true);
        printf("Got: %s\n",res->toString().c_str());

        for(size_t i=0; i<res->size(); i++)
        {
            observationData.push_back( res->get(i).asDouble() );
            //std(observationData[i]);
        }*/
        int stat;
        while( ! pRpcClientCart->stat(stat,observationData) );

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

        //std::cout<<" THE OBSERVATION IS :::::::::: "<<observationData<<std::endl;
        std::cout<<" In the Time Step "<<t<<" the position of the robot is "<<observationData<<std::endl;


        observationClean.push_back(observationData[0]); //X
        observationClean.push_back(observationData[1]); //Y
        observationClean.push_back(observationData[2]); //Z
        observationClean.push_back(observationData[6]); //Fz

        //********************************FITNESS CALCULATION STEP******************************************************//

        //The fit is the L2 norm between the current features, and the t+1 feature environment.
        //fit=sqrt(pow((percentage-Const_target_iron[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

        for(int i=0;i<NFEATURES;i++){
            double aux_elem;
            if(i==3){
                aux_elem=(observationClean[i]-target_iron[t][i])/300;
                aux_elem=aux_elem*aux_elem;
                aux_fit=aux_fit+aux_elem;
                std::cout<<" target_iron "<< target_iron[t][i]<<" OBERVACIÓN "<<observationClean[i]<<" FIT "<<aux_fit<<std::endl;

            }
            else{
                aux_elem=observationClean[i]-target_iron[t][i];
                std::cout<<aux_elem<<" "<<std::endl;
                aux_elem=aux_elem*aux_elem;
                aux_fit=aux_fit+aux_elem;
                //std::cout<<"aux_fit is at some time step: "<<aux_fit<<std::endl;
                std::cout<<" target_iron "<< target_iron[t][i]<<" OBERVACIÓN "<<observationClean[i]<<" FIT "<<aux_fit<<std::endl;

            }


        }
        fit=fit+sqrt(aux_fit);
    } //cierre bucle trayectoria completa



    return fit;

}

/************************************************************************/

void CgdaIronFitnessFunction::registerParameters(StateP state) {
    state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaIronFitnessFunction::initialize(StateP state) {

    voidP sptr = state->getRegistry()->getEntry("function"); // get parameter value
//    stringstream msg;
//    _objPtr = penv->GetKinBody("object");
//    _wall = penv->GetKinBody("wall");

//    if(!_objPtr) {
//        fprintf(stderr,"error: object \"object\" does not exist.\n");
//    } else printf("sucess: object \"object\" exists.\n");

//    if(!_wall) {
//        fprintf(stderr,"error: object \"wall\" does not exist.\n");
//    } else printf("sucess: object \"wall\" exists.\n");


//    usleep(1.0 * 1000000.0);

    return true;
}

/************************************************************************/

FitnessP CgdaIronFitnessFunction::evaluate(IndividualP individual) {

    // evaluation creates a new fitness object using a smart pointer
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

void CgdaIronFitnessFunction::trajectoryExecution(vector<double> result_trajectory){

    //GOAL: Execute the obtained best values to obtain the STATS executed trajectory

    //Generalized
    double target_iron[NTPOINTS][NFEATURES] = {
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


    //Obtain the actual state of the feature environment.

    for(int t=0;t<NTPOINTS;t++) {

        //********************************SIMULATED EXECUTION****************************************************//


        std::vector<double> dEncRaw(6);  // NUM_MOTORS
        dEncRaw[0] = result_trajectory[3*t+0];  // simple
        dEncRaw[1] = -result_trajectory[3*t+1];  // simple
        dEncRaw[3] = result_trajectory[3*t+2];  // simple

        //Actually move the robot
        mentalPositionControl->positionMove(dEncRaw.data());



        //********************************OBSERVATION STEP*******************************************************//

        std::vector<double> observationData, observationClean;
        observationData.clear();

        //POSITION
        yarp::os::Time::delay(DEFAULT_DELAY_S);
        //yarp::os::Bottle cmd,res;
        //cmd.addString("stat");

        /*cmd.addString("world");
        cmd.addString("whereis");
        cmd.addString("tcp");
        cmd.addString("rightArm");*/
        //pRpcClientCart->write(cmd,res);/usr/local/share/teo/contexts/kinematics/rightArmKinematics.ini
        /*yarp::os::Bottle* res = pRpcClientCart->read(true);
        printf("Got: %s\n",res->toString().c_str());

        for(size_t i=0; i<res->size(); i++)
        {
            observationData.push_back( res->get(i).asDouble() );
            //std(observationData[i]);
        }*/
        int stat;
        while( ! pRpcClientCart->stat(stat,observationData) );

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

        //std::cout<<" THE OBSERVATION IS :::::::::: "<<observationData<<std::endl;



        observationClean.push_back(observationData[0]); //X
        observationClean.push_back(observationData[1]); //Y
        observationClean.push_back(observationData[2]); //Z
        observationClean.push_back(observationData[3]); //Fz
        std::cout<<" In the iteration "<<t<<" the trajectory obtained was "<<observationClean<<std::endl;

        //********************************FITNESS CALCULATION STEP******************************************************//

        //The fit is the L2 norm between the current features, and the t+1 feature environment.
        double fit=0;
        //fit=sqrt(pow((percentage-Const_target_iron[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.

        for(int i=0;i<NFEATURES;i++){
            double aux_elem;
            if(i==3){
                aux_elem=(observationClean[i]-target_iron[t][i])/300;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
                std::cout<<" target_iron "<< target_iron[t][i]<<" OBERVACIÓN "<<observationClean[i]<<" FIT "<<fit<<std::endl;

            }
            else{
                aux_elem=observationClean[i]-target_iron[t][i];
                std::cout<<aux_elem<<" "<<std::endl;
                aux_elem=aux_elem*aux_elem;
                fit=fit+aux_elem;
                //std::cout<<"Fit is at some time step: "<<fit<<std::endl;
                std::cout<<" target_iron "<< target_iron[t][i]<<" OBERVACIÓN "<<observationClean[i]<<" FIT "<<fit<<std::endl;

            }


        }

        fit=sqrt(fit);

    //    std::cout<<" target_iron X "<< target_iron[timeStep+1][0]<<" OBERVACIÓN "<<observationData[0]<<std::endl;
    //    std::cout<<" target_iron Y "<< target_iron[timeStep+1][1]<<" OBERVACIÓN "<<observationData[1]<<std::endl;
    //    std::cout<<" target_iron Z "<< target_iron[timeStep+1][2]<<" OBERVACIÓN "<<observationData[2]<<std::endl;

        std::cout<<" FIT "<<fit<<std::endl;




        //featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

        cout << std::endl << " fit: " << fit << " "<<std::endl;

//        std::cout<<" "<<std::endl;
//        std::cout<<" "<<std::endl;
//        std::cout<<" "<<std::endl;
//        std::cout<<" "<<std::endl;

        //GOING BACK TO INIT POSITION
        //std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
        //Actually move the robot
        //mentalPositionControl->positionMove(dEncRaw2.data());


        //********************************Trajectory *******************************************************************//

        std::ofstream myfile1;
        myfile1.open("Trajectory.txt", std::ios_base::app);

        std::cout<<" THE SIZE OF OBSERVATION IS "<<observationClean.size()<<std::endl;
        if (myfile1.is_open()){
            for(int i=0;i<observationClean.size();i++)
            {
                //std::cout<<" LA OBSERVACIÓN ES "<<observation[i]<<std::endl;
                myfile1<<observationClean[i]<< " ";
                //myfile1<<"1 ";
                //myfile1<< psqFeatures->operator[](i) << " ";
                //P myfile1<< psqFeatures->operator [](i);
                //Pmyfile1<< " ";
                //std::cout<<psqFeatures->operator[](i) << " ";
                //Pstd::cout<< psqFeatures->operator [](i);
                //Pstd::cout<< " ";
            }
            myfile1<<fit<<" ";
            myfile1<<std::endl;
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
}

/************************************************************************/


}  // namespace teo

