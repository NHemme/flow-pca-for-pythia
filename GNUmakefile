# =============================================================
#  Makefile                             Christopher J. Plumberg
# =============================================================

all: single_particle_vn.e pair_particle_vn.e

single_particle_vn.e:
		(cd single_particle_vn; make distclean; make install)
pair_particle_vn.e:
		(cd pair_particle_vn; make distclean; make install)

distclean:
		(cd single_particle_vn; make distclean)
		(cd pair_particle_vn; make distclean)
		rm *.e
