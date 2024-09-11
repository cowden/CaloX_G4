
#include "RunManagerMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAnInteger.hh"

B4c::RunManagerMessenger::RunManagerMessenger(B4c::TheRunManager * rm)
:G4UImessenger(), rm_(rm)
{
    dir_ = new G4UIdirectory("/trm/");
    dir_->SetGuidance("Control TheRunManager");

    seedCmd_ = new G4UIcmdWithAnInteger("/trm/seed", this);
    seedCmd_->SetGuidance("Set the starting seed for the RNG.");
}

B4c::RunManagerMessenger::~RunManagerMessenger() {
    delete seedCmd_;
    delete dir_;
}

void B4c::RunManagerMessenger::SetNewValue(G4UIcommand * cmd, G4String sval)
{
    if ( cmd == seedCmd_ )
        rm_->SetSeed( atoi(sval) ); 
}

G4String B4c::RunManagerMessenger::GetCurrentValue(G4UIcommand *cmd)
{
    G4String cv;
    if ( cmd == seedCmd_ )
        cv = seedCmd_->ConvertToString(rm_->GetSeed());

    return cv;
}
