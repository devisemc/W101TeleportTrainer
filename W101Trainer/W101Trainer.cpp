#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"


int main()
{
    /*WIZARD 101 COORD SETUP
    * y hex = y
    * y hex - 4 = z
    * y hex - 8 = x
    */
    //Get Process ID of Wizard101
    DWORD procId = GetProcId(L"WizardGraphicalClient.exe");

    //Get the Module's Base Address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"WizardGraphicalClient.exe");

    //Get a Handle to the Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    //Resolve the Base Address of Pointer Chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x03039A48;

    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve the Y Value Pointer Chain and Calculate other Axis Addresses off it
    std::vector<unsigned int> yOffsets = { 0x8 };
    uintptr_t yAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, yOffsets);
    uintptr_t zAddr = yAddr - 4;
    uintptr_t xAddr = zAddr - 4;
    std::cout << "yAddr = " << "0x" << std::hex << yAddr << std::endl;
    std::cout << "zAddr = " << "0x" << std::hex << zAddr << std::endl;
    std::cout << "xAddr = " << "0x" << std::hex << xAddr << std::endl;
    
    //Print Y Value
    float yValue = 0;
    float zValue = 0;
    float xValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)yAddr, &yValue, sizeof(yValue), nullptr);
    std::cout << "Current Y = " << std::dec << yValue << std::endl;
    ReadProcessMemory(hProcess, (BYTE*)zAddr, &zValue, sizeof(zValue), nullptr);
    std::cout << "Current Z = " << std::dec << zValue << std::endl;
    ReadProcessMemory(hProcess, (BYTE*)xAddr, &xValue, sizeof(xValue), nullptr);
    std::cout << "Current X = " << std::dec << xValue << std::endl;
    
    //Write to Coord Addresses (Example: dock in commons is x=3259.85, z=-912.75, y=15)
    float newY = 0;
    float newZ = 0;
    float newX = 0;
    std::cout << "X: ";
    std::cin >> newX;
    std::cout << "Z: ";
    std::cin >> newZ;
    std::cout << "Y: ";
    std::cin >> newY;

    WriteProcessMemory(hProcess, (BYTE*)yAddr, &newY, sizeof(newY), nullptr);
    WriteProcessMemory(hProcess, (BYTE*)zAddr, &newZ, sizeof(newZ), nullptr);
    WriteProcessMemory(hProcess, (BYTE*)xAddr, &newX, sizeof(newX), nullptr);
    //Read out new Coord Values
    ReadProcessMemory(hProcess, (BYTE*)yAddr, &yValue, sizeof(yValue), nullptr);
    std::cout << "New Y = " << std::dec << yValue << std::endl;
    
    getchar();
    return 0;
}
