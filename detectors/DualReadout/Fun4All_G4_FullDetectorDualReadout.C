#ifndef MACRO_FUN4ALLG4EICDETECTOR_C
#define MACRO_FUN4ALLG4EICDETECTOR_C

#include <GlobalVariables.C>

#include <DisplayOn.C>
#include <G4Setup_DualReadoutDetector.C>
#include <G4_Bbc.C>
#include <G4_CaloTrigger.C>
#include <G4_DSTReader_EICDetector.C>
#include <G4_FwdJets.C>
#include <G4_Global.C>
#include <G4_HIJetReco.C>
#include <G4_Input.C>
#include <G4_Jets.C>
#include <G4_Production.C>
#include <G4_User.C>
#include <QA.C>

#include <TROOT.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <PHPy6GenTrigger.h>
#include <PHPy6ParticleTrigger.h>
#include <PHPy6JetTrigger.h>
#include <phool/recoConsts.h>

#include <g4eval/EventEvaluator.h>

R__LOAD_LIBRARY(libfun4all.so)

void ParseTString(TString &specialSetting);

int Fun4All_G4_FullDetectorDualReadout(
    const int nEvents = 1,
    const double particlemomMin = -1,
    const double particlemomMax = -1,
    TString specialSetting = "ALLSILICON-FTTLS3LC-ETTL-CTTL",
    TString generatorSettings = "",
    const string &inputFile = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
    const string &outputFile = "G4EICDetector.root",
    const string &embed_input_file = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
    const int skip = 0,
    const string &outdir = ".")
{
// translate the option TString into subsystem namespace options
  ParseTString(specialSetting); 
  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  //Opt to print all random seed used for debugging reproducibility. Comment out to reduce stdout prints.
  //PHRandomSeed::Verbosity(1);

  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as initial seed
  // which will produce identical results so you can debug your code
  //rc->set_IntFlag("RANDOMSEED", 12345);

  //===============
  // Input options
  //===============

  // pythia6
  // Use Pythia 6
  if(particlemomMin==-1 && particlemomMax==-1){
    Input::PYTHIA6 = true;
  }
  // Simple multi particle generator in eta/phi/pt ranges
  Input::SIMPLE = false;
  if (particlemomMin>-1 && particlemomMax>-1){
    Input::SIMPLE = true;
    Input::SIMPLE_VERBOSITY = 0;
 }
  // Input::SIMPLE_NUMBER = 2; // if you need 2 of them
 
  Input::VERBOSITY = 0;
  INPUTHEPMC::filename = inputFile;


  Enable::QA = true;

  //-----------------
  // Initialize the selected Input/Event generation
  //-----------------
  InputInit();
  //--------------
  // Set generator specific options
  //--------------
  // can only be set after InputInit() is called

  // Simple Input generator:
  // if you run more than one of these Input::SIMPLE_NUMBER > 1
  // add the settings for other with [1], next with [2]...
  if (Input::SIMPLE){
    if (generatorSettings.Contains("SimplePion"))
      INPUTGENERATOR::SimpleEventGenerator[0]->add_particles("pi-", 1);
    else if (generatorSettings.Contains("SimpleKaon"))
      INPUTGENERATOR::SimpleEventGenerator[0]->add_particles("kaon-", 1);
    else if (generatorSettings.Contains("SimpleProton"))
      INPUTGENERATOR::SimpleEventGenerator[0]->add_particles("proton", 1);
    else if (generatorSettings.Contains("SimplePhoton"))
      INPUTGENERATOR::SimpleEventGenerator[0]->add_particles("gamma", 1);
    else if (generatorSettings.Contains("SimpleElectron"))
      INPUTGENERATOR::SimpleEventGenerator[0]->add_particles("e-", 1);
    else if (generatorSettings.Contains("SimplePiZero"))
      INPUTGENERATOR::SimpleEventGenerator[0]->add_particles("pi0", 1);
    else {
      std::cout << "You didn't specify which particle you wanted to generate, exiting" << std::endl;
      return 0;
    }
    std::cout << "running " << generatorSettings.Data() << " generation" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " generation" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " generation" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " generation" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " generation" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " generation" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " generation" << std::endl;
    INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
                                                                              PHG4SimpleEventGenerator::Uniform,
                                                                              PHG4SimpleEventGenerator::Uniform);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_mean(0., 0., 0.);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_width(0., 0., 5.);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_eta_range(2.0, 3.0);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_phi_range(-M_PI, M_PI);
    // INPUTGENERATOR::SimpleEventGenerator[0]->set_pt_range(particlemomMin, particlemomMax);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_p_range(particlemomMin, particlemomMax);
 
  }
  if(particlemomMin>-1 && particlemomMax == -1){
    std::cout << "running " << generatorSettings.Data() << " PHG4ParticleGenerator" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " PHG4ParticleGenerator" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " PHG4ParticleGenerator" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " PHG4ParticleGenerator" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " PHG4ParticleGenerator" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " PHG4ParticleGenerator" << std::endl;
    std::cout << "running " << generatorSettings.Data() << " PHG4ParticleGenerator" << std::endl;
    PHG4ParticleGenerator *gen = new PHG4ParticleGenerator("PGENERATOR");
    gen->set_name("pi-");
    gen->set_vtx(0, 0, 0);
    gen->set_eta_range(1.5, 3.4);            // around midrapidity
    if(particlemomMin > -1)
      gen->set_mom_range(particlemomMin, particlemomMin);                   // fixed 4 GeV/c
    else
      gen->set_mom_range(5, 80);                   // fixed 4 GeV/c
    gen->set_phi_range(0., 2* M_PI);  // 0-90 deg
    // gen->Verbosity(1);  // 0-90 deg
    se->registerSubsystem(gen);
  }
  // pythia6
  if (Input::PYTHIA6){
    if (generatorSettings.CompareTo("e10p250MB") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_ep.cfg");
    else if (generatorSettings.CompareTo("e10p250pTHard5") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_ep_MinPartonP5GeV.cfg");
    else if (generatorSettings.CompareTo("e10p250pTHard10") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_ep_MinPartonP10GeV.cfg");
    else if (generatorSettings.CompareTo("e10p250pTHard20") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_ep_MinPartonP20GeV.cfg");
    else if (generatorSettings.CompareTo("e5p100MB") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e5p100.cfg");
    else if (generatorSettings.CompareTo("e5p100pTHard5") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e5p100_MinPartonP5GeV.cfg");
    else if (generatorSettings.CompareTo("e10p275MB") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e10p275.cfg");
    else if (generatorSettings.CompareTo("e10p275pTHard5") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e10p275_MinPartonP5GeV.cfg");
    else if (generatorSettings.CompareTo("e10p275pTHard10") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e10p275_MinPartonP10GeV.cfg");
    else if (generatorSettings.CompareTo("e18p275MB") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e18p275.cfg");
    else if (generatorSettings.CompareTo("e18p275pTHard5") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e18p275_MinPartonP5GeV.cfg");
    else if (generatorSettings.CompareTo("e18p275pTHard10") == 0)
      INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_e18p275_MinPartonP10GeV.cfg");
    else 
      INPUTGENERATOR::Pythia6->set_config_file(generatorSettings.Data());
    
    if (specialSetting.Contains("FPartTrigg")){
      PHPy6ParticleTrigger *ptrig = new PHPy6ParticleTrigger();
      ptrig->SetPtLow(1);
      ptrig->SetEtaHighLow(1,5);
      INPUTGENERATOR::Pythia6->register_trigger(ptrig);
    }
    if (specialSetting.Contains("FJetTrigg")){
      PHPy6JetTrigger *trig = new PHPy6JetTrigger();
      trig->SetEtaHighLow(1,5);
      if (generatorSettings.Contains("pTHard5"))
        trig->SetMinJetPt(5);
      else if (generatorSettings.Contains("pTHard10"))
        trig->SetMinJetPt(10);
      else if (generatorSettings.Contains("pTHard20"))
        trig->SetMinJetPt(20);
      else 
        trig->SetMinJetPt(1);
      trig->SetJetR(0.7);
      INPUTGENERATOR::Pythia6->register_trigger(trig);
    }
  }

  // register all input generators with Fun4All
  InputRegister();


  //======================
  // Write the DST
  //======================

//  Enable::DSTOUT = true;
  DstOut::OutputDir = outdir;
  DstOut::OutputFile = outputFile;
  Enable::DSTOUT_COMPRESS = false;  // Compress DST files

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  Enable::DSTREADER = false;

  // turn the display on (default off)
  if(specialSetting.Contains("display"))
    Enable::DISPLAY = true;

  //======================
  // What to run
  //======================
  // Global options (enabled for all subsystems - if implemented)
  //  Enable::ABSORBER = true;
  //  Enable::OVERLAPCHECK = true;
  //  Enable::VERBOSITY = 1;

  //  Enable::BBC = true;
  Enable::BBCFAKE = true; // Smeared vtx and t0, use if you don't want real BBC in simulation

  // whether to simulate the Be section of the beam pipe
  if(!specialSetting("DRCALOONLY"))Enable::PIPE = true;
  // EIC beam pipe extension beyond the Be-section:
  if(!specialSetting("DRCALOONLY"))G4PIPE::use_forward_pipes = true;

  if (specialSetting.Contains("EGEM"))
    Enable::EGEM = true;
  if (specialSetting.Contains("EGEMOO")) // only last 2 EGEM layers
    Enable::EGEM_FULL = false;
  //Forward GEM
  if (specialSetting.Contains("FGEM"))
    Enable::FGEM = true;
  
  // barrel tracker (LANL)
  if (specialSetting.Contains("BARREL"))
    Enable::BARREL = true;
  if(specialSetting.Contains("FST"))
    Enable::FST = true;
  
  // all silicon tracker version (LBL)
  if(specialSetting.Contains("ALLSILICON")){
    Enable::ALLSILICON = true;
    Enable::ALLSILICON_ABSORBER = true;
  }
  
  // LGAD layers
  if(specialSetting.Contains("FTTL"))
    Enable::FTTL = true;
  if(specialSetting.Contains("ETTL"))
    Enable::ETTL = true;
  if(specialSetting.Contains("CTTL"))
    Enable::CTTL = true;

  // mvtx/tpc tracker
  if(specialSetting.Contains("MVTX")){
    Enable::MVTX = true;
    Enable::TPC = true;
  }
  
  if(specialSetting.Contains("ENDCAPTPC"))
   Enable::TPC_ENDCAP = true;

  Enable::TRACKING = true;
  Enable::TRACKING_EVAL = Enable::TRACKING && true;
  if (specialSetting.Contains("TREXTOUT"))
    Enable::TRACKING_EVAL_DETAILED = Enable::TRACKING_EVAL && true;
  
  G4TRACKING::DISPLACED_VERTEX = true;  // this option exclude vertex in the track fitting and use RAVE to reconstruct primary and 2ndary vertexes
                                         // projections to calorimeters
  if(!specialSetting("DRCALOONLY"))G4TRACKING::PROJECTION_CEMC = true;
  G4TRACKING::PROJECTION_DRCALO = true;

 if(!specialSetting("DRCALOONLY")) Enable::CEMC = true;
  //  Enable::CEMC_ABSORBER = true;
  Enable::CEMC_CELL = Enable::CEMC && true;
  Enable::CEMC_TOWER = Enable::CEMC_CELL && true;
  Enable::CEMC_CLUSTER = Enable::CEMC_TOWER && true;
  Enable::CEMC_EVAL = Enable::CEMC_CLUSTER && true;

  if(!specialSetting("DRCALOONLY"))Enable::HCALIN = true;
  //  Enable::HCALIN_ABSORBER = true;
  Enable::HCALIN_CELL = Enable::HCALIN && true;
  Enable::HCALIN_TOWER = Enable::HCALIN_CELL && true;
  Enable::HCALIN_CLUSTER = Enable::HCALIN_TOWER && true;
  Enable::HCALIN_EVAL = Enable::HCALIN_CLUSTER && true;

  // Enable::MAGNET = true;

  if(!specialSetting("DRCALOONLY"))Enable::HCALOUT = true;
  //  Enable::HCALOUT_ABSORBER = true;
  Enable::HCALOUT_CELL = Enable::HCALOUT && true;
  Enable::HCALOUT_TOWER = Enable::HCALOUT_CELL && true;
  Enable::HCALOUT_CLUSTER = Enable::HCALOUT_TOWER && true;
  Enable::HCALOUT_EVAL = Enable::HCALOUT_CLUSTER && true;

  // EICDetector geometry - barrel
  if(!specialSetting("DRCALOONLY"))Enable::DIRC = true;

  // EICDetector geometry - 'hadron' direction
  if(!specialSetting("DRCALOONLY"))Enable::RICH = true;
  if(!specialSetting("DRCALOONLY"))Enable::AEROGEL = true;


  Enable::DRCALO = true;
  Enable::DRCALO_VERBOSITY = 1;
  //  Enable::DRCALO_ABSORBER = true;
  Enable::DRCALO_CELL = Enable::DRCALO && true;
  Enable::DRCALO_TOWER = Enable::DRCALO_CELL && true;
  Enable::DRCALO_CLUSTER = Enable::DRCALO_TOWER && true;
  Enable::DRCALO_EVAL = Enable::DRCALO_CLUSTER && true;

  // EICDetector geometry - 'electron' direction
  if(!specialSetting("DRCALOONLY"))Enable::EEMC = true;
  Enable::EEMC_CELL = Enable::EEMC && true;
  Enable::EEMC_TOWER = Enable::EEMC_CELL && true;
  Enable::EEMC_CLUSTER = Enable::EEMC_TOWER && true;
  Enable::EEMC_EVAL = Enable::EEMC_CLUSTER && true;

  if(!specialSetting("DRCALOONLY"))Enable::PLUGDOOR = false;

  // Other options
  Enable::GLOBAL_RECO = true;
  Enable::GLOBAL_FASTSIM = true;

  Enable::CALOTRIGGER = true && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER;

  // Select only one jet reconstruction- they currently use the same
  // output collections on the node tree!
  Enable::JETS = false;
  Enable::JETS_EVAL = Enable::JETS && true;

  Enable::FWDJETS = true;
  Enable::FWDJETS_EVAL = Enable::FWDJETS && true;

  // HI Jet Reco for jet simulations in Au+Au (default is false for
  // single particle / p+p simulations, or for Au+Au simulations which
  // don't care about jets)
  Enable::HIJETS = false && Enable::JETS && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER;

  // new settings using Enable namespace in GlobalVariables.C
  Enable::BLACKHOLE = true;
  //Enable::BLACKHOLE_SAVEHITS = false; // turn off saving of bh hits
  //BlackHoleGeometry::visible = true;

  //Enable::USER = true;

  //---------------
  // World Settings
  //---------------
  //  G4WORLD::PhysicsList = "QGSP_BERT"; //FTFP_BERT_HP best for calo
  //  G4WORLD::WorldMaterial = "G4_AIR"; // set to G4_GALACTIC for material scans

  //---------------
  // Magnet Settings
  //---------------
  if(specialSetting.Contains("3T")){
    const string magfield = "3.0"; // alternatively to specify a constant magnetic field, give a float number, which will be translated to solenoidal field in T, if string use as fieldmap name (including path)
    G4MAGNET::magfield = magfield;
    //  G4MAGNET::magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root");  // default map from the calibration database
  } else {
    G4MAGNET::magfield_rescale = -1.4 / 1.5;  // make consistent with expected Babar field strength of 1.4T
  }
  //---------------
  // Pythia Decayer
  //---------------
  // list of decay types in
  // $OFFLINE_MAIN/include/g4decayer/EDecayType.hh
  // default is All:
  // G4P6DECAYER::decayType = EDecayType::kAll;

  // Initialize the selected subsystems
  G4Init();

  //---------------------
  // GEANT4 Detector description
  //---------------------

  // If "readhepMC" is also set, the Upsilons will be embedded in Hijing events, if 'particles" is set, the Upsilons will be embedded in whatever particles are thrown
  if (!Input::READHITS) G4Setup(specialSetting);

  //------------------
  // Detector Division
  //------------------
  if (Enable::BBC || Enable::BBCFAKE) Bbc_Reco();
  if (Enable::CEMC_CELL) CEMC_Cells();
  if (Enable::HCALIN_CELL) HCALInner_Cells();
  if (Enable::HCALOUT_CELL) HCALOuter_Cells();
  if (Enable::EEMC_CELL) EEMC_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------
  if (Enable::CEMC_TOWER) CEMC_Towers();
  if (Enable::CEMC_CLUSTER) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------
  if (Enable::HCALIN_TOWER) HCALInner_Towers();
  if (Enable::HCALIN_CLUSTER) HCALInner_Clusters();

  if (Enable::HCALOUT_TOWER) HCALOuter_Towers();
  if (Enable::HCALOUT_CLUSTER) HCALOuter_Clusters();

  //-----------------------------
  // e, h direction Calorimeter  towering and clustering
  //-----------------------------

  if (Enable::DRCALO_TOWER) DRCALO_Towers();
  if (Enable::DRCALO_CLUSTER) DRCALO_Clusters();

  if (Enable::EEMC_TOWER) EEMC_Towers();
  if (Enable::EEMC_CLUSTER) EEMC_Clusters();

  if (Enable::DSTOUT_COMPRESS) ShowerCompress();

  //--------------
  // SVTX tracking
  //--------------
  if (Enable::TRACKING) Tracking_Reco(specialSetting);

  //-----------------
  // Global Vertexing
  //-----------------
  if (Enable::GLOBAL_RECO) Global_Reco();
  else if (Enable::GLOBAL_FASTSIM) Global_FastSim();

  //-----------------
  // Calo Trigger Simulation
  //-----------------
  if (Enable::CALOTRIGGER) CaloTrigger_Sim();

  //---------
  // Jet reco
  //---------
  if (Enable::JETS) Jet_Reco();
  if (Enable::HIJETS) HIJetReco();
  if (Enable::FWDJETS) Jet_FwdReco();

  string outputroot = outputFile;
  string remove_this = ".root";
  size_t pos = outputroot.find(remove_this);
  if (pos != string::npos){
    outputroot.erase(pos, remove_this.length());
  }

  if (Enable::DSTREADER) G4DSTreader_EICDetector(outputroot + "_DSTReader.root");

  //----------------------
  // Simulation evaluation
  //----------------------
  Bool_t doFullEventTree = true;
  if(doFullEventTree){
    EventEvaluator *eval = new EventEvaluator("EVENTEVALUATOR", "eventtree.root");
    eval->set_reco_tracing_energy_threshold(0.02);
    eval->Verbosity(1);
    eval->set_do_FHCAL(false);
    eval->set_do_FEMC(false);
    eval->set_do_DRCALO(true);
    // eval->set_do_HITS(true);
    eval->set_do_TRACKS(true);
    eval->set_do_CLUSTERS(true);
    eval->set_do_VERTEX(true);
    eval->set_do_PROJECTIONS(true);
    eval->set_do_MCPARTICLES(true);
    se->registerSubsystem(eval);
  }

  //--------------
  // Set up Input Managers
  //--------------

  InputManagers();

  //--------------
  // Set up Output Manager
  //--------------
  if (Enable::PRODUCTION){
    Production_CreateOutputDir();
  }

  if (Enable::DSTOUT){
    string FullOutFile = DstOut::OutputDir + "/" + DstOut::OutputFile;
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", FullOutFile);
    if (Enable::DSTOUT_COMPRESS) DstCompress(out);
    se->registerOutputManager(out);
  }

  //-----------------
  // Event processing
  //-----------------
  if (Enable::DISPLAY){
    DisplayOn();
    gROOT->ProcessLine("Fun4AllServer *se = Fun4AllServer::instance();");
    // gROOT->ProcessLine("PHG4Reco *g4 = QTGui();"); // alternative to DisplayOn
    gROOT->ProcessLine("PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco(\"PHG4RECO\");");

    cout << "-------------------------------------------------" << endl;
    cout << "You are in event display mode. Run one event with" << endl;
    cout << "se->run(1)" << endl;
    cout << "Run Geant4 command with following examples" << endl;
    // gROOT->ProcessLine("displaycmd()");

    return 0;
  }
  // if we use a negative number of events we go back to the command line here
  if (nEvents < 0){
    return 0;
  }
  // if we run any of the particle generators and use 0 it'll run forever
  if (nEvents == 0 && !Input::READHITS && !Input::HEPMC && !Input::READEIC){
    cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
    cout << "it will run forever, so I just return without running anything" << endl;
    return 0;
  }

  se->skip(skip);
  se->run(nEvents);

  if (Enable::QA) QA_Output(outputroot + "_qa.root");

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  if (Enable::PRODUCTION){
    Production_MoveOutput();
  }
  gSystem->Exit(0);
  return 0;
}

void ParseTString(TString &specialSetting)
{
// Barrel settings
  if (specialSetting.Contains("BARRELV1"))
  {
    G4BARREL::SETTING::BARRELV1 = true;
  }
  else if (specialSetting.Contains("BARRELV2"))
  {
    G4BARREL::SETTING::BARRELV2 = true;
  }
  else if (specialSetting.Contains("BARRELV3"))
  {
    G4BARREL::SETTING::BARRELV3 = true;
  }
  else if (specialSetting.Contains("BARRELV4"))
  {
    G4BARREL::SETTING::BARRELV4 = true;
  }
  else if (specialSetting.Contains("BARREL"))
  {
    G4BARREL::SETTING::BARRELV0 = true;
  }

// FST settings
  if (specialSetting.Contains("FSTV1"))
  {
    G4FST::SETTING::FSTV1 = true;
  }
  else if (specialSetting.Contains("FSTV2"))
  {
    G4FST::SETTING::FSTV2 = true;
  }
  else if (specialSetting.Contains("FSTV3"))
  {
    G4FST::SETTING::FSTV3 = true;
  }
  else if (specialSetting.Contains("FSTV4"))
  {
    G4FST::SETTING::FSTV4 = true;
  }
  else if (specialSetting.Contains("FSTV41"))
  {
    G4FST::SETTING::FSTV41 = true;
  }
  else if (specialSetting.Contains("FSTV42"))
  {
    G4FST::SETTING::FSTV42 = true;
  }
  else if (specialSetting.Contains("FST"))
  {
    G4FST::SETTING::FSTV0 = true;
  }

// FHCAL/FEMC settings
  if (specialSetting.Contains("DRLARGE"))
  {
    G4DRCALO::SETTING::LargeTowers = true;
  }
}

#endif
