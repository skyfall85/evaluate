#!/bin/sh
python2.7 python_to_matlab.py
/usr/local/matlab/r2010b/bin/matlab   -nodesktop -nosplash -r "watershed_script; exit"; reset
python2.7 python_to_txt.py  
g++ orientation_defect_identifier.cpp -ltiff
./a.out
rm a.out
# rm *.txt
# rm *.mat