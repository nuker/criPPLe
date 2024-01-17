#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "ioctl.h"
#include "core.h"

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\H00");
UNICODE_STRING Sym = RTL_CONSTANT_STRING(L"\\??\\H00");

// Deny a process from being spawned

// Major Functions

NTSTATUS IRPCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp); // For making handles to the driver

NTSTATUS IRPClose(PDEVICE_OBJECT DeviceObject, PIRP Irp); // For closing handles to the driver

NTSTATUS DeviceCTRL(PDEVICE_OBJECT DeviceObject, PIRP Irp);

// Driver Unload

void Unload(PDRIVER_OBJECT DriverObject);

// Driver Entry

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);

