#!/bin/bash

function clean_files() {
	FILES_TO_REMOVE=$(find . -name "*.o" -o -name "*.bin" -o -name "*.img" -o -name "*.iso")
	for file in $FILES_TO_REMOVE; do
		rm $file
	done
	if [ -d iso_temp ]; then
		rm -rf iso_temp/*
	else
		mkdir iso_temp
	fi
	rm output/bin/* > /dev/null 2>&1
}

function compile() {
	export PATH=$PATH:/usr/local/i386elfgcc/bin
	
	# Compila os arquivos .asm
	nasm "src/boot/bootloader.asm" -f bin -o "output/bin/bootloader.bin"
	nasm "src/boot/kernel_entry.asm" -f elf -o "output/bin/kernel_entry.o"
	
	# Compila os arquivos .c
	C_FILES=$(find src -name "*.c")

	for file in $C_FILES; do
		name=$(find src -type f -wholename $file -execdir basename {} .c \;); 
		echo $name
		i386-elf-gcc -ffreestanding -m32 -g -c $file -o "output/bin/$name.o"
	done
	#i386-elf-gcc -ffreestanding -m32 -g -c "src/drivers/ports.c" -o "output/bin/ports.o"
	#i386-elf-gcc -ffreestanding -m32 -g -c "src/kernel.c" -o "output/bin/kernel.o"
	
	# Linka os arquivos gerados
	i386-elf-ld -o "output/bin/full_kernel.bin" -Ttext 0x1000 "output/bin/kernel_entry.o" "output/bin/kernel.o" "output/bin/ports.o" "output/bin/video.o" --oformat binary
	
	# Une o arquivo do bootloader com o do kernel
	cat "output/bin/bootloader.bin" "output/bin/full_kernel.bin" > "output/bin/OS.bin"

}

function create_iso() {
	# Cria um disco floppy inicialmente zerado com o equivalente a 1.44MB
	dd if=/dev/zero of=OS.img bs=1024 count=1440
	# Coloca o binário dentro do primeiro setor da imagem floppy criada, com o tamanho padrão de 512 bytes (padrão do MBR) e especifica o primeiro setor do disco.
	dd if=output/bin/OS.bin of=OS.img seek=0 count=3 conv=notrunc
	# Move a imagem para dentro da pasta auxiliar e gera uma iso bootável 
	mv OS.img iso_temp/
	genisoimage -quiet -V 'Skirtshot OS' -input-charset iso8859-1 -o output/OS.iso -b OS.img -hide OS.img iso_temp/
}

function run() {
	# Executa a iso no emulador qemu
	qemu-system-x86_64 -cdrom output/OS.iso
	#qemu-system-x86_64 output/bin/$1.bin
	
	#qemu-system-x86_64 -drive format=raw,file="output/bin/$1.bin",index=0,if=floppy,  -m 128M
	#qemu-system-x86_64 -drive format=raw,file="output/bin/OS.bin",index=0,if=floppy,  -m 128M
}

clean_files
compile
create_iso
run
