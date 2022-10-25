#include "ModuleTemplate_sPHENIX.h"

/// Cluster/Calorimeter includes
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calotrigger/CaloTriggerInfo.h>

#include <phool/phool.h>

/// Jet includes
#include <g4jets/Jetv1.h>
#include <g4jets/JetMap.h>

/// Tracking includes
#include <g4vertex/GlobalVertex.h>
#include <g4vertex/GlobalVertexMap.h>
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxVertex.h>
#include <trackbase_historic/SvtxVertexMap.h>

/// Truth evaluation includes
#include <g4eval/JetEvalStack.h>
#include <g4eval/SvtxEvalStack.h>

/// HEPMC truth includes
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>

/// Fun4All includes
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <KFParticle.h>
#include <kfparticle_sphenix/KFParticle_Container.h>

#include <g4jets/FastJetAlgo.h>
#include <g4jets/Jet.h>
#include <g4jets/Jetv1.h>
//#include <jetbackground/FastJetAlgoSub.h>

#include <fastjet/ClusterSequence.hh>
#include <fastjet/FunctionOfPseudoJet.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>

#include <g4main/PHG4Particle.h>            // for PHG4Particle
#include <g4main/PHG4TruthInfoContainer.h>  // for PHG4TruthInfoContainer
#include <g4main/PHG4VtxPoint.h>            // for PHG4VtxPoint
#include <trackbase_historic/SvtxPHG4ParticleMap_v1.h>
#include <kfparticle_sphenix/KFParticle_truthAndDetTools.h>

/// ROOT includes
#include <TFile.h>
#include <TH1.h>
#include <TH1I.h>
#include <TMath.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>

/// C++ includes
#include <cassert>
#include <sstream>
#include <string>

using namespace std;

/**
 * ModuleTemplate_sPHENIX is a class developed to reconstruct jets containing a D-meson
 * The class can be adapted to tag jets using any kind of particle
 * Author: Antonio Silva (antonio.sphenix@gmail.com)
 */

/**
 * Constructor of module
 */
ModuleTemplate_sPHENIX::ModuleTemplate_sPHENIX(const std::string &name, const std::string &filename)
  : SubsysReco(name)
  , m_outfilename(filename)
  , m_hm(nullptr)
{
  /// Initialize variables and trees so we don't accidentally access
  /// memory that was never allocated
}

/**
 * Destructor of module
 */
ModuleTemplate_sPHENIX::~ModuleTemplate_sPHENIX()
{
  delete m_hm;
}

/**
 * Initialize the module and prepare looping over events
 */
int ModuleTemplate_sPHENIX::Init(PHCompositeNode *topNode)
{
  if (Verbosity() > 5)
  {
    cout << "Beginning Init in ModuleTemplate_sPHENIX" << endl;
  }

  m_outfile = new TFile(m_outfilename.c_str(), "RECREATE");

  return 0;
}

/**
 * Main workhorse function where each event is looped over and
 * data from each event is collected from the node tree for analysis
 */
int ModuleTemplate_sPHENIX::process_event(PHCompositeNode *topNode)
{

  JetMapv1* jetContainer = new JetMapv1();

  PHNodeIterator nodeIter(topNode);
  PHNode *findNode = dynamic_cast<PHNode *>(nodeIter.findFirst("DST"));
  if (findNode)
  {
    findNode = dynamic_cast<PHNode *>(nodeIter.findFirst("D0Jets_Jet_Container"));
    if (findNode)
    {
      jetContainer = findNode::getClass<JetMapv1>(topNode, "D0Jets_Jet_Container");
      //cout << "****************** CONTAINER FOUND with size: " << kfContainer->size() << endl;
    }
  }

  cout << "Jet Container Found!" << endl;

  cout << "Size: " << jetContainer->size() << endl;

  Jet *D0jet = 0;

  for(JetMapv1::Iter ijet = jetContainer->begin(); ijet != jetContainer->end(); ++ijet)
  {
    D0jet = ijet->second;

    cout << "D0-Jet pT: " << D0jet->get_pt() << endl;
  }



  return Fun4AllReturnCodes::EVENT_OK;
}

/**
 * End the module and finish any data collection. Clean up any remaining
 * loose ends
 */
int ModuleTemplate_sPHENIX::End(PHCompositeNode *topNode)
{
  if (Verbosity() > 1)
  {
    cout << "Ending ModuleTemplate_sPHENIX analysis package" << endl;
  }

  /// Change to the outfile
  m_outfile->cd();

  /// Write and close the outfile
  m_outfile->Write();
  m_outfile->Close();

  /// Clean up pointers and associated histos/trees in TFile
  delete m_outfile;

  if (Verbosity() > 1)
  {
    cout << "Finished ModuleTemplate_sPHENIX analysis package" << endl;
  }

  return 0;
}
