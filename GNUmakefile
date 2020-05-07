# =============================================================
#  Makefile                             Christopher J. Plumberg
# =============================================================

all: pair_particle_vn.e main_Pythia_for_PCA.e

pair_particle_vn.e:
		(cd pair_particle_vn; make distclean; make install)
main_Pythia_for_PCA.e:
		(cd pythiaCode; make distclean; make install)


distclean:
		(cd pair_particle_vn; make distclean)
		(cd pythiaCode; make distclean)
		rm *.e
