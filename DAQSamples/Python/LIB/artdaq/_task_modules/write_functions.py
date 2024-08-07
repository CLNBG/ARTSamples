from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import ctypes
import numpy

from artdaq._lib import lib_importer, wrapped_ndpointer, c_bool32
from artdaq.constants import FillMode
from artdaq.errors import check_for_error


def _write_analog_f_64(
        task_handle, write_array, num_samps_per_chan, auto_start, timeout,
        data_layout=FillMode.GROUP_BY_CHANNEL):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteAnalogF64
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double, ctypes.c_int,
                    wrapped_ndpointer(dtype=numpy.float64, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        data_layout.value, write_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value


def _write_analog_scalar_f_64(task_handle, value, auto_start, timeout):
    cfunc = lib_importer.windll.ArtDAQ_WriteAnalogScalarF64
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, c_bool32, ctypes.c_double,
                    ctypes.c_double, ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, auto_start, timeout, value, None)
    check_for_error(error_code)


def _write_binary_i_16(
        task_handle, write_array, num_samps_per_chan, auto_start, timeout,
        data_layout=FillMode.GROUP_BY_CHANNEL):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteBinaryI16
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double, ctypes.c_int,
                    wrapped_ndpointer(dtype=numpy.int16, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        data_layout.value, write_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value


def _write_binary_u_16(
        task_handle, write_array, num_samps_per_chan, auto_start, timeout,
        data_layout=FillMode.GROUP_BY_CHANNEL):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteBinaryU16
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double, ctypes.c_int,
                    wrapped_ndpointer(dtype=numpy.uint16, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        data_layout.value, write_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value


def _write_digital_u_8(
        task_handle, write_array, num_samps_per_chan, auto_start, timeout,
        data_layout=FillMode.GROUP_BY_CHANNEL):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteDigitalU8
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double, ctypes.c_int,
                    wrapped_ndpointer(dtype=numpy.uint8, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        data_layout.value, write_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value


def _write_digital_u_16(
        task_handle, write_array, num_samps_per_chan, auto_start, timeout,
        data_layout=FillMode.GROUP_BY_CHANNEL):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteDigitalU16
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double, ctypes.c_int,
                    wrapped_ndpointer(dtype=numpy.uint16, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        data_layout.value, write_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value


def _write_digital_u_32(
        task_handle, write_array, num_samps_per_chan, auto_start, timeout,
        data_layout=FillMode.GROUP_BY_CHANNEL):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteDigitalU32
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double, ctypes.c_int,
                    wrapped_ndpointer(dtype=numpy.uint32, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        data_layout.value, write_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value


def _write_digital_scalar_u_32(task_handle, value, auto_start, timeout):
    cfunc = lib_importer.windll.WriteDigitalScalarU32
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, c_bool32, ctypes.c_double,
                    ctypes.c_uint, ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, auto_start, timeout, value, None)
    check_for_error(error_code)


def _write_digital_lines(
        task_handle, write_array, num_samps_per_chan, auto_start, timeout,
        data_layout=FillMode.GROUP_BY_CHANNEL):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteDigitalLines
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double, ctypes.c_int,
                    wrapped_ndpointer(dtype=numpy.uint8, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        data_layout.value, write_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value


def _write_ctr_freq(
        task_handle, freq, duty_cycle, num_samps_per_chan, auto_start, timeout):
    num_samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteCtrFreq
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double,
                    wrapped_ndpointer(dtype=numpy.float64, flags=('C', 'W')),
                    wrapped_ndpointer(dtype=numpy.float64, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        freq, duty_cycle,
        ctypes.byref(num_samps_per_chan_written), None)
    check_for_error(error_code)

    return num_samps_per_chan_written.value


def _write_ctr_freq_scalar(task_handle, freq, duty_cycle, auto_start, timeout):
    cfunc = lib_importer.windll.ArtDAQ_WriteCtrFreqScalar
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, c_bool32, ctypes.c_double,
                    ctypes.c_double, ctypes.c_double, ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, auto_start, timeout, freq, duty_cycle, None)
    check_for_error(error_code)


def _write_ctr_time(
        task_handle, high_time, low_time, num_samps_per_chan, auto_start,
        timeout):
    num_samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteCtrTime
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double,
                    wrapped_ndpointer(dtype=numpy.float64, flags=('C', 'W')),
                    wrapped_ndpointer(dtype=numpy.float64, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout, high_time, low_time,
        ctypes.byref(num_samps_per_chan_written), None)
    check_for_error(error_code)

    return num_samps_per_chan_written.value


def _write_ctr_time_scalar(
        task_handle, high_time, low_time, auto_start, timeout):
    cfunc = lib_importer.windll.ArtDAQ_WriteCtrTimeScalar
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, c_bool32, ctypes.c_double,
                    ctypes.c_double, ctypes.c_double,
                    ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, auto_start, timeout, high_time,
        low_time, None)
    check_for_error(error_code)


def _write_ctr_ticks(
        task_handle, high_tick, low_tick, num_samps_per_chan, auto_start,
        timeout):
    num_samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteCtrTicks
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double,
                    wrapped_ndpointer(dtype=numpy.uint32, flags=('C', 'W')),
                    wrapped_ndpointer(dtype=numpy.uint32, flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout,
        high_tick, low_tick,
        ctypes.byref(num_samps_per_chan_written), None)
    check_for_error(error_code)

    return num_samps_per_chan_written.value

def _write_position_comparsion_data(
        task_handle,num_sample_per_chan,position_array, pulse_width_array):

    num_samps_written = ctypes.c_int()
    cfunc = lib_importer.windll.ArtDAQ_WritePositionComparsionData
    if cfunc.argtypes is None:
        with cfunc.arglock:
            cfunc.argtypes = [
                lib_importer.task_handle, ctypes.c_int,
                wrapped_ndpointer(dtype=numpy.uint32, flags=('C', 'W')),
                wrapped_ndpointer(dtype=numpy.uint32, flags=('C', 'W')),
                ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

        error_code = cfunc(
            task_handle, num_sample_per_chan, position_array, pulse_width_array,
            ctypes.byref(num_samps_written), None)
        check_for_error(error_code)


def _write_ctr_ticks_scalar(
        task_handle, high_ticks, low_ticks, auto_start, timeout):
    cfunc = lib_importer.windll.ArtDAQ_WriteCtrTicksScalar
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, c_bool32, ctypes.c_double,
                    ctypes.c_uint, ctypes.c_uint, ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, auto_start, timeout, high_ticks, low_ticks, None)
    check_for_error(error_code)


def _write_raw(
        task_handle, num_samps_per_chan, numpy_array, auto_start, timeout):
    samps_per_chan_written = ctypes.c_int()

    cfunc = lib_importer.windll.ArtDAQ_WriteRaw
    if cfunc.argtypes is None:
        with cfunc.arglock:
            if cfunc.argtypes is None:
                cfunc.argtypes = [
                    lib_importer.task_handle, ctypes.c_int, c_bool32,
                    ctypes.c_double,
                    wrapped_ndpointer(dtype=numpy_array.dtype,
                                      flags=('C', 'W')),
                    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(c_bool32)]

    error_code = cfunc(
        task_handle, num_samps_per_chan, auto_start, timeout, numpy_array,
        ctypes.byref(samps_per_chan_written), None)
    check_for_error(error_code)

    return samps_per_chan_written.value





