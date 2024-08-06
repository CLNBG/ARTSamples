===========  =================================================================================================================================
Info         Contains a Python API for interacting with ART-DAQ. 
Author       ART
===========  =================================================================================================================================

About
=====

The **artdaq** package contains an API (Application Programming Interface)
for interacting with the ART-DAQ driver. The package is implemented in Python.
This package was created and is supported by ART. The package is implemented as a 
complex, highly object-oriented wrapper around the ART-DAQ C API using the 
`ctypes <https://docs.python.org/2/library/ctypes.html>`_ Python library.

**artdaq** 1.0 supports all versions of the ART-DAQ driver that ships with the
C API. The C API is included in any version of the driver that supports it. The 
**artdaq** package does not require installation of the C header files.

**artdaq** supports only the Windows operating system.

**artdaq** supports CPython 2.7, 3.4+, PyPy2, and PyPy3.


Features
========
The following represents a non-exhaustive list of supported features for **artdaq**:

- Fully-object oriented
- Fully-featured Task class
- ART-DAQ Errors
- ART-DAQ Events
- ART-DAQ Streams
- `Enums <https://docs.python.org/3/library/enum.html>`_ support in both Python 2 and 3
- Exceptions support
- `Warnings <https://docs.python.org/2/library/warnings.html>`_ support
- Collections that emulate Python container types
- Single, dynamic read and write methods
- Performant, NumPy-based reader and writer classes
- Optional parameters


Installation
============

Running **artdaq** requires ART-DAQ or ART-DAQ Runtime. Visit the
`ArtDAQ_1_8_26 <http://www.art-control.com/FileUPLoad/DownLoadFile/ArtDAQ_1_8_26.zip>`_ to download the latest version 
of ART-DAQ.