#! /usr/bin/env bash

# rename some command-line parameters
target=$1
projectile=$2
beamEnergy=$3
nEvents=$4
lower=$5
upper=$6
SSMode=$7
CRMode=$8

# create results directory for this run
resultsDir=results_${target}_${projectile}_${beamEnergy}GeV_${nEvents}events_Nch_${lower}_to_${upper}_SS${SSMode}_CR${CRMode}
mkdir -p $resultsDir

./main_Pythia_for_PCA.e $target $projectile     \
                        $beamEnergy $nEvents    \
						$lower $upper           \
                        $SSMode $CRMode $resultsDir

./pair_particle_vn.e $nEvents "$resultsDir/eigensystem.dat" $resultsDir/*
