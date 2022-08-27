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

EFI_GUID VendorGuid = EFI_GUID_TEST;

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

VOID PrintdecisionW (
	VOID
)
{
  Print (L"You now can enter your desired reboot delay time \n");
  Print (L"But please keep in mind to choose a value smaller than 60s. \n");
  Print (L"Your submission can be entered as a Hex or decimal number\n");
}

/**
 * @brief Init Routine
 *
 * This routine initializes the protocols used in this application.
 *
 * @return EFI_STATUS
 */
EFI_STATUS EFIAPI InitRoutine (VOID) {
	EFI_STATUS	Status = EFI_SUCCESS;

	Status |= gBS->LocateProtocol (&gEfiDevicePathToTextProtocolGuid,
					NULL, (VOID **)&pDevicePathToText);

	Status |= gBS->LocateProtocol (&gEfiUnicodeCollation2ProtocolGuid,
					NULL,(VOID **)&pUnicodeCollation);

	Status |= gBS->LocateProtocol (&gEfiSimpleTextInProtocolGuid,
					NULL, (VOID **)&pTextInput);

	return Status;

}

CHAR16 SingleKeyCheck (
	VOID
)
{
	EFI_INPUT_KEY Key;
	UINTN EventIndex;
	CHAR16 String;
	gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex);
	gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
	String = Key.UnicodeChar;
	return (String);
} // SingleKeyCheck

/**
 * @brief		reAllocatepool
 *
 *			This function reallocates the pool for
 *			previously allocated variable by its own size,
 *			so it doubles the original size and returns
 *			the newly generated array. The input array is freed
 *			after the data was transcribed.
 *
 * @param PoolType	The type of pool to allocate.
 *			MemoryType values in the range 0x70000000..0x7FFFFFFF
 *			are reserved for OEM use. MemoryType values in the range
 *			0x80000000..0xFFFFFFFF are reserved for use
 *			by UEFI OS loaders that are provided
 *			by operating system vendors.
 * @param Size		The number of bytes to allocate from the pool
 *			and returns the number of bytes allocated
 *			to the new pool.
 * @param Array		A pointer to a pointer to the allocated buffer
 * 			if the call succeeds undefined otherwise.
 * @return EFI_STATUS
 */
EFI_STATUS EFIAPI reAllocatepool (
	IN EFI_MEMORY_TYPE PoolType,
	IN OUT UINTN       *Size,
	IN OUT VOID        **Array
)
{
	EFI_STATUS	Status = EFI_SUCCESS;
	CHAR16		*Temp;

	*Size = *Size * 2;
	Status = gBS->AllocatePool (PoolType, (*Size * sizeof(CHAR16)),
							(VOID **)&Temp);
	gBS->CopyMem (Temp, (VOID **)Array, *Size / 2);
	gBS->FreePool (Array);
	Array = &Temp;

	return (Status);
} // reAllocatepool

/**
 * @brief
 *
 * @param Exclusion
 * @param String
 * @param Counter
 * @return BOOLEAN
 */
BOOLEAN ReadKeyBoard (
	IN CHAR16 *Exclusion,
	OUT CHAR16 **String,
	IN UINTN Counter
)
{
	EFI_INPUT_KEY	Key;
	UINTN		Size = 10;
	UINTN		EventIndex;

	gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
	gBS->AllocatePool (EfiBootServicesData, Size, (VOID **)String);
	while ((Key.UnicodeChar != CHAR_CARRIAGE_RETURN) &&
						(Key.ScanCode != SCAN_ESC)) {

		gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex);
		gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

		if (((Exclusion != NULL) && (Key.UnicodeChar != *Exclusion) ) ||
							(Exclusion == NULL)) {
			*(*String + Counter) = Key.UnicodeChar;
			Print (L"%c", *(*String + Counter));

			if ((Key.UnicodeChar == 0x08) && (Counter >= 0)) {
				Print (L" \b");
				Counter--;
			} else if (Counter >= 0) {
				Counter++;
			}

			if (Counter >= Size) {
				reAllocatepool (EfiBootServicesData, &Size,
							(VOID **)String);
			}
		}
	}

	if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
		*(*String + (Counter - 1)) = L'\0';
		return (TRUE);
	} else {
		Print (L"\n");
		gBS->FreePool ((VOID *)String);
		return (FALSE);
	}
} // ReadKeyBoard

INTN EFIAPI wcsicmp(
	IN CHAR16 *Str1,
	IN CHAR16 *Str2
)
{
	return (pUnicodeCollation->StriColl(pUnicodeCollation, Str1, Str2));
}


/**
 * UEFI application entry point which has an interface similar to a
 * standard C main function.
 *
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
	CHAR16		*mInput;
	CHAR16		*TextInput;
	CHAR16		*Selection;
	UINT64		Time;

	Status = InitRoutine();

	if (EFI_ERROR (Status)) {
		Print (L"Error: Locating Protocols failed %r \n" ,Status);
		return Status;
	}

	gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

	while (Key.ScanCode != SCAN_ESC) {
		PrintVersion ();

		if ( ReadKeyBoard (NULL, &mInput, 0) ) {
			Print (L"Input: %s \n", mInput);
		} else {
			continue;
		}

		if (wcsicmp(L"-wait", mInput) == 0) {
			UINTN RebootCounter;
			UINTN dataSize;

			PrintdecisionW ();
			dataSize = sizeof(dataSize);
			TextInput = L"counter";
			gRT->GetVariable (TextInput, &VendorGuid, &Atts,
						&dataSize, &RebootCounter);
			Print (L"RebootNr: %d\n", RebootCounter);
			Key.UnicodeChar = 0;

			if ( !ReadKeyBoard (NULL, &Selection, 0) ) {
				break;
			}

			Print (L"Input: %s \n", Selection);
			if ((ShellIsHexOrDecimalNumber(Selection, 0, 1)) == 0) {
				break;
			}

			ShellConvertStringToUint64 (Selection, &Time, 0, 1);
			Print (L"Time chosen: %d s \n", Time);

			if (ShellIsDecimalDigitCharacter(*Selection)) {
				Print (L"Input is decimal \n");
			}
			else if (ShellIsHexaDecimalDigitCharacter(*Selection)) {
				Print (L"Input is hex \n");
			}

			if ( Time > 60 ) {
				Print (L"Please enter a value smaller\
							than 60. \n", Time);
				break;
			}

			Print (L"The System will restart in %d \
				seconds.\nDo you wish to continue?\n", Time);
			if (SingleKeyCheck() != 'y') {
				Print (L"You aborted the restart. \n");
				break;
			}
			while ((Key.ScanCode != SCAN_ESC) && (Time > 0)) {
				Print (L" Time remaining: %d s \r", Time);
				gBS->Stall (1000000);
				Time--;
				gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
			}
			if (Key.ScanCode != SCAN_ESC) {
				Print (L"System restarts now \n");
				gBS->Stall (500000);
				RebootCounter++;
				gRT->SetVariable (TextInput, &VendorGuid,
						Atts, dataSize, &RebootCounter);
				gRT->ResetSystem (EfiResetCold, EFI_SUCCESS,
								0, NULL);
			}
			else {
				Print (L"Restart was aborted. \n");
				break;
			}
		}

		break;
	}

	return Status;
}