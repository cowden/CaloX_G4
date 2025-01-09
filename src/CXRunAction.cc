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
/// \file CXRunAction.cc
/// \brief Implementation of the CXRunAction class

#include <sstream>

#include "CXRunAction.hh"
#include "CXRunData.hh"
#include "CXAnalysis.hh"
#include "RunManager.hh"
#include "CXHDF5.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CXRunAction::CXRunAction(CX::CXHDF5 * output)
 : G4UserRunAction(), data_out_(output)
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CXRunAction::~CXRunAction()
{
  //delete G4AnalysisManager::Instance();  
  delete data_out_;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* CXRunAction::GenerateRun()
{
  return (new CXRunData);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CXRunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout << "### Run " << run->GetRunID() << " start on "
    << " thread "
    << G4Threading::G4GetThreadId() << G4endl;

  if ( G4Threading::G4GetThreadId() >= 0 ) {

      // get geometry store
      auto calorLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Calorimeter");
      G4Box *calorBox = dynamic_cast<G4Box*>(calorLV->GetSolid());

      double caloXhalf = calorBox->GetXHalfLength();
      double caloYhalf = calorBox->GetYHalfLength();
      double caloZhalf = calorBox->GetZHalfLength();

      auto layerLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Layer");
      G4Box *layerBox = dynamic_cast<G4Box*>(layerLV->GetSolid());
      double caloDz = layerBox->GetZHalfLength()*2.;
      double caloDx = caloDz;
      double caloDy = caloDz;

      unsigned Nx = ((caloXhalf*2.)/caloDx) + 0.01;
      unsigned Ny = ((caloYhalf*2.)/caloDy) + 0.01;
      unsigned Nz = ((caloZhalf*2.)/caloDz) + 0.01;

      G4cout << "Initializing HDF5 data with " <<
        "(" << Nx << ", " << Ny << ", " << Nz << ")" << G4endl;

      std::vector<int> dims(3);
      dims[0] = Nx;
      dims[1] = Ny;
      dims[2] = Nz;
      data_out_->initialize(dims);

      int thrd = G4Threading::G4GetThreadId();
      std::stringstream ofilename;
      ofilename << data_out_->get_base_name() 
        << "_t_"
        << G4Threading::G4GetThreadId()
        << ".h5";
      data_out_->open_file(ofilename.str());
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CXRunAction::EndOfRunAction(const G4Run* /*aRun*/)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
