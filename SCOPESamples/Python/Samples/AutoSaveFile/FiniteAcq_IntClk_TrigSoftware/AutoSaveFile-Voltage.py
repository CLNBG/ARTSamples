import sys
sys.path.append(r"../../../")

from ART_SCOPE_Lib.functions import Functions
from ART_SCOPE_Lib.constants import *
from ART_SCOPE_Lib.lib import *
from ART_SCOPE_Lib.errors import check_for_error, ArtScopeError

import ctypes
import numpy
import pprint
import msvcrt

pp = pprint.PrettyPrinter(indent=4)
taskName = input("Enter device name in DMC(e.g Dev1)")  # device name in DMC
taskHandle = lib_importer.task_handle(0)                # task handle
#create task
error_code = Functions.ArtScope_init(taskName, taskHandle)
if error_code < 0:
    check_for_error(error_code)

#set acquisition mode
acquisitionMode = SampleMode.FINITE
error_code = Functions.ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode)

#set vertical parameters
channelName = '0'                                       # channel name
verticalRange = InputRange.RANGE_10VPP                  # range
verticalOffset = 0                                      # offset
verticalCoupling = CouplingType.DC                      # coupling
probeAttenuation = 1                                    # not used
enabled = 1                                             # not used
error_code = Functions.ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, probeAttenuation, enabled)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#set horizontal parameters
minSampleRate = 10000000.0                              # sample rate
minRecordLength = 1000                                  # sample length
refPosition = 0                                         # trigger position
error_code = Functions.ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minRecordLength, refPosition)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#set software trigger
error_code = Functions.ArtScope_ConfigureTriggerSoftWare(taskHandle)

#auto save file
bSaveFile = 1                                           # auto save file is true
fileName = 'D:\scope_data.dat'                          # file name
error_code = Functions.ArtScope_AutoSaveFile(taskHandle, bSaveFile, fileName)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

##get actual channel count
numWfms = ctypes.c_uint32(0)
error_code = Functions.ArtScope_ActualNumWfms(taskHandle, numWfms)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#initialize acquisition
error_code = Functions.ArtScope_InitiateAcquisition(taskHandle)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#get actual sample length
actualRecordLength = ctypes.c_uint32(0)                  # 返回的实际每通道采集数据长度
error_code = Functions.ArtScope_ActualRecordLength(taskHandle, actualRecordLength)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#data buffer
sumLength = actualRecordLength.value * numWfms.value
waveformPtr = numpy.zeros(sumLength, dtype=numpy.double)

#start acquisition
error_code = Functions.ArtScope_StartAcquisition(taskHandle)

timeout = 5.0                                           # timeout unit：s
readLength = actualRecordLength                         # read length
wfmInfo = ArtScope_wfmInfo()                            # information for analyze
wfmInfo.actualSamples = 0
wfmInfo.pAvailSampsPoints = 0

error_code = Functions.ArtScope_FetchVoltage(taskHandle, timeout, readLength, waveformPtr, wfmInfo)
if error_code < 0:
    Functions.ArtScope_StopAcquisition(taskHandle)
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

for index in range(numWfms.value):
    print("%8.2f (mV)\t" %(waveformPtr[index]))
print("\n")

#stop acquisition
error_code = Functions.ArtScope_StopAcquisition(taskHandle)
if error_code < 0:
    Functions.ArtScope_ReleaseAcquisition(taskHandle)
    Functions.ArtScope_Close(taskHandle)

#release acquisition
error_code = Functions.ArtScope_ReleaseAcquisition(taskHandle)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)

#close task
error_code = Functions.ArtScope_Close(taskHandle)

text=input("Press enter to exit.")




