#pragma once

#include <stdint.h>

#define IDT_ENTRIES 256

typedef struct{
    /**
     * low_offset = 16 bits baixos do endereço da função handler
     * selector   = Seletor de segmento de código do kernel
     * always0    = 0
     * Flags -> Tipo, D, DPL, P e 0
     * b7 Interrupção está presente                                   | P (1b)
     * b6 - b5 Nível de privilégio (0 = kernel .. 3 = user)           | DPL (2b)
     * b4 Configura em 0 para interrupções de portas                  | 0
     * b3 - b0 bits 1110 = decimal 14 = "32 bit porta de interrupção" | Type
    */
    uint16_t low_offset;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;
/* __attribute__((packed)) serve para instruir o compilador a não fazer 
 * o alinhamento de bytes na estrutura. Os compiladores podem fazer alinhamento
 * para melhorar o desempenho de leitura e gravação de dados na memória.
*/

typedef struct {
    /**
     * Um ponteiro para o array de manipuladores de interrupções.
    */
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;


void set_idt_gate(int n, uint32_t handler);

void load_idt();