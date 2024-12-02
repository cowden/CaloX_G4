
#include "RunManager.hh"

#include "RunManagerMessenger.hh"

CX::TheRunManager::TheRunManager()
:G4MTRunManager()	
{
    msngr_ = new RunManagerMessenger(this);
}

CX::TheRunManager::~TheRunManager()
{
    delete msngr_;
}

G4bool CX::TheRunManager::InitializeSeeds(G4int nevnts) {
    auto helper = G4RNGHelper::GetInstance();
    helper->Clear();

    for (unsigned i=0; i != 2*nevnts; i++ )
        helper->AddOneSeed(seed_+i*2);

    return true;
}
