
void port_byte_out(unsigned short port, unsigned char data) {
    /* 
    * Copia o valor do segundo operando (al -> data) para a porta de E/S
    * especificada no operador de destino (dx -> port).
    */
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));   // out dx, al
}

unsigned char port_byte_in(unsigned short port) {
    unsigned char result;

    /* Chamada para instrução em assembly onde:
    *  Especificador de saída: "=a" (result)  
    *  Especificador de entrada: "d" 
    *  A ordem dos operadores de origem e destino são invertidas
    */
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
    unsigned short result;

    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}