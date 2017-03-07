// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaExecutionIET.hpp"

namespace teo
{

/************************************************************************/
bool CgdaExecutionIET::init() {

    timespec tsStart; //Start first timer
    clock_gettime(CLOCK_REALTIME, &tsStart);

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
    vector< double >* presults= &results;
    //int const_evaluations;
    //int* pconst_evaluations= &const_evaluations;
    //*pconst_evaluations=0;
    double evaluations;
    double fit;
    double ev_time_n[NTPOINTS];
    int ev_time_s[NTPOINTS];

    for(unsigned int i=0; i<NTPOINTS; i++) {
        timespec tsEvStart; //Start second timer
        clock_gettime(CLOCK_REALTIME, &tsEvStart);

        StateP state (new State);

        //PSOInheritance
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
        functionMinEvalOp->setPRpcClient(&rpcClient);
        functionMinEvalOp->setResults(presults);
        //Uncomment only for CgdaConstrained
        state->setEvalOp(functionMinEvalOp);

        unsigned int* pIter= &i;
        functionMinEvalOp->setIter(pIter);

        printf("---------------------------> i:%d\n",i);
        int newArgc = 2;
        //PAINT
        char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecutionIET/conf/evMono_ecf_params.xml" };
        //WAX
        //char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecutionIET/conf/evMono_ecf_params_WAX.xml" };


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

        evaluations+=state->getEvaluations(); // Evaluations is the sum of all the evluations for each iter.
        fit=bestInd[0]->fitness->getValue(); //Fit current gen.

        //           std::cout<<"EL NUMERO DE EVALUACIONES ES::::"<<const_evaluations<<std::endl;
        //           nalg1.reset();
        //           nalg2.reset();
        //           nalg3.reset();
        state.reset();
        delete functionMinEvalOp;

        timespec tsEvEnd;
        clock_gettime(CLOCK_REALTIME, &tsEvEnd);
        ev_time_n[i]=(tsEvEnd.tv_nsec-tsEvStart.tv_nsec);
        ev_time_n[i]=ev_time_n[i]/1000000000;
        ev_time_s[i]=(tsEvEnd.tv_sec-tsEvStart.tv_sec);
    }


    std::vector<double> percentage;
    CgdaPaintFitnessFunction* functionMinEvalOp = new CgdaPaintFitnessFunction;
    percentage=functionMinEvalOp->trajectoryExecution(results);

    //Total time
    double total_time=0;
    timespec tsEnd;
    clock_gettime(CLOCK_REALTIME, &tsEnd);
    total_time=(tsEnd.tv_sec-tsStart.tv_sec);
    std::cout<<"TOTAL TIME IS:   "<<total_time<<std::endl;

    //*******************************************************************************************//
    //                              FILE OUTPUT FOR DEBUGGING                                    //
    //*******************************************************************************************//

    std::cout<<std::endl<<"THE TOTAL NUMBER OF EVALUATIONS IS: "<<evaluations<<std::endl;

    std::ofstream myfile1;
    myfile1.open("DataIET.txt", std::ios_base::app);
    if (myfile1.is_open()){
        myfile1<<"0: ";
        myfile1<<evaluations<<" ";
        myfile1<<fit<<" "; //Take only the fit of the last gen.
        for(int i=0; i<NTPOINTS;i++){
            myfile1<<percentage[i]<<" ";
        }
        for(int i=0; i<NTPOINTS;i++){
            if(ev_time_s[i]==0){
                myfile1<<ev_time_n[i]<<" ";
            }
            else{
                myfile1<<ev_time_s[i]<<" ";
            }
        }
        myfile1<<total_time<<" "<<std::endl;
    }

    //           std::ofstream myfile2;
    //           myfile2.open("PercentageWall.txt", std::ios_base::app);
    //           if (myfile2.is_open()){
    //               myfile2<<std::endl;
    //           }

    //           std::ofstream myfile3;
    //           myfile3.open("X.txt", std::ios_base::app);
    //           if (myfile3.is_open()){
    //               myfile3<<std::endl;
    //           }

    //           std::ofstream myfile4;
    //           myfile4.open("Y.txt", std::ios_base::app);
    //           if (myfile4.is_open()){
    //               myfile4<<std::endl;
    //           }

    //           std::ofstream myfile5;
    //           myfile5.open("Z.txt", std::ios_base::app);
    //           if (myfile5.is_open()){
    //               myfile5<<std::endl;
    //           }

    //*******************************************************************************************//


    //*******************************************************************************************//
    //                                      END                                                  //
    //*******************************************************************************************//

    printf("-begin-\n");
    for(unsigned int i=0;i<results.size();i++) printf("%f, ",results[i]);
    printf("\n-end-\n");
    return true;
}

/************************************************************************/

} //namespace TEO
