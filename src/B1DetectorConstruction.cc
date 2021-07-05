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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
#include "B1TungstenSD.hh"
#include "B1CopperSD.hh"
//#include "B1WaterSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0),
  fScoringVolume2(0),
  fScoringVolume3(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 100*cm, env_sizeZ = 250*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  //     
  // Shape 1 - tungsten disc
  //  
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_W");
        
  // Cylinder shape       
  G4double shape1_rmin = 0. * mm;
  G4double shape1_rmax = 5. * mm;
  G4double shape1_hz = 0.5* mm;
  G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
  G4Tubs* solidShape1 =    
    new G4Tubs("Shape1", 
    shape1_rmin, shape1_rmax, shape1_hz, shape1_phimin, shape1_phimax);
                      

  G4LogicalVolume* logicShape1 =                         
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name


  G4ThreeVector pos1 = G4ThreeVector(0., 0., -750*mm -1.*shape1_hz);
  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicShape1,             //its logical volume
                    "Shape1",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //     
  // Shape 2 - copper disc
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Cu");
  

  // Cylinder shape       
  G4double shape2_rmin = 0. * mm;
  G4double shape2_rmax = 5. * mm;
  G4double shape2_hz = 0.75 * mm;
  G4double shape2_phimin = 0. * deg, shape2_phimax = 360. * deg;
  G4Tubs* solidShape2 =    
    new G4Tubs("Shape2",                      //its name
    shape2_rmin, shape2_rmax, shape2_hz, shape2_phimin, shape2_phimax);
                
  G4LogicalVolume* logicShape2 =                         
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name
               
  G4ThreeVector pos2 = G4ThreeVector(0, 0, -750*mm + shape2_hz);
  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  
  G4double water_size = 1000. * mm;
  G4Material* water_mat = nist->FindOrBuildMaterial("G4_WATER");

  G4Box* solidWater =
      new G4Box("Water",                       //its name
          0.5 * water_size, 0.5 * water_size, 0.5 * water_size);     //its size

  G4LogicalVolume* logicShape3 =
      new G4LogicalVolume(solidWater,          //its solid
          water_mat,           //its material
          "Water");            //its name

 
  new G4PVPlacement(0,                     //no rotation
          G4ThreeVector(0,0,750*mm),       //at (0,0,0)
          logicWorld,            //its logical volume
          "Water",               //its name
          0,                     //its mother  volume
          false,                 //no boolean operation
          0,                     //copy number
          checkOverlaps);        //overlaps checking


  // Set Shape2 as scoring volume
  //
  fScoringVolume  = logicShape1;
  fScoringVolume2 = logicShape2;
  fScoringVolume3 = logicShape3;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1DetectorConstruction::ConstructSDandField()
{
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    // 
    // Sensitive detectors
    //
    // tungsten target layer
    B1TungstenSD* tungstenSD
        = new B1TungstenSD("TungstenSD", "TungstenHitsCollection", 0);
    G4SDManager::GetSDMpointer()->AddNewDetector(tungstenSD);
    SetSensitiveDetector("Shape1", tungstenSD);

    // copper target layer
    B1CopperSD* CopperSD
        = new B1CopperSD("CopperSD", "CopperHitsCollection", 0);
    G4SDManager::GetSDMpointer()->AddNewDetector(CopperSD);
    SetSensitiveDetector("Shape2", CopperSD);
}