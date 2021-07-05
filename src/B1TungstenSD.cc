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
// $Id$
//
/// \file B1TungstenSD.cc
/// \brief Implementation of the B1TungstenSD class
//

#include "B1TungstenSD.hh"
#include "B1Analysis.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1TungstenSD::B1TungstenSD(const G4String& name,
    const G4String& hitsCollectionName,
    G4int ntupleId)
    : G4VSensitiveDetector(name),
    fHitsCollection(0),
    fNtupleId(ntupleId)
{
    collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1TungstenSD::~B1TungstenSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1TungstenSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection
        = new B1TungstenHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce
    G4int hcID
        = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B1TungstenSD::ProcessHits(G4Step* step,
    G4TouchableHistory* /*history*/)
{
    // Change the following line to get the charge of the tracked particle
    G4double charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
    if (charge == 0.) return false;

    // Create new hit
    B1TungstenHit* newHit = new B1TungstenHit();

    // Layer number
    // = copy number of mother volume
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4VPhysicalVolume* motherPhysical = touchable->GetVolume(1); // mother
    G4int copyNo = motherPhysical->GetCopyNo();
    newHit->SetLayerNumber(copyNo);

    // Time
    G4double time = preStepPoint->GetGlobalTime();
    newHit->SetTime(time);

    // Position
    G4ThreeVector position = preStepPoint->GetPosition();
    newHit->SetPosition(position);

    // Energy Deposit
    G4double edep = step->GetTotalEnergyDeposit();
    newHit->AddEdep(edep);

    // Add hit in the collection
    fHitsCollection->insert(newHit);

    // Add hits properties in the ntuple
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, 0, copyNo);
    analysisManager->FillNtupleDColumn(0, 1, position.x());
    analysisManager->FillNtupleDColumn(0, 2, position.y());
    analysisManager->FillNtupleDColumn(0, 3, position.z());
    analysisManager->FillNtupleDColumn(0, 4, edep);
    analysisManager->AddNtupleRow(0);

    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1TungstenSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
    G4cout << "\n-------->" << fHitsCollection->GetName()
        << ": in this event: " << G4endl;

    G4int nofHits = fHitsCollection->entries();
    for (G4int i = 0; i < nofHits; i++) (*fHitsCollection)[i]->Print();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......