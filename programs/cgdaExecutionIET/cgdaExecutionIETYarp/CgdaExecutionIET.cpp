// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaExecutionIET.hpp"

namespace teo
{
/************************************************************************/
//int numberOfPoints=17;
int numberOfPoints=9;
double time=0;
double evaluations=0;
bool CgdaExecutionIET::init() {
    //Set-up TeoSim and ECF state

    //Uncomment for pause before start
    //std::cin.get();

    printf("Note: requires a running instance of teoSim\n");
       if (!yarp::os::Network::checkNetwork())
       {
           printf("Please start a yarp name server first\n");
           return(-1);
       }

    //Configure Drivers
    yarp::os::Property options; //create an instance of Property, a nice YARP class for storing name-value (key-value) pairs
    options.put("device","remote_controlboard"); //we add a name-value pair that indicates the YARP device
    options.put("remote","/teoSim/rightArm"); //we add info on to whom we will connect
    options.put("local","/local"); //we add info on how we will call ourselves on the YARP network
    dd.open(options); //Configure the YARP multi-use driver with the given options

    if(!dd.isValid())
    {
      printf("/teoSim/rightArm device not available.\n");
      dd.close();
      yarp::os::Network::fini(); //disconnect from the YARP network
      return 1;
    }

    bool ok = dd.view(pos); // connect 'pos' interface to 'dd' device
    if (!ok)
    {
        printf("[warning] Problems acquiring robot interface\n");
        return false;
    }
    else
        printf("[success] testAsibot acquired robot interface\n");


    //Pos mode
    pos->setPositionMode(); //use the object to set the device to position mode (as opposed to velocity mode)

//    printf("test positionMove(1,-35)\n");
//    pos->positionMove(1, -35);

//    printf("Delaying 5 seconds...\n");
//    yarp::os::Time::delay(5);

    ok = dd.view(enc); // connect 'enc' interface to 'dd' device
//    double d;
//    enc->getEncoder(0,&d);
//    printf("test getEncoder(0) -> is at: %f\n", d);

//    ok = dd.view(vel); // connect 'vel' interface to 'dd' device

//    //Velocity mode
//    vel->setVelocityMode(); //use the object to set the device to velocity mode (as opposed to position mode)
//    printf("test velocityMove(0,10)\n");
//    vel->velocityMove(0,10);

//    printf("Delaying 5 seconds...\n");
//    yarp::os::Time::delay(5);

//    dd.close();

    vector< double > results;
    vector< double >* presults= &results;
    int const_evaluations;
    int* pconst_evaluations= &const_evaluations;
    *pconst_evaluations=0;

    //***********************************************SETUP-STATE*****************************************************//
    for(unsigned int i=0; i<numberOfPoints; i++) {
           StateP state (new State);

           //PSOInheritance
           PSOInheritanceP nalg1 = (PSOInheritanceP) new PSOInheritance;
           state->addAlgorithm(nalg1);

           //PSOFuzzy
           PSOFuzzyP nalg2 = (PSOFuzzyP) new PSOFuzzy;
           state->addAlgorithm(nalg2);

           //ConstrainedSST
           ConstrainedSSTP nalg3 = (ConstrainedSSTP) new ConstrainedSST;
           state->addAlgorithm(nalg3);

           // set the evaluation operator
           //CgdaPaintFitnessFunction* functionMinEvalOp = new CgdaPaintFitnessFunction;
           //CgdaWaxFitnessFunction* functionMinEvalOp = new CgdaWaxFitnessFunction;
           //Constrained Cost functions
           CgdaConstrainedPaintFitnessFunction* functionMinEvalOp = new CgdaConstrainedPaintFitnessFunction;
           //CgdaConstrainedWaxFitnessFunction* functionMinEvalOp = new CgdaConstrainedWaxFitnessFunction;
           functionMinEvalOp->setEvaluations(pconst_evaluations); //Uncomment only if CgdaFitnessFunction is uncomment

           //Pass to the CgdaFitness the simulation variables
           functionMinEvalOp->setResults(presults);
           functionMinEvalOp->setPControl(pos);
           functionMinEvalOp->setEControl(enc);

           state->setEvalOp(functionMinEvalOp);

           unsigned int* pIter= &i;
           functionMinEvalOp->setIter(pIter);

           printf("---------------------------> i:%d\n",i);
           int newArgc = 2;
           char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecutionIET/conf/evMono_ecf_params.xml" };


           state->initialize(newArgc, newArgv);

           state->run();

           vector<IndividualP> bestInd;
           FloatingPoint::FloatingPoint* genBest;
           vector<double> bestPoints;

           bestInd = state->getHoF()->getBest();
           genBest = (FloatingPoint::FloatingPoint*) bestInd.at(0)->getGenotype().get();
           bestPoints = genBest->realValue;
           results.push_back(bestPoints[0]);
           results.push_back(bestPoints[1]);
           results.push_back(bestPoints[2]);

           //*******************************************************************************************//
           //                              FILE OUTPUT FOR DEBUGGING                                    //
           //*******************************************************************************************//
           evaluations+=state->getEvaluations();
           time+=state->getElapsedTime();

           std::cout<<std::endl<<"THE TOTAL NUMBER OF EVALUATIONS IS: "<<evaluations<<std::endl<<"THE NUMBER OF EVALUATIONS IN THIS ITERATION IS: "<<state->getEvaluations() <<std::endl;
           std::cout<<std::endl<<"THE TIME TAKEN TO DO THIS IS:"<<time <<std::endl;
           //IndividualP bestParticle = selBestOp->select( *deme );

           std::ofstream myfile1;
           myfile1.open("TrajectoryterationsvsEvaluations.txt", std::ios_base::app);
           if (myfile1.is_open()){
               myfile1<<i<<" ";
               myfile1<<time<<" ";
               myfile1<<const_evaluations<<" ";
               myfile1<<bestInd[0]->fitness->getValue()<<std::endl;
           }          

           std::ofstream myfile2;
           myfile2.open("PercentageWall.txt", std::ios_base::app);
           if (myfile2.is_open()){
               myfile2<<std::endl;
           }

           std::ofstream myfile3;
           myfile3.open("X.txt", std::ios_base::app);
           if (myfile3.is_open()){
               myfile3<<std::endl;
           }

           std::ofstream myfile4;
           myfile4.open("Y.txt", std::ios_base::app);
           if (myfile4.is_open()){
               myfile4<<std::endl;
           }

           std::ofstream myfile5;
           myfile5.open("Z.txt", std::ios_base::app);
           if (myfile5.is_open()){
               myfile5<<std::endl;
           }

           //*******************************************************************************************//


           //*******************************************************************************************//
           //                                      END                                                  //
           //*******************************************************************************************//
           std::cout<<"EL NUMERO DE EVALUACIONES ES::::"<<const_evaluations<<std::endl;
           nalg1.reset();
           nalg2.reset();
           nalg3.reset();
           state.reset();
           delete functionMinEvalOp;
       }

       printf("-begin-\n");
       for(unsigned int i=0;i<results.size();i++) printf("%f, ",results[i]);
       printf("\n-end-\n");
    return true;
}

/************************************************************************/

} //namespace TEO
