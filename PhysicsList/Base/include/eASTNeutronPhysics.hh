////////////////////////////////////////////////////////////////////////////////
// 
//  eASTNeutronPhysics.hh  
//  Neutron hadronic physics constructor for eASTPhysicsList  
//                                                       
//    Jun.21.2018 : original implementation - Dennis H. Wright (SLAC)
//    May.06.2021 : migration to eAST - Makoto Asai (SLAC)
//                                                      
////////////////////////////////////////////////////////////////////////////////

#ifndef eASTNeutronPhysics_h
#define eASTNeutronPhysics_h 1

#include "G4VPhysicsConstructor.hh"

class G4TheoFSGenerator;
class G4FTFModel;
class G4ExcitedStringDecay;
class G4LundStringFragmentation;
class G4GeneratorPrecompoundInterface;


class eASTNeutronPhysics: public G4VPhysicsConstructor
{
  public:
    eASTNeutronPhysics();
    ~eASTNeutronPhysics();

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;

  private: 
    G4TheoFSGenerator* ftfp;
    G4FTFModel* stringModel;
    G4ExcitedStringDecay* stringDecay;
    G4LundStringFragmentation* fragModel;
    G4GeneratorPrecompoundInterface* preCompoundModel;
 
};

#endif
