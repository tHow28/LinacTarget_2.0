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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Analysis.hh"
#include "G4UnitsTable.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  fScoringVolume2(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  //if (!fScoringVolume) { 
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
    fScoringVolume2 = detectorConstruction->GetScoringVolume2();
  //}

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "eBrem") {
      G4double ePre = step->GetPreStepPoint()->GetKineticEnergy();
      G4double eDepStep = step->GetTotalEnergyDeposit();
      G4double ePost = step->GetPostStepPoint()->GetKineticEnergy();
      G4double photonEnergy = ePre - ePost - eDepStep;
      fEventAction->AddEgamma(photonEnergy);
      G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
      analysisManager->FillNtupleDColumn(2, 0, photonEnergy);
      analysisManager->AddNtupleRow(2);
 }


  //if (step->GetTrack()->GetDefinition()->GetParticleName() == "gamma") {
  //    G4String procName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  //    G4double photonEnergy = step->GetTrack()->GetTotalEnergy();
  //    // Add hits properties in the ntuple
  //    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //    analysisManager->FillNtupleDColumn(2, 0, photonEnergy);
  //    analysisManager->AddNtupleRow(2);
  //    G4cout << "Photon process in this step: " << procName << G4endl;
  //}

      
  // check if we are in tungsten 
  if (volume == fScoringVolume) {

      // collect energy deposited in this step
      G4double edepStep = step->GetTotalEnergyDeposit();
      fEventAction->AddEdep(edepStep);
      //G4cout << "Edep in Tungsten: " << G4BestUnit(edepStep, "Energy") << G4endl;
  }
  // check if we are in copper 
  if (volume == fScoringVolume2) {

      // collect energy deposited in this step
      G4double edepStep2 = step->GetTotalEnergyDeposit();
      fEventAction->AddEdep2(edepStep2);
      //G4cout << "Edep in Copper: " << G4BestUnit(edepStep2, "Energy") << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

