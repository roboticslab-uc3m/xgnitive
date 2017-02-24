// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CgdaFitnessFunction.hpp"


namespace teo
{

/************************************************************************/

double CgdaFitnessFunction::getCustomFitness(vector <double> genPoints){

     // // reset square color for opencv
//    for(int i=0; i<4; i++){
//        for(int j=0; j<4; j++){
//            stringstream rr;
//            rr << "square" << i << j;
//            _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.5, 0.5, 0.5));
//            rr.str("");
//        }}

//    const int rows=4; //setting wall parameters
//    const int cols=4;
    //const int numbPoints=16;
    double percentage[NTPOINTS];
    int sqPainted [NSQUARES] = { }; //setting number of changed square as zero

    //The Generalized feature trajectory (TEST example is):
    std::vector < std::vector < double > > generalized;
    //The first dimension is the feature trajectory the second is the number of features
    generalized.push_back({0});
    generalized.push_back({6.25});
    generalized.push_back({12.5});
    generalized.push_back({18.75});
    generalized.push_back({25});
    generalized.push_back({31.25});
    generalized.push_back({37.5});
    generalized.push_back({43.75});
    generalized.push_back({50});
    generalized.push_back({56.25});
    generalized.push_back({62.5});
    generalized.push_back({68.75});
    generalized.push_back({75});
    generalized.push_back({81.25});
    generalized.push_back({87.5});
    generalized.push_back({93.75});
    generalized.push_back({100});


    for(int t=0;t<NTPOINTS;t++) {

//        //--------just OPENRAVE things to move TEO-----------------------------------------------------------
//        //things about moving arm. OpenRave things. dReal is a floating in dynamics ODE.
//        std::vector<double>  dEncRaw(6);  // NUM_MOTORS
//        dEncRaw[0] = genPoints[t*3+0];  // simple
//        dEncRaw[1] = -genPoints[t*3+1];  // simple
//        dEncRaw[3] = genPoints[t*3+2];  // simple

//        dEncRaw[4] = 45;
//        //NOTE: In the right arm there is an additional DOF for the wrist, that can be used if needed.

//        printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");

//        yarp::os::Time::delay(5);

//        printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 2 \n");

        std::vector<double> dEncRaw(6);  // NUM_MOTORS
        dEncRaw[0] = genPoints[3*t+0];  // simple
        dEncRaw[1] = -genPoints[3*t+1];  // simple
        dEncRaw[3] = genPoints[3*t+2];  // simple

        dEncRaw[4] = 45;

        //Actually move the robot
        mentalPositionControl->positionMove(dEncRaw.data());

        yarp::os::Time::delay(DEFAULT_DELAY_S);
        yarp::os::Bottle cmd2,res2;
        cmd2.addString("get");
        pRpcClient->write(cmd2,res2);
        double Npaint=0;
        for(int i=0;i<res2.size();i++)
        {
            //std::cout<<"The res2 string is: "<<res2.get(i).asInt()<<std::endl;
            //std::cout<<"The sqpainted at i is: "<<sqPainted[i]<<std::endl;
            if ( res2.get(i).asInt() || sqPainted[i]){  // logic OR;
                Npaint ++;
                sqPainted[i]=1;
            }
        }

        percentage[t]=(Npaint/NSQUARES)*100;
        //std::cout<<"El porcentaje es "<< percentage[t]<<std::endl;


         //Fitness = percentage of wall painted
//         std::valarray<int> myvalarray (sqPainted,rows*cols);
//         percentage[t]= ( (float)myvalarray.sum()/(rows*cols))*100;

     //cout << std::endl << " d: " << myvalarray.sum() << " " << percentage[t] << " ";
    } //cierre bucle trayectoria completa


    //The fit is obtained using the DTW algorithm.
    //The feature is the percentage of wall painted
    CgdaRecognition* featureTrajectories;
    featureTrajectories = new DtwCgdaRecognition;
    featureTrajectories->setGeneralized(generalized);

    //Get the discrepancy value between what we have and what we want
    //Init feature attemp vector
    std::vector< std::vector< double > > attempVectforSimpleDiscrepancy;
    for(int t=0;t<NTPOINTS;t++)
    {
        attempVectforSimpleDiscrepancy.push_back({percentage[t]});
    }

    //Console output.
    for(int i=0; i<attempVectforSimpleDiscrepancy[0].size(); i++){ //For each vector of characteristics(each column). In this case should be 1.
        std::cout<<std::endl<<std::endl;
        for(int j=0; j<attempVectforSimpleDiscrepancy.size(); j++){ //For each trajectory step
            std::cout<<"trajectory step "<<j<<" ==> " <<attempVectforSimpleDiscrepancy[j][i]<<std::endl;
        }
    }

    double fit;
    featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

    yarp::os::Bottle cmd3,res3;
    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);

    cout << std::endl << " fit: " << fit << " ";
    return fit;
}

/************************************************************************/

void CgdaFitnessFunction::registerParameters(StateP state) {
    state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaFitnessFunction::initialize(StateP state) {

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

FitnessP CgdaFitnessFunction::evaluate(IndividualP individual) {

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

double CgdaFitnessFunction::trajectoryExecution( vector<double> result_trajectory){
    //std::cout<<"I have entered Execution"<<std::endl;

    int sqPainted [NSQUARES] = { }; //setting number of changed square as cero

    //Reset squares
    yarp::os::Bottle cmd3,res3;
    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);
    yarp::os::Time::delay(DEFAULT_DELAY_S);



    //Move robot
    for(int t=0;t<=NTPOINTS;t++) {
            std::cout<<"Time interval"<<t<<std::endl;

            std::vector<double> dEncRaw(6);  // NUM_MOTORS
            dEncRaw[0] = result_trajectory[3*t+0];  // simple
            dEncRaw[1] = -result_trajectory[3*t+1];  // simple
            dEncRaw[3] = result_trajectory[3*t+2];  // simple

            dEncRaw[4] = 45;

            //Actually move the robot
            mentalPositionControl->positionMove(dEncRaw.data());

            yarp::os::Time::delay(DEFAULT_DELAY_S);
            yarp::os::Bottle cmd2,res2;
            cmd2.addString("get");
            pRpcClient->write(cmd2,res2);
            for(int i=0;i<res2.size();i++)
            {
                //std::cout<<"The res2 string is: "<<res2.get(i).asInt()<<std::endl;
                //std::cout<<"The sqpainted at i is: "<<sqPainted[i]<<std::endl;
                if ( res2.get(i).asInt()){  // logic OR;
                    sqPainted[i]=1;
                }
            }

            //sleep(1);
    }
    double Npaint=0;
    for(int i=0;i<NSQUARES;i++){
        if(sqPainted[i])Npaint++;
    }

    double percentage;
    percentage=(Npaint/NSQUARES)*100;
    return percentage;

    //Reset squares
    pRpcClient->write(cmd3,res3);
    yarp::os::Time::delay(DEFAULT_DELAY_S);

}

/************************************************************************/


}  // namespace teo

