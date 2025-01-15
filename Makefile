TARGET = result/BOOTX64.EFI result/kernel

clean:
	rm tmp/*.so tmp/*.o $(TARGET)

bootloader:
	make -f makefile.bootloader

kernel:
	make -f makefile.kernel