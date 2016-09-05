// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaConstrainedWaxFitnessFunction.hpp"

namespace teo
{

/************************************************************************/



double CgdaConstrainedWaxFitnessFunction::getCustomFitness(vector <double> genPoints){

    //First the new position is checked if its in the allowed space
    KDL::JntArray qInRad = KDL::JntArray(GdL);
    qInRad(0) = -genPoints[0]*M_PI/180.0;  // simple
    qInRad(1) = -genPoints[1]*M_PI/180.0;  // simple
    qInRad(2) = -genPoints[2]*M_PI/180.0;  // simple
    KDL::Frame kf;
    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(chain);
    fksolver.JntToCart(qInRad,kf);

    std::cout << "x: FK: " << kf.p.x() << " | ";
    std::cout << "y: FK: " << kf.p.y() << " | ";
    std::cout << "z: FK: " << kf.p.z() << std::endl;

    // x: FK: 0 OR: 0.00630739 | y: FK: -0.3469 OR: -0.340613 | z: FK: -0.220306 OR: -2.43642e-114

    //region            //Wall Coord.   //+-0.01(++Adjusted) //+-0.1(Adjusted) //+-0.2 (intermediate) //+-0.3 (large) //+-0.05 (Adjusted+)
    float xl=0.3;      //0.6            0.59                0.5               0.4                    0.3             0.55
    float xu=0.9;      //0.6            0.61                0.7               0.8                    0.9             0.65
    float yl=-1.1;     //-0.8          -0.81               -0.9               -1                     -1.1            -0.85
    float yu=0.1;     //-0.2          -0.19               -0.1               0                      0.1             -0.15
    float zl=0;      //0.32           0.31                0.2               0.1                    0               0.25
    float zu=1.2;      //0.92           0.93                1                 1.1                    1.2             0.95

    //if not in the allowed region
    if(!(((kf.p.x()>xl) && (kf.p.x()<xu)) && ((kf.p.y()>yl) && (kf.p.y()<yu)) && ((kf.p.z()>zl) && (kf.p.z()<zu)))){
        std::cout<<"***************************************SPACE LIMITED********************************"<<std::endl;
        return 10000;
    }

    else{
        std::vector<std::vector<double>> target;
        double kinect_offset_x=1100;
        double kinect_offset_y=-500;
        double kinect_offset_z=300;
        target.push_back({kinect_offset_x-6.38762472700000e+02,kinect_offset_y-2.72626977500000e+00,kinect_offset_z-(-3.97845084999999e+00)});
        target.push_back({kinect_offset_x-6.42391811965000e+02,kinect_offset_y-1.68573559950000e+01,kinect_offset_z-(-3.02628304000000e+00)});
        target.push_back({kinect_offset_x-6.80641917184079e+02,kinect_offset_y-8.88397871243782e+01,kinect_offset_z-1.23043924875622e+00});
        target.push_back({kinect_offset_x-7.83047694335000e+02,kinect_offset_y-1.20799710345000e+02,kinect_offset_z-6.47513742499999e+00});
        target.push_back({kinect_offset_x-8.64809906691542e+02,kinect_offset_y-5.41469797114429e+01,kinect_offset_z-7.08611930348259e+00});
        target.push_back({kinect_offset_x-8.16733444044999e+02,kinect_offset_y-(-5.19248806450000e+01),kinect_offset_z-(-9.54816400000000e-01)});
        target.push_back({kinect_offset_x-6.91319608447761e+02,kinect_offset_y-(-6.46731068507463e+01),kinect_offset_z-(-6.88174848258707e+00)});
        target.push_back({kinect_offset_x-6.38455344905001e+02,kinect_offset_y-(-1.57018014900000e+01),kinect_offset_z-(-4.27471647499999e+00)});
        target.push_back({kinect_offset_x-6.36920685935323e+02,kinect_offset_y-3.73245194029851e-01,kinect_offset_z-(-3.83094820398010e+00)});

        std::vector<std::vector<double>> attempTrajectory;

        for(int t=0;t<=*pIter;t++) {
                std::vector<OpenRAVE::dReal> dEncRaw(probot->GetDOF());  // NUM_MOTORS
                if (t<*pIter){
    //                cout << "< t: " << t << " *pIter: " << *pIter << std::endl;
    //                cout << "pF0: " << pFresults->operator [](t*3+0) << std::endl;
    //                cout << "pF1: " << pFresults->operator [](t*3+1) << std::endl;
    //                cout << "pF2: " << pFresults->operator [](t*3+2) << std::endl;

                    dEncRaw[0+4] = -1*(pFresults->operator [](t*3+0))*M_PI/180.0;  // simple
                    dEncRaw[1+4] = -1*(pFresults->operator [](t*3+1))*M_PI/180.0;  // simple
                    dEncRaw[3+4] = -1*(pFresults->operator [](t*3+2))*M_PI/180.0;  // simple


                }
                else if (t==*pIter){
    //                cout << "== t: " << t << " *pIter: " << *pIter << std::endl;
    //                cout << "gp0: " << genPoints[0] << std::endl;
    //                cout << "gp1: " << genPoints[1] << std::endl;
    //                cout << "gp2: " << genPoints[2] << std::endl;

                    dEncRaw[0+4] = -genPoints[0]*M_PI/180.0;  // simple
                    dEncRaw[1+4] = -genPoints[1]*M_PI/180.0;  // simple
                    dEncRaw[3+4] = -genPoints[2]*M_PI/180.0;  // simple
                }
                else{cerr << "ERROR IN pIter or t" << std::endl;}

                dEncRaw[4+4] = -45*M_PI/180.0;
                probot->SetJointValues(dEncRaw);
                pcontrol->SetDesired(dEncRaw); // This function "resets" physics
                while(!pcontrol->IsDone()) {
                    boost::this_thread::sleep(boost::posix_time::milliseconds(1));
                }
                penv->StepSimulation(0.0001);  // StepSimulation must be given in seconds
                T_base_object = _objPtr->GetTransform();
                double T_base_object_x = T_base_object.trans.x*1000.0; //pass to mm
                double T_base_object_y = T_base_object.trans.y*1000.0; //pass to mm
                double T_base_object_z = T_base_object.trans.z*1000.0; //pass to mm

    //            target[t][0]=T_base_kinect.trans.x*1000.0 - target[t][0];
    //            target[t][1]=T_base_kinect.trans.y*1000.0 - target[t][1];
    //            target[t][2]=T_base_kinect.trans.z*1000.0 - target[t][2];


                attempTrajectory.push_back({T_base_object_x,T_base_object_y,T_base_object_z});

        } //cierre bucle trayectoria completa

        std::ofstream myfile3;
        myfile3.open("X.txt", std::ios_base::app);
        if (myfile3.is_open()){
            myfile3<<attempTrajectory[*pIter][0]<<" ";
        }

        std::ofstream myfile4;
        myfile4.open("Y.txt", std::ios_base::app);
        if (myfile4.is_open()){
            myfile4<<attempTrajectory[*pIter][1]<<" ";
        }

        std::ofstream myfile5;
        myfile5.open("Z.txt", std::ios_base::app);
        if (myfile5.is_open()){
            myfile5<<attempTrajectory[*pIter][2]<<" ";
        }


        //The fit is obtained using the DTW algorithm.
        //The feature is the percentage of wall painted
        CgdaRecognition* featureTrajectories;
        featureTrajectories = new DtwCgdaRecognition;
        //Current Generalized trajectory
        std::vector < std::vector < double > > current_target;
        for(int i=0; i<=*pIter;i++){
            current_target.push_back({target[i]});
        }

        featureTrajectories->setGeneralized(current_target);

        //Get the discrepancy value between what we have and what we want

        //Console output.
        for(int i=0; i<attempTrajectory[0].size(); i++){ //For each vector of characteristics(each column). In this case should be 1.
            std::cout<<std::endl<<std::endl;
            for(int j=0; j<attempTrajectory.size(); j++){ //For each trajectory step
                std::cout<<"trajectory step "<<j<<"-"<<i<<" ==> " <<attempTrajectory[j][i]<<std::endl;
                std::cout<<"target "<<j<<"-"<<i<<" ==> " <<target[j][i]<<std::endl;
            }
        }

        double fit;
        featureTrajectories->compare(attempTrajectory,fit);

        cout << std::endl << " fit: " << fit << " ";

        return fit;
    }
}

/************************************************************************/

void CgdaConstrainedWaxFitnessFunction::registerParameters(StateP state) {
	state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaConstrainedWaxFitnessFunction::initialize(StateP state) {

    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::None),KDL::Frame(KDL::Vector(0.0,-0.338,(0.1932+0.305+0.3)))));

    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Vector(0,0,0))));
    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Vector(0,0,-0.32901))));
    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Vector(0,0,-0.4692))));

	voidP sptr = state->getRegistry()->getEntry("function"); // get parameter value
    stringstream msg;
    _objPtr = penv->GetKinBody("object");

    if(!_objPtr) {
        fprintf(stderr,"error: object \"object\" does not exist.\n");
    } else printf("sucess: object \"object\" exists.\n");
    T_base_object = _objPtr->GetTransform();
    printf("object \"object\" at %f %f %f.\n", T_base_object.trans.x, T_base_object.trans.y, T_base_object.trans.z);

    usleep(1.0 * 1000000.0);

	return true;
}

/************************************************************************/

FitnessP CgdaConstrainedWaxFitnessFunction::evaluate(IndividualP individual) {

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

} // namespace teo