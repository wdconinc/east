////////////////////////////////////////////////////////////////////////////////
//
//  eASTHyperonPhysics.cc
//  Hyperon hadronic physics constructor for eASTPhysicsList
//
//    Jun.21.2018 : original implementation - Dennis H. Wright (SLAC)
//    May.02.2021 : migration to Geant4 version 10.7 - Dennis H. Wright (SLAC)
//    May.06.2021 : migration to eAST - Makoto Asai (SLAC)
//
////////////////////////////////////////////////////////////////////////////////


#include "eASTHyperonPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4LambdaInelasticProcess.hh"
#include "G4SigmaPlusInelasticProcess.hh"
#include "G4SigmaMinusInelasticProcess.hh"
#include "G4XiZeroInelasticProcess.hh"
#include "G4XiMinusInelasticProcess.hh"
#include "G4OmegaMinusInelasticProcess.hh"

#include "G4HadronElasticProcess.hh"
#include "G4HadronicAbsorptionBertini.hh"

#include "G4CascadeInterface.hh"
#include "G4TheoFSGenerator.hh"
#include "G4FTFModel.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4LundStringFragmentation.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4HadronElastic.hh"

#include "G4ChipsHyperonElasticXS.hh"
#include "G4ChipsHyperonInelasticXS.hh"
#include "G4SystemOfUnits.hh"


eASTHyperonPhysics::eASTHyperonPhysics()
{}


eASTHyperonPhysics::~eASTHyperonPhysics()
{
  delete stringDecay;
  delete stringModel;
  delete fragModel;
  delete preCompoundModel;
}


void eASTHyperonPhysics::ConstructParticle()
{}


void eASTHyperonPhysics::ConstructProcess()
{
  G4ProcessManager* procMan = 0;

  // One elastic model for all hyperon energies
  G4HadronElastic* elModel = new G4HadronElastic();

  // Use Bertini cascade for low energies
  G4CascadeInterface* loInelModel = new G4CascadeInterface;
  loInelModel->SetMinEnergy(0.0);
  loInelModel->SetMaxEnergy(6.0*GeV);

  // Use FTFP for high energies   ==>>   eventually replace this with new class FTFPInterface
  ftfp = new G4TheoFSGenerator("FTFP");
  stringModel = new G4FTFModel;
  stringDecay =
    new G4ExcitedStringDecay(fragModel = new G4LundStringFragmentation);
  stringModel->SetFragmentationModel(stringDecay);
  preCompoundModel = new G4GeneratorPrecompoundInterface();

  ftfp->SetHighEnergyGenerator(stringModel);
  ftfp->SetTransport(preCompoundModel); 
  ftfp->SetMinEnergy(4*GeV);
  ftfp->SetMaxEnergy(100*TeV);

  // Elastic and inelastic cross section sets
  G4ChipsHyperonElasticXS* chipsElastic = new G4ChipsHyperonElasticXS;
  G4ChipsHyperonInelasticXS* chipsInelastic = new G4ChipsHyperonInelasticXS;

  //////////////////////////////////////////////////////////////////////////////
  //   Lambda                                                                 // 
  //////////////////////////////////////////////////////////////////////////////

  procMan = G4Lambda::Lambda()->GetProcessManager();

  // elastic
  G4HadronElasticProcess* lamProcEl = new G4HadronElasticProcess;
  lamProcEl->RegisterMe(elModel);
  lamProcEl->AddDataSet(chipsElastic);
  procMan->AddDiscreteProcess(lamProcEl);

  // inelastic 
  G4LambdaInelasticProcess* lamProcInel = new G4LambdaInelasticProcess;
  lamProcInel->RegisterMe(loInelModel);
  lamProcInel->RegisterMe(ftfp);
  lamProcInel->AddDataSet(chipsInelastic);
  procMan->AddDiscreteProcess(lamProcInel);

  //////////////////////////////////////////////////////////////////////////////
  //   Sigma+                                                                 // 
  //////////////////////////////////////////////////////////////////////////////

  procMan = G4SigmaPlus::SigmaPlus()->GetProcessManager();

  // elastic
  G4HadronElasticProcess* spProcEl = new G4HadronElasticProcess;
  spProcEl->RegisterMe(elModel);
  spProcEl->AddDataSet(chipsElastic);
  procMan->AddDiscreteProcess(spProcEl);

  // inelastic
  G4SigmaPlusInelasticProcess* spProcInel = new G4SigmaPlusInelasticProcess;
  spProcInel->RegisterMe(loInelModel);
  spProcInel->RegisterMe(ftfp);
  spProcInel->AddDataSet(chipsInelastic);
  procMan->AddDiscreteProcess(spProcInel);

  //////////////////////////////////////////////////////////////////////////////
  //   Sigma-                                                                 // 
  //////////////////////////////////////////////////////////////////////////////

  procMan = G4SigmaMinus::SigmaMinus()->GetProcessManager();

  // elastic
  G4HadronElasticProcess* smProcEl = new G4HadronElasticProcess;
  smProcEl->RegisterMe(elModel);
  smProcEl->AddDataSet(chipsElastic);
  procMan->AddDiscreteProcess(smProcEl);

  // inelastic
  G4SigmaMinusInelasticProcess* smProcInel = new G4SigmaMinusInelasticProcess;
  smProcInel->RegisterMe(loInelModel);
  smProcInel->RegisterMe(ftfp);
  smProcInel->AddDataSet(chipsInelastic);
  procMan->AddDiscreteProcess(smProcInel);

  // stopping
  G4HadronicAbsorptionBertini* smAbsorb = new G4HadronicAbsorptionBertini;
  procMan->AddRestProcess(smAbsorb);

  //////////////////////////////////////////////////////////////////////////////
  //   Xi0                                                                    // 
  //////////////////////////////////////////////////////////////////////////////

  procMan = G4XiZero::XiZero()->GetProcessManager();

  // elastic
  G4HadronElasticProcess* xzProcEl = new G4HadronElasticProcess;
  xzProcEl->RegisterMe(elModel);
  xzProcEl->AddDataSet(chipsElastic);
  procMan->AddDiscreteProcess(xzProcEl);

  // inelastic
  G4XiZeroInelasticProcess* xzProcInel = new G4XiZeroInelasticProcess;
  xzProcInel->RegisterMe(loInelModel);
  xzProcInel->RegisterMe(ftfp);
  xzProcInel->AddDataSet(chipsInelastic);
  procMan->AddDiscreteProcess(xzProcInel);

  //////////////////////////////////////////////////////////////////////////////
  //   Xi-                                                                    // 
  //////////////////////////////////////////////////////////////////////////////

  procMan = G4XiMinus::XiMinus()->GetProcessManager();

  // elastic
  G4HadronElasticProcess* xmProcEl = new G4HadronElasticProcess;
  xmProcEl->RegisterMe(elModel);
  xmProcEl->AddDataSet(chipsElastic);
  procMan->AddDiscreteProcess(xmProcEl);

  // inelastic
  G4XiMinusInelasticProcess* xmProcInel = new G4XiMinusInelasticProcess;
  xmProcInel->RegisterMe(loInelModel);
  xmProcInel->RegisterMe(ftfp);
  xmProcInel->AddDataSet(chipsInelastic);
  procMan->AddDiscreteProcess(xmProcInel);

  // stopping
  G4HadronicAbsorptionBertini* xmAbsorb = new G4HadronicAbsorptionBertini;
  procMan->AddRestProcess(xmAbsorb);

  //////////////////////////////////////////////////////////////////////////////
  //   Omega-                                                                 // 
  //////////////////////////////////////////////////////////////////////////////

  procMan = G4OmegaMinus::OmegaMinus()->GetProcessManager();

  // elastic
  G4HadronElasticProcess* omProcEl = new G4HadronElasticProcess;
  omProcEl->RegisterMe(elModel);
  omProcEl->AddDataSet(chipsElastic);
  procMan->AddDiscreteProcess(omProcEl);

  // inelastic
  G4OmegaMinusInelasticProcess* omProcInel = new G4OmegaMinusInelasticProcess;
  omProcInel->RegisterMe(loInelModel);
  omProcInel->RegisterMe(ftfp);
  omProcInel->AddDataSet(chipsInelastic);
  procMan->AddDiscreteProcess(omProcInel);

  // stopping
  G4HadronicAbsorptionBertini* omAbsorb = new G4HadronicAbsorptionBertini;
  procMan->AddRestProcess(omAbsorb);

}

void eASTHyperonPhysics::TerminateWorker()
{}

