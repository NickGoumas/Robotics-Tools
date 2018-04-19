#!/usr/bin/env python2

import sys
import pandas as pd
import numpy as np
import lcm
import xsens_t

log_path = sys.argv[1]

lcm_log = lcm.EventLog(log_path, mode='r')

#print 'Log Size:{} bytes'.format(lcm_log.size())

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

xsens_object = xsens_t.xsens_t()

#xsens_dataframe = pd.DataFrame(data=None, columns=xsens_attrs)

list_of_dicts = []
counter = 0

def createDict(event):
    msg_dict = {}
    msg = xsens_object.decode(event.data)
    for attribute in xsens_attrs:
        msg_dict[attribute] = getattr(msg, attribute)
    return msg_dict


for event in lcm_log:
    if (event.channel == "DROPIVER.XSENS") and (counter < 5):
        msg = xsens_object.decode(event.data)
        list_of_dicts.append(createDict(event))
        #counter += 1

xsens_dataframe = pd.DataFrame(list_of_dicts)
print(len(xsens_dataframe))
xsens_dataframe.to_csv('test.csv', sep=';', index=False)