
#include "RunManager.hh"

#include "RunManagerMessenger.hh"

B4c::TheRunManager::TheRunManager()
:G4MTRunManager()	
{
    msngr_ = new RunManagerMessenger(this);
}

B4c::TheRunManager::~TheRunManager()
{
    delete msngr_;
}

G4bool B4c::TheRunManager::InitializeSeeds(G4int nevnts) {
    auto helper = G4RNGHelper::GetInstance();
    helper->Clear();

    for (unsigned i=0; i != 2*nevnts; i++ )
        helper->AddOneSeed(seed_+i*2);

    return true;
}
