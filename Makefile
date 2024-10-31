CC = gcc
CFLAGS = -Ignu-efi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args
LFLAGS = -shared -Bsymbolic -Lgnu-efi/x86_64/lib -Lgnu-efi/x86_64/gnuefi -Tgnu-efi/gnuefi/elf_x86_64_efi.lds gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o
OFLAGS = -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10


result/BOOTX64.EFI : tmp/boot.so
	objcopy $(OFLAGS) $^ $@

tmp/boot.so : tmp/boot.o
	ld $(LFLAGS) $< -o $@ -lgnuefi -lefi

tmp/boot.o : src/boot.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm tmp/*.o tmp/*.so result/*.EFI