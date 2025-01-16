#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>

void ShowNotification(const std::string& message, bool isError) 
{
    if (isError) 
    {
        std::cerr << "Error: " << message << std::endl;
    }
    else
    {
        std::cout << "Success: " << message << std::endl;
    }
}

bool CheckRegistryKeyExists(HKEY hKeyRoot, const std::string& subKey)
{
    HKEY hKey;
    LONG result = RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_READ, &hKey);

    if (result == ERROR_SUCCESS) 
    {
        RegCloseKey(hKey);
        return true;
    }
    else 
    {
        ShowNotification("Failed to open registry key: " + subKey, true);
        return false;
    }
}

int main() {
    std::vector<std::string> registryEntries = 
    {
        "HARDWARE\\DEVICEMAP\\Scsi",
        "HARDWARE\\DESCRIPTION\\System\\MultifunctionAdapter\\0\\DiskController\\0\\DiskPeripheral",
        "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName",
        "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName",
        "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
        "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces",
        "SYSTEM\\CurrentControlSet\\Control\\IDConfigDB\\Hardware Profiles\\0001",
        "SOFTWARE\\Microsoft\\Cryptography",
        "SOFTWARE\\Microsoft\\SQMClient",
        "SYSTEM\\CurrentControlSet\\Control\\SystemInformation",
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\WindowsUpdate",
        "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}",
        "SYSTEM\\CurrentControlSet\\Control\\Nsi\\{eb004a03-9b1a-11d4-9123-0050047759bc}\\26",
        "HARDWARE\\DESCRIPTION\\System\\BIOS"
        // ADD MORE
    };

    std::vector<std::string> missingEntries;
    for (const auto& entry : registryEntries)
    {
        if (!CheckRegistryKeyExists(HKEY_LOCAL_MACHINE, entry)) 
        {
            missingEntries.push_back(entry);
        }
    }

    if (!missingEntries.empty()) 
    {
        std::ostringstream errorMessage;
        errorMessage << "Registry entries not found:\n";
        for (const auto& entry : missingEntries)
        {
            errorMessage << entry << "\n";
        }
        ShowNotification(errorMessage.str(), true);
    }
    else 
    {
        ShowNotification("All registry entries exist.", false);
    }

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}