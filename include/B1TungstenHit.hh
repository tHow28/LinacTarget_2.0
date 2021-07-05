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
/// \file B1TungstenHit.hh
/// \brief Definition of the B1TungstenHit class
//

#ifndef B1TungstenHit_h
#define B1TungstenHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class B1TungstenHit : public G4VHit
{
public:
    B1TungstenHit();
    virtual ~B1TungstenHit();
    B1TungstenHit(const B1TungstenHit& right);
    const B1TungstenHit& operator=(const B1TungstenHit& right);
    int operator==(const B1TungstenHit& right) const;

    inline void* operator new(size_t);
    inline void  operator delete(void* hit);

    virtual void Print();
    virtual void Draw();

    // setter methods
    void SetLayerNumber(G4int number) { fLayerNumber = number; }
    void SetTime(G4double time) { fTime = time; }
    void SetPosition(G4ThreeVector position) { fPosition = position; }
    void AddEdep(G4double edep) { fEdep += edep; }

    // getter methods
    G4int          GetLayerNumber() const { return fLayerNumber; }
    G4double       GetTime() const { return fTime; }
    G4ThreeVector  GetPosition() const { return fPosition; }
    G4double GetEdep() const { return fEdep; }

private:
    // data members
    G4int          fLayerNumber;
    G4double       fTime;
    G4ThreeVector  fPosition;
    G4double       fEdep;
};

typedef G4THitsCollection<B1TungstenHit> B1TungstenHitsCollection;

extern G4Allocator<B1TungstenHit>* B1TungstenHitAllocator;

inline void* B1TungstenHit::operator new(size_t)
{
    if (!B1TungstenHitAllocator)
        B1TungstenHitAllocator = new G4Allocator<B1TungstenHit>;
    return (void*)B1TungstenHitAllocator->MallocSingle();
}

inline void B1TungstenHit::operator delete(void* hit)
{
    B1TungstenHitAllocator->FreeSingle((B1TungstenHit*)hit);
}

#endif



