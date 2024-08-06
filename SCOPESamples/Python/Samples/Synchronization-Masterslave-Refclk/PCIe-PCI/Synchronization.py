import sys
sys.path.append(r"../../")

from ART_SCOPE_Lib.functions import Functions
from ART_SCOPE_Lib.constants import *
from ART_SCOPE_Lib.lib import *
from ART_SCOPE_Lib.errors import check_for_error, ArtScopeError

import ctypes
import numpy
import pprint
import msvcrt
import threading

#thread to read data of master card
def ReadThread_Master(taskhandle, readtimeout, samplelens, databuffer, wfminfo):
    while True:
        if msvcrt.kbhit():
            if ord(msvcrt.getch()) != None:
                break
        error_code = Functions.ArtScope_FetchBinary16(taskhandle, readtimeout, samplelens, databuffer, wfminfo)
        if error_code < 0:
            Functions.ArtScope_StopAcquisition(taskhandle)
            Functions.ArtScope_Close(taskhandle)
            break
        for index in range(wfminfo.channelCount):
            print("%d(Binary)\t" % (databuffer[index]))
        print("\n")
        break

    #stop
    error_code = Functions.ArtScope_StopAcquisition(taskhandle)
    #release
    error_code = Functions.ArtScope_ReleaseAcquisition(taskhandle)
    #close
    error_code = Functions.ArtScope_Close(taskhandle)

##############################################################################
#thread to read data of slave card
def ReadThread_Slave(taskhandle, readtimeout, samplelens, databuffer, wfminfo):
    while True:
        if msvcrt.kbhit():
            if ord(msvcrt.getch()) != None:
                break
        error_code = Functions.ArtScope_FetchBinary16(taskhandle, readtimeout, samplelens, databuffer, wfminfo)
        if error_code < 0:
            Functions.ArtScope_StopAcquisition(taskhandle)
            Functions.ArtScope_Close(taskhandle)
            break
        for index in range(wfminfo.channelCount):
            print("%d(Binary)\t" % (databuffer[index]))
        print("\n")
        break

    #stop
    error_code = Functions.ArtScope_StopAcquisition(taskhandle)
    #release
    error_code = Functions.ArtScope_ReleaseAcquisition(taskhandle)
    #close
    error_code = Functions.ArtScope_Close(taskhandle)

##################################################################################
pp = pprint.PrettyPrinter(indent=4)
taskName_master = input("Enter the main device name in DMC(e.g Dev1)")  # master card name in DMC
taskHandle_master = lib_importer.task_handle(0)                         # task handle of master card

taskName_slave = input("Enter the slave device name in DMC(e.g Dev1)")  # slave card name in DMC
taskHandle_slave = lib_importer.task_handle(0)                          # task handle of slave card

#create task
error_code = Functions.ArtScope_init(taskName_master, taskHandle_master)
if error_code < 0:
    check_for_error(error_code)

#set acquisition mode
acquisitionMode = SampleMode.FINITE
error_code = Functions.ArtScope_ConfigureAcquisitionMode(taskHandle_master, acquisitionMode)

#set vertical parameters
channelName = '0,1'                                     # channel name
verticalRange = InputRange.RANGE_10VPP                  # range
verticalOffset = 0                                      # offset
verticalCoupling = CouplingType.DC                      # coupling
probeAttenuation = 1                                    # not used
enabled = 1                                             # channel enable
error_code = Functions.ArtScope_ConfigureVertical(taskHandle_master, channelName, verticalRange, verticalOffset, verticalCoupling, probeAttenuation, enabled)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#set horizontal parameters
minSampleRate = 10000000.0                              # sample rate
minRecordLength = 1000                                  # sample length
refPosition = 0                                         # trigger position
error_code = Functions.ArtScope_ConfigureHorizontalTiming(taskHandle_master, minSampleRate, minRecordLength, refPosition)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#set sychronous trigger source
triggerSyncPulseSouce = TriggerSyncSrc.TRIG_SYNC_0
error_code = Functions.ArtScope_ConfigureTriggerSyncSrc(taskHandle_master, triggerSyncPulseSouce)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_master)
    check_for_error(error_code)

#set trigger parameters
triggerSource = TriggerSource.TRIGSRC_CH0
triggerLevel = 0.0
triggerSlope = TriggerSlope.TRIGDIR_NEGATIVE
triggerCoupling = CouplingType.DC
triggerCount = 1
triggerSensitivity = 50
error_code = Functions.ArtScope_ConfigureTriggerEdge(taskHandle_master, triggerSource, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_master)
    check_for_error(error_code)

#set clock parameters
inputClockSource = InputClockSource.CLK_ONBOARD
error_code = Functions.ArtScope_ConfigureClock(taskHandle_master, inputClockSource, 1, 1)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_master)
    check_for_error(error_code)

#get actual channel count
numWfms = ctypes.c_uint32(0)
error_code = Functions.ArtScope_ActualNumWfms(taskHandle_master, numWfms)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_master)
    check_for_error(error_code)

#initialize acquisition
error_code = Functions.ArtScope_InitiateAcquisition(taskHandle_master)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_master)
    check_for_error(error_code)

#get actual sample length
actualRecordLength = ctypes.c_uint32(0)
error_code = Functions.ArtScope_ActualRecordLength(taskHandle_master, actualRecordLength)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_master)
    check_for_error(error_code)

#data buffer
sumLength = actualRecordLength.value * numWfms.value
waveformPtr_master = numpy.zeros(sumLength, dtype=numpy.uint16)

timeout = 5.0                                           # timeout unit：s
readLength_master = actualRecordLength                  # read length
wfmInfo_master = ArtScope_wfmInfo()                     # information for analyze
wfmInfo_master.actualSamples = 0
wfmInfo_master.pAvailSampsPoints = 0

#create task
error_code = Functions.ArtScope_init(taskName_slave, taskHandle_slave)
if error_code < 0:
    check_for_error(error_code)

#set acquisition mode
acquisitionMode = SampleMode.FINITE
error_code = Functions.ArtScope_ConfigureAcquisitionMode(taskHandle_slave, acquisitionMode)

#set vertical parameters
error_code = Functions.ArtScope_ConfigureVertical(taskHandle_slave, channelName, verticalRange, verticalOffset, verticalCoupling, probeAttenuation, enabled)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#set horizontal parameters
error_code = Functions.ArtScope_ConfigureHorizontalTiming(taskHandle_slave, minSampleRate, minRecordLength, refPosition)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle)
    check_for_error(error_code)

#set sychronous trigger source
error_code = Functions.ArtScope_ConfigureTriggerSyncSrc(taskHandle_slave, triggerSyncPulseSouce)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_slave)
    check_for_error(error_code)

#set trigger parameters
triggerSource = TriggerSource.TRIGSRC_SYNC
error_code = Functions.ArtScope_ConfigureTriggerEdge(taskHandle_slave, triggerSource, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_slave)
    check_for_error(error_code)

#set clock parameters
inputClockSource = InputClockSource.CLK10M_MASTER
error_code = Functions.ArtScope_ConfigureClock(taskHandle_slave, inputClockSource, 1, 0)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_slave)
    check_for_error(error_code)

#get actual channel count
numWfms = ctypes.c_uint32(0)
error_code = Functions.ArtScope_ActualNumWfms(taskHandle_slave, numWfms)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_slave)
    check_for_error(error_code)

#initialize acquisition
error_code = Functions.ArtScope_InitiateAcquisition(taskHandle_slave)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_slave)
    check_for_error(error_code)

#get actual sample length
actualRecordLength = ctypes.c_uint32(0)
error_code = Functions.ArtScope_ActualRecordLength(taskHandle_slave, actualRecordLength)
if error_code < 0:
    Functions.ArtScope_Close(taskHandle_slave)
    check_for_error(error_code)

#data buffer
sumLength = actualRecordLength.value * numWfms.value
waveformPtr_slave = numpy.zeros(sumLength, dtype=numpy.uint16)

timeout = 5.0                                           # timeout unit：s
readLength_slave = actualRecordLength                   # read length
wfmInfo_slave = ArtScope_wfmInfo()                      # information for analyze
wfmInfo_slave.actualSamples = 0
wfmInfo_slave.pAvailSampsPoints = 0

#first start slave card
error_code = Functions.ArtScope_StartAcquisition(taskHandle_slave)
if error_code < 0:
    Functions.ArtScope_ReleaseAcquisition(taskHandle_slave)
    Functions.ArtScope_Close(taskHandle_slave)
    check_for_error(error_code)
thread_slave = threading.Thread(target=ReadThread_Slave, args=(taskHandle_slave, timeout, readLength_slave, waveformPtr_slave, wfmInfo_slave))
thread_slave.start()

#then start master card
error_code = Functions.ArtScope_StartAcquisition(taskHandle_master)
if error_code < 0:
    Functions.ArtScope_ReleaseAcquisition(taskHandle_master)
    Functions.ArtScope_Close(taskHandle_master)
    check_for_error(error_code)
thread_master = threading.Thread(target=ReadThread_Master, args=(taskHandle_master, timeout, readLength_master, waveformPtr_master, wfmInfo_master))
thread_master.start()

thread_slave.join()
thread_master.join()

text=input("Press enter to exit.")




