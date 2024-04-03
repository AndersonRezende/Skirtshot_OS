# Skirtshot_OS
Projeto de sistema operacional.

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

## Execução
Para executar, basta dar permissão de execução ao script compiler.sh e depois executar com o parametro do nome do bootloader.
```sh
$ sudo chmod +x compiler.sh
$ ./compiler bootloader
```

## Modo real x Modo protegido
O modo real é o modo de operação inicial dos processadores x86. Nesse modo, o processador opera em um ambiente onde não há proteção de memória entre os diferentes processos e o S.O. O endereçamento é feito utilizando segmentação de 16 bits e endereços físicos. As ínterrupções tem acesso direto ao hardware e memória do sistema e não há suporte a multitarefas e nem recursos de proteção de memória.
O modo protegido é um modo de inicialização mais avançada dos processadores x86 que pode ser confiugrada após a inicialização do sistema. Nesse modo, o processador opera em um ambiente onde a memória é protegida e segmentada em segmentos lógicos, cada um com suas próprias permissões. Esse modo permite o uso do GDT (Global Descriptor Table), uma estrutura de dados binária que possibilita controlar o acesso a diferentes partes da memória. As instruções nesse modo tem acesso a recursos avançados como multitarefa, proteção de memória e memória virtual.