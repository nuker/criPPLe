#include "driver.h"

// Deny a process from being spawned

// Major Functions

NTSTATUS IRPCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

    UNREFERENCED_PARAMETER(DeviceObject);

    DbgPrint("[+] IRPCreate.\n");

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

NTSTATUS IRPClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

    UNREFERENCED_PARAMETER(DeviceObject);

    DbgPrint("[+] IRPClose.\n");

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

NTSTATUS DeviceCTRL(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

    UNREFERENCED_PARAMETER(DeviceObject);

    ULONG_PTR len = 0;
    NTSTATUS stat = STATUS_SUCCESS;

    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

    switch (stack->Parameters.DeviceIoControl.IoControlCode) {
        case IOCTL_TEST:

            DbgPrint("[+] IOCTL_TEST\n");

            // Check if the input buffer is large enough to hold the struct

            if (stack->Parameters.DeviceIoControl.InputBufferLength < sizeof(TEST_DATA)) {

                DbgPrint("[+] Input buffer is too small to hold TEST_DATA.\n");
                stat = STATUS_BUFFER_TOO_SMALL;
                break;
            }

            PTEST_DATA test = (PTEST_DATA)stack->Parameters.DeviceIoControl.Type3InputBuffer;

            // Make sure pointer is not NULL

            if (test == NULL) {

                DbgPrint("[+] TEST_DATA was NULL.\n");
                stat = STATUS_INVALID_PARAMETER;
                break;

            }

            // Print data recieved from usermode / client

            DbgPrint("[+] TEST_DATA: %d", test->data);

            // Send data back to client

            // Check if the input buffer is large enough to hold the struct

            if (stack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(TEST_RESP)) {

                DbgPrint("[+] Output buffer is too small to hold TEST_RESP.\n");
                stat = STATUS_BUFFER_TOO_SMALL;
                break;

            }

            // Cast userbuffer to resp struct

            PTEST_RESP resp = (PTEST_RESP)Irp->UserBuffer;

            // Add 5 to whatever int was sent
            resp->data = test->data + 5;

            // assign return len
            len = sizeof(TEST_RESP);

            break;

        case IOCTL_SKINPROT:

            DbgPrint("[+] IOCTL_SKINPROT\n");

            // Check if the input buffer is large enough to hold the struct

            if (stack->Parameters.DeviceIoControl.InputBufferLength < sizeof(TARGET)) {

                DbgPrint("[+] Input buffer is too small to hold TARGET.\n");
                stat = STATUS_BUFFER_TOO_SMALL;
                break;
            }

            // Cast data from the caller to our struct

            PTARGET target = (PTARGET)stack->Parameters.DeviceIoControl.Type3InputBuffer;

            if (target == NULL) {

                DbgPrint("[+] Target was NULL.\n");
                stat = STATUS_INVALID_PARAMETER;
                break;

            } else {

                // Make sure pid is not 0

                if (target->pid <= 0) {

                    DbgPrint("[+] Target was NULL.\n");
                    stat = STATUS_INVALID_PARAMETER;
                    break;

                }
            }

            DbgPrint("[+] Getting EPROCESS structure for: %d.\n", target->pid);

            PEPROCESS eproc;
            stat = PsLookupProcessByProcessId((HANDLE)target->pid, &eproc);

            PPS_PROT PsProt = (PPS_PROT)(((ULONG_PTR)eproc) + PS_PROT_OFFSET);

            if (PsProt == NULL) {

                DbgPrint("[+] PsProt was NULL.\n");
                stat = STATUS_INVALID_PARAMETER;

                /*

                    The output of PsLookupProcessByProcessId is reference counted.  
                    This count is used by the kernel to track callers that have references 
                    to objects in memory to prevent them from being disposed whilst still in use.  
                    Whenever we are finished with a reference-counted object, we must call 
                    ObDereferenceObject. Failing to do so will cause kernel memory leaks.
                
                */

                ObDereferenceObject(eproc);

                break; 

            }

            // Disable the process protection by setting all fields to 0

            PsProt->Level = 0;
            PsProt->Type = 0;
            PsProt->Audit = 0;
            PsProt->Signer = 0;
            
            ObDereferenceObject(eproc);

            break;

        case IOCTL_LOADPROT:

            DbgPrint("[+] IOCTL_LOADPROT\n");

            // Check if the input buffer is large enough to hold the struct

            if (stack->Parameters.DeviceIoControl.InputBufferLength < sizeof(TARGET)) {

                DbgPrint("[+] Input buffer is too small to hold TARGET.\n");
                stat = STATUS_BUFFER_TOO_SMALL;
                break;
            }

            // Cast data from the caller to our struct

            PTARGET friend = (PTARGET)stack->Parameters.DeviceIoControl.Type3InputBuffer;

            if (friend == NULL) {

                DbgPrint("[+] Target was NULL.\n");
                stat = STATUS_INVALID_PARAMETER;
                break;

            } else {

                // Make sure pid is not 0

                if (friend->pid <= 0) {

                    DbgPrint("[+] Target was NULL.\n");
                    stat = STATUS_INVALID_PARAMETER;
                    break;

                }
            }

            DbgPrint("[+] Getting EPROCESS structure for: %d.\n", friend->pid);

            PEPROCESS eproc2;
            stat = PsLookupProcessByProcessId((HANDLE)friend->pid, &eproc2);

            PPS_PROT Prot = (PPS_PROT)(((ULONG_PTR)eproc2) + PS_PROT_OFFSET);

            if (Prot == NULL) {

                DbgPrint("[+] PsProt was NULL.\n");
                stat = STATUS_INVALID_PARAMETER;
                ObDereferenceObject(eproc2);

                break; 

            }

            // Set custom process protection

            Prot->Level = 0x62;
            Prot->Type = 0x2;     // PsProtectedTypeProtected = 0n2
            Prot->Audit = 0;      
            Prot->Signer = 0x6;   // PsProtectedSignerWinTcb = 0n6
            
            ObDereferenceObject(eproc2);

            break;
        
        default:

            stat = STATUS_INVALID_DEVICE_REQUEST;
            DbgPrint("[+] What?.\n");

            break;
    }

    Irp->IoStatus.Status = stat;
    Irp->IoStatus.Information = len;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

// Unload

void Unload(PDRIVER_OBJECT DriverObject) {

    // Cleanup

    IoDeleteSymbolicLink(&Sym);

    IoDeleteDevice(DriverObject->DeviceObject);

    // Done

    DbgPrint("[+] Leaving.\n");

}

// DriverEntry

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS stat;

    PDEVICE_OBJECT DeviceObject;

    // Major Functions / Dispatch routines

    DriverObject->MajorFunction[IRP_MJ_CREATE] = IRPCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = IRPClose;

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceCTRL;

    DriverObject->DriverUnload = Unload;
    
    // Start

    DbgPrint("[+] H00.\n");

    // Get current win version

    RTL_OSVERSIONINFOW osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(osInfo);

    stat = RtlGetVersion(&osInfo);

    if (!NT_SUCCESS(stat)) {

        DbgPrint("[+] Can't get version info.\n");

        return STATUS_NOT_SUPPORTED;

    }

    DbgPrint("[+] Version: %lu.%lu.%lu\n", osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.dwBuildNumber);

    // Sub to Routines

    // Create Device

    stat = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);

    if (!NT_SUCCESS(stat)) {

        DbgPrint("[+] Can't create device.\n");

        return stat;
    }

    // Create Symlink

    stat = IoCreateSymbolicLink(&Sym, &DeviceName);

    if (!NT_SUCCESS(stat)) {

        DbgPrint("[+] Can't create symlink.\n");

        return stat;
    }

    // If DriverEntry doesn't return STATUS_SUCCESS, user mode process that called StartService will receive an error even the code in the driver is executed. 

    return STATUS_SUCCESS;
}