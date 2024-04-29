#include "isr.h"
#include "idt.h"
#include "../drivers/video.h"
#include "../drivers/ports.h"

isr_t interrupt_handlers[256];

/* ISR - Rotina de serviço de interrupção.
 * Quando uma interrupção ocorre, o processador suspende a execução normal
 * do programa e transfere o controle para uma ISR específica para lidar com
 * a interrupção.
*/

void isr_install() {
    set_idt_gate(0, (uint32_t) isr0);
    set_idt_gate(1, (uint32_t) isr1);
    set_idt_gate(2, (uint32_t) isr2);
    set_idt_gate(3, (uint32_t) isr3);
    set_idt_gate(4, (uint32_t) isr4);
    set_idt_gate(5, (uint32_t) isr5);
    set_idt_gate(6, (uint32_t) isr6);
    set_idt_gate(7, (uint32_t) isr7);
    set_idt_gate(8, (uint32_t) isr8);
    set_idt_gate(9, (uint32_t) isr9);
    set_idt_gate(10, (uint32_t) isr10);
    set_idt_gate(11, (uint32_t) isr11);
    set_idt_gate(12, (uint32_t) isr12);
    set_idt_gate(13, (uint32_t) isr13);
    set_idt_gate(14, (uint32_t) isr14);
    set_idt_gate(15, (uint32_t) isr15);
    set_idt_gate(16, (uint32_t) isr16);
    set_idt_gate(17, (uint32_t) isr17);
    set_idt_gate(18, (uint32_t) isr18);
    set_idt_gate(19, (uint32_t) isr19);
    set_idt_gate(20, (uint32_t) isr20);
    set_idt_gate(21, (uint32_t) isr21);
    set_idt_gate(22, (uint32_t) isr22);
    set_idt_gate(23, (uint32_t) isr23);
    set_idt_gate(24, (uint32_t) isr24);
    set_idt_gate(25, (uint32_t) isr25);
    set_idt_gate(26, (uint32_t) isr26);
    set_idt_gate(27, (uint32_t) isr27);
    set_idt_gate(28, (uint32_t) isr28);
    set_idt_gate(29, (uint32_t) isr29);
    set_idt_gate(30, (uint32_t) isr30);
    set_idt_gate(31, (uint32_t) isr31);

    /**IRQ - Interrupt Request
     * É um sinal de hardware enviado ao processador que interrompe temporariamente 
     * um programa em execução e permite que um programa especial (handler)
     * seja executado.
    */

    /** Remapear o PIC - Controlador de interrupção programável
     * A programação ocorre através do envio de quatro palavras de comando
     * de inicialização (ICWs)
     * 
     * ICW1 - Envio do comando de inicialização. 
     * ICW2 - Deslocamento de IDT - Especifica o vetor de interrupção inicial
     * ICW3 - Fiação entre PICs - Configura a conexão entre o principal e o secundário
     * ICW4 - Modo - Modo de operação dos PICs
    */
    port_byte_out(0x20, 0x11);  // 0x11 indica que estamos configurando os PICs em modo de inicialização e que será usado o modo cascata.
    port_byte_out(0xA0, 0x11);

    port_byte_out(0x21, 0x20);  // 0x20 define o vetor inicial do PIC principal como 0x20
    port_byte_out(0xA1, 0x28);  // 0x28 define o vetor inicial do PIC secundário como 0x28

    port_byte_out(0x21, 0x04);  // 0x04 indica que o PIC principal está conectado ao pino IRQ2
    port_byte_out(0xA1, 0x02);  // 0x02 indica que o PIC secundário está conectado ao pino IRQ2

    port_byte_out(0x21, 0x01);  // 0x01 define o modo de operação em modo 8086/8088 que é o padrão de sistemas x86
    port_byte_out(0xA1, 0x01);  // 0x01 define o modo de operação em modo 8086/8088 que é o padrão de sistemas x86

    port_byte_out(0x21, 0x0);   // Desabilita todas as interrupções nos PICs durante a inicialização.
    port_byte_out(0xA1, 0x0);

    /** Inicializa as IRQs - Solicitações de requisição
     * 2 PICs
     * Os IRQ's estão definidos no interrupt.asm, são referências para funções
     * que serão executadas quando ocorrerem requisições de interrupção (IRQ).
    */
    set_idt_gate(32, (uint32_t) irq0);
    set_idt_gate(33, (uint32_t) irq1);
    set_idt_gate(34, (uint32_t) irq2);
    set_idt_gate(35, (uint32_t) irq3);
    set_idt_gate(36, (uint32_t) irq4);
    set_idt_gate(37, (uint32_t) irq5);
    set_idt_gate(38, (uint32_t) irq6);
    set_idt_gate(39, (uint32_t) irq7);
    set_idt_gate(40, (uint32_t) irq8);
    set_idt_gate(41, (uint32_t) irq9);
    set_idt_gate(42, (uint32_t) irq10);
    set_idt_gate(43, (uint32_t) irq11);
    set_idt_gate(44, (uint32_t) irq12);
    set_idt_gate(45, (uint32_t) irq13);
    set_idt_gate(46, (uint32_t) irq14);
    set_idt_gate(47, (uint32_t) irq15);

    load_idt();
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t *r) {
    /** Função manipuladora genérica responsável por extrair todas as 
     * informações necessárias a interrupção
     * 
    */
    print_string("received interrupt: ");
    print_string(exception_messages[r->int_no]);
    print_nl();
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}


void irq_handler(registers_t *r) {
    if (interrupt_handlers[r->int_no] != 0) {
        // Obtém a função associada do vertor
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }

    /** Informa ao PIC o comando de fim de interrupçao (EOI).
     * É necessário para que o PIC saiba que a interrupção foi tratada
     * e pode enviar mais interrupções.
    */
    port_byte_out(0x20, 0x20);      // EOI primário
    if (r->int_no < 40) {
        port_byte_out(0xA0, 0x20);  // EOI secundário
    }
}