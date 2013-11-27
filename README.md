
What is vbasym?
===============

`vbasym` is a software package designed to analyse the data collected by the
STAR detector in the Brookhaven National Lab


Motivation from the physicists point of view
============================================

When protons accelerated by the Relativistic Heavy Ion Collider (RHIC) collide
at &radic;s = 500 GeV the Z and W bosons can be produced at STAR. In this
analysis we measure the asymmetry of the W and Z bosons produced in pp
collisions with the spin of the protons perpendicular to the beam. During the
2011 run STAR has collected such data for the first time. The bosons cannot be
detected directly but their kinematics can be reconstructed by detecting the
decay products. The Z bosons can be easily reconstructed in the di-electron
channel. However, the reconstruction of the W's decaying into a
electron-neutrino pair is challenging due to the neutrino completely escaping
the detector. The W has never been reconstructed with the STAR detector. This
analysis is the first attempt to reconstruct the kinematics of the W bosons at
STAR. 


How to produce the analysis ROOT trees
======================================

The following environment variables must be set:

    VBASYM_DIR
    VBASYM_RESULTS_DIR

Example scripts setting up these variables can be found in the `scripts/`
directory:

    scripts/setup.sh
    scripts/setup.csh

To produce the jet root trees do:

    cd runlists/
    ln -s run12_pp_j3 run12_pp_j3_jets
    scripts/submit_jobs.sh run12_pp_j3_jets -z -r12 --jets

Then to produce the analysis root trees do:

    scripts/submit_jobs.sh run12_pp_j3 -z -r12

Other examples:

    scripts/submit_jobs.sh run11_pp_transverse --jets
    scripts/submit_jobs.sh run11_pp_transverse


How to check condor jobs output
===============================

In the past it has been noticed that some of the jobs returned by condor do not
finish properly. While the reason for this is not very clear one should check
the consistency of the returned output. To check it two "markers" in the log
files can be used:

    grep terminate /path/to/log/* > /tmp/check_jobs_bad &
    grep tralala /path/to/log/* > /tmp/check_jobs_tralala &

    diff --suppress-common-lines -y $VBASYM_DIR/runlists/run11_pp_transverse /tmp/check_jobs_tralala


