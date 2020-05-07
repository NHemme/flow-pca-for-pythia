// main01.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>

#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"

using namespace Pythia8;

std::unordered_map<bool, string> boolean_toggle
	= {
		{ true,  "on"  },
		{ false, "off" }
	  };

std::unordered_map<string, string> particle_IDs
	= {
		{ "p"  , "2212"       },
		{ "Pb" , "1000822080" },
	  };


int main(int argc, char *argv[])
{

	// Process command-line arguments here
	string target                = particle_IDs[argv[1]];
	string projectile            = particle_IDs[argv[2]];
	string beam_energy           = argv[3];
	long number_of_events        = std::stoi( argv[4] );
	int lower_multiplicity_limit = std::stoi( argv[5] );
	int upper_multiplicity_limit = std::stoi( argv[6] );
	bool stringShovingMode       = static_cast<bool>( std::stoi(argv[7]) );
	bool colourReconnectionMode  = static_cast<bool>( std::stoi(argv[8]) );
	string resultsPath           = argv[9];
	resultsPath					+= "/";

	// Generator. Process selection. LHC initialization. Histogram.
	Pythia pythia;
	pythia.readString("Beams:idA = " + target);
	pythia.readString("Beams:idB = " + projectile);
	pythia.readString("Beams:eCM = " + beam_energy);

	pythia.readString("Beams:frameType       = 1");
	pythia.readString("SoftQCD:all           = on");
	//pythia.readString("HardQCD:all           = on");
	//pythia.readString("PhaseSpace:pTHatMin   = 20.");
	//pythia.readString("Random:setSeed        = on");
	pythia.readString("HeavyIon:SigFitErr    = 0.02,0.02,0.1,0.05,0.05,0.0,0.1,0.0");
	pythia.readString("HeavyIon:SigFitDefPar = 17.24,2.15,0.33,0.0,0.0,0.0,0.0,0.0");
	pythia.readString("HeavyIon:SigFitNGen   = 20");
	
	// Rope Hadronization
	pythia.readString("Ropewalk:RopeHadronization = " + boolean_toggle[stringShovingMode]);
	pythia.readString("Ropewalk:doShoving         = " + boolean_toggle[stringShovingMode]);
	pythia.readString("PartonVertex:setVertex     = " + boolean_toggle[stringShovingMode]);
	
	// Colour Reconnection
	pythia.readString("ColourReconnection:reconnect = " + boolean_toggle[colourReconnectionMode]);
	
	// Initialize and run.
	pythia.init();


	// Set output file(s)
	const int max_events_per_file = 1000;
	int current_file_index = 0;
	string file_index_string = "";
	if (number_of_events > max_events_per_file)
		file_index_string = "_" + std::to_string(current_file_index);
	/*string path = "results_" + target + projectile + "_" + beam_energy
					+ "GeV_" + std::to_string( number_of_events ) + "ev_Nch"
					+ std::to_string( lower_multiplicity_limit ) + "to"
					+ std::to_string( upper_multiplicity_limit ) + "_SS"
					+ boolean_toggle[stringShovingMode] + "_CR"
					+ boolean_toggle[colourReconnectionMode] + "/";*/
	string outfilename = resultsPath + "dataset" + file_index_string + ".dat";
	ofstream output(outfilename.c_str(), ios::out);
	const int w = 12;

	// Begin event loop. Generate event. Skip if error. List first one.
	int iEvent = 0;
	//for (int iEvent = 0; iEvent < number_of_events; ++iEvent)
	while ( iEvent < number_of_events )
	{
		if ( !pythia.next() ) continue;

		// Find number of all final charged particles and fill histogram.
		int multiplicity_this_event = 0;
		vector<Particle> particles_to_output;
		for (int i = 0; i < pythia.event.size(); ++i)
		{
			Particle & p = pythia.event[i];
			if ( pythia.event[i].isFinal() )
			{
				if ( p.isCharged() ) multiplicity_this_event++;
				if ( p.id() == 211 ) particles_to_output.push_back( p );
			}
		}

		// if this event was in chosen multiplicity class, print collected particles
		if ( lower_multiplicity_limit <= multiplicity_this_event
				and multiplicity_this_event <= upper_multiplicity_limit )
		{
			output << setprecision(8);
			for ( const auto & p : particles_to_output )
				output << fixed
					<< setw(w) << p.pT() << "   "
					<< setw(w) << p.eta() << "   "
					<< setw(w) << p.phi() << "   "
					<< setw(w) << iEvent << endl;

			// If too many events for single file, set-up new file here
			if ( (iEvent + 1) % max_events_per_file == 0
				and number_of_events > max_events_per_file
				and iEvent + 1 < number_of_events )
			{
				output.close();
				file_index_string = "_" + std::to_string(++current_file_index);
				outfilename = resultsPath + "dataset" + file_index_string + ".dat";
				output.open(outfilename.c_str(), ios::out);
			}


			// finally, count this event and move on
			iEvent++;
		}

	}

	output.close();

	return 0;
}
