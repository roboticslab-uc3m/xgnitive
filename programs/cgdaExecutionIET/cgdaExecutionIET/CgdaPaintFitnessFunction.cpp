// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaPaintFitnessFunction.hpp"

namespace teo
{

/************************************************************************/


//double target[10]={0, 10, 20, 30, 40, 50, 60, 70, 80, 100};
double Const_target[17]={0, 6.25, 12.5, 18.75, 25, 31.25, 37.5
                   , 43.75, 50, 56.25, 62.5, 68.75, 75, 81.25, 87.5, 93.75, 100};

double CgdaPaintFitnessFunction::getCustomFitness(vector <double> genPoints){

    double percentage[NTPOINTS]={ };
    int sqPainted [NSQUARES] = { }; //setting number of changed square as cero

    yarp::os::Bottle cmd3,res3;
    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);
//    yarp::os::Time::delay(DEFAULT_DELAY_S);

    yarp::os::Bottle cmd2,res2;
    cmd2.addString("get");
//    pRpcClient->write(cmd2,res2);
//    for(int i=0;i<res2.size();i++)
//    {
//        if ( res2.get(i).asInt() || sqPainted[i]){  // logic OR;
//            std::cout<<"SOMETHING GONE WRONG THERE IS A SPOT ON MY WALL:::::::::: "<<sqPainted[i]<<"   "<<res2.get(i).asInt()<<std::endl;
//        }
//    }

    for(int t=0;t<=*pIter;t++) {
            std::vector<double> dEncRaw(6);  // NUM_MOTORS
            if (t<*pIter){
                //cout << "< t: " << t << " *pIter: " << *pIter << std::endl;
//                cout << " pF0: " << pFresults->operator [](t*3+0);
//                cout << " pF1: " << pFresults->operator [](t*3+1);
//                cout << " pF2: " << pFresults->operator [](t*3+2);

                dEncRaw[0] = (pFresults->operator [](t*3+0));  // simple
                dEncRaw[1] = -(pFresults->operator [](t*3+1));  // simple
                dEncRaw[3] = (pFresults->operator [](t*3+2));  // simple


            }
            else if (t==*pIter){
//                cout << "== t: " << t << " *pIter: " << *pIter << std::endl;
//                cout << "gp0: " << genPoints[0] << std::endl;
//                cout << "gp1: " << genPoints[1] << std::endl;
//                cout << "gp2: " << genPoints[2] << std::endl;

                dEncRaw[0] = genPoints[0];  // simple
                dEncRaw[1] = -genPoints[1];  // simple
                dEncRaw[3] = genPoints[2];  // simple
            }
            else{cerr << "ERROR IN pIter or t" << std::endl;}

            dEncRaw[4] = 45;

            //Actually move the robot
            //std::cout<<"NUMERO DE ITERACION: "<<*pIter<<std::endl;
//            std::cout<<dEncRaw[0];
//            std::cout<<dEncRaw[1];
//            std::cout<<dEncRaw[3]<<"  t:  "<<t<<"   //////////// ";

            mentalPositionControl->positionMove(dEncRaw.data());

            yarp::os::Time::delay(DEFAULT_DELAY_S);
            pRpcClient->write(cmd2,res2);
            double Npaint=0;
            for(int i=0;i<res2.size();i++)
            {
                //std::cout<<"The res2 string is: "<<res2.get(i).asInt()<<std::endl;
                //std::cout<<"The sqpainted at i is: "<<sqPainted[i]<<std::endl;
                if ( res2.get(i).asInt() || sqPainted[i]){  // logic OR;
                    //std::cout<<"I HAVE PAINTED SOME WALL casue sqpainted    :::::"<<sqPainted[i] <<"  or  ::"<<res2.get(i).asInt();
                    Npaint ++;
                    sqPainted[i]=1;
                }
            }

            percentage[t]=(Npaint/NSQUARES)*100;
            //std::cout<<"TRAYECTORY STEP :::  "<<t<<" HAS A PERCENTAGE WALL :::" <<percentage[t]<<std::endl;

            //Fitness = percentage of wall painted


    } //cierre bucle trayectoria completa

//    //Output to file
//    std::ofstream myfile2;
//    myfile2.open("PercentageWall.txt", std::ios_base::app);
//    if (myfile2.is_open()){
//        myfile2<<percentage[*pIter]<<" ";
//    }

    // calculate fit /percentage of painted wall
    //double fit = abs(percentage-target[*pIter]);

    //The fit is obtained using the DTW algorithm.
    //The feature is the percentage of wall painted
    CgdaRecognition* featureTrajectories;
    featureTrajectories = new DtwCgdaRecognition;
    //Current Generalized trajectory
    std::vector < std::vector < double > > current_target;
    for(int i=0; i<=*pIter;i++){
        current_target.push_back({Const_target[i]});
//        std::cout<<"The current target "<<i<<"is :::::::::::::::::::::"<<current_target[i]<<std::endl;
    }
    featureTrajectories->setGeneralized(current_target);

    //Get the discrepancy value between what we have and what we want
    //Init feature attemp vector
    std::vector< std::vector< double > > attempVectforSimpleDiscrepancy;
    for(int t=0;t<=*pIter;t++)
    {
        attempVectforSimpleDiscrepancy.push_back({percentage[t]});
//        std::cout<<std::endl<<"ATTEMP TRAJECTORY ::::"<<t<<" : "<<percentage[t]<<std::endl;
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

    //cout << std::endl << " perc   entage: "<< percentage[0] << ","<< percentage[1] << ","<< percentage[2] << ","<< percentage[3] << ","<< percentage[4];
    cout << std::endl << " fit: " << fit << " ";

//    yarp::os::Bottle cmd3,res3;
//    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);
    std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    //Actually move the robot
    mentalPositionControl->positionMove(dEncRaw2.data());

    return fit;
}

/************************************************************************/

void CgdaPaintFitnessFunction::registerParameters(StateP state) {
	state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaPaintFitnessFunction::initialize(StateP state) {

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

FitnessP CgdaPaintFitnessFunction::evaluate(IndividualP individual) {

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

std::vector<double> CgdaPaintFitnessFunction::individualExecution(vector<double> result_trajectory){
    //std::cout<<"I have entered Execution"<<std::endl;

    int sqPainted [NSQUARES] = { }; //setting number of changed square as cero

    std::vector<double> percentage;
    //Reset squares
    yarp::os::Bottle cmd3,res3;
    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);
    yarp::os::Time::delay(DEFAULT_DELAY_S);



    //Move robot
    for(int t=0;t<=NTPOINTS;t++) {
            //std::cout<<"Time interval"<<t<<std::endl;

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

            double Npaint=0;
            for(int i=0;i<NSQUARES;i++){
                if(sqPainted[i])Npaint++;
            }

            percentage.push_back((Npaint/NSQUARES)*100);
            //sleep(1);
    }

    //Reset squares
    pRpcClient->write(cmd3,res3);
    yarp::os::Time::delay(DEFAULT_DELAY_S);


    //std::cout<<"Percentage "<<percentage<<std::endl;
    return percentage;
}

/************************************************************************/


} // namespace teo
