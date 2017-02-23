// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CgdaExecution.hpp"

namespace teo
{

/************************************************************************/

bool CgdaExecution::init() {

    std::clock_t start = std::clock();

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

    //-- ROBOT ARM
    std::stringstream ss;
    ss << portNum;
    yarp::os::Property ddOptions;
    ddOptions.put("device","remote_controlboard");
    std::string remote("/");
    remote.append( ss.str() );
    remote.append( "/teoSim/rightArm" );
    ddOptions.put("remote",remote);
    std::string local("/cgda/");
    local.append( ss.str() );
    local.append( "/teoSim/rightArm" );
    ddOptions.put("local",local);
    dd.open(ddOptions);
    if(!dd.isValid()) {
       CD_ERROR("Robot device not available.\n");
       dd.close();
       yarp::os::Network::fini();
       return 1;
    }
    CD_SUCCESS("Robot device available.\n");

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

    std::cout << "---------------Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

	StateP state (new State);

//    PSOInheritanceP nalg1 = (PSOInheritanceP) new PSOInheritance;
//    state->addAlgorithm(nalg1);

//    PSOFuzzyP nalg2 = (PSOFuzzyP) new PSOFuzzy;
//    state->addAlgorithm(nalg2);

    // set the evaluation operator, init CgdaFitnessFunction
    CgdaFitnessFunction* functionMinEvalOp = new CgdaFitnessFunction; 
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

    vector<IndividualP> bestInd;
    FloatingPoint::FloatingPoint* genBest;
    vector<double> bestPoints;

    // final result
    bestInd = state->getHoF()->getBest();
    genBest = (FloatingPoint::FloatingPoint*) bestInd.at(0)->getGenotype().get();
    bestPoints = genBest->realValue;

    printf("-begin-\n");
    for(unsigned int i=0;i<bestPoints.size();i++)
        printf("%f, ",bestPoints[i]);
    printf("\n-end-\n");

    return true;
}

/************************************************************************/
}
