#include "idt.h"

#define low_16(address) (uint16_t) ((address) & 0xffff)
#define high_16(address) (uint16_t) (((address) >> 16) & 0xffff)

/* IDT - Tabela descritora de interrupção.
 * É uma estrutura de dados importante em sistemas x86 que mapeia vetores
 * de interrupção para rotinas de tratamento de interrupção. Quando uma 
 * interrupção ocorre, o SO precisa saber para onde direcionar o fluxo de controle
 * para lidar com a interrupção.
 * Cada entrada no IDT contém informações sobre como lidar com um tipo específico
 * de interrupção ou exceção. Isso inclui o Endereço da rotina de tratamento de 
 * interrupção associada, que seria como uma espécie de método associado e o 
 * Seletor de segmento para o código de tratamento de interrupção, que seria 
 * como uma espécie de indicador de segmento de código pertece a rotina.
 * O IDT é usado para determinar a resposta correta a interrupções e exceções.
 * IDT pode ser acionado por interrupçoẽs de hardware, software e exceções do 
 * processador, juntas possuem 256 vetores de interrupçṍes.
*/

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler) {
    /**
     * Função para registrar um manipulador (handler) para a interrupção n.
    */
    idt[n].low_offset = low_16(handler);
    idt[n].selector = 0x08;      // Indica que o código associado ao tratamento da interrupção está localizado no segmento de cogio do kernel.
    idt[n].always0 = 0;
    idt[n].flags = 0x8e;         // 0x8e = 1 00 0 1 110 -> P DPL 0 D Type
    idt[n].high_offset = high_16(handler);
}

void load_idt() {
    /** Pega o endereço base, que é a referência de onde está o idt e
     * calcula o limite baseado na quantidade de entradas e o tamanho da
     * estrutura.
     * Executa a instrução a nível 
     * Volatile indica que o código em montagem pode modificar ou ser modificado
     * por variáveis fora do seu controle, garantindo que o compilador não
     * otimize o código e não interfira no processo.
    */
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_reg));
}