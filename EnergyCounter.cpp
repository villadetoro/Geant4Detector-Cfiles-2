#include "EnergyCounter.h"

#include "g4csv.hh"

EnergyCounter::EnergyCounter( const G4String& name, const G4int id )
  : G4VSensitiveDetector( name ) // Run the constructor of the parent class
{
  // Set which histogram ID / ntuple column to use
  m_ID = id;
}

EnergyCounter::~EnergyCounter()
{
}

// At the start of the event, zero the energy counter
void EnergyCounter::Initialize( G4HCofThisEvent* )
{
  m_totalEnergy = 0.0;
  m_totalElectron = 0.0;
}

// Analyse anything that hits the detector
G4bool EnergyCounter::ProcessHits( G4Step* step, G4TouchableHistory* )
{
  // Get the energy deposited by this hit
  G4double edep = step->GetTotalEnergyDeposit();

  // Add to the total energy in this object
  m_totalEnergy += edep;

  G4Track * track = step->GetTrack();
  // Get the type of particle that made this track
  std::string particle = track->GetParticleDefinition()->GetParticleName();

  if (particle == "e-") m_totalElectron += edep;

  return true;
}

// At the end of an event, store the energy collected in this detector
void EnergyCounter::EndOfEvent( G4HCofThisEvent* )
{
  // Get the analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Fill histogram (histogram 0, bin by layer ID)
  analysisManager->FillH1( 0, m_ID, m_totalElectron / m_totalEnergy );

  // Fill ntuple (it's always ntuple 0, column by layer ID)
  analysisManager->FillNtupleDColumn( 0, m_ID, m_totalElectron /m_totalEnergy );
}
