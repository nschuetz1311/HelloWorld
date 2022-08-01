//
// Basic UEFI Libraries
//
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ShellLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

#include <Library/NetLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol\DevicePathToText.h>
#include <Protocol\UnicodeCollation.h>

#define EFI_GUID_TEST                                                          \
  {                                                                            \
    0xb13df96d, 0xa1bc, 0x44a0, {                                              \
      0xa6, 0x50, 0xa9, 0x4d, 0xe8, 0xc5, 0xc2, 0x60                           \
    }                                                                          \
  }

//
// These things were added to support the modified EntryPoint ShellAppMain
//
#include <Protocol\ShellParameters.h>

INTN EFIAPI ShellAppMain(IN UINTN Argc, IN CHAR16 **Argv);