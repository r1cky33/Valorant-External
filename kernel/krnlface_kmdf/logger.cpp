#include "stdafx.h"

#include <ntstrsafe.h>

namespace logger {
#define BUFFER_SIZE 30

    void errlog(char* error) {
        ImpDef(RtlInitUnicodeString);
        ImpDef(KeGetCurrentIrql);
        ImpDef(ZwCreateFile);
        ImpDef(RtlStringCbPrintfA);
        ImpDef(RtlStringCbLengthA);
        ImpDef(ZwWriteFile);
        ImpDef(ZwClose);

        ImpSet(RtlInitUnicodeString);
        ImpSet(KeGetCurrentIrql);
        ImpSet(ZwCreateFile);
        ImpSet(RtlStringCbPrintfA);
        ImpSet(RtlStringCbLengthA);
        ImpSet(ZwWriteFile);
        ImpSet(ZwClose);

        UNICODE_STRING     uniName;
        OBJECT_ATTRIBUTES  objAttr;

        ImpCall(RtlInitUnicodeString, &uniName, L"\\SystemRoot\\krnl_log.txt");
        InitializeObjectAttributes(&objAttr, &uniName,
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL, NULL);

        HANDLE   handle;
        NTSTATUS ntstatus;
        IO_STATUS_BLOCK    ioStatusBlock;

        if (ImpCall(KeGetCurrentIrql) != PASSIVE_LEVEL) return;

        ntstatus = ImpCall(ZwCreateFile, &handle,
            GENERIC_WRITE,
            &objAttr, &ioStatusBlock, NULL,
            FILE_ATTRIBUTE_NORMAL,
            0,
            FILE_OPEN_IF,
            FILE_SYNCHRONOUS_IO_NONALERT,
            NULL, 0);

        CHAR     buffer[BUFFER_SIZE];
        size_t  cb;

        if (NT_SUCCESS(ntstatus)) {
            ntstatus = ImpCall(RtlStringCbPrintfA, buffer, sizeof(buffer), error, 0x0);
            if (NT_SUCCESS(ntstatus)) {
                ntstatus = ImpCall(RtlStringCbLengthA, buffer, sizeof(buffer), &cb);
                if (NT_SUCCESS(ntstatus)) {
                    ntstatus = ImpCall(ZwWriteFile, handle, NULL, NULL, NULL, &ioStatusBlock,
                        buffer, (ULONG)cb, NULL, NULL);
                }
            }
            ImpCall(ZwClose, handle);
        }
    }
}