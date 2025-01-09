#ifndef CXACTIONMESSENGER_HH
#define CXACTIONMESSENGER_HH

#include "G4UImessenger.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class CXActionInitialization;

namespace CX {

class  ActionMessenger : public G4UImessenger {
    public:
    ActionMessenger(CXActionInitialization *);
    virtual ~ActionMessenger();

    void SetNewValue(G4UIcommand *, G4String );
    G4String GetCurrentValue(G4UIcommand *);

    private:
    CXActionInitialization * ai_;

    G4UIdirectory * dir_;
    G4UIcmdWithAString * nameCmd_;

};

}

#endif
