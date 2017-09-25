// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CgdaExecution.hpp"

namespace teo
{

/************************************************************************/

bool CgdaExecution::init() {

    timespec tsStart; //Start first timer
    clock_gettime(CLOCK_REALTIME, &tsStart);

    //Uncomment for iron
//    forcePort.open("/force:i");
//    do {
//        yarp::os::Network::connect("/forceEstimator:o","/force:i");
//        printf("Wait to connect to forces...\n");
//        yarp::os::Time::delay(DEFAULT_DELAY_S);
//    } while( forcePort.getInputCount() == 0 );

//    rpcClientWorld.open("/world:c");
//    do {
//        yarp::os::Network::connect("/world:c","/worldRpcResponder/rpc:s");
//        printf("Wait to connect to world...\n");
//        yarp::os::Time::delay(DEFAULT_DELAY_S);
//    } while( rpcClientWorld.getOutputCount() == 0 );

//    rpcClientCart.open("/cart:c");
//    do {
//        yarp::os::Network::connect("/cart:c","/CartesianControl/rpc_transform:s");
//        printf("Wait to connect to world...\n");
//        yarp::os::Time::delay(DEFAULT_DELAY_S);
//    } while( rpcClientCart.getOutputCount() == 0 );

    //std::cout<<"HASTA AQUI LLEGUE"<<std::endl;

    //-- MENTAL ROBOT ARM
    yarp::os::Property mentalOptions;
    mentalOptions.put("device","remote_controlboard");
    //std::string remoteMental("/");
    //remoteMental.append( ss.str() );
    //remoteMental.append( "/teoSim/rightArm" );
    std::string remoteMental("/teoSim/rightArm");
    mentalOptions.put("remote",remoteMental);
    //std::string localMental("/cgdaMental/");
    //localMental.append( ss.str() );
    std::string localMental("/cgdaMental");
    localMental.append( "/teoSim/rightArm" );
    mentalOptions.put("local",localMental);
    mentalDevice.open(mentalOptions);
    if(!mentalDevice.isValid()) {
       CD_ERROR("Mental robot device not available.\n");
       mentalDevice.close();
       yarp::os::Network::fini();
       return 1;
    }
    CD_SUCCESS("Mental robot device available.\n");

//    //-- REAL ROBOT ARM
//    yarp::os::Property realOptions;
//    realOptions.put("device","remote_controlboard");
//    std::string realRemote;
//    bool realRealRemote = true;
//    if( realRealRemote )
//    {
//        realRemote.append( "/teo/rightArm" );
//    }
//    else
//    {
//        realRemote.append( "/1/teoSim/rightArm" );
//    }
//    realOptions.put("remote",realRemote);
//    std::string realLocal("/cgdaReal/");
//    realLocal.append( ss.str() );
//    realLocal.append( "/teo/rightArm" );
//    realOptions.put("local",realLocal);
//    realDevice.open(realOptions);
//    if(!realDevice.isValid()) {
//       CD_ERROR("Real robot device not available.\n");
//       realDevice.close();
//       yarp::os::Network::fini();
//       return 1;
//    }
//    CD_SUCCESS("Real robot device available.\n");

    //-- Paint server (uncomment for paint)
    //std::string remotePaint("/");
    //remotePaint.append( ss.str() );
    //remotePaint.append( "/openraveYarpPaintSquares/rpc:s" );
    std::string remotePaint("/openraveYarpPaintSquares/rpc:s");
    //std::string localPaint("/cgda/");
    //localPaint.append( ss.str() );
    std::string localPaint("/cgda");
    localPaint.append( "/openraveYarpPaintSquares/rpc:c" );
    rpcClient.open(localPaint);
    do {
        yarp::os::Network::connect(localPaint,remotePaint);
        printf("Wait to connect to paint server...\n");
        yarp::os::Time::delay(DEFAULT_DELAY_S);
    } while( rpcClient.getOutputCount() == 0 );
    CD_SUCCESS("Paint server available.\n");

    CD_SUCCESS("----- All good for %d.\n",portNum);

    timespec tsEvStart; //Start second timer
    clock_gettime(CLOCK_REALTIME, &tsEvStart);

	StateP state (new State);

//    PSOInheritanceP nalg1 = (PSOInheritanceP) new PSOInheritance;
//    state->addAlgorithm(nalg1);

//    PSOFuzzyP nalg2 = (PSOFuzzyP) new PSOFuzzy;
//    state->addAlgorithm(nalg2);

    // set the evaluation operator, init CgdaFitnessFunction
    //CgdaIronFitnessFunction* functionMinEvalOp = new CgdaIronFitnessFunction;
    CgdaPaintFitnessFunction* functionMinEvalOp = new CgdaPaintFitnessFunction;


    mentalDevice.view(functionMinEvalOp->mentalPositionControl);
    functionMinEvalOp->setPRpcClient(&rpcClient);

    //Uncomment for iron
//    functionMinEvalOp->setPRpcClientWorld(&rpcClientWorld);
//    functionMinEvalOp->setPRpcClientCart(&rpcClientCart);
//    functionMinEvalOp->setPForcePort(&forcePort);

//    functionMinEvalOp->setPRobot(probot);
//    functionMinEvalOp->setPenv(penv);
//    functionMinEvalOp->setPcontrol(pcontrol);
    //Here the functionMinEvalOp is set as the evaulation function
    state->setEvalOp(functionMinEvalOp);

    //por aqui se muestran resultados
    printf("---------------------------> ");

    int newArgc = 2;
    char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecution/conf/evMono_ecf_params.xml" };

    state->initialize(newArgc, newArgv);

    //Debugging
//    Population pop;
//    pop.initialize(state);
//    AlgorithmP alg;
//    alg=state->getAlgorithm();

//    std::cout << "Population: " << pop[0]->getSize() << std::endl;
//    std::cout << "Algorithm: " << alg->getName() << std::endl;

    state->run();
    double ev_time;
    timespec tsEvEnd;
    clock_gettime(CLOCK_REALTIME, &tsEvEnd);
    ev_time=(tsEvEnd.tv_sec-tsEvStart.tv_sec);
    std::cout<<"EVAL TIME IS:   "<<ev_time<<std::endl;

    vector<IndividualP> bestInd;
    FloatingPoint::FloatingPoint* genBest;
    vector<double> bestPoints;

    // final result
    vector< double > results;
    //Uncomment for paint
    std::vector<double> percentage;
    double evaluations=0;
    double total_time=0;
    bestInd = state->getHoF()->getBest();
    genBest = (FloatingPoint::FloatingPoint*) bestInd.at(0)->getGenotype().get();
    bestPoints = genBest->realValue;
    for(int i=0; i<NTPOINTS; i++){
        results.push_back(bestPoints[i*3+0]);
        results.push_back(bestPoints[i*3+1]);
        results.push_back(bestPoints[i*3+2]);
    }

    //Execute best trajectory to get % of the wall painted

    //Uncomment for iron
    //functionMinEvalOp->trajectoryExecution(results);
    //uncomment for paint
    percentage=functionMinEvalOp->trajectoryExecution(results);

//    printf("-begin-\n");
//    for(unsigned int i=0;i<bestPoints.size();i++)
//        printf("%f, ",bestPoints[i]);
//    printf("\n-end-\n");
//    port.close();

    timespec tsEnd;
    clock_gettime(CLOCK_REALTIME, &tsEnd);
    total_time=(tsEnd.tv_sec-tsStart.tv_sec);

    //*******************************************************************************************//
    //                              FILE OUTPUT FOR DEBUGGING                                    //
    //*******************************************************************************************//
    evaluations=state->getEvaluations();

    total_time=total_time-(0.1*evaluations);
    ev_time=ev_time-(0.1*evaluations);

    std::cout<<"TOTAL TIME IS:   "<<total_time<<std::endl;

    std::cout<<std::endl<<"THE TOTAL NUMBER OF EVALUATIONS IS: "<<evaluations<<std::endl<<"THE NUMBER OF EVALUATIONS IN THIS ITERATION IS: "<<state->getEvaluations() <<std::endl;

    std::ofstream myfile1;
    myfile1.open("DataFull.txt", std::ios_base::app);
    if (myfile1.is_open()){
        myfile1<<"0: ";
        myfile1<<evaluations<<" ";
        myfile1<<bestInd[0]->fitness->getValue()<<" ";

        //uncomment for paint
        for(int i=0; i<NTPOINTS;i++){
            myfile1<<percentage[i]<<" ";
        }

        for(int i=0; i<NSQUARES; i++){
            myfile1<<results[i*3+0]<<" ";
            myfile1<<results[i*3+1]<<" ";
            myfile1<<results[i*3+2]<<" ";
        }
        myfile1<<total_time<<" ";
        myfile1<<ev_time<<std::endl;
    }

    //*******************************************************************************************//
    //                                      END                                                  //
    //*******************************************************************************************//

    return true;
}

/************************************************************************/
}
