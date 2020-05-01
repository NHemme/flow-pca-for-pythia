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
#include "../include/pair_particle_vn_Plumberg.h"
#include "../include/pair_particle_vn_Heinz.h"
#include "../include/pair_particle_vn_w_errors.h"

using namespace std;

int main(int argc, char *argv[])
{
  // This stuff is commented out as of 4/27/2020
  /*  string filename = "./datasets/dataset_3D.dat.mpg";
  bool use_random = false;
  //if ( argc > 1 ) filename = string( argv[1] );
  if ( argc < 2 ) use_random = true;

  int n_pT = 6;
  double max_pT = 3.0;
  int order = 2;
  int eta_low = 0.0;
  int eta_high = 2.0;

  // Load dataset.
  vector<vector<double> > eta_vec(n_pT, vector<double> (0));
  vector<vector<double> > phi_vec(n_pT, vector<double> (0));
  vector<vector<int> > event_vec(n_pT, vector<int> (0));
  int N_events;

  arrange_data( eta_vec, phi_vec, event_vec, N_events, n_pT, max_pT, filename );
  cout << "Arrange Data : Complete" << endl;

  // Compute flow covariance matrix.
  vector<vector<double> > V_n_vec(n_pT, vector<double> (n_pT));
  get_V_n(V_n_vec, eta_vec, phi_vec, order, eta_low, eta_high);
  cout << "Covariance Matrix : Complete" << endl;

  int w = 14;
  cout << setprecision(8);
  for(int i = 0 ; i < int(V_n_vec.size()) ; i++)
  {
    for(int j = 0 ; j < int(V_n_vec[i].size()) ; j++)
      cout << setw(w) << V_n_vec[i][j] << " ";
    cout << endl;
  }

  // Perform Eigendecomposition
  vector<vector<double> > eigenvectors(n_pT, vector<double> (n_pT));
  vector<double> eigenvalues(n_pT);
  get_eigen(V_n_vec, eigenvectors, eigenvalues);
  cout << "Eigendecomposition : Complete" << endl;

  // Determine eigenmode 
  int mode = 0;
  string data_title = "./datasets/eigenvector_500_6.dat";
  double eigenvalue_mode;
  vector<double> eigenvector_mode;
  get_eigenmode(eigenvectors, eigenvalues, mode, data_title, eigenvalue_mode, eigenvector_mode);
  cout << "Get Eigenmode : Complete" << endl;

   error(eta_vec, phi_vec, event_vec, eigenvector_mode, V_n_vec, N_events, n_pT, order, eta_low, eta_high);
   cout << "Error complete" << endl;*/

	constexpr bool use_plumberg = false;
	constexpr bool use_heinz = false;
	constexpr bool use_PCA = true;

	if ( use_plumberg )
	{
		cout << endl;
		cout << "<<<======================================>>>" << endl;
		cout << "<<<            BEGIN PLUMBERG            >>>" << endl;
		// Load dataset.
		const int nLoops = 1;
		for (int iLoop = 0; iLoop < nLoops; iLoop++)
		{
			cout << "Loop " << iLoop + 1 << "/" << nLoops << endl;
	
			// generate dataset randomly
			plumberg::generate_random_data();
			
			// update Q vectors
			plumberg::update_Q_n();
		}
	
		// Finally evaluate ensemble-averaged Vn
		plumberg::get_V_n_and_eigendecomposition();

		cout << "<<<             END PLUMBERG             >>>" << endl;
		cout << "<<<======================================>>>" << endl;
		cout << endl;
	}

	if ( use_heinz )
	{
		cout << endl;
		cout << "<<<======================================>>>" << endl;
		cout << "<<<             BEGIN  HEINZ             >>>" << endl;
		// Load dataset.
		const int nLoops = 10;
		heinz::use_seed = false;
		for (int iLoop = 0; iLoop < nLoops; iLoop++)
		{
			cout << "Loop " << iLoop + 1 << "/" << nLoops << endl;
			if ( not heinz::use_seed )
				heinz::seed_index = iLoop;

			// generate dataset randomly
			heinz::generate_random_data();
			
			// update Q vectors
			heinz::update_Q_n();
		}
	
		// Finally evaluate ensemble-averaged Vn
		heinz::get_V_n_and_eigendecomposition();
		cout << "<<<              END  HEINZ              >>>" << endl;
		cout << "<<<======================================>>>" << endl;
		cout << endl;
	}

	if ( use_PCA )
	{
		// --------------------------
		// Define parameters for PCA.
		// --------------------------
		PCA::Vn_mode                = 1;
		PCA::n_pT                   = 6;
		PCA::max_pT                 = 3.0;
		PCA::order                  = 2;
		PCA::eta_low                = 0.0;
		PCA::eta_high               = 2.0;
		// --------------------------
		const int nLoops            = 10;
		const int n_events_per_loop = 10000;
		PCA::N_particles_per_event  = 1000;
		PCA::N_events_to_generate   = n_events_per_loop;
		//PCA::N_total_events         = nLoops*n_events_per_loop;
		PCA::N_total_events = 1000;
		// --------------------------
		PCA::use_seed = true;
		// --------------------------

		// Start the calculation.
		cout << endl;
		cout << "<<<======================================>>>" << endl;
		cout << "<<<              BEGIN  PCA              >>>" << endl;

		// Do this once
		PCA::initialize_vectors();

		// if passing in filenames, process these
		if ( argc > 1 )
		{
			PCA::N_total_events = stoi( argv[1] );

			for ( int iArg = 2; iArg < argc; iArg++ )
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
	}


  return 0;
}

