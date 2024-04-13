# Skirtshot_OS
Projeto de sistema operacional.

## Instalação
A instalação consiste em clonar e instalar algumas ferramentas para fazer a compilação cruzada.
```sh
$ git clone https://github.com/AndersonRezende/Skirtshot_OS.git
$ cd Skirtshot_OS/setup
$ ./setup-gcc-debian.sh
```

## Execução
Para executar, basta executar o arquivo "compiler.sh".
```sh
$ ./compiler
```

## Bootloader
Bootloader é um pequeno software que executa o papel de carregar e preparar um sistema operacional ao inicializar um computador.
Suas funções são:
- Carregar o kernel na memória.
- Repassar informações para que o kernel execute corretamente.
- Passar o controle de execução para o kernel.

Após a inicialização do computador, a BIOS (Basic Input/Output System) assume o controle e faz algumas tarefas, uma delas é identificar um disco bootável através de um processo chamado de "bootstrapping" (inicialização), e repassar o controle do computador. Esse processo ocorre da seguinte forma:
- Detecção de dispositivos de inicialização: a BIOS inicia a sua operação verificando os dispositivos de inicialização configurados (Disquete, CD/DVD, USB, HDD, ...). A ordem que estiver configurada é a sequência que a BIOS segue ao buscar o disco inicializável.
- Verificação do setor de inicialização (boot sector): em cada disco que a BIOS estiver checando, ela vai procurar no primeiro setor (512 bytes), também conhecido como MasterBootRecord (MBR), se o mesmo possui a assinatura de inicialização, também conhecido como número mágico.
- Identificação do disco inicializável (bootable): A BIOS verifica se o boot sector contém o número mágico 0xAA55 nos dois últimos bytes do primeiro setor (511 e 512). Caso o disco possua, então ele é considerado inicializável.
- Carregar o bootloader: a BIOS carrega o código de inicialização contido no MBR do dispositivo para a RAM, no endereço 0000h:7C00h, e repassa o controle do sistema para o mesmo.

## Modo real x Modo protegido
O modo real é o modo de operação inicial dos processadores x86. Nesse modo, o processador opera em um ambiente onde não há proteção de memória entre os diferentes processos e o S.O. O endereçamento é feito utilizando segmentação de 16 bits e endereços físicos. As ínterrupções tem acesso direto ao hardware e memória do sistema e não há suporte a multitarefas e nem recursos de proteção de memória.
O modo protegido é um modo de inicialização mais avançada dos processadores x86 que pode ser confiugrada após a inicialização do sistema. Nesse modo, o processador opera em um ambiente onde a memória é protegida e segmentada em segmentos lógicos, cada um com suas próprias permissões. Esse modo permite o uso do GDT (Global Descriptor Table), uma estrutura de dados binária que possibilita controlar o acesso a diferentes partes da memória. As instruções nesse modo tem acesso a recursos avançados como multitarefa, proteção de memória e memória virtual.

## Execução no modo protegido
No modo protegido passamos a ter controle sobre a proteção de memória, níveis de execução, acesso de memórias de até 4GB, suporte a interrupção de hardware e memória virtual.

# Arquitetura do sistema
A família de processadores x86 segue o modelo de Van Nuemann, que é uma especificação de projeto que afirma que um sistema de computador típico possui três componentes principais:
- Unidade de processamento central (CPU)
- Memória
- Entrada/Saída (IO)

## Barramentos
O barramento de sistema é uma combinação de barramento de dados, barramento de endereço e barramento de controle. Cada linha eletrônica representa um bit.
- Barramento de dados: usado para transportar dados. Um processador de 32 bits pode operar sobre 32 linhas com instruções de mesmo tamanho, logo dizer que um processador é de 32 ou 64 bits é o mesmo de dizer que ele trabalha com essa quantidade de linhas no barramento.
- Barramento de endereço: são as linhas que referenciam a memória. As informações passadas aqui referenciam um número que é gerenciado pelo controlador de memória e o mesmo aponta para um chip de ram específico.
- Barramento de controle: é o barramento responsável por identificar o que um dispositivo está tentando fazer. Ele possui bits para representar informações como: leitura ou escrita, sinalizar um dispositivo, checar se a informação está pronta. 

## Sistema de E/S
Este é o sistema que representa a interface entre os controladores de hardware e software. Essa interface ocorre por meio de portas podendo elas serem de hardware ou software:
- Portas de hardware: é a interface entre dois dispositivos físicos. Exemplo: porta serial, porta paralela, porta PS/2 e USB. 
- Portas de software: é a interface do software com o hardware. É um número que representa um controlador de hardware (endereço).
Um endereço de porta é um número especial que cada controlador escuta. Ao inicializar, o BIOS atribui números diferentes a esses dispositivos controladores.
O acesso a estas portas é feito através das instruções IN e OUT.

## Tabelas descritoras
Uma tabela descritora define ou mapeia algo, nesse caso seria a memória e como ela será usada. 
A GDT (tabela descritora global) define o mapa de memória global. Ele define qual memória pode ser executada e qual área contém dados. A GDT descreve os endereços iniciais e básicos, limites de segmento e até memória virtual. Existem três descritores: descritor nulo, descritor de código e descritor de dados.