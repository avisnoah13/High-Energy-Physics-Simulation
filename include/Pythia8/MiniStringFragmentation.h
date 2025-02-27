// MiniStringFragmentation.h is a part of the PYTHIA event generator.
// Copyright (C) 2025 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This file contains the class for "cluster" fragmentation.
// MiniStringFragmentation: handle the fragmentation of low-mass systems.

#ifndef Pythia8_MiniStringFragmentation_H
#define Pythia8_MiniStringFragmentation_H

#include "Pythia8/FragmentationModel.h"

namespace Pythia8 {

//==========================================================================

// The MiniStringFragmentation class contains the routines to fragment
// occasional low-mass colour singlet partonic systems, where the string
// approach is not directly applicable (for technical reasons).

class MiniStringFragmentation : public FragmentationModel {

public:

  // Constructor.
  MiniStringFragmentation() : FragmentationModel(), setVertices(),
    constantTau(), smearOn(), nTryMass(), hadronVertex(), bLund(), xySmear(),
    kappaVtx(), mc(), mb(), isClosed(), mSum(), m2Sum() {}

  // Initialize and save pointers.
  bool init(StringFlav* flavSelPtrIn = nullptr, StringPT* pTSelPtrIn = nullptr,
    StringZ* zSelPtrIn = nullptr, FragModPtr fragModPtrIn = nullptr) override;

  // Do the fragmentation: driver routine.
  bool fragment(int iSub, ColConfig& colConfig, Event& event,
    bool isDiff = false, bool systemRecoil = true) override;

private:

  // Constants: could only be changed in the code itself.
  static const int    NTRYDIFFRACTIVE, NTRYLASTRESORT, NTRYFLAV;

  // Initialization data, read from Settings.
  bool   setVertices, constantTau, smearOn;
  int    nTryMass, hadronVertex;
  double bLund, xySmear, kappaVtx, mc, mb;

  // Data members.
  bool   isClosed, isJunctionSystem;
  double mSum, m2Sum;
  Vec4   pSum;
  vector<int> iParton;
  FlavContainer flav1, flav2, flavj3;

  // Information from the fragmentation process.
  vector<StringVertex> ministringVertices;

  // Reduce the junction size by absorbing gluons into the quarks/diquarks.
  bool reduce2SimpleJunction(Event& event); // HS

  // Reduce the junction to a string by merging q + q -> qq diquark.
  void reduce2SimpleString(Event& event); // HS

  // Attempt to produce two hadrons from a mini-junction.
  bool minijunction2two(int nTry, Event& event);

  // Attempt to produce two particles from a cluster.
  bool ministring2two( int nTry, Event& event, bool findLowMass = false);

  // Attempt to produce one particle from a cluster.
  bool ministring2one( int iSub, ColConfig& colConfig, Event& event,
    bool findLowMass = false, bool systemRecoil = true);

  // Set hadron production points in space-time picture.
  void setHadronVertices(Event& event, StringRegion& region,
    int iFirst, int iLast);

  // Internal class to make sure that junction-handling-related changes
  // in iParton and in the event record are reset when the job is done.
  // Advantage: many return true/false clauses do destructor commands.
  struct SaveJunctionState {

    // Empty constructor.
    SaveJunctionState(MiniStringFragmentation& msfIn, Event& eventIn)
      : msf(msfIn), iParton(msfIn.iParton), event(eventIn),
        oldSize(eventIn.size()) {}

    void saveMomenta() {
      for (int i = 1; i < int(iParton.size()); ++i) {
        // First two partons from two legs.
        if (iParton[i] < 0) {
          savedMomenta[iParton[i-1]] = event[iParton[i-1]].p();
        }
      }
      // The last parton from the third leg.
      savedMomenta[iParton[iParton.size()-1]] =
        event[iParton[iParton.size()-1]].p();
    }

    // Destructor restoring the old state of the event record and iParton.
    ~SaveJunctionState() {
      if ( savedMomenta.empty() || event.size() <= oldSize ) return;

      // Restore the junction edge partons' original momenta
      for ( auto & mom : savedMomenta ) event[mom.first].p(mom.second);
      int iFirst = oldSize;
      int iLast = event.size() - 1;
      int imother = iParton.size() -1;
      // Mark original partons as hadronized and set their daughter range.
      for (int ip : iParton ) {
        if (ip >= 0) {
          event[ip].statusNeg();
          event[ip].daughters(iFirst, iLast);
        }
      }
      event[iFirst].mothers(iParton[1], iParton[imother]);
      event[iLast].mothers(iParton[1], iParton[imother]);
    }

    MiniStringFragmentation & msf;
    vector<int> iParton;
    Event & event;
    int np{}, oldSize{};
    map<int,Vec4> savedMomenta{};

  };

};

//==========================================================================

} // end namespace Pythia8

#endif // Pythia8_MiniStringFragmentation_H
