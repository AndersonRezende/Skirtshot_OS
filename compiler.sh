#!/bin/bash

function clean_or_create_files() {
	# Limpa os arquivos binário, img e iso
	if [ -d iso_temp ]; then
		rm -rf iso_temp/*
	else
		mkdir iso_temp
	fi
	rm output/bin/$1.bin $1.iso $1.img > /dev/null 2>&1
}

function compile() {
	# Compila o arquivo e gera o binário
	nasm -f bin -o output/bin/$1.bin src/$1.asm > /dev/null 2>&1
	# Cria um disco floppy inicialmente zerado com o equivalente a 1.44MB
	dd if=/dev/zero of=$1.img bs=1024 count=1440
	# Coloca o binário dentro do primeiro setor da imagem floppy criada, com o tamanho padrão de 512 bytes (padrão do MBR) e especifica o primeiro setor do disco.
	dd if=output/bin/$1.bin of=$1.img seek=0 count=1 conv=notrunc
	# Move a imagem para dentro da pasta auxiliar e gera uma iso bootável 
	mv $1.img iso_temp/
	genisoimage -quiet -V 'Skirtshot OS' -input-charset iso8859-1 -o output/$1.iso -b $1.img -hide $1.img iso_temp/
}

function run() {
	# Executa a iso no emulador qemu
	qemu-system-x86_64 -cdrom output/$1.iso
}


if [ $# -eq 0 ]; then
	echo "Nenhum parâmetro informando o nome do arquivo!"
	exit
fi

clean_or_create_files
compile $1
run $1
