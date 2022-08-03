//----------------------------------------------------------------------------
//   Copyright (C) 2022-2022 N.Schuetz.
//   This program contains proprietary and confidential information.
//   All rights reserved, except as may be permitted by prior written consent.
//----------------------------------------------------------------------------
#include "..\Header\Header.h"

/**
 * UEFI application entry point which has an interface similar to a
 * standard C main function.
 *
 * The ShellCEntryLib library instance wrappers the actual UEFI application
 * entry point and calls this ShellAppMain function.
 *
 * @param[in] Argc	The number of items in Argv.
 * @param[in] Argv	Array of pointers to strings.
 *
 * @retval  0		The application exited normally.
 * @retval  Other	An error occurred.
 *
 */
INTN EFIAPI ShellAppMain(IN UINTN Argc, IN CHAR16 **Argv) {
	EFI_STATUS Status = 0;

	return Status;
}