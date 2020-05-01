// main01.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

#include <iostream>
#include <fstream>
#include <iomanip>
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"

using namespace Pythia8;

int main() {
  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 1000822080");
  pythia.readString("Beams:eCM = 5002.");
  pythia.readString("Beams:frameType = 1");
  pythia.readString("HardQCD:all = on");
  pythia.readString("PhaseSpace:pTHatMin = 20.");
  pythia.readString("Random:setSeed = on");
  // Rope Hadronization
  pythia.readString("Ropewalk:RopeHadronization = off");
  pythia.readString("Ropewalk:doShoving = off");
  pythia.readString("PartonVertex:setVertex = off");
  // Colour Reconnection
  pythia.readString("ColourReconnection:reconnect=off");

  pythia.readString("HeavyIon:SigFitErr = "
                    "0.02,0.02,0.1,0.05,0.05,0.0,0.1,0.0");
  // These parameters are typicall suitable for sqrt(S_NN)=5TeV
  pythia.readString("HeavyIon:SigFitDefPar = "
                    "17.24,2.15,0.33,0.0,0.0,0.0,0.0,0.0");
  // A simple genetic algorithm is run for 20 generations to fit the
  // parameters.
  pythia.readString("HeavyIon:SigFitNGen = 20");

  pythia.init();
  ofstream output("ppB-V1.dat", ios::out);
  const int w = 12;
  output << setw(w) << "pT" << setw(w) << "eta" << setw(w) << "phi" << setw(w) << "event" << endl; 
  // Begin event loop. Generate event. Skip if error. List first one.
  for (int iEvent = 0; iEvent < 100000; ++iEvent) {
    if (!pythia.next()) continue;
    // Find number of all final charged particles and fill histogram.
    for (int i = 0; i < pythia.event.size(); ++i){
      if (pythia.event[i].isFinal() && pythia.event[i].id()==211){
        output << setprecision(8);
        output << fixed
        << setw(w) << pythia.event[i].pT() << " "
        << setw(w) << pythia.event[i].eta() << " "
        << setw(w) << pythia.event[i].phi() << " "
        << setw(w) << iEvent << endl;}
  // End of event loop. Statistics. Histogram. Done.
}}
  output.close();
  // pythia.stat();
  return 0;
}
