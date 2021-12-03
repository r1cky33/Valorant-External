#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1

#include <iostream>
#include <Windows.h>
#include <intrin.h>
#include <stdint.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <thread>
#include <filesystem>

#include <cassert>
#include <tchar.h>

#include <fstream>
#include <strsafe.h>

#include "xor.hpp"
#include "utils.h"
#include "crypt.h"
#include "service.h"
#include "antidbg.h"
#include "auth.h"

#include "mem_protection.h"

#define msg_box(str, title) MessageBoxA(nullptr, str, title, 0)


