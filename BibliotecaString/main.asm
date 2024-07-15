# Breno Klywer Olegario de Moura
# Esse arquivo "main.asm" tem a finalidade apenas de testar as bibliotecas implementadas "string.asm".

# Observação, o espaço das strings que são lidas não foi alocado dinâmicamente, então conforme for usando o looping
# o espaço definido vai encher, então se parar ou der erro nas operações é porquê o espaço encheu.
# fazer com memória alocada dinâmicamente iria dar muito trabalho.
.data

destino: .space 1000 # Espaço máximo após muito uso do looping esse espaço encherá
origem: .space 1000 # Espaço máximo após muito uso do looping esse espaço encherá
menu: .asciiz "\n[0]Sair\n[1]Strlen\n[2]Strcmp\n[3]Strcat\n[4]Strncat\n[5]Strncpy\nOpção: "
msg1: .asciiz "\nDigite a string de destino: "
msg2: .asciiz "\nDigite a string de origem: "
msg3: .asciiz "\nQuantos caracteres: "
msg4: .asciiz "Caracteres: "
msg5: .asciiz "Resultado da operação: "

.text

.globl main
.include "string.asm" # Incluir o conteúdo do arquivo "string.asm"

main:
la $a0,menu # carregando o menu
li $v0, 4 # chamando o sistema para imprimir
syscall 

li $v0, 5 # chamando o sistema para ler um inteiro
syscall

beq $v0,0,encerrar # se o usuário digitar 0 encerra o programa.
beq $v0,1,Funstrlen # se o usuário digitar 1 vai para a função strlen.
beq $v0,2,Funstrcmp # se o usuário digitar 2 vai para a função strcmp.
beq $v0,3,Funstrcat # se o usuário digitar 3 vai para a função strcat.
beq $v0,4,Funstrncat # se o usuário digitar 3 vai para a função strncat.
beq $v0,5,Funstrncpy # se o usuário digitar 3 vai para a função strncpy.

Funstrlen:
# 1° teste da função strlen

la $a0, msg1 # carregando o endereço da mensagem 1
li $v0, 4 # imprimir uma string
syscall

la $a0, destino # carregando o espaço de memória destinado a string de destino
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall 

la $a0, destino # carregando o endereço da string

move $v0,$0 # limpando o $v0

jal strlen # chama a função strlen

move $s0, $v0 # Move o valor de retorno da função strlen para $s0

la $a0,msg4 # carregando a mensagem
li $v0, 4 # chamnado o sistema para imprimir
syscall 

move $a0, $s0 # Move o valor de retorno da função strlen para $a0

addi $v0,$0, 1 # coloca o código de 1 dentro de $v0 para imprimir um inteiro
#usei o addi só para ficar claro que eu sei usar as instruções padrões não só as pseudos.

syscall # chamando o sistema para imprimir o resultado da função.

j main

#########################################################################################################################

Funstrcmp:

# 2° teste da função strcmp

la $a0, msg1 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall

la $a0, destino # carregando o espaço de memória destinado a string de destino
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

la $a0, msg2 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall 

la $a0, origem # carregando o espaço de memória destinado a string de origem
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

la $a0, destino # carregando o endereço da string
la $a1, origem # carregando o endereço da string 

add $v0,$0,$0 # limpando o $v0

jal strcmp # chama a função strcmp

move $s0, $v0 # Move o valor de retorno da função strlen para $s0

la $a0,msg5 # carregando a mensagem
li $v0, 4 # chamnado o sistema para imprimir
syscall 

move $a0, $s0 # Move o valor de retorno da função strlen para $a0

li $v0, 1  # coloca o código de 1 dentro de $v0 para imprimir um inteiro

syscall # chamando o sistema para imprimir o resultado da função.

j main

#########################################################################################################################

Funstrcat:

# 3° teste da função strcat

la $a0, msg1 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall 

la $a0, destino # carregando o espaço de memória destinado a string de destino
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

 la $a0, msg2 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall  

la $a0, origem # carregando o espaço de memória destinado a string de origem
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

la $a0, destino # carregando o endereço da string
la $a1, origem # carregando o endereço da string 

add $v0,$0,$0 # limpando o $v0

jal strcat # chama a função strlen
 
la $s0, ($v0) # Move o valor de retorno da função strlen para $s0

la $a0,msg5 # carregando a mensagem
li $v0, 4 # chamnado o sistema para imprimir
syscall 

la $a0, ($s0) # Move o valor de retorno da função strlen para $a0

li $v0, 4  # coloca o código de 1 dentro de $v0 para imprimir um inteiro

syscall # chamando o sistema para imprimir o resultado da função.

j main

#########################################################################################################################

Funstrncat:

# 4° teste da função strncat

la $a0, msg1 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall 

la $a0, destino # carregando o espaço de memória destinado a string de destino
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

la $a0, msg2 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall  

la $a0, origem # carregando o espaço de memória destinado a string de origem
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

la $a0, msg3 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall 

li $v0,5 # Coloca o código de leitura de um inteiro
syscall

move $a2, $v0 # copia o valor lido para $a2 que é, por convenção, um registrador destinado a parâmetros.

la $a0, destino # carregando o endereço da string
la $a1, origem # carregando o endereço da string

jal strncat # chama a função strlen

la $s0,($v0) # Move o valor de retorno da função strlen para $s0

la $a0,msg5 # carregando a mensagem
li $v0, 4 # chamnado o sistema para imprimir
syscall 

la $a0, ($s0) # Move o valor de retorno da função strlen para $a0

li $v0, 4  # coloca o código de 1 dentro de $v0 para imprimir um inteiro

syscall # chamando o sistema para imprimir o resultado da função.

j main

#########################################################################################################################

Funstrncpy:

# 5° teste da função strncpy

la $a0, msg1 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall 

la $a0, destino # carregando o espaço de memória destinado a string de destino
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

la $a0, msg2 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall  

la $a0, origem # carregando o espaço de memória destinado a string de origem
li $a1,100 # tamanho máximo da string a ser lida
li $v0, 8 # ler uma string
syscall

la $a0, msg3 # carregando o endereço da mensagem 
li $v0, 4 # imprimir uma string
syscall 

li $v0,5 # Coloca o código de leitura de um inteiro
syscall

move $a2, $v0 # copia o valor lido para $a2 que é, por convenção, um registrador destinado a parâmetros.

la $a0, destino # carregando o endereço da string
la $a1, origem # carregando o endereço da string

jal strncpy # chama a função strlen

la $s0,($v0) # Move o valor de retorno da função para $s0

la $a0,msg5 # carregando a mensagem
li $v0, 4 # chamnado o sistema para imprimir
syscall 

la $a0, ($s0) # Move o valor de retorno da função para $a0

li $v0, 4  # coloca o código de 1 dentro de $v0 para imprimir um inteiro

syscall # chamando o sistema para imprimir o resultado da função.

j main

encerrar:
li $v0,10 # encerrar o programa
syscall

