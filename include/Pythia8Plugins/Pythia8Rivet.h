// Pythia8Rivet.h is a part of the PYTHIA event generator.
// Copyright (C) 2025 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Author: Christian Bierlich, September 2020.

#ifndef PYTHIA8RIVET_H
#define PYTHIA8RIVET_H

#include "Rivet/AnalysisHandler.hh"
#include "Rivet/Tools/RivetPaths.hh"
#include "Rivet/Config/RivetConfig.hh"

// Check that a proper version code is available (wrong for 4.0.0).
#if RIVET_VERSION_CODE - 1 == -1
#define RIVET_VERSION_CODE 40000
#endif

// Deterine which Rivet version is used. If beyond 4, require HepMC3.
#if RIVET_VERSION_CODE >= 40000
#define RIVET_ENABLE_HEPMC_3
#define PYTHIA_USING_RIVET_4
#endif

// Set the version of HepMC being used (either from Rivet or set above).
#ifndef RIVET_ENABLE_HEPMC_3
#include "Pythia8Plugins/HepMC2.h"
#else
#include "Pythia8Plugins/HepMC3.h"
#endif
#include "Pythia8/HIInfo.h"
#include <set>
#include <vector>
#include <string>

namespace Pythia8 {

// Simplified interface to the Rivet program. Remember to link with
// pythia and -lhepmcinterface -lHepMC -lRivet
//
// Usage: (1) Create an object giving the pythia object and a filename
// as arguments. (2) Repeatedly specify (the name of an) analysis with
// the addAnalysis() function, possibly with analysis parameters.
// (3) initialize the underlying Rivet  object with the init() function.
// (4) Analyze an event with the operator() function. (5) Dump the
// histograms to a file with the done() function.
class Pythia8Rivet {

public:

  // The constructor needs to have the main Pythia object, and the
  // name of the file where the histograms are dumped.
  Pythia8Rivet(Pythia & pytin, string fname)
    : pythia(&pytin), filename(fname), rivet(0), igBeam(false), nDump(-1),
      dumpFile("") {}

  // The destructor will write the histogram file if this has not
  // already been done.
  ~Pythia8Rivet() {done();}

  // Add the name of an analysis to be performed, with a list
  // of analysis parameters.
  void addAnalysis(string ana) {analyses.insert(ana);}

  // Add a YODA file pre-load pre-filled histogram from.
  void addPreload(string prel) {preloads.push_back(prel);}

  // Set "ignore beams" flag.
  void ignoreBeams(bool flagIn) {igBeam = flagIn;}

  // Set Rivet dump period and file name for intermittent histograms.
  // If no dumpfile given, the default output file is used.
  void dump(int period, string fname = "") {
    nDump = period;
    dumpFile = fname;
  }

  // Add an attribute to the current event.
  void addAttribute(const string& name, double val) {
    eventAttributes[name] = val;}

  // Add an (optional) run name for Rivet internal use.
  void addRunName(const string runname) {rname = runname;}

  // Initialize Rivet. Will do nothing if Rivet was already initialized
  void init() {
    if ( rivet ) return;
    rivet = new Rivet::AnalysisHandler(rname);
#ifndef PYTHIA_USING_RIVET_4
    rivet->setIgnoreBeams(igBeam);
    if (nDump > 0) {
      if (dumpFile == "")
        rivet->dump(filename, nDump);
      else
        rivet->dump(dumpFile, nDump);
    }
#else
    rivet->setCheckBeams(!igBeam);
    if (nDump > 0) {
      if (dumpFile == "")
        rivet->setFinalizePeriod(filename, nDump);
      else
        rivet->setFinalizePeriod(dumpFile, nDump);
    }
#endif
    initpath();
    for(int i = 0, N = preloads.size(); i < N; ++i)
      rivet->readData(preloads[i]);
    for (set<string>::iterator it = analyses.begin();
      it != analyses.end(); ++it) {
      rivet->addAnalysis(*it);
    }
    rivet->init(converter.event());
  }

  // Helper function to set analysis path.
  void initpath() const {
    for ( const string & path : Rivet::getAnalysisLibPaths() )
      if ( path == "." ) return;
    Rivet::addAnalysisLibPath(".");
  }

  // Analyze the default Pythia event. Will do nothing if Rivet has
  // not been intialized.
  void operator()() {this->operator()(pythia->event);}

  // Analyze the given event.
  void operator()(Event & event) {
    converter.fillNextEvent(*pythia);
    if ( !rivet ) init();
#ifdef RIVET_ENABLE_HEPMC_3
    for (auto att : eventAttributes)
      converter.addAttribute(att.first, att.second);
#endif
    rivet->analyze(converter.event());
    eventAttributes.clear();
  }

  // Writes histograms to file and deletes the Rivet object. Does
  // nothing if Rivet was not initialized.
  void done() {
    if ( !rivet ) return;
    rivet->finalize();
    rivet->writeData(filename);
    delete rivet;
    rivet = 0;
  }

private:

  // The main pythia object.
  Pythia * pythia;

  // The name of the file where the histograms are dumped.
  string filename;

  // Analyses with optional analysis parameters.
  set<string> analyses;

  // The names of YODA files to preload.
  vector<string> preloads;

  // The Rivet object.
  Rivet::AnalysisHandler * rivet;

  // The HepMC converter.
  Pythia8ToHepMC converter;

  // The Rivet run name.
  string rname;

  // Ignore beams flag.
  bool igBeam;

  // Dump period.
  int nDump;

  // Optional alternative dumpfile name.
  string dumpFile;

  // Additional event attributes of double type to send to Rivet.
  map<const string, double> eventAttributes;

};

}

#endif
