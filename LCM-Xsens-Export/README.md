## LCM XSENS Eporter
Files:
### lcm-log-export-xsens.py
Python script to generate a CSV file of XSENS data stored in an LCM log. Written to work with a custom XSENS IMU software driver used on the UMichigan DROP lab's Iver3-3047 stereo camera AUV. 

Usage: Just point the tool at an LCM log file containing XSENS data. It will create a CSV in the same directory as the LCM log.

### home_bin_add.sh
Bash script to add 'lcm-log-export-xsens.py' to the ~/bin directory. This allows it to be callable from the terminal.

### xsens_t.py
XSENS LCM python type definition module generated when building LCM on the Iver.