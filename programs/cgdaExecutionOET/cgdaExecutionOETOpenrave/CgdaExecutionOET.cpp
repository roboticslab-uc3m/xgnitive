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
    std::clock_t start = std::clock();

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

    vector< double > results;
    vector< double >* presults= &results;
    int const_evaluations;
    int* pconst_evaluations= &const_evaluations;
    *pconst_evaluations=0;

    //yarp::os::Time::delay(1);

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

           dd.view(functionMinEvalOp->iPositionControl);
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

           //printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 1 \n");

           //yarp::os::Time::delay(20);

           //printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 2 \n");

           functionMinEvalOp->individualExecution(results);

           //printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 3 \n");


//           //*******************************************************************************************//
//           //                              FILE OUTPUT FOR DEBUGGING                                    //
//           //*******************************************************************************************//
//           evaluations+=state->getEvaluations();
//           time+=state->getElapsedTime();

//           std::cout<<std::endl<<"THE TOTAL NUMBER OF EVALUATIONS IS: "<<evaluations<<std::endl<<"THE NUMBER OF EVALUATIONS IN THIS ITERATION IS: "<<state->getEvaluations() <<std::endl;
//           std::cout<<std::endl<<"THE TIME TAKEN TO DO THIS IS:"<<time <<std::endl;
//           //IndividualP bestParticle = selBestOp->select( *deme );

//           std::ofstream myfile1;
//           myfile1.open("TrajectoryterationsvsEvaluations.txt", std::ios_base::app);
//           if (myfile1.is_open()){
//               //myfile1<<i<<" ";
//               myfile1<<time<<" ";
//               //if(const_evaluations==0){ //If we are not using the constrained version output normal
//               //     myfile1<<evaluations<<" ";
//               //}
//               //else{
//                   myfile1<<const_evaluations<<" ";
//               //}
//               myfile1<<bestInd[0]->fitness->getValue()<<std::endl;
//           }

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

//           //This following line is only for the execution of the result trajectory of the paint task
////           if(i==(numberOfPoints-1)){
////                //std::cout<<"LETS EXECUTE"<<std::endl;
////                functionMinEvalOp->trajectoryExecution(numberOfPoints, results);
////           }
//           //*******************************************************************************************//


//           //*******************************************************************************************//
//           //                                      END                                                  //
//           //*******************************************************************************************//
//           std::cout<<"EL NUMERO DE EVALUACIONES ES::::"<<const_evaluations<<std::endl;
//           nalg1.reset();
//           nalg2.reset();
//           nalg3.reset();
     //  }

//       printf("-begin-\n");
//       for(unsigned int i=0;i<results.size();i++) printf("%f, ",results[i]);
//       printf("\n-end-\n");

    port.close();
    printf("bye!\n");
    return 0;
}

/************************************************************************/

} //namespace TEO
