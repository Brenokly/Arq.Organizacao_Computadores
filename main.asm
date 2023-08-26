strlen: # Função para contar os caracteres de uma string que está no registrador $a0, retorna o valor int em $v0.
#sdadasdasdd
#dasdsadad
#aswdsadada
#adsddsad
addi $sp, $sp,-8 # Desce o ponteiro que aponta para pilha 3 vezes, ou seja, aloca 3 espaços na pilha:.asdad#$#><
:#.
sw $t0, 4($sp) # Carrega os valores contidos em $s0,$t0 e $t1 para a pilha.
sw $t1, 0($sp) #
#sdadasdasdd
#dasdsadad
#aswdsadada
#adsddsad
add $v0,$0,$0 # limpa o $v0
add $t0,$0,$0 # limpa o $t0
add $t1,$0,$0 # limpa o $t1
#sdadasdasdd
#dasdsadad
#aswdsadada
#adsddsad 
loop1: # Loopingzinho
#sdadasdasdd
#dasdsadad
#aswdsadada
#adsddsad     

:


#sdadasdasdd
#dasdsadad
#aswdsadada
#adsddsad

#
.

lb $t1, 0($a0) # Carrega o primeiro caracter da string que está em $a0 para $t1
beq $t1, $0, end0 # Se o caracter que está em $t1 não for o \0, ou seja, 0 ele continua contando.
beq $t1, 10, end0 # se ele encontrar o \n ele vai para remove
addi $a0, $a0, 1 # pega o próximo caracter somando 1 ao endereço da string.
addi $t0, $t0, 1 # Soma 1 ao registrador $t0 que representa quantos caracteres tem na string.

j loop1 # Volta para o strlen para testar o próximo caracter.

remove:
lb $0,0($t1) # joga o \0 dentro do $t1 na posição do \n.
#Eu sei que o senhor não pediu para tirar o \n, mas eu optei por tirar porquê não faz sentido deixar ele sendo que temos
# funções como strcat que concatena duas strings, com ele não fica legal a concatenação.

addi $a0, $a0, 1 # pega o próximo caracter somando 1 ao endereço da string.

j loop1 # Volta para o strlen para testar o próximo caracter.

end0:
add $v0,$v0,$t0 # move o valor de $t0 para $v0 que é por convenção o registrador de retorno.

lw $t1, 0($sp)  #
lw $t0, 4($sp)  # Restaura os valores de $t1,$t0,$s0 que foram copiados para a pilha.
addi $sp, $sp, 8 # Restaura ao ponteiro da pilha para o lugar original.

jr $ra # Volta para quem chamou.

#########################################################################################################################

strcmp:#sdadasdasdd
#dasdsadad
#aswdsadada
#adsddsad
addi $sp, $sp,-16 # Desce o ponteiro que aponta para pilha 3 vezes, ou seja, aloca 3 espaços na pilha.
sw $s0, 12($sp)# 
sw $t0, 8($sp) # Carrega os valores contidos em $s0,$t0,$t1 e $ra para a pilha.
sw $t1, 4($sp) #
sw $ra, 0($sp) #

add $v0,$0,$0 # limpa o $v0
add $s0,$0,$0 # limpa o $s0
add $t0,$0,$0 # limpa o $t0
add $t1,$0,$0 # limpa o $t1

jal strlen # Chama a função strlen

move $t0, $v0 # Move o valor de retorno da função strlen para $t0

add $v0,$0,$0 # limpa o $v0

move $a0,$a1 # Move o endereço da string que está em $a1 para $a0 para cálcular o tamanho dela agora.

jal strlen # Chama a função strlen

move $t1, $v0 # Move o valor de retorno da função strlen para $t1

add $v0,$0,$0 # limpa o $v0

beq $t0,$t1,iguais # Se as duas strings forem iguais pula para o label iguais. Se não forem iguais ele continua.

slt $s0, $t0, $t1 # Se a primeira string for menor que a segunda retorna 1, se não, retorna 0.

beq $s0,1, negativo # Se $s0 for igual a 1 significa que a primeira é menor.

positivo:
addi $v0,$v0,1 # adiciona 1 ao $v0 siginificando que a primeira string é maior que a segunda.
j end1

negativo:
addi $v0,$v0,-1 # adiciona -1 ao $v0 siginificando que a primeira string é menor que a segunda.
j end1

iguais:
addi $v0,$v0,0 # adiciona 0 ao $v0 siginificando que as  strings são iguais.

end1:
lw $ra, 0($sp)
lw $t1, 4($sp) #
lw $t0, 8($sp) # Restaura os valores de $t1,$t0 e $s0 que foram copiados para a pilha.
lw $s0, 12($sp) # 
addi $sp, $sp, 16 # Restaur ao ponteiro da pilha para o lugar original.

jr $ra # Volta para quem chamou.


strcat: # contatena uma string src = fonte a uma string dest = destino.
addi $sp, $sp,-12 # Desce o ponteiro que aponta para pilha 3 vezes, ou seja, aloca 3 espaços na pilha.
sw $t0, 8($sp) # Carrega os valores contidos em $s0,$t0,$t1 e $ra para a pilha.
sw $t1, 4($sp) #
sw $ra, 0($sp) #

add $v0,$0,$0 # limpa o $v0
add $s0,$0,$0 # limpa o $s0
add $t0,$0,$0 # limpa o $t0
add $t1,$0,$0 # limpa o $t1

# String destino $a0
# String origem $a1

jal strlen # chamando a função strlen para cálcular o tamanho da string dest = destino

move $t0, $v0 # copiando o tamanho da string destino para $t0

loop2:

lb $t1, 0($a1) # Carrega o primeiro caracter da string que está em $a1 para $t1

sb $t1, 0($a0) # Concateno o caracter da string de origem na string de destino que no caso está em $a0

addi $a0, $a0, 1   # Avança para o próximo byte na string de destino
addi $a1, $a1, 1   # Avança para o próximo byte na string de origem
addi $t0,$t0,1     # incrementando o valor de $t0 para saber qual endereço voltar no final da função

beq $t1, $0,end2 # Se o caracter que está em $t1 não for o \0, ou seja, 0 ele continua concatenando.
j loop2

end2:
sub $a0,$a0,$t0 # subtraindo $a0 que contem o endereço da string de $t0 para voltar para o início dela
la $v0,($a0) # carregando o endereço do primeiro caracter da string para retornar.
lw $ra, 0($sp)  #
lw $t1, 4($sp)  #
lw $t0, 8($sp)  # Restaura os valores de $t1,$t0 e $s0 que foram copiados para a pilha.
addi $sp, $sp, 12 # Restaur ao ponteiro da pilha para o lugar original.

jr $ra # Volta para quem chamou.



strncat: # contatena uma string src = fonte a uma string dest = destino.
addi $sp, $sp,-12 # Desce o ponteiro que aponta para pilha 3 vezes, ou seja, aloca 3 espaços na pilha.
sw $t0, 8($sp) # Carrega os valores contidos em $s0,$t0,$t1 e $ra para a pilha.
sw $t1, 4($sp) #
sw $ra, 0($sp) #

add $v0,$0,$0 # limpa o $v0
add $s0,$0,$0 # limpa o $s0
add $t0,$0,$0 # limpa o $t0
add $t1,$0,$0 # limpa o $t1

# String destino $a0
# String origem $a1
# count $a2

jal strlen # chamando a função strlen para cálcular o tamanho da string dest = destino

move $t0, $v0 # copiando o tamanho da string destino para $t0
add $a2,$a2,1 # adicionando um vez a mais no looping para ele adicionar o \0
loop3:

lb $t1, 0($a1) # Carrega o primeiro caracter da string que está em $a1 para $t1

sb $t1, 0($a0) # Concateno o caracter da string de origem na string de destino que no caso está em $a0

addi $a0, $a0, 1   # Avança para o próximo byte na string de destino
addi $a1, $a1, 1   # Avança para o próximo byte na string de origem
addi $t0,$t0,1     # incrementando o valor de $t0 para saber qual endereço voltar no final da função
addi $t3,$t3,1     # criando um contador para encerrar o laço.

beq $t3, $a2, end3 # Se o caracter que está em $t1 não for o \0, ou seja, 0 ele continua concatenando.
j loop3

end3:
sb $0, 0($a0) # colocando o \0 no final.
sub $a0,$a0,$t0 # subtraindo $a0 que contem o endereço da string de $t0 para voltar para o início dela
la $v0,($a0) # carregando o endereço do primeiro caracter da string para retornar.
lw $ra, 0($sp)  #
lw $t1, 4($sp)  #
lw $t0, 8($sp)  # Restaura os valores de $t1,$t0 e $s0 que foram copiados para a pilha.
addi $sp, $sp, 12 # Restaur ao ponteiro da pilha para o lugar original.

jr $ra # Volta para quem chamou.


strncpy: # Copia alguns caracteres determinados da string de origem para string destino
addi $sp, $sp,-16 # Desce o ponteiro que aponta para pilha 3 vezes, ou seja, aloca 3 espaços na pilha.
sw $s0, 12($sp)# 
sw $t0, 8($sp) # Carrega os valores contidos em $s0,$t0,$t1 e $ra para a pilha.
sw $t1, 4($sp) #
sw $ra, 0($sp) #

add $v0,$0,$0 # limpa o $v0
add $s0,$0,$0 # limpa o $s0
add $t0,$0,$0 # limpa o $t0
add $t1,$0,$0 # limpa o $t1

# string origem $a1
# string destino $a0
# count $a2

addi $sp, $sp, -4 # abre um espaço na stack para guardar o endereço que está em $a0
sw $a0, 0($sp) # guarda na pilha o endereço da string desti na pilha

la $a0,($a1)  # carrega o endereço que está em $a1 para o $a0 porquê a função strlen conta os caracteres do $a0

jal strlen # chama a função strlen

move $t1,$v0 # move o resultado da função para $t1

bgt $a2,$t1,erro # Se a quantidade de caracteres escolhidos pelo usuário for maior que a string origem, dará erro.

lw $a0, 0($sp) # recupera o endereço da string destino da pilha.

addi $sp, $sp, 4 # Restaura a pilha.

loop4:

lb $t0, 0($a1) # Carrega o caracter (src) que está em $a1

sb $t0, 0($a0) # copia o caracter para (dest) a string destino que está em $a0

addi $a1,$a1,1 # Soma 1 ao endereço da string (src) que está em $a1
addi $a0,$a0,1 # Soma 1 ao endereço da string (dest) que está em $a0
addi $s0,$s0,1 # Soma 1 ao $s0 que está agindo como um contador

beq $a2, $s0, end4 # Se o caracter que está em $t0 não for o \0, ou seja, 0 ele continua concatenando.
j loop4

end4:
sub $a0,$a0,$s0 # subtraindo $a0 que contem o endereço da string de $t0 para voltar para o início dela
la $v0,($a0) # carregando o endereço do primeiro caracter da string para retornar.

lw $ra, 0($sp)  #
lw $t1, 4($sp)  #
lw $t0, 8($sp)  # Restaura os valores de $t1,$t0 e $s0 que foram copiados para a pilha.
lw $s0, 12($sp) # 
addi $sp, $sp, 16 # Restaur ao ponteiro da pilha para o lugar original.

jr $ra # Volta para quem chamou.

erro: # erro caso o usuário peça para copiar mais caracteres do que existe na string origem
la $a0,msgErro # carregando a mensagem de erro
li $v0,4 # código para imprimir uma string
syscall

li $v0,10 # código para encerrar o programa
syscall