#!/usr/bin/env python2
from __future__ import print_function
import sys
import argparse
import pandas as pd
import numpy as np
import lcm
import xsens_t

parser = argparse.ArgumentParser(
    description='A python script to convert XSENS data to CSV files. '+
    'The CSV output is dropped in the same dir as the lcm log.',
    epilog='Created by Nick Goumas')

parser.add_argument('input_directory',
    help='Directory of raw .tif images to debayer.')

args = parser.parse_args()

log_path = args.input_directory

def createDict(event):
    msg_dict = {}
    msg = xsens_object.decode(event.data)
    for attribute in xsens_attrs:
        msg_dict[attribute] = getattr(msg, attribute)
    return msg_dict

# Create empty xsens object.
xsens_object = xsens_t.xsens_t()
# Open lcm log.
lcm_log = lcm.EventLog(log_path, mode='r')
# xsens attributes available in the log.
xsens_attrs = ["utime",
              "count",
              "heading",
              "roll",
              "pitch",
              "temp",
              "mag_x",
              "mag_y",
              "mag_z",
              "acc_x",
              "acc_y",
              "acc_z",
              "gyr_x",
              "gyr_y",
              "gyr_z"]


data_found = False
list_of_dicts = []
#counter = 0


for event in lcm_log:
    if (event.channel == "DROPIVER.XSENS"):
        msg = xsens_object.decode(event.data)
        list_of_dicts.append(createDict(event))
        data_found = True
        #counter += 1

    #if counter > 5:
    #    break

xsens_dataframe = pd.DataFrame(list_of_dicts)
output_filename = args.input_directory + 'XSENS.csv'
xsens_dataframe.to_csv(output_filename, sep=';', index=False)

if data_found:
    print('XSENS data found and converted')
else:
    print('No XSENS data found in log')
# insert warning if no xsens is found.