#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <complex>
#include <vector>
#include <iomanip>
#include <string>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

#include "../include/pair_particle_vn.h"
#include "../include/pair_particle_vn_w_errors.h"

using namespace std;

int main(int argc, char *argv[])
{

		// --------------------------
		// Define parameters for PCA.
		// --------------------------
		PCA::Vn_mode                = 0;
		PCA::n_pT                   = 6;
		PCA::max_pT                 = 3.0;
		PCA::order                  = 2;
		PCA::eta_low                = 2.0;
		PCA::eta_high               = 10.0;
		// --------------------------
		const int nLoops            = 10;
		const int n_events_per_loop = 10000;
		PCA::N_particles_per_event  = 1000;
		PCA::N_events_to_generate   = n_events_per_loop;
		PCA::N_total_events         = nLoops*n_events_per_loop;
		//PCA::N_total_events = 100000;
		// --------------------------
		PCA::use_seed = true;
		// --------------------------

		// Start the calculation.
		cout << endl;
		cout << "<<<======================================>>>" << endl;
		cout << "<<<              BEGIN  PCA              >>>" << endl;

		// if passing in filenames, process these
		if ( argc > 1 )
		{
			// Overwrite default set above from command-line
			PCA::N_total_events = stoi( argv[1] );

			// Do this once
			PCA::initialize_vectors();

			// Set output filename from second command-line argument
			PCA::output_name = argv[2];

			// Remaining command-line arguments are files to analyze
			for ( int iArg = 3; iArg < argc; iArg++ )
			{
				// Load dataset from file.
				string filename = string( argv[iArg] );
				PCA::read_in_data( filename );
	
				// Compute Q vectors.
				PCA::update_Q_n();
			}
		}
		else
		{
			// Do this once
			PCA::initialize_vectors();

			// generate dataset(s) randomly
			for (int iLoop = 0; iLoop < nLoops; iLoop++)
			{
				cout << "Loop " << iLoop + 1 << "/" << nLoops << endl;
				if ( not PCA::use_seed )
					PCA::seed_index = iLoop;
		
				// generate dataset randomly
				PCA::generate_random_data();
				
				// update Q vectors
				PCA::update_Q_n();
			}
		}
	
		// Finally evaluate ensemble-averaged Vn,
		// jackknife estimates, and eigen-decompose.
		PCA::get_V_n_with_errors_and_eigendecomposition();
		cout << "<<<               END  PCA               >>>" << endl;
		cout << "<<<======================================>>>" << endl;
		cout << endl;


  return 0;
}

