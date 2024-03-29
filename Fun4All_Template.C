#include <g4main/Fun4AllDstPileupInputManager.h>

#include <G4_Magnet.C>
#include <G4_Tracking.C>

#include <moduletemplate_sphenix/ModuleTemplate_sPHENIX.h>

#include <FROG.h>
#include <decayfinder/DecayFinder.h>
#include <fun4all/Fun4AllDstInputManager.h>
//#include <qa_modules/QAG4SimulationKFParticle.h>

#include <g4eval/SvtxEvaluator.h>
#include <g4eval/SvtxTruthRecoTableEval.h>

#include <caloreco/RawClusterBuilderTopo.h>
#include <particleflowreco/ParticleFlowReco.h>

//R__LOAD_LIBRARY(libqa_modules.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmoduletemplate_sphenix.so)



using namespace std;
//Inspired by Cameron Fun4All code
/****************************/
/*     MDC2 Reco for MDC2     */
/* Cameron Dean, LANL, 2021 */
/*      cdean@bnl.gov       */
/****************************/

void Fun4All_Template(vector<string> myInputLists = {"productionFiles-JET_30GEV-dst_tracks-00000.list","productionFiles-JET_30GEV-dst_vertex-00000.list","productionFiles-JET_30GEV-dst_truth-00000.list","productionFiles-JET_30GEV-dst_trkr_g4hit-00000.list", "productionFiles-JET_30GEV-dst_trkr_hit-00000.list","productionFiles-JET_30GEV-dst_trackseeds-00000.list","productionFiles-JET_30GEV-dst_trkr_cluster-00000.list","productionFiles-JET_30GEV-dst_calo_cluster-00000.list","productionFiles-JET_30GEV-dst_bbc_g4hit-00000.list"}, const int nEvents = 100)
{
  int verbosity = 1;

  gSystem->Load("libg4dst.so");
  gSystem->Load("libFROG.so");
  FROG *fr = new FROG();

  //The next set of lines figures out folder revisions, file numbers etc
  string outDir = "./";
  if (outDir.substr(outDir.size() - 1, 1) != "/") outDir += "/";
  outDir +=  "Output/";

  string fileNumber = myInputLists[0];
  size_t findLastDash = fileNumber.find_last_of("-");
  if (findLastDash != string::npos) fileNumber.erase(0, findLastDash + 1);
  string remove_this = ".list";
  size_t pos = fileNumber.find(remove_this);
  if (pos != string::npos) fileNumber.erase(pos, remove_this.length());
  string outputFileName = "outputData_" + fileNumber + ".root";

  string outputRecoDir = outDir + "/inReconstruction/";
  string makeDirectory = "mkdir -p " + outputRecoDir;
  system(makeDirectory.c_str());
  string outputRecoFile = outputRecoDir + outputFileName;

  //Create the server
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(verbosity);

  //Add all required input files
  for (unsigned int i = 0; i < myInputLists.size(); ++i)
  {
    Fun4AllInputManager *infile = new Fun4AllDstInputManager("DSTin_" + to_string(i));
    infile->AddListFile(myInputLists[i]);
    se->registerInputManager(infile);
  }

  SvtxTruthRecoTableEval *tables = new SvtxTruthRecoTableEval();

  tables->Verbosity(0);

  se->registerSubsystem(tables);

  ModuleTemplate_sPHENIX *module = new ModuleTemplate_sPHENIX("module", outputRecoFile);
  module->Verbosity(1);
  se->registerSubsystem(module);

  se->run(nEvents);
  se->End();

  ifstream file(outputRecoFile.c_str());
  if (file.good())
  {
    string moveOutput = "mv " + outputRecoFile + " " + outDir;
    system(moveOutput.c_str());
  }


  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);

  return;
}
