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
/// \file CXEventAction.cc
/// \brief Implementation of the CXEventAction class

#include "CXEventAction.hh"
#include "CXRunData.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"

#include "G4Step.hh"

#include "CXPrimaryGeneratorAction.hh"

#include "Randomize.hh"
#include <iomanip>

// -- for root ntuple --
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TDirectory.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TText.h"
#include "TPaveText.h"
#include "TROOT.h"

// -- for CaloX data --
#include "CaloDataStruc.h"
#include "CXHDF5.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CXEventAction::CXEventAction(CXDetectorConstruction* det,CXPrimaryGeneratorAction* prim, CX::CXHDF5 * output)
 : G4UserEventAction(),fDetector(det),primary(prim),data_out_(output)
{  
   raw_data_.resize(64*64, 0);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CXEventAction::~CXEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CXEventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
  //clearTTreeVectors();
  getCellSize();

  //G4cout << "Resizing raw data " << data_out_->size() << G4endl;
  raw_data_.resize(data_out_->size());

  const unsigned size = raw_data_.size();
  for ( unsigned i=0; i != size; i++ )
      raw_data_[i] = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CXEventAction::EndOfEventAction(const G4Event* event)
{   

    const unsigned eid = event->GetEventID();
    data_out_->write_event(eid, &raw_data_[0]);

    float primE = primary->GetParticleGun()->GetParticleEnergy();
    data_out_->write_primary(eid, &primE);

} //  end of CXEventAction::EndOfEventAction  


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CXEventAction::AccumulateCaloHits(CaloStepData aHit){
       //std::cout<<"CXEventAction::AccumulateCaloHits  track "<<aHit.trackid<<"  pid "<<aHit.pid<<"  edep "<<aHit.edep << " time " << TimeTOFAdjusted(aHit.globaltime, aHit.z)  <<std::endl;
   // if(aHit.edep>0.000001) { 
   if(aHit.edep>0.0) { 
      double tA=TimeTOFAdjusted(aHit.globaltime,aHit.z);
      if(tA>0. && tA<100000) {
         int kx=(aHit.x+caloXhalf)/caloDx;
         int ky=(aHit.y+caloYhalf)/caloDy;
         int kz=(aHit.z+caloZhalf)/caloDz;
         int index=kx+ky*50+kz*(50*50);
            caloHits[index]=caloHits[index]+aHit.edep;
       //std::cout<<"CXEventAction::AccumulateCaloHits  track "<<aHit.trackid<<"  pid "<<aHit.pid<<"  edep "<<aHit.edep << " time " << TimeTOFAdjusted(aHit.globaltime, aHit.z)  <<std::endl;
            if( tA <=5.) raw_data_[index] += aHit.edep;
         //
         if(_CaloXG4map2PidEdepON>0) {
            tkPidEdep[aHit.pid]=tkPidEdep[aHit.pid]+aHit.edep;
         }
      } // end of if(tA>d_CaloXG4mapTcutMin && tA<d_CaloXG4mapTcutMax)
   }  // end of if(aHit.edep>0.0)
}

// -----------------------------------------------------------------------
void CXEventAction::FillSecondaries(const G4Step* step){

}  // end of CXEventAction::FillSecondaries.

// -----------------------------------------------------------------------
double CXEventAction::TimeTOFAdjusted(double t, double  z){
    // return the time with subtraction of TOF along Z. 
    return t-(z+worldZhalf)/300.0;  // 300 mm/1 ns
}

// -----------------------------------------------------------------------
void CXEventAction::StepAnalysis(const G4Step* step){

}

// -----------------------------------------------------------------------
void CXEventAction::clearTTreeVectors(){
}

// -----------------------------------------------------------------------
void CXEventAction::getCellSize(){

  auto runData
    = static_cast<CXRunData*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->Reset(); 

  //   World volume...
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox=dynamic_cast<G4Box*>(worldLV->GetSolid());
  worldZhalf=worldBox->GetZHalfLength(); 

  // Clorimeter volume...
  auto calorLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Calorimeter");
  G4Box* calorBox=dynamic_cast<G4Box*>(calorLV->GetSolid());
  caloXhalf=calorBox->GetXHalfLength();
  caloYhalf=calorBox->GetYHalfLength();
  caloZhalf=calorBox->GetZHalfLength();

  // Layer volume...
  auto layerLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Layer");
  G4Box* layerBox=dynamic_cast<G4Box*>(layerLV->GetSolid());
  caloDz=layerBox->GetZHalfLength()*2.0;  // fult layer thickness, i.e. cell size
//  caloDx=caloDz/4.; //  a cell is cube. 
//  caloDy=caloDz/4.;  
  caloDx=caloDz; //  a cell is cube. 
  caloDy=caloDz;  

  caloNx=((caloXhalf*2.0)/caloDx)+0.01;  // 0.01 to avoid round-off issue
  caloNy=((caloYhalf*2.0)/caloDy)+0.01;  // 0.01 to avoid round-off issue
  caloNz=((caloZhalf*2.0)/caloDz)+0.01;  // 0.01 to avoid round-off issue

  /* 
  std::cout<<"worldZhalf= "<<worldZhalf<<std::endl;
  std::cout<<"caloXhalf = "<<caloXhalf<<"  caloDx "<<caloDx<<"  CaloNx "<<caloNx<<std::endl; 
  std::cout<<"caloYhalf = "<<caloYhalf<<"  caloDy "<<caloDy<<"  CaloNy "<<caloNy<<std::endl; 
  std::cout<<"caloZhalf = "<<caloZhalf<<"  caloDz "<<caloDz<<"  CaloNz "<<caloNz<<std::endl; 
  */

  // save those in TTree...
  
  mNxCell=caloNx;
  mNyCell=caloNy;
  mNzCell=caloNz;
  mDxCell=caloDx;
  mDyCell=caloDy;
  mDzCell=caloDz;
  mzhalfWorld=worldZhalf;
}
