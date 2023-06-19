#ifndef MODULETEMPLATE_SPHENIX_H__
#define MODULETEMPLATE_SPHENIX_H__


#define HomogeneousField


#include <fun4all/SubsysReco.h>

#include <fastjet/ClusterSequence.hh>
#include <fastjet/FunctionOfPseudoJet.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>

#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calotrigger/CaloTriggerInfo.h>

#include <jetbase/Jetv1.h>
#include <jetbase/JetMapv1.h>

#include <vector>

/// Class declarations for use in the analysis module
class Fun4AllHistoManager;
class PHCompositeNode;
class TFile;
class TTree;
class TH1;
class TH1I;
class PHCompositeNode;
class RawClusterContainer;
class RawCluster;
class SvtxTrackMap;
class GlobalVertex;
class PHHepMCGenEventMap;
class JetRecoEval;
class SvtxTrackEval;
class PHG4TruthInfoContainer;
class PHHepMCGenEvent;
class SvtxTrack;
class PHG4Particle;

/// Definition of this analysis module class
class ModuleTemplate_sPHENIX : public SubsysReco
{
 public:

  /// Constructor
  ModuleTemplate_sPHENIX(const std::string &name = "ModuleTemplate_sPHENIX",
              const std::string &fname = "ModuleTemplate_sPHENIX.root");

  // Destructor
  virtual ~ModuleTemplate_sPHENIX();

  /// SubsysReco initialize processing method
  int Init(PHCompositeNode *);

  /// SubsysReco event processing method
  int process_event(PHCompositeNode *);

  /// SubsysReco end processing method
  int End(PHCompositeNode *);



 private:
  /// String to contain the outfile name containing the trees
  std::string m_outfilename;
  std::string m_jetcontainer_name;

  /// Fun4All Histogram Manager tool
  Fun4AllHistoManager *m_hm;
  TH1 *m_htracks_pt;



  /// TFile to hold the following TTrees and histograms
  TFile *m_outfile;


  /**
   * Make variables for the relevant trees
   */


};

#endif
