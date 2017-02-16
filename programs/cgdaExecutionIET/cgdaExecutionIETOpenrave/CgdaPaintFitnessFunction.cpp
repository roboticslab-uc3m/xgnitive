// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaPaintFitnessFunction.hpp"

namespace teo
{

/************************************************************************/

//double target[10]={0, 10, 20, 30, 40, 50, 60, 70, 80, 100};
double Const_target[17]={0, 6.25, 12.5, 18.75, 25, 31.25, 37.5
                   , 43.75, 50, 56.25, 62.5, 68.75, 75, 81.25, 87.5, 93.75, 100};

/************************************************************************/

void CgdaPaintFitnessFunction::trajectoryExecution(int NumberPoints, vector<double> result_trajectory){
    //std::cout<<"I have entered Execution"<<std::endl;

   // // reset square color
   for(int i=0; i<(psqPainted->size()); i++){
           stringstream rr;
           rr << "square" << i;
           _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.5, 0.5, 0.5));
           rr.str("");
       }
    //Move robot
    for(int t=0;t<=NumberPoints;t++) {
            std::cout<<"Time interval"<<t<<std::endl;
            std::vector<dReal> dEncRaw(probot->GetDOF());  // NUM_MOTORS

            dEncRaw[0+4] = result_trajectory[t*3+0]*M_PI/180.0;  // simple
            dEncRaw[1+4] = -result_trajectory[t*3+1]*M_PI/180.0;  // simple
            dEncRaw[3+4] = result_trajectory[t*3+2]*M_PI/180.0;  // simple

            dEncRaw[4+4] = 45*M_PI/180.0;
            probot->SetJointValues(dEncRaw);
            pcontrol->SetDesired(dEncRaw); // This function "resets" physics
            while(!pcontrol->IsDone()) {
                boost::this_thread::sleep(boost::posix_time::milliseconds(1));
            }
            penv->StepSimulation(0.0001);  // StepSimulation must be given in seconds
            //std::cout<<"W8ing"<<std::endl;
            T_base_object = _objPtr->GetTransform();
            double T_base_object_x = T_base_object.trans.x;
            double T_base_object_y = T_base_object.trans.y;
            double T_base_object_z = T_base_object.trans.z;

            //change square color in function of dist (end-effector,square)
            for(int i=0; i<(psqPainted->size()); i++){
                    stringstream ss;
                    ss << "square" << i;
                    Transform pos_square = _wall->GetLink(ss.str())->GetGeometry(0)->GetTransform();

                    double pos_square_x = pos_square.trans.x;
                    double pos_square_y = pos_square.trans.y;
                    double pos_square_z = pos_square.trans.z;
                    double dist = sqrt(pow(T_base_object_x-pos_square_x,2)
                                  + pow(T_base_object_y-pos_square_y,2)
                                  + pow(T_base_object_z-pos_square_z,2) );

                       if (dist < 0.13){
                        _wall->GetLink(ss.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.0, 0.0, 1.0));
                        psqPainted->operator[](i)=1;
                        //std::cout<<"I have painted a happy little tree "<<std::endl;
                    }
                    ss.str("");
            }

            sleep(1);


    }

}

/************************************************************************/

void CgdaPaintFitnessFunction::registerParameters(StateP state) {
	state->getRegistry()->registerEntry("function", (voidP) (new uint(1)), ECF::UINT);
}

/************************************************************************/

bool CgdaPaintFitnessFunction::initialize(StateP state) {

    voidP sptr = state->getRegistry()->getEntry("function"); // get parameter value
    stringstream msg;
    _objPtr = penv->GetKinBody("object");
    _wall = penv->GetKinBody("wall");

    if(!_objPtr) {
        fprintf(stderr,"error: object \"object\" does not exist.\n");
    } else printf("sucess: object \"object\" exists.\n");

    if(!_wall) {
        fprintf(stderr,"error: object \"wall\" does not exist.\n");
    } else printf("sucess: object \"wall\" exists.\n");

    usleep(1.0 * 1000000.0);
	return true;
}

/************************************************************************/

FitnessP CgdaPaintFitnessFunction::evaluate(IndividualP individual) {
    printf("HASTA AQUI LLEGUE    0!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 0");
	// evaluation creates a new fitness object using a smart pointer
	// in our case, we try to minimize the function value, so we use FitnessMin fitness (for minimization problems)
	FitnessP fitness (new FitnessMin);

	// we define FloatingPoint as the only genotype (in the configuration file)
	FloatingPoint::FloatingPoint* gen = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();

	// we implement the fitness function 'as is', without any translation
	// the number of variables is read from the genotype itself (size of 'realValue' vactor)

    double value =0;
    printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!3");
    value= getCustomFitness(gen->realValue);
    fitness->setValue(value);
    printf("HASTA AQUI LLEGUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!4");
    return fitness;
}

/************************************************************************/

double CgdaPaintFitnessFunction::getCustomFitness(vector <double> genPoints){
    printf("    0!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 0");
    double percentage;
    //int sqPaintedAux [psqPainted->size()] = { };
    int sqPaintedAux [4*4] = { }; //Variable used in order to not change psqPainted
    int timeStep;
    int Npaint;

    printf("sqPainted check %d", psqPainted->operator [](0));

    // Set Wall color to psqPainted variable and init sqPainted auxiliary variable
    for(int i=0; i<(psqPainted->size()); i++)
    {
        stringstream rr;
        rr << "square" << i;
        if(psqPainted->operator [](i)==0){ //Only reset the ones being zero
            _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.5, 0.5, 0.5));
        }
        else{
            sqPaintedAux[i]=1;
            Npaint++;
        }
        rr.str("");
    }

    //Percentage of the wall painted before evolution
    percentage=(Npaint/sizeof(sqPaintedAux))*100;

    //std::valarray<int> myvalarray (sqPaintedAux,sizeof(sqPaintedAux));
    //percentage.push_back(( (float)myvalarray.sum()/(sizeof(sqPaintedAux)))*100);

    //Serch the timeStep where we are
    double diff;
    for(int i=0;i<sizeof(Const_target);i++){ //We give priority to the elements at the last positions
        double diff_aux;
        diff_aux=sqrt(pow((percentage-Const_target[i]),2)); //Diff is the euclidean distance
        if(diff_aux<diff){
            timeStep=i;
        }
    }

    //Set new positions of the robot using dEncRaw
    std::vector<dReal> dEncRaw(probot->GetDOF());  // NUM_MOTORS
    dEncRaw[0+4] = genPoints[0]*M_PI/180.0;  // simple
    dEncRaw[1+4] = -genPoints[1]*M_PI/180.0;  // simple
    dEncRaw[3+4] = genPoints[2]*M_PI/180.0;  // simple

    dEncRaw[4+4] = 45*M_PI/180.0;

    //Actually move the robot
    probot->SetJointValues(dEncRaw);
    pcontrol->SetDesired(dEncRaw); //This function "resets" physics
    while(!pcontrol->IsDone()) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }
    penv->StepSimulation(0.0001);  // StepSimulation must be given in seconds

    //Get new object (pen) position
    T_base_object = _objPtr->GetTransform();
    double T_base_object_x = T_base_object.trans.x;
    double T_base_object_y = T_base_object.trans.y;
    double T_base_object_z = T_base_object.trans.z;

    //change square color in function of dist (end-effector,square)
    for(int i=0; i<(psqPainted->size()); i++){
        stringstream ss;
        ss << "square" << i;
        Transform pos_square = _wall->GetLink(ss.str())->GetGeometry(0)->GetTransform();

        double pos_square_x = pos_square.trans.x;
        double pos_square_y = pos_square.trans.y;
        double pos_square_z = pos_square.trans.z;
        double dist = sqrt(pow(T_base_object_x-pos_square_x,2)
                           + pow(T_base_object_y-pos_square_y,2)
                           + pow(T_base_object_z-pos_square_z,2) );

        if (dist < 0.13){
            _wall->GetLink(ss.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.0, 0.0, 1.0));
            sqPaintedAux[i]=1;
            //std::cout<<"I have painted a happy little tree "<<std::endl;
        }
            ss.str("");
   }

    //Fitness of the actual point= percentage of the wall painted

    //Calculate percentage
    for(int i=0;i<sizeof(sqPaintedAux);i++){
        if(sqPaintedAux[i]==1){
            Npaint++;
        }
    }
    percentage=(Npaint/sizeof(sqPaintedAux))*100;

//    std::valarray<int> myvalarray (sqPaintedAux,sizeof(sqPaintedAux));
//    percentage.push_back(( (float)myvalarray.sum()/(sizeof(sqPaintedAux)))*100);

    //Output to file
//    std::ofstream myfile2;
//    myfile2.open("PercentageWall.txt", std::ios_base::app);
//    if (myfile2.is_open()){
//        myfile2<<percentage[*pIter]<<" ";
//    }

    // calculate fit /percentage of painted wall
    //double fit = abs(percentage-target[*pIter]);

//    //The fit is obtained using the DTW algorithm.
//    //The feature is the percentage of wall painted
//    CgdaRecognition* featureTrajectories;
//    featureTrajectories = new DtwCgdaRecognition;
//    //Current Time step

//    //Get the discrepancy value between what we have and what we want
//    //Init feature attemp vector
//    std::vector< std::vector< double > > attempVectforSimpleDiscrepancy;
//    for(int t=0;t<percentage.size();t++)
//    {
//        attempVectforSimpleDiscrepancy.push_back({percentage[t]});
//        //std::cout<<std::endl<<"ATTEMP TRAJECTORY ::::"<<t<<" : "<<percentage[t]<<std::endl;
//    }

//    //Console output.
//    for(int i=0; i<attempVectforSimpleDiscrepancy[0].size(); i++){ //For each vector of characteristics(each column). In this case should be 1.
//        std::cout<<std::endl<<std::endl;
//        for(int j=0; j<attempVectforSimpleDiscrepancy.size(); j++){ //For each trajectory step
//            std::cout<<"trajectory step "<<j<<" ==> " <<attempVectforSimpleDiscrepancy[j][i]<<std::endl;
//        }
//    }

    //Here we dont need the DTW since we are not comparing trajectories only points


    //The fit is the L2 norm between the current features, and the t+1 feature environment.
    double fit;
    fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1

    //featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

    //TODO: fit= sqrt(percentage²-percentage_now²) This has to be equivalent to the DTW discrepancy i think is the L2 norm.





    //  cout << std::endl << " percentage: "<< percentage[0] << ","<< percentage[1] << ","<< percentage[2] << ","<< percentage[3] << ","<< percentage[4];
    cout << std::endl << " fit: " << fit << " ";
    return fit;
}

/************************************************************************/

} // namespace teo
