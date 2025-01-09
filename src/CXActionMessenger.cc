#include "CXActionMessenger.hh"
#include "CXActionInitialization.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

CX::ActionMessenger::ActionMessenger(CXActionInitialization * ini)
:G4UImessenger(),
ai_(ini)
{
    // dir_
    dir_ = new G4UIdirectory("/uui/");
    dir_->SetGuidance("Control user action initialization classes.");

    // nameCmd_
    nameCmd_ = new G4UIcmdWithAString("/uui/name", this);
    nameCmd_->SetGuidance("Set the output data base name.");

}

CX::ActionMessenger::~ActionMessenger()
{
    delete nameCmd_;
    delete dir_;
}

void CX::ActionMessenger::SetNewValue(G4UIcommand *cmd, G4String value)
{
    if ( cmd == nameCmd_ )
        ai_->SetBaseName( value );
}

G4String CX::ActionMessenger::GetCurrentValue(G4UIcommand *cmd)
{
    G4String cv;
    if ( cmd == nameCmd_ )
        cv = ai_->GetBaseName();

    return cv;
}
