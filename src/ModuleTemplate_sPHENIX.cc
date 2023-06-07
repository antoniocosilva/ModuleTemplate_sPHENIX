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
#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxVertex.h>
#include <trackbase_historic/SvtxVertexMap.h>

/// Truth evaluation includes
#include <g4eval/JetEvalStack.h>
#include <g4eval/SvtxEvalStack.h>

/// HEPMC truth includes
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#pragma GCC diagnostic pop
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

  m_htracks_pt = new TH1F("htracks_pt", ";#it{p}_{T} (GeV/#it{c});Entries", 20, 0., 10.);

  return 0;
}

/**
 * Main workhorse function where each event is looped over and
 * data from each event is collected from the node tree for analysis
 */
int ModuleTemplate_sPHENIX::process_event(PHCompositeNode *topNode)
{
  if (Verbosity() > 5)
  {
    cout << "Beginning process_event in AnaTutorial" << endl;
  }

  SvtxTrackMap *trackmap = findNode::getClass<SvtxTrackMap>(topNode, "SvtxTrackMap");

  if (!trackmap)
  {
    cout << PHWHERE
         << "SvtxTrackMap node is missing, can't collect tracks"
         << endl;
    return 0;
  }

  SvtxTrack *track = 0;

  for (SvtxTrackMap::Iter iter = trackmap->begin();
       iter != trackmap->end();
       ++iter)
  {
    track = iter->second;

    if(!track) continue;

    m_htracks_pt->Fill(track->get_pt());
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
