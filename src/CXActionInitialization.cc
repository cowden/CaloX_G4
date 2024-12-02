//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file CXActionInitialization.cc
/// \brief Implementation of the CXActionInitialization class

#include "CXActionInitialization.hh"
#include "CXPrimaryGeneratorAction.hh"
#include "CXRunAction.hh"
#include "CXEventAction.hh"
#include "CXTrackingAction.hh"
#include "CXSteppingAction.hh"
#include "G4MTRunManager.hh"
#include "CXDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CXActionInitialization::CXActionInitialization(CXDetectorConstruction* det)
 : G4VUserActionInitialization(),fDetector(det)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CXActionInitialization::~CXActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CXActionInitialization::BuildForMaster() const
{
  SetUserAction(new CXRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CXActionInitialization::Build() const
{
  CXPrimaryGeneratorAction* primary = new CXPrimaryGeneratorAction(fDetector);
  SetUserAction(primary);
  //SetUserAction(new CXPrimaryGeneratorAction);

  SetUserAction(new CXRunAction);
  // SetUserAction(new CXEventAction);
  CXEventAction* eventaction = new CXEventAction(fDetector,primary);
  SetUserAction(eventaction);  

  SetUserAction(new CXTrackingAction);
  SetUserAction(new CXSteppingAction(eventaction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
