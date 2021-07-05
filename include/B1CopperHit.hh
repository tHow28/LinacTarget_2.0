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
/// \file B1CopperHit.hh
/// \brief Definition of the B1CopperHit class
//

#ifndef B1CopperHit_h
#define B1CopperHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class B1CopperHit : public G4VHit
{
public:
    B1CopperHit();
    virtual ~B1CopperHit();
    B1CopperHit(const B1CopperHit& right);
    const B1CopperHit& operator=(const B1CopperHit& right);
    int operator==(const B1CopperHit& right) const;

    inline void* operator new(size_t);
    inline void  operator delete(void* hit);

    virtual void Print();
    virtual void Draw();

    // setter methods
    void SetLayerNumber(G4int number) { fLayerNumber = number; }
    void SetTime(G4double time) { fTime = time; }
    void SetPosition(G4ThreeVector position) { fPosition = position; }
    void AddEdep(G4double edep2) { fEdep2 += edep2; }

    // getter methods
    G4int          GetLayerNumber() const { return fLayerNumber; }
    G4double       GetTime() const { return fTime; }
    G4ThreeVector  GetPosition() const { return fPosition; }
    G4double GetEdep() const { return fEdep2; }

private:
    // data members
    G4int          fLayerNumber;
    G4double       fTime;
    G4ThreeVector  fPosition;
    G4double       fEdep2;
};

typedef G4THitsCollection<B1CopperHit> B1CopperHitsCollection;

extern G4Allocator<B1CopperHit>* B1CopperHitAllocator;

inline void* B1CopperHit::operator new(size_t)
{
    if (!B1CopperHitAllocator)
        B1CopperHitAllocator = new G4Allocator<B1CopperHit>;
    return (void*)B1CopperHitAllocator->MallocSingle();
}

inline void B1CopperHit::operator delete(void* hit)
{
    B1CopperHitAllocator->FreeSingle((B1CopperHit*)hit);
}

#endif



