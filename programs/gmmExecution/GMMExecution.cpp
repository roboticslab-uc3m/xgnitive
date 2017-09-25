// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaExecutionOET.hpp"

namespace teo
{
/************************************************************************/

int CgdaExecutionOET::init(int argc, char **argv)
{

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

    //-- Paint server
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

    vector< double > results;

    // set the evaluation operator
    CgdaPaintFitnessFunction* functionMinEvalOp = new CgdaPaintFitnessFunction;
    //CgdaWaxFitnessFunction* functionMinEvalOp = new CgdaWaxFitnessFunction;
    //Constrained Cost functions
    //CgdaConstrainedPaintFitnessFunction* functionMinEvalOp = new CgdaConstrainedPaintFitnessFunction;
    //CgdaConstrainedWaxFitnessFunction* functionMinEvalOp = new CgdaConstrainedWaxFitnessFunction;
    //functionMinEvalOp->setEvaluations(pconst_evaluations); //Uncomment only if CgdaFitnessFunction is uncomment


    results.push_back(bestPoints[0]);
    results.push_back(bestPoints[1]);
    results.push_back(bestPoints[2]);

    functionMinEvalOp->individualExecution(results);

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
        myfile1<<"0: ";
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
