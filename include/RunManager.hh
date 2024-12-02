#pragma once

#include "G4MTRunManager.hh"
#include "globals.hh"

namespace CX {

class RunManagerMessenger;

class TheRunManager: public G4MTRunManager {
    public:
    TheRunManager();
    ~TheRunManager();

    void SetSeed(G4int sd) { seed_ = sd; }
    G4int GetSeed() { return seed_; }

    protected:
    virtual G4bool InitializeSeeds(G4int nevnts);

    private:
    RunManagerMessenger * msngr_;
    G4int seed_;

};
}
