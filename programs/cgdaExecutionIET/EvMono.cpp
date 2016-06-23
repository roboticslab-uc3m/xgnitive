// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//Copyright: Universidad Carlos III de Madrid (C) 2016
//Authors: jgvictores, raulfdzbis, smorante

#include "EvMono.hpp"

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

int numberOfPoints=17;
bool EvMono::init() {
    RaveInitialize(true); // start openrave core
    penv = RaveCreateEnvironment(); // create the main environment
    RaveSetDebugLevel(Level_Debug);
    string viewername = "qtcoin";
    boost::thread thviewer(boost::bind(SetViewer,penv,viewername));
    string scenefilename = "../../programs/models/teo_cgda_iros.env.xml";
    penv->Load(scenefilename); // load the scene
    //-- Get Robot 0
    std::vector<RobotBasePtr> robots;
    penv->GetRobots(robots);
    std::cout << "Robot 0: " << robots.at(0)->GetName() << std::endl;  // default: teo
    probot = robots.at(0);

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
    probot->SetActiveManipulator("m2");
    probot->Grab(objPtr);

	StateP state (new State);

    PSOInheritanceP nalg1 = (PSOInheritanceP) new PSOInheritance;
    state->addAlgorithm(nalg1);

    PSOFuzzyP nalg2 = (PSOFuzzyP) new PSOFuzzy;
    state->addAlgorithm(nalg2);

    vector< double > results;
    vector< double >* presults= &results;
    // set the evaluation operator
    CgdaExecutionIET* functionMinEvalOp = new CgdaExecutionIET; 
    functionMinEvalOp->setPRobot(probot);
    functionMinEvalOp->setPenv(penv);
    functionMinEvalOp->setPcontrol(pcontrol);
    functionMinEvalOp->setResults(presults);

    state->setEvalOp(functionMinEvalOp);


       for(unsigned int i=0; i<numberOfPoints; i++) {
           unsigned int* pIter= &i;
           functionMinEvalOp->setIter(pIter);

           printf("---------------------------> i:%d\n",i);
           int newArgc = 2;
           char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecutionIET/conf/evMono_ecf_params.xml" };

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


       }

       printf("-begin-\n");
       for(unsigned int i=0;i<results.size();i++) printf("%f, ",results[i]);
       printf("\n-end-\n");
    return true;
}

/************************************************************************/

} //namespace TEO
