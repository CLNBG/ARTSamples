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
acquisitionMode = SampleMode.CONTINUOUS
error_code = Functions.ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode)

#set vertical parameters
channelName = '0,1'                                     # channel name
verticalRange = InputRange.RANGE_10VPP                  # range
verticalOffset = 0                                      # offset
verticalCoupling = CouplingType.DC                      # coupling
probeAttenuation = 1                                    # not used
enabled = 1                                             # channel enable
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

#get actual channel count
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
actualRecordLength = ctypes.c_uint32(0)
error_code = Functions.ArtScope_ActualRecordLength(taskHandle, actualRecordLength)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#data buffer
sumLength = actualRecordLength.value * numWfms.value
waveformPtr = numpy.zeros(sumLength, dtype=numpy.double)

#start acquisition
error_code = Functions.ArtScope_StartAcquisition(taskHandle)
if error_code < 0:
    Functions.ArtScope_ReleaseAcquisition(taskHandle)
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

timeout = 5.0                                           # timeout unit：s
readLength = actualRecordLength                         # read length
wfmInfo = ArtScope_wfmInfo()                            # information for analyze
wfmInfo.actualSamples = 0
wfmInfo.pAvailSampsPoints = 0

pointsFetched = 0
chunksFetched = 0
while True:
    if msvcrt.kbhit():
        if ord(msvcrt.getch()) != None:
            break
    error_code = Functions.ArtScope_FetchVoltage(taskHandle, timeout, readLength, waveformPtr, wfmInfo)
    if error_code < 0:
        Functions.ArtScope_StopAcquisition(taskHandle)
        Functions.ArtScope_Close(taskHandle)
        check_for_error(error_code)
    pointsFetched += wfmInfo.actualSamples
    chunksFetched += 1
    print("Press any key to exit...")
    print("Chunks Fetched :%d\tPoints Fetched : %d" %(chunksFetched, pointsFetched))
    for index in range(numWfms.value):
        print("%d(mV)\t" %(waveformPtr[index]))
    print("\n")

#stop acquisitio
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




