
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


How to configure and build vbasym
=================================

The best way to start with the analysis is to clone the latest version of the
code from the git repository. From the location where the package will be
installed on your machine issue the following command:

    git clone git@github.com:plexoos/vbasym.git
    cd vbasym
    git submodule init
    git submodule update

Before you can build and run the program the following environment variables must
be set:

    VBASYM_DIR
       This variable contains the path to the project directory
    VBASYM_RESULTS_DIR
       This variable contains the path to the output directory where the results will be put

The following environment variables are assumed to be set in the standard STAR
session:

    STAR_VERSION
    STAR_HOST_SYS
    ROOTSYS

Example scripts setting up these variables can be found in the `scripts/`
directory:

    scripts/setup.sh
    scripts/setup.csh

To build the library run a slightly modified `cons` command in the terminal

    cd $VBASYM_DIR
    cons CXXFLAGS="-m32 -fPIC -pipe -Wall -Woverloaded-virtual -Wno-long-long" \
         EXTRA_CXXFLAGS="-I${OPTSTAR}/include -Icontrib/root-helper" \
         CPPPATH="#:#StRoot:#.sl64_gcc447/include:${ROOTSYS}/include:./contrib/root-helper"

The binaries are compiled by issuing the following command:

    mkdir build
    cd build
    cmake28 .. -DBOOST_ROOT=${OPTSTAR}
    make

How to split the Monte-Carlo file lists
=======================================

Embedded Monte-Carlo (MC) files relevant for this analysis were produced using PYTHIA
and are stored on the STAR data disks. The file lists are containted in
`$VBASYM_DIR/runlists` with the following format used for their names: `<run
period>_mc_<process type>`. For example, `run11_mc_Wp2enu` is a file list for
the _W+ -> ev_ MC embedded with Run 11 zero bias events.

The list may contain a large number of files. It is convenient, when submitting
a job to *condor*, to split very long lists into several sublists or "runs". To
split it do:

    cd $VBASYM_DIR/runlists/
    split -d -l <# of lines in each sublist> <list name> <list name>_

For example, executing the command

    split -d -l 5 run11_mc_Z02ee run11_mc_Z02ee_

will split the content of the list `run11_mc_Z02ee` in many sublist each
containing 5 lines of the original list and numbered in numerical order starting
with 00. In your directory you should see files named:

    run11_mc_Z02ee_00
    run11_mc_Z02ee_01
    run11_mc_Z02ee_02
    ...

Now all you have to do is to create a text file containing the names of this
sublists you just created. For example create the file named

    run11_mc_Z02ee_

and copy in it the list

    run11_mc_Z02ee_00
    run11_mc_Z02ee_01
    run11_mc_Z02ee_02
    ...

Now all what is needed it to submit to condor the file `run11_mc_Z02ee_`. The
next section explains how to submit to condor.


How to produce the analysis ROOT trees
======================================

To produce the jet root trees do:

    cd $VBASYM_DIR/scripts
    submit_jobs.sh run12_pp_j3 -z -r12 --jets

Then to produce the analysis root trees do:

    submit_jobs.sh run12_pp_j3 -z -r12

Other examples:

    submit_jobs.sh run11_pp_transverse --jets
    submit_jobs.sh run11_pp_transverse
    submit_jobs.sh run11_mc_Wp2enu_ -m --jets


How to check condor jobs output
===============================

One can check the output files returned by condor by verifying the "tralala"
marker in the log files. There should be just one entry per log file:

    grep tralala /path/to/log/* > /tmp/check_jobs_tralala &
    diff --suppress-common-lines -y $VBASYM_DIR/runlists/run11_pp_transverse /tmp/check_jobs_tralala


How to produce histograms from ROOT trees
=========================================

Various sets of histograms can be produced from the ROOT trees generated with
`stana`. This stage of the analysis is done with the help of `vbana` executable.
One can run

    vbana -f run11_pp_transverse

and on the MC samples:

    vbana -f run11_mc_Wp2enu_

For help with other options run `vbana -h`.
