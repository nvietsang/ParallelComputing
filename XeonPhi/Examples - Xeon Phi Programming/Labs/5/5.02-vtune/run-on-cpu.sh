#!/bin/bash

source /opt/share/intel/parallel_studio_xe_2017.4.056/bin/psxevars.sh
export OMP_SCHEDULE=static

./app-CPU
