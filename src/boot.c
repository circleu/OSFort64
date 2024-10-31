#include <efi.h>
#include <efilib.h>


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
  InitializeLib(ImageHandle, SystemTable);

  Print(L"Hello, World!\n\r");

  return EFI_SUCCESS;
}