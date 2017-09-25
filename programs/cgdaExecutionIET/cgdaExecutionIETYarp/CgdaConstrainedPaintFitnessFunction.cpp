// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaConstrainedPaintFitnessFunction.hpp"

namespace teo
{

/************************************************************************/

double target[17]={0, 6.25, 12.5, 18.75, 25, 31.25, 37.5
                   , 43.75, 50, 56.25, 62.5, 68.75, 75, 81.25, 87.5, 93.75, 100};

void CgdaConstrainedPaintFitnessFunction::trajectoryExecution(int NumberPoints, vector<double> result_trajectory){
    //std::cout<<"I have entered Execution"<<std::endl;

    const int rows=4; //setting wall parameters
    const int cols=4;
    int sqPainted [rows*cols] = { }; //setting number of changed square as cero

    // reset square color (Visualization)

    //Move robot
    for(int t=0;t<=NumberPoints;t++) {
            std::cout<<"Time interval"<<t<<std::endl;

            //***MOVE ROBOT***

            //***END MOVE***

            //***GET POS***

            //***END GET



            //change square color in function of dist (end-effector,square)
            for(int i=0; i<(rows*cols); i++){
                    stringstream ss;
                    ss << "square" << i;
                    sqPainted[i]=1;
                    ss.str("");
            }

            sleep(1);


    }

}


double CgdaConstrainedPaintFitnessFunction::getCustomFitness(vector <double> genPoints){

    double fit;

    //Aqui meter la condición de si está fuera del limite espacial impuesto
    double kdlrespoints[3];
    double* pkdlrespoints= kdlrespoints;
    double kdlgenpoints[3];
    double* pkdlgenpoints= kdlgenpoints;
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

    kdlgenpoints[0]=kf.p.x()*100;
    kdlgenpoints[1]=kf.p.y()*100;
    kdlgenpoints[2]=kf.p.z()*100;

    //region            //Wall Coord.   //+-0.01(++Adjusted) //+-0.1(Adjusted) //+-0.2 (intermediate) //+-0.3 (large) //+-0.05 (Adjusted+)
    float xl=-100;      //0.6            0.59                0.5               0.4                    0.3             0.55
    float xu=100;      //0.6            0.61                0.7               0.8                    0.9             0.65
    float yl=-100;     //-0.8          -0.81               -0.9               -1                     -1.1            -0.85
    float yu=100;     //-0.2          -0.19               -0.1               0                      0.1             -0.15
    float zl=-100;      //0.32           0.31                0.2               0.1                    0               0.25
    float zu=100;      //0.92           0.93                1                 1.1                    1.2             0.95

    //if not in the allowed region
    if(1==2){//!(((kf.p.x()>xl) && (kf.p.x()<xu)) && ((kf.p.y()>yl) && (kf.p.y()<yu)) && ((kf.p.z()>zl) && (kf.p.z()<zu)))){
        std::cout<<"***************************************SPACE LIMITED********************************"<<std::endl;
        return 10000;
    }

    else{
        const int rows=4; //setting wall parameters
        const int cols=4;
        std::vector<double> percentage;
        int sqPainted [rows*cols] = { }; //setting number of changed square as cero

        //***reset square color (Visualization)***

        //***                                  ***
        for(int t=0;t<=*pIter;t++) {

            //***MOVE ROBOT***
            if (t<*pIter){
                //KDL vector
                KDL::JntArray qInRad = KDL::JntArray(GdL);
                qInRad(0) = -1*(presults->operator [](t*3+0))*M_PI/180.0;
                qInRad(1) = -1*(presults->operator [](t*3+1))*M_PI/180.0;
                qInRad(2) = -1*(presults->operator [](t*3+2))*M_PI/180.0;
                KDL::Frame kf;
                KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(chain);
                fksolver.JntToCart(qInRad,kf);

                std::cout << "x: FK: " << kf.p.x() << " | ";
                std::cout << "y: FK: " << kf.p.y() << " | ";
                std::cout << "z: FK: " << kf.p.z() << std::endl;
                kdlrespoints[0]=kf.p.x()*100;
                kdlrespoints[1]=kf.p.y()*100;
                kdlrespoints[2]=kf.p.z()*100;

                pos->positionMove(pkdlrespoints);
                printf("Delaying 10 seconds...\n");
                yarp::os::Time::delay(10);

            }
            else if (t==*pIter){
                std::cout<<pkdlgenpoints[0]<<std::endl;
                std::cout<<pkdlgenpoints[1]<<std::endl;
                std::cout<<pkdlgenpoints[2]<<std::endl;
                pos->positionMove(pkdlgenpoints);
                printf("Delaying 10 seconds...\n");
                yarp::os::Time::delay(10);
            }
            else{cerr << "ERROR IN pIter or t" << std::endl;}

            //***END MOVE***

            //change square color in function of dist (end-effector,square)
            for(int i=0; i<(rows*cols); i++){

                stringstream ss;
                ss << "square" << i;
                sqPainted[i]=1;
                }

            //Fitness = percentage of wall painted
            std::valarray<int> myvalarray (sqPainted,rows*cols);
            percentage.push_back(( (float)myvalarray.sum()/(rows*cols))*100);

        } //cierre bucle trayectoria completa

        //Output to file
        std::ofstream myfile2;
        myfile2.open("PercentageWall.txt", std::ios_base::app);
        if (myfile2.is_open()){
            myfile2<<percentage[*pIter]<<" ";
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
        std::vector< std::vector< double > > attempVectforSimpleDiscrepancy; //Init feature attemp vector
        for(int t=0;t<percentage.size();t++)
        {
            attempVectforSimpleDiscrepancy.push_back({percentage[t]});
            //std::cout<<std::endl<<"ATTEMP TRAJECTORY ::::"<<t<<" : "<<percentage[t]<<std::endl;
        }

        //Console output.
        for(int i=0; i<attempVectforSimpleDiscrepancy[0].size(); i++){ //For each vector of characteristics(each column). In this case should be 1.
            std::cout<<std::endl<<std::endl;
            for(int j=0; j<attempVectforSimpleDiscrepancy.size(); j++){ //For each trajectory step
                std::cout<<"trajectory step "<<j<<" ==> " <<attempVectforSimpleDiscrepancy[j][i]<<std::endl;
            }
        }

        featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);
        *const_evaluations=*const_evaluations+1;
    }


  //  cout << std::endl << " percentage: "<< percentage[0] << ","<< percentage[1] << ","<< percentage[2] << ","<< percentage[3] << ","<< percentage[4];
    cout << std::endl << " fit: " << fit << " ";
    return fit;
}

/************************************************************************/

void CgdaConstrainedPaintFitnessFunction::registerParameters(StateP state) {
	state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaConstrainedPaintFitnessFunction::initialize(StateP state) {

    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::None),KDL::Frame(KDL::Vector(0.0,-0.338,(0.1932+0.305+0.3)))));

    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Vector(0,0,0))));
    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Vector(0,0,-0.32901))));
    chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Vector(0,0,-0.4692))));

	voidP sptr = state->getRegistry()->getEntry("function"); // get parameter value
    stringstream msg;
    //_objPtr = penv->GetKinBody("object");
    //_wall = penv->GetKinBody("wall");

    //if(!_objPtr) {
    //    fprintf(stderr,"error: object \"object\" does not exist.\n");
    //} else printf("sucess: object \"object\" exists.\n");

    //if(!_wall) {
    //    fprintf(stderr,"error: object \"wall\" does not exist.\n");
    //} else printf("sucess: object \"wall\" exists.\n");

    usleep(1.0 * 1000000.0);

	return true;
}

/************************************************************************/

FitnessP CgdaConstrainedPaintFitnessFunction::evaluate(IndividualP individual) {

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
