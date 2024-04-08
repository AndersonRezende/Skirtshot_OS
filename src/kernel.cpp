// Extern necessário para corrigir o problema do name mangling, onde o compilador pode alterar o nome para propósitos próprios.
extern "C" void main() {
	/*
		Configuração de cor, texto e endereço de memória de vídeo VGA para ser modificada de acordo com os valores em hello
	*/
	const short color = 0x0F00;
	const char* hello = "Hello cpp world!";
	short* vga = (short*)0xb8000;
	for (int i = 0; i < 16; i++) {
		vga[i + 80] = color | hello[i];
	}
}