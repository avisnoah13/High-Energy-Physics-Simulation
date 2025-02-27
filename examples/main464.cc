// main464.cc is a part of the PYTHIA event generator.
// Copyright (C) 2025 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Marius Utheim

// Keywords: rescattering; low energy; cross sections

// Calculate all cross sections for the specified process and plot them.

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/InputParser.h"
using namespace Pythia8;

//==========================================================================

int main(int argc, char* argv[]) {

  // Set up command line options.
  InputParser ip("Calculate all cross section for specified process.",
    {"./main464 -a 2212 -b 2212"});
  ip.add("a", "0", "ID for beam A.", {"-idA"});
  ip.add("b", "0", "ID for beam B.", {"-idB"});
  ip.add("n", "300", "Number of bins.", {"-nbins"});

  // Initialize the parser and exit if necessary.
  InputParser::Status status = ip.init(argc, argv);
  if (status != InputParser::Valid) return status;

  // Initialize Pythia.
  Pythia pythia;
  pythia.readFile("main464.cmnd");
  if (!pythia.init()) {
    cout << " Pythia failed to initialize." << endl;
    return 1;
  }

  int idA = ip.get<int>("a");
  int idB = ip.get<int>("b");
  if (idA == 0) idA = pythia.mode("Main:spareMode1");
  if (idB == 0) idB = pythia.mode("Main:spareMode2");
  double eMin = pythia.parm("Main:spareParm1");
  double eMax = pythia.parm("Main:spareParm2");
  if (eMin < pythia.particleData.m0(idA) + pythia.particleData.m0(idB)) {
    eMin = pythia.particleData.m0(idA) + pythia.particleData.m0(idB);
    cout << "Warning, setting eMin to nominal mass sum of " << eMin << ".\n";
  }
  int nBin = ip.get<int>("n");

  ParticleData& particleData = pythia.particleData;

  HistPlot plt("plot464");
  plt.frame("fig464", "Cross section for " + particleData.name(idA)
    + " + " + particleData.name(idB), "$\\sqrt{s}$ (GeV)", "$\\sigma$ (mb)");

  // Basic cross sections: non-diffractive, elastic and diffractive.
  Hist sigND = Hist::plotFunc(
    [&](double eCM) { return pythia.getSigmaPartial(idA, idB, eCM, 1); },
    "Non-diffractive", nBin, eMin, eMax);
  plt.add(sigND, "-");

  Hist sigEla = Hist::plotFunc(
    [&](double eCM) { return pythia.getSigmaPartial(idA, idB, eCM, 2); },
    "Elastic", nBin, eMin, eMax);
  plt.add(sigEla, "-");

  Hist sigSD = Hist::plotFunc(
    [&](double eCM) { return pythia.getSigmaPartial(idA, idB, eCM, 3)
                           + pythia.getSigmaPartial(idA, idB, eCM, 4); },
    "Single diffractive", nBin, eMin, eMax);
  plt.add(sigSD, "-");

  Hist sigDD = Hist::plotFunc(
    [&](double eCM) { return pythia.getSigmaPartial(idA, idB, eCM, 5); },
    "Double diffractive", nBin, eMin, eMax);
  plt.add(sigDD, "-");

  // Add nucleon excitation cross section for NN.
  if ((abs(idA) == 2212 || abs(idA) == 2112)
   && (abs(idB) == 2212 || abs(idB) == 2112)
   && idA * idB > 0) {
    Hist sigEx = Hist::plotFunc(
      [&](double eCM) { return pythia.getSigmaPartial(idA, idB, eCM, 7); },
      "Excitation", nBin, eMin, eMax);
    plt.add(sigEx, "-");
  }

  // Add annihilation cross section of baryon-antibaryon.
  if (idA * idB < 0 && particleData.isBaryon(idA)
                    && particleData.isBaryon(idB)) {
    Hist sigAnn = Hist::plotFunc(
      [&](double eCM) { return pythia.getSigmaPartial(idA, idB, eCM, 8); },
      "Annihilation", nBin, eMin, eMax);
    plt.add(sigAnn, "-");
  }

  // Add resonance cross sections if applicable.
  if (pythia.hadronWidths.hasResonances(idA, idB)) {
    Hist sigRes = Hist::plotFunc(
      [&](double eCM) { return pythia.getSigmaPartial(idA, idB, eCM, 9); },
      "Resonant", nBin, eMin, eMax);
    plt.add(sigRes, "-");
  }

  // Add total cross section at the end.
  Hist sigTot = Hist::plotFunc(
    [&](double eCM) { return pythia.getSigmaTotal(idA, idB, eCM); },
    "Total", nBin, eMin, eMax);
  plt.add(sigTot, "k-");

  // Plot.
  plt.plot();

}
