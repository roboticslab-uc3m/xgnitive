// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaPaintFitnessFunction.hpp"

#define NTPOINTS 17
#define NSQUARES 16

namespace teo
{

/************************************************************************/

void CgdaPaintFitnessFunction::individualExecution(vector<double> results){

    //Move robot
    std::vector<double> dEncRaw(6);  // NUM_MOTORS

    dEncRaw[0] = results[0];  // simple
    dEncRaw[1] = -results[1];  // simple
    dEncRaw[3] = results[2];  // simple

    dEncRaw[4] = 45;

    mentalPositionControl->positionMove(dEncRaw.data());

    yarp::os::Time::delay(DEFAULT_DELAY_S);
    yarp::os::Bottle cmd,res;
    cmd.addString("get");
    pRpcClient->write(cmd,res);
    printf("got: %s\n",res.toString().c_str());
    for(int i=0;i<res.size();i++)
    {
        //std::cout << "past: ";
        //std::cout << psqPainted->operator [](i);
        //std::cout << " present: ";
        psqPainted->operator [](i) |= res.get(i).asInt();  // logic OR
        //std::cout << psqPainted->operator [](i);
        //std::cout << std::endl;
    }

    std::ofstream myfile1;
    myfile1.open("memoryOET.txt", std::ios_base::out );
    if (myfile1.is_open()){
        for(int i=0;i<res.size();i++)
        {
            //myfile1<<"1 ";
            //myfile1<< psqPainted->operator[](i) << " ";
            myfile1<< psqPainted->operator [](i);
            myfile1<< " ";
            //std::cout<<psqPainted->operator[](i) << " ";
            std::cout<< psqPainted->operator [](i);
            std::cout<< " ";
        }
        std::cout<<std::endl;
        //myfile1<<bestInd[0]->fitness->getValue()<<std::endl; //Fitness
    }
    myfile1.close();

    //-- Move the mental robot to 0 so env can be reset
    std::vector<double> dEncRaw2(6,0);  // NUM_MOTORS
    mentalPositionControl->positionMove(dEncRaw2.data());
    //-- Reset the mental
    yarp::os::Bottle cmd3,res3;
    cmd3.addString("reset");
    pRpcClient->write(cmd3,res3);

}

/************************************************************************/

} // namespace teo
