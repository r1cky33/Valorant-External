#include "stdafx.h"

void branding();

HANDLE hConsole = nullptr;

// lib entrypoint
#pragma comment(lib, "pom_valo.lib")
extern int valo_main();

void print_message(const char* msg, int col) {
    SetConsoleTextAttribute(hConsole, col);
    printf(msg);
    SetConsoleTextAttribute(hConsole, 15);

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

int main()
{
    antidbg::init();

    if (!utils::IsProcessElevated()) {
        msg_box(_xor_("Run application as Administrator!").c_str(), _xor_("Error").c_str());
        Sleep(7000);
        return -1;
    }

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	branding();

    if (utils::query_system_buildnumber() < (ULONG)19041) {
        msg_box(_xor_("Windows version is not supported!").c_str(), _xor_("Error").c_str());
        return -1;
    }

    if (utils::get_proc_id_by_name(L"BEService.exe")) {
        msg_box(_xor_("AntiCheat is loaded!").c_str(), _xor_("Error").c_str());
        return -1;
    }

    if (!service::install()) {
        return -1;
    }

    valo_main();

    /*switch (auth::authenticate()) {
    case auth::auth_state::valid:
        if (!service::install()) {
            return -1;
        }

        valo_main();
        break;

    case auth::auth_state::banned:
        print_message(_xor_("\n ... access denied!\n").c_str(), 12);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return -1;
        break;

    case auth::auth_state::expired:
        print_message(_xor_("\n ... subscription expired!\n").c_str(), 12);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return -1;
        break;

    case auth::auth_state::invalid_request:
        print_message(_xor_("\n ... invalid request!\n").c_str(), 12);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return -1;
        break;

    case auth::auth_state::unknown:
        print_message(_xor_("\n ... unknown error occured!\n").c_str(), 12);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return -1;
        break;

    default:
        return -1;
    }*/
    return -1;
}

void branding() {
	SetConsoleTextAttribute(hConsole, 12);
}
