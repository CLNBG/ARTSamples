===========  =================================================================================================================================
Info         Contains a Python API for interacting with ART-SCOPE. 
Author       ART
===========  =================================================================================================================================

About
=====

The **ArtScope** package contains an API (Application Programming Interface)
for interacting with the ART-SCOPE driver. The package is implemented in Python.
This package was created and is supported by ART. The package is implemented as a 
complex, highly object-oriented wrapper around the ART-SCOPE C API using the 
`ctypes <https://docs.python.org/2/library/ctypes.html>`_ Python library.

**ArtScope** 1.20.0032 supports all versions of the ART-SCOPE driver that ships with the
C API. The C API is included in any version of the driver that supports it. The 
**ArtScope** package does not require installation of the C header files.

**ArtScope** supports only the Windows operating system.

**ArtScope** supports CPython 2.7, 3.4+, PyPy2, and PyPy3.


Features
========
The following represents a non-exhaustive list of supported features for **ArtScope**:

- Fully-object oriented
- Fully-featured Task class
- ART-SCOPE Errors
- ART-SCOPE Events
- ART-SCOPE Streams
- `Enums <https://docs.python.org/3/library/enum.html>`_ support in both Python 2 and 3
- Exceptions support
- `Warnings <https://docs.python.org/2/library/warnings.html>`_ support
- Collections that emulate Python container types
- Single, dynamic read and write methods
- Performant, NumPy-based reader and writer classes
- Optional parameters


Installation
============

Running **ArtScope** requires ART-SCOPE or ART-SCOPE Runtime. Visit the
`ART-SCOPE <http://www.art-control.com/downlist_T2.html>`_ to download the latest version 
of ART-SCOPE.