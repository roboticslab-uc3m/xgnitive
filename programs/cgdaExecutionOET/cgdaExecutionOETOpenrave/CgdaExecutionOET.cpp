// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaExecutionOET.hpp"

namespace teo
{
/************************************************************************/

// system::run("cgdaExecutionOET parameters.txt 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0")

int CgdaExecutionOET::init(int argc, char **argv)
{

    sqFeatures.resize(argc-2);

    for(int i=0;i<argc-2;i++)
    {
        stringstream ss(argv[i+2]);
        ss >> sqFeatures[i];
        printf("EL valor de sqFeatures %d es:::: %d \n", i, sqFeatures[i]);
    }

//    portNum = -1;
//    bool open = false;
//    while( ! open )
//    {
//        portNum++;
//        std::string s("/good");
//        std::stringstream ss;
//        ss << portNum;
//        s.append(ss.str());
//        open = port.open(s);
//    }
//    std::stringstream ss;
//    ss << portNum;

    forcePort.open("/force:i");
    do {
        yarp::os::Network::connect("/forceEstimator:o","/force:i");
        printf("Wait to connect to forces...\n");
        yarp::os::Time::delay(DEFAULT_DELAY_S);
    } while( forcePort.getInputCount() == 0 );

    //std::cout<<"HASTA AQUI LLEGUE"<<std::endl;

    rpcClientWorld.open("/world:c");
    do {
        yarp::os::Network::connect("/world:c","/worldRpcResponder/rpc:s");
        printf("Wait to connect to world...\n");
        yarp::os::Time::delay(DEFAULT_DELAY_S);
    } while( rpcClientWorld.getOutputCount() == 0 );

    rpcClientCart.open("/cart:c");
    do {
        yarp::os::Network::connect("/cart:c","/CartesianControl/rpc:s");
        printf("Wait to connect to world...\n");
        yarp::os::Time::delay(DEFAULT_DELAY_S);
    } while( rpcClientCart.getOutputCount() == 0 );

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

//    //-- Paint server
//    //std::string remotePaint("/");
//    //remotePaint.append( ss.str() );
//    //remotePaint.append( "/openraveYarpPaintSquares/rpc:s" );
//    std::string remotePaint("/openraveYarpPaintSquares/rpc:s");
//    //std::string localPaint("/cgda/");
//    //localPaint.append( ss.str() );
//    std::string localPaint("/cgda");
//    localPaint.append( "/openraveYarpPaintSquares/rpc:c" );
//    rpcClient.open(localPaint);
//    do {
//        yarp::os::Network::connect(localPaint,remotePaint);
//        printf("Wait to connect to paint server...\n");
//        yarp::os::Time::delay(DEFAULT_DELAY_S);
//    } while( rpcClient.getOutputCount() == 0 );
//    CD_SUCCESS("Paint server available.\n");

    CD_SUCCESS("----- All good for %d.\n",portNum);

    vector< double > results;

    timespec tsEvStart; //Start second timer
    clock_gettime(CLOCK_REALTIME, &tsEvStart);

    StateP state (new State);

    //NOTE: WITH PARALLEL EXECUTION NEW ALGORITHMS FAIL.
    //           //PSOInheritance
    //           PSOInheritanceP nalg1 = (PSOInheritanceP) new PSOInheritance;
    //           state->addAlgorithm(nalg1);

    //           //PSOFuzzy
    //           PSOFuzzyP nalg2 = (PSOFuzzyP) new PSOFuzzy;
    //           state->addAlgorithm(nalg2);

    //           //ConstrainedSST
    //           ConstrainedSSTP nalg3 = (ConstrainedSSTP) new ConstrainedSST;
    //           state->addAlgorithm(nalg3);

    // set the evaluation operator
    CgdaIronFitnessFunction* functionMinEvalOp = new CgdaIronFitnessFunction;
    //CgdaPaintFitnessFunction* functionMinEvalOp = new CgdaPaintFitnessFunction;
    //CgdaWaxFitnessFunction* functionMinEvalOp = new CgdaWaxFitnessFunction;
    //Constrained Cost functions
    //CgdaConstrainedPaintFitnessFunction* functionMinEvalOp = new CgdaConstrainedPaintFitnessFunction;
    //CgdaConstrainedWaxFitnessFunction* functionMinEvalOp = new CgdaConstrainedWaxFitnessFunction;
    //functionMinEvalOp->setEvaluations(pconst_evaluations); //Uncomment only if CgdaFitnessFunction is uncomment

    mentalDevice.view(functionMinEvalOp->mentalPositionControl);
    realDevice.view(functionMinEvalOp->realPositionControl);
    functionMinEvalOp->setPRpcClient(&rpcClient);
    functionMinEvalOp->setPRpcClientWorld(&rpcClientWorld);
    functionMinEvalOp->setPRpcClientCart(&rpcClientCart);
    functionMinEvalOp->setPForcePort(&forcePort);
    functionMinEvalOp->setPsqFeatures(&sqFeatures);

    state->setEvalOp(functionMinEvalOp);

    int newArgc = 2;
    char *newArgv[2] = { (char*)"unusedFirstParam", argv[1] };

    printf("Pre Initialization\n");
    bool ok = state->initialize(newArgc, newArgv);
    if (!ok) printf("Failed Initialization\n");
    else printf("State Initialized\n");

    state->run();

    FloatingPoint::FloatingPoint* genBest;
    vector<double> bestPoints;

    HallOfFameP phof = state->getHoF();
    //printf("phof: %p\n",phof.get());

    vector<IndividualP> bestInds = phof->getBest();
    //printf("bestInds.size: %d\n",bestInds.size());

    //printf("bestInds[0]: %p\n",bestInds[0].get());

    if( bestInds[0].get() == NULL )
    {
        //printf("bestInds[0].get() == NULL\n");
        port.close();
        printf("bye!\n");
        return 0;
    }

    //printf("indiv: %s\n",(bestInds[0])->toString().c_str() );

    genBest = (FloatingPoint::FloatingPoint*) (bestInds[0])->getGenotype().get();
    bestPoints = genBest->realValue;

    results.push_back(bestPoints[0]);
    results.push_back(bestPoints[1]);
    results.push_back(bestPoints[2]);

    functionMinEvalOp->individualExecution(results);

    double ev_time_n;
    double ev_time_s;
    timespec tsEvEnd;
    clock_gettime(CLOCK_REALTIME, &tsEvEnd);
    ev_time_n=(tsEvEnd.tv_nsec-tsEvStart.tv_nsec);
    ev_time_n=ev_time_n/1000000000; //nano seconds to seconds
    ev_time_s=(tsEvEnd.tv_sec-tsEvStart.tv_sec);

//       printf("-begin-\n");
//       for(unsigned int i=0;i<results.size();i++) printf("%f, ",results[i]);
//       printf("\n-end-\n");

    port.close();

    //*******************************************************************************************//
    //                              FILE OUTPUT FOR DEBUGGING                                    //
    //*******************************************************************************************//
    double evaluations;
    evaluations=state->getEvaluations();

    std::cout<<std::endl<<"THE TOTAL NUMBER OF EVALUATIONS IS: "<<evaluations<<std::endl<<"THE NUMBER OF EVALUATIONS IN THIS ITERATION IS: "<<state->getEvaluations() <<std::endl;

    std::ofstream myfile1;
    myfile1.open("DataOET++.txt", std::ios_base::app);
    if (myfile1.is_open()){
        myfile1<<evaluations<<" ";
        myfile1<<bestInds[0]->fitness->getValue()<<" ";
        if(ev_time_s==0){
            myfile1<<ev_time_n<<" ";
        }
        else{
            myfile1<<ev_time_s<<" ";
        }
    }

    //*******************************************************************************************//
    //                                      END                                                  //
    //*******************************************************************************************//

    //printf("bye!\n");
    return 0;
}

/************************************************************************/

} //namespace TEO
