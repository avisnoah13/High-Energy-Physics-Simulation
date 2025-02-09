// main329.cc is a part of the PYTHIA event generator.
// Copyright (C) 2025 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Torbjorn Sjostrand <torbjorn.sjostrand@fysik.lu.se>

// Keywords: hadronization; p pbar;

// Compare multiplicity distributions with UA5 att 200 and 900 GeV,
// and with E735 at 1800 GeV.
// Warning: do not take this too seriously; the assumed "trigger conditions"
// may not match well with reality, especially for E735.

#include "Pythia8/Pythia.h"
using namespace Pythia8;

//==========================================================================

int main() {

  // Number of events.
  int nEvent = 40000;

  // UA5 data points and errors at 200 GeV.
  // Mean and spread is 21.4 +- 10.8.
  double nch200[31] = {0.000,0.008,0.014,0.025,0.044,0.062,0.075,0.084,0.087,
     0.083,0.077,0.069,0.060,0.053,0.044,0.038,0.032,0.027,0.023,0.019,0.016,
     0.013,0.011,0.009,0.007,0.005,0.004,0.003,0.002,0.0014,0.0010};
  /*
  double ner200[31] = {0.000,0.004,0.007,0.009,0.005,0.005,0.005,0.006,0.006,
     0.006,0.006,0.006,0.005,0.005,0.004,0.004,0.004,0.003,0.003,0.003,0.003,
     0.002,0.002,0.002,0.002,0.002,0.001,0.001,0.001,0.0009,0.0007};
  */

  // UA5 data points and errors at 900 GeV.
  // Mean and spread is 35.6 +- 19.5.
  double nch900[51] = {0.000,0.000,0.002,0.005,0.014,0.018,0.025,0.033,0.041,
     0.050,0.053,0.057,0.056,0.054,0.050,0.047,0.042,0.039,0.036,0.033,0.030,
     0.028,0.025,0.023,0.021,0.020,0.019,0.017,0.016,0.015,0.014,0.013,0.012,
     0.011,0.010,0.009,0.008,0.007,0.007,0.006,0.005,0.004,0.004,0.003,0.0029,
     0.0025,0.0022,0.0018,0.0015,0.0013,0.0011};
  /*
  double ner900[51] = {0.000,0.002,0.004,0.008,0.002,0.002,0.003,0.003,0.003,
     0.004,0.004,0.004,0.004,0.004,0.004,0.004,0.003,0.003,0.003,0.003,0.003,
     0.003,0.003,0.003,0.002,0.002,0.002,0.002,0.002,0.002,0.002,0.002,0.002,
     0.002,0.002,0.002,0.002,0.001,0.001,0.001,0.001,0.001,0.001,0.001,
     0.0009,0.0008,0.0008,0.0007,0.0007,0.0006,0.0006};
  */

  // E735 data points and errors at 1800 GeV.
  double nch1800[129] = {0.0,0.1747E0,0.2236E2,0.2239E2,0.1668E2,0.1609E2,
    0.2314E2,0.2369E2,0.2467E2,0.2813E2,0.3418E2,0.3543E2,0.3688E2,0.3506E2,
    0.3604E2,0.3302E2,0.3409E2,0.3173E2,0.2951E2,0.2809E2,0.2554E2,0.2498E2,
    0.2323E2,0.2229E2,0.2063E2,0.2000E2,0.1956E2,0.1938E2,0.1792E2,0.1777E2,
    0.1571E2,0.1568E2,0.1483E2,0.1422E2,0.1418E2,0.1304E2,0.1232E2,0.1168E2,
    0.1073E2,0.1018E2,0.1002E2,0.8899E1,0.8739E1,0.8209E1,0.7822E1,0.7651E1,
    0.7107E1,0.6585E1,0.5518E1,0.5829E1,0.5471E1,0.5016E1,0.4706E1,0.4365E1,
    0.3991E1,0.3776E1,0.3634E1,0.3158E1,0.3047E1,0.2836E1,0.2643E1,0.2353E1,
    0.2292E1,0.1945E1,0.1674E1,0.1713E1,0.1686E1,0.1574E1,0.1288E1,0.1151E1,
    0.1049E1,0.9726E0,0.8791E0,0.7724E0,0.7103E0,0.5733E0,0.5936E0,0.5342E0,
    0.4989E0,0.4023E0,0.3486E0,0.3436E0,0.2958E0,0.3059E0,0.2465E0,0.2213E0,
    0.2068E0,0.1719E0,0.1526E0,0.1212E0,0.1231E0,0.1082E0,0.8530E-1,0.7703E-1,
    0.7515E-1,0.5812E-1,0.5269E-1,0.4825E-1,0.4339E-1,0.3629E-1,0.3278E-1,
    0.2741E-1,0.1910E-1,0.1955E-1,0.1507E-1,0.1553E-1,0.1026E-1,0.9704E-2,
    0.1086E-1,0.8862E-2,0.5725E-2,0.6354E-2,0.4519E-2,0.4178E-2,0.3216E-2,
    0.3401E-2,0.3640E-2,0.2714E-2,0.2221E-2,0.2295E-2,0.1722E-2,0.1572E-2,
    0.1294E-2,0.1393E-2,0.1128E-2,0.8180E-3,0.8552E-3,0.8784E-3,0.6769E-3};
  /*
  double ner1800[129] = {0.0,0.1038E3,0.2906E2,0.3269E2,0.1496E2,0.2016E2,0.
    1805E2,0.1109E2,0.1236E2,0.1048E2,0.8635E1,0.7995E1,0.7519E1,0.5159E1,
    0.3395E1,0.2884E1,0.2722E1,0.2491E1,0.2459E1,0.2139E1,0.1813E1,0.1425E1,
    0.1477E1,0.1434E1,0.1166E1,0.1349E1,0.1269E1,0.1531E1,0.1258E1,0.1362E1,
    0.1073E1,0.1221E1,0.9594E0,0.1083E1,0.1096E1,0.8296E0,0.8285E0,0.9070E0,
    0.8474E0,0.6639E0,0.6611E0,0.6707E0,0.5554E0,0.6139E0,0.4936E0,0.5201E0,
    0.6392E0,0.4166E0,0.5338E0,0.4249E0,0.3694E0,0.4056E0,0.3223E0,0.3155E0,
    0.2899E0,0.2639E0,0.2655E0,0.2299E0,0.2274E0,0.2323E0,0.2173E0,0.2502E0,
    0.2457E0,0.2334E0,0.2137E0,0.1697E0,0.1993E0,0.1632E0,0.1413E0,0.1451E0,
    0.1281E0,0.1412E0,0.1095E0,0.1058E0,0.9231E-1,0.7963E-1,0.7009E-1,
    0.6606E-1,0.6235E-1,0.6147E-1,0.4960E-1,0.4473E-1,0.3821E-1,0.4261E-1,
    0.3509E-1,0.2991E-1,0.2672E-1,0.2414E-1,0.1952E-1,0.1771E-1,0.2172E-1,
    0.1591E-1,0.1434E-1,0.1414E-1,0.1289E-1,0.9475E-2,0.8585E-2,0.1209E-2,
    0.7200E-2,0.7010E-2,0.9489E-2,0.7936E-2,0.5152E-2,0.5100E-2,0.4421E-2,
    0.3789E-2,0.2482E-2,0.2838E-2,0.2573E-2,0.2347E-2,0.2740E-2,0.1969E-2,
    0.1500E-2,0.1456E-2,0.9734E-3,0.1603E-2,0.1043E-2,0.7821E-3,0.7396E-3,
    0.8262E-3,0.7563E-3,0.8712E-3,0.4711E-3,0.6001E-3,0.5315E-3,0.2372E-3,
    0.2909E-3,0.2612E-3,0.3666E-3};
  */

  // Open for plotting of results.
  HistPlot hpl("plot329");

  // Loop over energies.
  for (int iEcm = 0; iEcm < 3; ++iEcm) {
    double         eCM =  200.;
    if (iEcm == 1) eCM =  900.;
    if (iEcm == 2) eCM = 1800.;

    // Generator. Process selection and output.
    Pythia pythia;
    pythia.readString("SoftQCD:nonDiffractive = on");
    pythia.readString("SoftQCD:singleDiffractive = on");
    pythia.readString("SoftQCD:doubleDiffractive = on");
    pythia.readString("Next:numberShowEvent = 0");
    pythia.readString("Next:numberCount = 10000");

    // Exclude products of weak decays and pi0. But assume no c/b tagging!
    pythia.readString("ParticleDecays:limitTau0 = on");
    pythia.readString("111:mayDecay = off");

    // SppbarS or Tevatron initialization.
    pythia.readString("Beams:idB = -2212");
    pythia.settings.parm("Beams:eCM", eCM);

    // If Pythia fails to initialize, exit with error.
    if (!pythia.init()) return 1;

    // Histograms for data and Monte Carlo.
    Hist nchData("n_charged data", 100, -1., 199.);
    Hist nchMCal("n_charged MC all", 100, -1., 199.);
    Hist nchMCtr("n_charged MC triggered", 100, -1., 199.);
    Hist nchComp("n_charged (MC-data)/(MC+data)", 100, -1., 199.);
    if (iEcm == 0) for (int i = 0; i < 31; ++i)
      nchData.fill( 2. * i, nch200[i]);
    if (iEcm == 1) for (int i = 0; i < 51; ++i)
      nchData.fill( 2. * i, nch900[i]);
    if (iEcm == 2) for (int i = 0; i < 129; ++i)
      nchData.fill( 2. * i, 0.001 * nch1800[i]);

    // Begin event loop. Generate event. Skip if error.
    int nTrig = 0;
    for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
      if (!pythia.next()) continue;

      // Analyze event: does it fulfill trigger conditions?
      int nch = 0;
      int nL  = 0;
      int nR  = 0;
      for (int i = 0; i < pythia.event.size(); ++i)
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()) {
        ++nch;
        double pT  = pythia.event[i].pT();
        double eta = pythia.event[i].eta();
        if (pT > 0.15 && iEcm < 2) {
          if (eta > -5.6 && eta < -2.0) ++nL;
          if (eta >  2.0 && eta <  5.6) ++nR;
        }
        if (pT > 0.15 && iEcm == 2) {
          if (eta > -3.25 && eta < -1.6 ) ++nL;
          if (eta >  1.6  && eta <  3.25) ++nR;
        }
      }

      // Fill statistics with and without trigger.
      nchMCal.fill( nch);
      if (nL >0 && nR > 0) {
        ++nTrig;
        nchMCtr.fill( nch);
      }

    // End of event loop. Statistics.
    }
    pythia.stat();

    // Normalize and print histograms.
    nchMCal /= nEvent;
    nchMCtr /= nTrig;
    nchComp = (nchMCtr - nchData) / (nchMCtr + nchData);
    cout << nchMCal << nchMCtr << nchData << nchComp;

    // Plot histogram and data.
    string title = " Charged multiplicity at " + to_string(int(eCM)) + " GeV";
    hpl.frame( "fig329", title, "$n_{\\mathrm{charged}}$", "Probability");
    hpl.add( nchMCal, "h,red", "all events");
    hpl.add( nchMCtr, "h,black", "triggered events");
    hpl.add( nchData, "*,blue", "data");
    hpl.plot(true);

  // End of energy loop.
  }

  return 0;
}
