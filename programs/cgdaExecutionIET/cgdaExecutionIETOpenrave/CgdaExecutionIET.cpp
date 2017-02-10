// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "CgdaExecutionIET.hpp"

namespace teo
{
/************************************************************************/

void SetViewer(EnvironmentBasePtr penv, const string& viewername) {
    ViewerBasePtr viewer = RaveCreateViewer(penv,viewername);
    BOOST_ASSERT(!!viewer);
    // attach it to the environment:
    penv->AttachViewer(viewer);
    // finally you call the viewer's infinite loop (this is why you need a separate thread):
   bool showgui = true;
 //   bool showgui = false;
    viewer->main(showgui);
}

/************************************************************************/

//int numberOfPoints=17;
int numberOfPoints=9;
double time=0;
double evaluations=0;
bool CgdaExecutionIET::init() {
    RaveInitialize(true); // start openrave core
    penv = RaveCreateEnvironment(); // create the main environment
    RaveSetDebugLevel(Level_Debug);
    string viewername = "qtcoin";
    boost::thread thviewer(boost::bind(SetViewer,penv,viewername));
    string scenefilename = "../../share/models/teo_cgda_iros.env.xml";
    penv->Load(scenefilename); // load the scene
    //-- Get Robot 0
    std::vector<RobotBasePtr> robots;
    penv->GetRobots(robots);
    std::cout << "Robot 0: " << robots.at(0)->GetName() << std::endl;  // default: teo
    probot = robots.at(0);    

    //Uncomment for pause before start
    //std::cin.get();

    pcontrol = RaveCreateController(penv,"idealcontroller");
    // Create the controllers, make sure to lock environment! (prevents changes)
    {
      EnvironmentMutex::scoped_lock lock(penv->GetMutex());
      std::vector<int> dofindices(probot->GetDOF());
      for(int i = 0; i < probot->GetDOF(); ++i) {
        dofindices[i] = i;
      }
      probot->SetController(pcontrol,dofindices,1); // control everything
    }

    KinBodyPtr objPtr = penv->GetKinBody("object");
    if(!objPtr) printf("[WorldRpcResponder] fail grab\n");
    else printf("[WorldRpcResponder] good grab\n");
    probot->SetActiveManipulator("rightArm");
    probot->Grab(objPtr);

    vector< double > results;
    vector< double >* presults= &results;
    int const_evaluations;
    int* pconst_evaluations= &const_evaluations;
    *pconst_evaluations=0;


       for(unsigned int i=0; i<numberOfPoints; i++) {
           StateP state (new State);

           //PSOInheritance
           PSOInheritanceP nalg1 = (PSOInheritanceP) new PSOInheritance;
           state->addAlgorithm(nalg1);

           //PSOFuzzy
           PSOFuzzyP nalg2 = (PSOFuzzyP) new PSOFuzzy;
           state->addAlgorithm(nalg2);

           //ConstrainedSST
           ConstrainedSSTP nalg3 = (ConstrainedSSTP) new ConstrainedSST;
           state->addAlgorithm(nalg3);

           // set the evaluation operator
           CgdaPaintFitnessFunction* functionMinEvalOp = new CgdaPaintFitnessFunction;
           //CgdaWaxFitnessFunction* functionMinEvalOp = new CgdaWaxFitnessFunction;
           //Constrained Cost functions
           //CgdaConstrainedPaintFitnessFunction* functionMinEvalOp = new CgdaConstrainedPaintFitnessFunction;
           //CgdaConstrainedWaxFitnessFunction* functionMinEvalOp = new CgdaConstrainedWaxFitnessFunction;
           //functionMinEvalOp->setEvaluations(pconst_evaluations); //Uncomment only if CgdaFitnessFunction is uncomment

           functionMinEvalOp->setPRobot(probot);
           functionMinEvalOp->setPenv(penv);
           functionMinEvalOp->setPcontrol(pcontrol);
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

//           int parameter=100;
//           voidP sptr;
//           sptr= (voidP)&parameter;
//           state->getRegistry()->modifyEntry("term.fitnessval", sptr);

//           if(state->getRegistry()->isModified("term.fitnessval")){
//               std::cout<<"Ha sido modificado!!!!"<<std::endl;
//               std::cout<<"Valor  "<<*par<<std::endl;
//           }



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

           //*******************************************************************************************//
           //                              FILE OUTPUT FOR DEBUGGING                                    //
           //*******************************************************************************************//
           evaluations+=state->getEvaluations();
           time+=state->getElapsedTime();

           std::cout<<std::endl<<"THE TOTAL NUMBER OF EVALUATIONS IS: "<<evaluations<<std::endl<<"THE NUMBER OF EVALUATIONS IN THIS ITERATION IS: "<<state->getEvaluations() <<std::endl;
           std::cout<<std::endl<<"THE TIME TAKEN TO DO THIS IS:"<<time <<std::endl;
           //IndividualP bestParticle = selBestOp->select( *deme );

           std::ofstream myfile1;
           myfile1.open("TrajectoryterationsvsEvaluations.txt", std::ios_base::app);
           if (myfile1.is_open()){
               myfile1<<i<<" ";
               myfile1<<time<<" ";
               //if(const_evaluations==0){ //If we are not using the constrained version output normal
               //     myfile1<<evaluations<<" ";
               //}
               //else{
                   myfile1<<const_evaluations<<" ";
               //}
               myfile1<<bestInd[0]->fitness->getValue()<<std::endl;
           }          

           std::ofstream myfile2;
           myfile2.open("PercentageWall.txt", std::ios_base::app);
           if (myfile2.is_open()){
               myfile2<<std::endl;
           }

           std::ofstream myfile3;
           myfile3.open("X.txt", std::ios_base::app);
           if (myfile3.is_open()){
               myfile3<<std::endl;
           }

           std::ofstream myfile4;
           myfile4.open("Y.txt", std::ios_base::app);
           if (myfile4.is_open()){
               myfile4<<std::endl;
           }

           std::ofstream myfile5;
           myfile5.open("Z.txt", std::ios_base::app);
           if (myfile5.is_open()){
               myfile5<<std::endl;
           }

           //This following line is only for the execution of the result trajectory of the paint task
//           if(i==(numberOfPoints-1)){
//                //std::cout<<"LETS EXECUTE"<<std::endl;
//                functionMinEvalOp->trajectoryExecution(numberOfPoints, results);
//           }
           //*******************************************************************************************//


           //*******************************************************************************************//
           //                                      END                                                  //
           //*******************************************************************************************//
           std::cout<<"EL NUMERO DE EVALUACIONES ES::::"<<const_evaluations<<std::endl;
           nalg1.reset();
           nalg2.reset();
           nalg3.reset();
           state.reset();
           delete functionMinEvalOp;
       }

       printf("-begin-\n");
       for(unsigned int i=0;i<results.size();i++) printf("%f, ",results[i]);
       printf("\n-end-\n");
    return true;
}

/************************************************************************/

} //namespace TEO
