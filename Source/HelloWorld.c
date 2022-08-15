//----------------------------------------------------------------------------
//   Copyright (C) 2022-2022 N.Schuetz.
//   This program contains proprietary and confidential information.
//   All rights reserved, except as may be permitted by prior written consent.
//----------------------------------------------------------------------------
#include "..\Header\Header.h"

static EFI_DEVICE_PATH_TO_TEXT_PROTOCOL	*pDevicePathToText;
static EFI_UNICODE_COLLATION_PROTOCOL	*pUnicodeCollation;
static EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*pTextInput;

UINT32 Atts = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS
					| EFI_VARIABLE_BOOTSERVICE_ACCESS;

STATIC VOID PrintVersion (
	VOID
)
{
	Print (L"Please choose your program now.\n");
	Print (L"You can either decide to reboot the system,\n");
	Print (L"or you can open/read/create a file,\n");
	Print (L"change the BootOrder by entering move.\n");
	Print (L"Get the date and time if you enter watch.\n");
	Print (L"Read the Description of the BootXXXX by\
			choosing the boot option.\n");
	Print (L"to reboot please enter -wait, to modify a txt\
			file -text and so on...\n");
}

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
	EFI_STATUS	Status = EFI_SUCCESS;
	EFI_INPUT_KEY	Key;

	Status |= gBS->LocateProtocol (&gEfiDevicePathToTextProtocolGuid,
					NULL, (VOID **)&pDevicePathToText);

	Status |= gBS->LocateProtocol (&gEfiUnicodeCollation2ProtocolGuid,
					NULL,(VOID **)&pUnicodeCollation);

	Status |= gBS->LocateProtocol (&gEfiSimpleTextInProtocolGuid,
					NULL, (VOID **)&pTextInput);

	if (EFI_ERROR (Status)) {
		Print (L"Error: Locating Protocols failed %r \n" ,Status);
		return Status;
	}

	gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

	while (Key.ScanCode != SCAN_ESC) {
		PrintVersion ();
		break;
	}

	return Status;
}