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
    //std::clock_t start = std::clock();

//    timespec tsStart; //Start first timer
//    clock_gettime(CLOCK_REALTIME, &tsStart);

    sqPainted.resize(argc-2);

    for(int i=0;i<argc-2;i++)
    {
        stringstream ss(argv[i+2]);
        ss >> sqPainted[i];
        //printf("EL valor de sqPainted %d es:::: %d \n", i, sqPainted[i]);
    }

    portNum = -1;
    bool open = false;
    while( ! open )
    {
        portNum++;
        std::string s("/good");
        std::stringstream ss;
        ss << portNum;
        s.append(ss.str());
        open = port.open(s);
    }
    std::stringstream ss;
    ss << portNum;

    //-- MENTAL ROBOT ARM
    yarp::os::Property mentalOptions;
    mentalOptions.put("device","remote_controlboard");
    std::string remoteMental("/");
    remoteMental.append( ss.str() );
    remoteMental.append( "/teoSim/rightArm" );
    mentalOptions.put("remote",remoteMental);
    std::string localMental("/cgdaMental/");
    localMental.append( ss.str() );
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

    //-- REAL->SIM ROBOT ARM
    yarp::os::Property realSimOptions;
    realSimOptions.put("device","remote_controlboard");
    std::string realSimRemote;
    realSimRemote.append( "/1/teoSim/rightArm" );
    realSimOptions.put("remote",realSimRemote);
    std::string realSimLocal("/cgdaRealSim/");
    realSimLocal.append( ss.str() );
    realSimLocal.append( "/teo/rightArm" );
    realSimOptions.put("local",realSimLocal);
    realSimDevice.open(realSimOptions);
    if(!realSimDevice.isValid()) {
       CD_ERROR("Real->sim robot device not available.\n");
       realSimDevice.close();
       yarp::os::Network::fini();
       return 1;
    }
    CD_SUCCESS("Real->sim robot device available.\n");

    //-- REAL->REAL ROBOT ARM
    yarp::os::Property realOptions;
    realOptions.put("device","remote_controlboard");
    std::string realRemote;
    realRemote.append( "/teo/rightArm" );
    realOptions.put("remote",realRemote);
    std::string realLocal("/cgdaRealReal/");
    realLocal.append( ss.str() );
    realLocal.append( "/teo/rightArm" );
    realOptions.put("local",realLocal);
    realDevice.open(realOptions);
    if(!realDevice.isValid()) {
       CD_ERROR("Real->real robot device not available.\n");
       realDevice.close();
       yarp::os::Network::fini();
       return 1;
    }
    CD_SUCCESS("Real->real robot device available.\n");

    //-- Paint server
    std::string remotePaint("/");
    remotePaint.append( ss.str() );
    remotePaint.append( "/openraveYarpPaintSquares/rpc:s" );
    std::string localPaint("/cgda/");
    localPaint.append( ss.str() );
    localPaint.append( "/openraveYarpPaintSquares/rpc:c" );
    rpcClient.open(localPaint);
    do {
        yarp::os::Network::connect(localPaint,remotePaint);
        printf("Wait to connect to paint server...\n");
        yarp::os::Time::delay(DEFAULT_DELAY_S);
    } while( rpcClient.getOutputCount() == 0 );
    CD_SUCCESS("Paint server available.\n");

    CD_SUCCESS("----- All good for %d.\n",portNum);

    vector< double > results;
//    vector< double >* presults= &results;
//    int const_evaluations;
//    int* pconst_evaluations= &const_evaluations;
//    *pconst_evaluations=0;

    //yarp::os::Time::delay(1);

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
    CgdaPaintFitnessFunction* functionMinEvalOp = new CgdaPaintFitnessFunction;
    //CgdaWaxFitnessFunction* functionMinEvalOp = new CgdaWaxFitnessFunction;
    //Constrained Cost functions
    //CgdaConstrainedPaintFitnessFunction* functionMinEvalOp = new CgdaConstrainedPaintFitnessFunction;
    //CgdaConstrainedWaxFitnessFunction* functionMinEvalOp = new CgdaConstrainedWaxFitnessFunction;
    //functionMinEvalOp->setEvaluations(pconst_evaluations); //Uncomment only if CgdaFitnessFunction is uncomment

    mentalDevice.view(functionMinEvalOp->mentalPositionControl);
    realDevice.view(functionMinEvalOp->realPositionControl);
    realSimDevice.view(functionMinEvalOp->realSimPositionControl);
    functionMinEvalOp->setPRpcClient(&rpcClient);
    //           functionMinEvalOp->setPRobot(probot);
    //           functionMinEvalOp->setPenv(penv);
    //           functionMinEvalOp->setPcontrol(pcontrol);
    //functionMinEvalOp->setResults(presults);
    functionMinEvalOp->setPsqPainted(&sqPainted);
    //Uncomment only for CgdaConstrained

    state->setEvalOp(functionMinEvalOp);

    //unsigned int* pIter= &i;
    ////unsigned int j=0;
    ////unsigned int* pIter= &j;
    ////functionMinEvalOp->setIter(pIter);

    //printf("---------------------------> i:%d\n",i);
    int newArgc = 2;
    //PAINT
    char *newArgv[2] = { (char*)"unusedFirstParam", argv[1] };
    //char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params.xml" };
    //WAX
    //char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params_WAX.xml" };

    printf("Pre Initialization\n");
    bool ok = state->initialize(newArgc, newArgv);
    if (!ok) printf("Failed Initialization\n");
    else printf("State Initialized\n");

    //printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 10 \n");
    //yarp::os::Time::delay(1);

    state->run();

    //printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 20 \n");
    //yarp::os::Time::delay(1);

    //           for(unsigned int i=0; i<numberOfPoints; i++) {

    FloatingPoint::FloatingPoint* genBest;
    vector<double> bestPoints;

    printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 0 \n");

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

//    double total_time;
//    timespec tsEnd;
//    clock_gettime(CLOCK_REALTIME, &tsEnd);
//    total_time=(tsEnd.tv_sec-tsStart.tv_sec);
//    std::cout<<"TOTAL TIME IS:   "<<total_time<<std::endl;

    //*******************************************************************************************//
    //                              FILE OUTPUT FOR DEBUGGING                                    //
    //*******************************************************************************************//
    double evaluations;
    evaluations=state->getEvaluations();

    std::cout<<std::endl<<"THE TOTAL NUMBER OF EVALUATIONS IS: "<<evaluations<<std::endl<<"THE NUMBER OF EVALUATIONS IN THIS ITERATION IS: "<<state->getEvaluations() <<std::endl;

    std::ofstream myfile1;
    myfile1.open("DataOET++.txt", std::ios_base::app);
    if (myfile1.is_open()){
        myfile1<<"0: ";
        myfile1<<evaluations<<" ";
        myfile1<<bestInds[0]->fitness->getValue()<<" ";
//        myfile1<<total_time<<" ";
        myfile1<<ev_time_s<<" ";;
        myfile1<<ev_time_n<<std::endl;
    }

    //*******************************************************************************************//
    //                                      END                                                  //
    //*******************************************************************************************//

//    std::cout << "---------------Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    //printf("bye!\n");
    return 0;
}

/************************************************************************/

} //namespace TEO
