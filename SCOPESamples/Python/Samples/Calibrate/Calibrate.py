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

pp = pprint.PrettyPrinter(indent=4)
taskName = input("Enter device name in DMC(e.g Dev1)")  # device name in DMC
taskHandle = lib_importer.task_handle(0)                # task handle
#create task
error_code = Functions.ArtScope_init(taskName, taskHandle)
if error_code < 0:
    check_for_error(error_code)

#calibrate
error_code = Functions.ArtScope_Calibration(taskHandle)

#close task
error_code = Functions.ArtScope_Close(taskHandle)

text=input("Press enter to exit.")




