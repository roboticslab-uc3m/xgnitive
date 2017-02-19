// -*- mode:C++; tab-width:3; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaPaintFitnessFunction.hpp"

#define NTPOINTS 17
#define NSQUARES 16

namespace teo
{

void CgdaPaintFitnessFunction::individualExecution(vector<double> results){
    std::cout<<"I have entered Execution"<<std::endl;

    // // reset square color
    for(int i=0; i<NSQUARES; i++){
           stringstream rr;
           rr << "square" << i;
           _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.5, 0.5, 0.5));
           rr.str("");
    }

    // set squares to sqpainted
    for(int i=0; i<(NSQUARES); i++)
    {
        stringstream rr;
        rr << "square" << i;
        if(psqPainted->operator [](i)==0){ //Only reset the ones being zero
            _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.5, 0.5, 0.5));
        }
        else{
            _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.0, 0.0, 1.0));
        }
        rr.str("");
    }

    //Move robot

    std::vector<dReal> dEncRaw(probot->GetDOF());  // NUM_MOTORS

    dEncRaw[0+4] = results[0]*M_PI/180.0;  // simple
    dEncRaw[1+4] = -results[1]*M_PI/180.0;  // simple
    dEncRaw[3+4] = results[2]*M_PI/180.0;  // simple

    dEncRaw[4+4] = 45*M_PI/180.0;

    probot->SetJointValues(dEncRaw);
    /*pcontrol->SetDesired(dEncRaw); // This function "resets" physics
    std::cout<<"I have entered Execution 2"<<std::endl;
    while(!pcontrol->IsDone()) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }*/
    std::cout<<"I have entered Execution 3"<<std::endl;

    penv->StepSimulation(0.0001);  // StepSimulation must be given in seconds
    T_base_object = _objPtr->GetTransform();
    double T_base_object_x = T_base_object.trans.x;
    double T_base_object_y = T_base_object.trans.y;
    double T_base_object_z = T_base_object.trans.z;


    //Update psqpainted to the new values
    for(int i=0; i<(NSQUARES); i++){
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
            for(int i=0;i<NSQUARES;i++){ //Ugly way of delimiting the size of sqpainted in the loop
                std::cout<<psqPainted->operator[](i)<<" ";
            }
         //   std::cout<<"<"<<std::endl;
            psqPainted->operator [](i)=1;
//            for(int i=0;i<NSQUARES;i++){ //Ugly way of delimiting the size of sqpainted in the loop
//                std::cout<<psqPainted->operator[](i)<<" ";
//            }
//            std::cout<<">"<<std::endl;
           // std::cout<<"I have painted a happy little tree \n"<<std::endl;
        }
        else{
          //  std::cout<<"NO HAPPY TREE"<<std::endl;
        }
        ss.str("");
    }
    //std::cout<<"HASTA AQUI LLEGUE"<<std::endl;
    std::ofstream myfile1;
    myfile1.open("memoryOET.txt", std::ios_base::out );
    if (myfile1.is_open()){
        for(int i=0;i<psqPainted->size();i++)
        {
            //std::cout<<"HASTA AQUI LLEGUE"<<std::endl;
            //myfile1<<"1 ";
            myfile1<< psqPainted->operator[](i) << " ";
            std::cout<<psqPainted->operator[](i) << " ";
        }
        std::cout<<std::endl;
        //myfile1<<bestInd[0]->fitness->getValue()<<std::endl; //Fitness
    }
    myfile1.close();

    sleep(1);

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

    //usleep(1.0 * 1000000.0);
	return true;
}

/************************************************************************/

FitnessP CgdaPaintFitnessFunction::evaluate(IndividualP individual) {
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

double CgdaPaintFitnessFunction::getCustomFitness(vector <double> genPoints){

    double Const_target[NTPOINTS]={0, 6.25, 12.5, 18.75, 25, 31.25, 37.5
                       , 43.75, 50, 56.25, 62.5, 68.75, 75, 81.25, 87.5, 93.75, 100};

    double percentage;
    //int sqPaintedAux [psqPainted->size()] = { };
    int sqPaintedAux [NSQUARES] = { }; //Variable used in order to not change psqPainted
    int timeStep;
    double Npaint=0;

    //printf("sqPainted check %d \n", psqPainted->operator [](0));

    //reset square color
   for(int i=0; i<(16); i++){
           stringstream rr;
           rr << "square" << i;
           _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.5, 0.5, 0.5));
           rr.str("");
    }

    // DEBUG
    for(int i=0; i<(psqPainted->size()); i++){

        //std::cout<<"El cuadrado "<<i<<" está pintado? "<<psqPainted->operator [](i)<<std::endl;
    }
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
            _wall->GetLink(rr.str())->GetGeometry(0)->SetDiffuseColor(RaveVector<float>(0.0, 0.0, 1.0));
            Npaint++;
        }
        rr.str("");
    }
    //Percentage of the wall painted before evolution
    printf("EL NPAINT ES::::::: %f \n",Npaint);
    percentage=(Npaint/NSQUARES)*100;
    printf("EL PERCENTAGE ES::::::: %f \n",percentage);

    //std::valarray<int> myvalarray (sqPaintedAux,sizeof(sqPaintedAux));
    //percentage.push_back(( (float)myvalarray.sum()/(sizeof(sqPaintedAux)))*100);

    //Serch the timeStep where we are
    double diff=1000000;
    for(int i=0;i<NTPOINTS;i++){ //We give priority to the elements at the last positions
        double diff_aux;
        //printf("Percentage: %d \n",percentage);
        //printf("Target: %d \n", Const_target[i]);
        //diff_aux=sqrt(pow((percentage-Const_target[i]),2)); //Diff is the euclidean distance
        diff_aux=percentage-Const_target[i];
        //printf("Diff_aux %d \n",diff_aux );
        diff_aux=diff_aux*diff_aux;
        //printf("Diff_aux %d \n",diff_aux );
        diff_aux=sqrt(diff_aux);
        //printf("Diff_aux %d \n",diff_aux );
        //printf("Diff %d \n",diff);


        if(diff_aux<diff){
            //std::cout<<" diff aux is "<<diff_aux<<" diff is "<<diff<<" i is "<<i<<std::endl;
            timeStep=i;
            diff=diff_aux;
            //printf("i -> %d", i);
            //printf("Time Step %d \n",timeStep);

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
    /*pcontrol->SetDesired(dEncRaw); //This function "resets" physics
    while(!pcontrol->IsDone()) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }*/
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

    //Calculate new percentage
    Npaint=0;
    for(int i=0;i<NSQUARES;i++){
        if(sqPaintedAux[i]==1){
            Npaint++;
        }
    }
    percentage=(Npaint/NSQUARES)*100;

//    std::valarray<int> myvalarray (sqPaintedAux,sizeof(sqPaintedAux));
//    percentage.push_back(( (float)myvalarray.sum()/(sizeof(sqPaintedAux)))*100);

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
    printf("EL TIME STEP ES EL SIGUIENTE:: %d \n",timeStep);
    printf("El percentage es:: %f", percentage);
    printf("El Consta target es:: %f \n", Const_target[timeStep+1]);
    //fit=sqrt(pow((percentage-Const_target[timeStep+1]),2)); //The fit is the euclidean distance between current feature and t+1. Since 1 dimension euclidean distance equals difference.
    fit=percentage-Const_target[timeStep+1];
    fit=fit*fit;
    //printf("Diff_aux %d \n",diff_aux );
    fit=sqrt(fit);
    //featureTrajectories->compare(attempVectforSimpleDiscrepancy,fit);

    //TODO: fit= sqrt(percentage²-percentage_now²) This has to be equivalent to the DTW discrepancy i think is the L2 norm.





    //  cout << std::endl << " percentage: "<< percentage[0] << ","<< percentage[1] << ","<< percentage[2] << ","<< percentage[3] << ","<< percentage[4];
    cout << std::endl << " fit: " << fit << " ";
    return fit;
}

/************************************************************************/

} // namespace teo
