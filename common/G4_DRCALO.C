#ifndef MACRO_G4DR_C
#define MACRO_G4DR_C

#include <GlobalVariables.C>

#include <g4calo/RawTowerBuilderDRCALO.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4eiccalos/PHG4ForwardCalCellReco.h>
#include <drcalo/PHG4ForwardDualReadoutSubsystem.h>

#include <g4eval/CaloEvaluator.h>

#include <g4main/PHG4Reco.h>

#include <caloreco/RawClusterBuilderFwd.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawTowerCalibration.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)

namespace Enable
{
  bool DRCALO = false;
  bool DRCALO_ABSORBER = false;
  bool DRCALO_CELL = false;
  bool DRCALO_TOWER = false;
  bool DRCALO_CLUSTER = false;
  bool DRCALO_EVAL = false;
  bool DRCALO_OVERLAPCHECK = false;
  int DRCALO_VERBOSITY = 0;
}  // namespace Enable

namespace G4DRCALO
{
  // from ForwardDualReadout/mapping/towerMap_DRCALO_v005.txt
  double Gz0 = 400.;
  double Gdz = 100.;
  double outer_radius = 262.;
  enum enu_FHcal_clusterizer
  {
    kFHcalGraphClusterizer,
    kFHcalTemplateClusterizer
  };
  //template clusterizer, as developed by Sasha Bazilevsky
  enu_FHcal_clusterizer FHcal_clusterizer = kFHcalTemplateClusterizer;
  // graph clusterizer
  //enu_FHcal_clusterizer FHcal_clusterizer = kFHcalGraphClusterizer;
  namespace SETTING
  {
    bool ECalReplace = false;
    bool LargeTowers = false;
  }  // namespace SETTING
}  // namespace G4DRCALO

void DRCALOInit()
{
  // simple way to check if only 1 of the settings is true
  if (( (G4DRCALO::SETTING::ECalReplace ? 1 : 0) + (G4DRCALO::SETTING::LargeTowers ? 1 : 0)) > 1)
  {
    cout << "use only  G4DRCALO::SETTING::FullEtaAcc=true or G4DRCALO::SETTING::HC2x=true or G4DRCALO::SETTING::HC4x=true" << endl;
    gSystem->Exit(1);
  }

  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4DRCALO::outer_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4DRCALO::Gz0 + G4DRCALO::Gdz / 2.);
}

void DRCALOSetup(PHG4Reco *g4Reco)
{
  const bool AbsorberActive = Enable::ABSORBER || Enable::DRCALO_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::DRCALO_OVERLAPCHECK;
  Fun4AllServer *se = Fun4AllServer::instance();

  /** Use dedicated DRCALO module */
  PHG4ForwardDualReadoutSubsystem *hhcal = new PHG4ForwardDualReadoutSubsystem("DRCALO");
  hhcal->OverlapCheck(OverlapCheck);
  hhcal->SuperDetector("DRCALO");
  hhcal->SetActive();

  ostringstream mapping_drcalo;

  // Switch to desired calo setup
  // full HCal Fe-Scint with nominal acceptance
  if (G4DRCALO::SETTING::LargeTowers)
    mapping_drcalo << getenv("CALIBRATIONROOT") << "/DRCALO/mapping/towerMap_DRCALO_large_tower.txt";
    // mapping_drcalo << getenv("EICCONFIGS") << "/towerMap_DRCALO_default_singleTower.txt";
  // full HCal Fe-Scint with enlarged beam pipe opening for Mar 2020 beam pipe
  else
    mapping_drcalo << getenv("CALIBRATIONROOT") << "/DRCALO/mapping/towerMap_DRCALO_default.txt";
    // mapping_drcalo << getenv("EICCONFIGS") << "/towerMap_DRCALO_replaceEMC.txt";

  hhcal->SetTowerMappingFile(mapping_drcalo.str());


  if (AbsorberActive) hhcal->SetAbsorberActive();

  g4Reco->registerSubsystem(hhcal);
}

void DRCALO_Cells(int verbosity = 0)
{
  return;
}

void DRCALO_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::DRCALO_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_drcalo;

  // Switch to desired calo setup
  // full HCal Fe-Scint with nominal acceptance
  if (G4DRCALO::SETTING::LargeTowers)
    mapping_drcalo << getenv("CALIBRATIONROOT") << "/DRCALO/mapping/towerMap_DRCALO_large_tower.txt";
    // mapping_drcalo << getenv("EICCONFIGS") << "/towerMap_DRCALO_default_singleTower.txt";
  // full HCal Fe-Scint with nominal acceptance doubled granularity
  else
    mapping_drcalo << getenv("CALIBRATIONROOT") << "/DRCALO/mapping/towerMap_DRCALO_default.txt";
    // mapping_drcalo << getenv("EICCONFIGS") << "/towerMap_DRCALO_replaceEMC.txt";


  RawTowerBuilderDRCALO *tower_DRCALO = new RawTowerBuilderDRCALO("TowerBuilder_DRCALO");
  tower_DRCALO->Detector("DRCALO");
  tower_DRCALO->set_sim_tower_node_prefix("SIM");
  tower_DRCALO->GeometryTableFile(mapping_drcalo.str());

  se->registerSubsystem(tower_DRCALO);

  cout << "def: using default for DRCALO towers" << endl;
  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("DRCALORawTowerDigitizer");
  TowerDigitizer->Detector("DRCALO");
  TowerDigitizer->Verbosity(1);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer);

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("DRCALORawTowerCalibration");
  TowerCalibration->Detector("DRCALO");
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1. / 0.03898);  // calibrated with muons
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration);
}

void DRCALO_Clusters()
{
  // int verbosity = std::max(Enable::VERBOSITY, Enable::DRCALO_VERBOSITY);
  // Fun4AllServer *se = Fun4AllServer::instance();

  // if (G4DRCALO::FHcal_clusterizer == G4DRCALO::kFHcalTemplateClusterizer)
  // {
  //   RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("DRCALORawClusterBuilderTemplate");
  //   ClusterBuilder->Detector("DRCALO");
  //   ClusterBuilder->SetPlanarGeometry();  // has to be called after Detector()
  //   ClusterBuilder->Verbosity(verbosity);
  //   ClusterBuilder->set_threshold_energy(0.100);
  //   se->registerSubsystem(ClusterBuilder);
  // }
  // else if (G4DRCALO::FHcal_clusterizer == G4DRCALO::kFHcalTemplateClusterizer)
  // {
  //   RawClusterBuilderFwd *ClusterBuilder = new RawClusterBuilderFwd("DRCALORawClusterBuilderFwd");
  //   ClusterBuilder->Detector("DRCALO");
  //   ClusterBuilder->Verbosity(verbosity);
  //   ClusterBuilder->set_threshold_energy(0.100);
  //   se->registerSubsystem(ClusterBuilder);
  // }
  // else
  // {
  //   cout << "DRCALO_Clusters - unknown clusterizer setting " << G4DRCALO::FHcal_clusterizer << endl;
  //   gSystem->Exit(1);
  // }

  return;
}

void DRCALO_Eval(const std::string &outputfile)
{
  // int verbosity = std::max(Enable::VERBOSITY, Enable::DRCALO_VERBOSITY);
  // Fun4AllServer *se = Fun4AllServer::instance();

  // CaloEvaluator *eval = new CaloEvaluator("DRCALOEVALUATOR", "DRCALO", outputfile.c_str());
  // eval->Verbosity(verbosity);
  // se->registerSubsystem(eval);

  return;
}
#endif
