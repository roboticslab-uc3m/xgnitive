// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EvMono.hpp"
#include "AlgPSOInheritance.hpp"
#include "AlgPSOFuzzy.hpp"

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

bool EvMono::init() {

    RaveInitialize(true); // start openrave core
    penv = RaveCreateEnvironment(); // create the main environment
    RaveSetDebugLevel(Level_Debug);
    string viewername = "qtcoin";
    boost::thread thviewer(boost::bind(SetViewer,penv,viewername));
    string scenefilename = "../../programs/cgdaExecution/models/teo_cgda_iros.env.xml";
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

    // set the evaluation operator, init CgdaExecution
    FunctionMinEvalOp* functionMinEvalOp = new FunctionMinEvalOp; 
    functionMinEvalOp->setPRobot(probot);
    functionMinEvalOp->setPenv(penv);
    functionMinEvalOp->setPcontrol(pcontrol);
    //Here the functionMinEvalOp is set as the evaulation function
    state->setEvalOp(functionMinEvalOp);

    //por aqui se muestran resultados
    printf("---------------------------> ");

    int newArgc = 2;
    char *newArgv[2] = { (char*)"unusedFirstParam", "../../programs/cgdaExecution/conf/evMono_ecf_params.xml" };

    state->initialize(newArgc, newArgv);
    //Debugging
    Population pop;
    pop.initialize(state);
    AlgorithmP alg;
    alg=state->getAlgorithm();

    std::cout << "Population: " << pop[0]->getSize() << std::endl;
    std::cout << "Algorithm: " << alg->getName() << std::endl;

    state->run();

    vector<IndividualP> bestInd;
    FloatingPoint::FloatingPoint* genBest;
    vector<double> bestPoints;

    // final result
    bestInd = state->getHoF()->getBest();
    genBest = (FloatingPoint::FloatingPoint*) bestInd.at(0)->getGenotype().get();
    bestPoints = genBest->realValue;

    printf("-begin-\n");
    for(unsigned int i=0;i<bestPoints.size();i++)
        printf("%f, ",bestPoints[i]);
    printf("\n-end-\n");

    return true;
}

/************************************************************************/
}
