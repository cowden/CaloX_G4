#pragma once

#include "G4UImessenger.hh"
#include "RunManager.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

namespace B4c {
class RunManagerMessenger: public G4UImessenger {
    public:
    RunManagerMessenger(TheRunManager *);
    ~RunManagerMessenger();

    void SetNewValue(G4UIcommand * command, G4String newValues);
    G4String GetCurrentValue(G4UIcommand * command);

    private:
    TheRunManager * rm_;

    G4UIdirectory * dir_;
    G4UIcmdWithAnInteger * seedCmd_;

};
}
