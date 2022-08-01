//----------------------------------------------------------------------------
//   Copyright (C) 2022-2022 N.Schuetz.
//   This program contains proprietary and confidential information.
//   All rights reserved, except as may be permitted by prior written consent.
//----------------------------------------------------------------------------
#include "..\Header\Header.h"

//
// We run on any UEFI Specification
//
extern CONST UINT32 _gUefiDriverRevision = 0;

//
// Our name
//
CHAR8 *gEfiCallerBaseName = "HelloWorld";

EFI_STATUS EFIAPI UefiUnload(IN EFI_HANDLE ImageHandle) {
  //
  // This code should be compiled out and never called
  //
  ASSERT(FALSE);
  return 0;
}

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle,
                           IN EFI_SYSTEM_TABLE *SystemTable) {
  INTN ReturnFromMain = -1;
  EFI_SHELL_PARAMETERS_PROTOCOL *EfiShellParametersProtocol = NULL;
  EFI_STATUS Status;

  Status = SystemTable->BootServices->OpenProtocol(
      ImageHandle, &gEfiShellParametersProtocolGuid,
      (VOID **)&EfiShellParametersProtocol, ImageHandle, NULL,
      EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR(Status))
    return Status;

  // Initializes the shell( to be able to use ShellExecute properly)
  ShellInitialize();

  //
  // use shell 2.0 interface
  //
  ReturnFromMain = ShellAppMain(EfiShellParametersProtocol->Argc,
                                EfiShellParametersProtocol->Argv);

  return Status;
}
