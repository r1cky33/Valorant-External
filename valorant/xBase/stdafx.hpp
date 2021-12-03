#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdint>
#include <map>
#include <tchar.h>
#include <ShObjIdl.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <list>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "core.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "xorstr.hpp"

// engine
#include "definitions.hpp"
#include "vector3.hpp"
#include "engine.hpp"

#include "magic.hpp"
#include "decryptor.hpp"

#include "srcns.hpp"

// renderer
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

// dcomp and dx
#include <wrl.h>
#include <dxgi.h>
#include <d2d1.h>
#include <d3d11.h>
#include <d2d1_2.h>
#include <dcomp.h>
#include <dwmapi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dcomp.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

#include "dx11_comp.hpp"
