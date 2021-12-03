#pragma once

#include "logger.h"

#define kprintf(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, __VA_ARGS__)
#define errlog(str) logger::errlog(str)

#define POOLTAG 'lieH'

#include "stdlib.h"
using namespace std;

#ifndef _COMMON_TYPES
#define _COMMON_TYPES
typedef unsigned short  WORD;
typedef unsigned char   BYTE;
typedef unsigned long   DWORD;

#define near
typedef BYTE near* PBYTE;
typedef int near* PINT;

typedef unsigned char byte;
#endif

#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>

#include <ntimage.h>
#include <intrin.h>

#include "defs.h"
#include "imports.h"

#include "globals.h"
#include "utils.h"
#include "krnlutils.h"
#include "core.h"

#include "xor.h"
#include "no_imp.h"