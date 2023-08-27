.data


.text
.globl main
main:

fact:
addi $sp, $sp, -8 #ajusta pilha para 2 itens
sw $ra, 4($sp) # salva o endereço de retorno
sw $a0, 0($sp) # salva o argumento n

#Condição
slti $t0, $a0, 1 # teste para n < 1
beq $t0, $zero, L1 # se n>=1 vai para L1

#Senão for maior que 1, devolve o valor 1.
addi $v0, $zero, 1 # retorna 1
addi $sp, $sp, 8 # retira 2 itens da pilha
jr $ra #retorna para depois de jal

#Se for maior que 1
L1: 
addi $a0, $a0, -1 #arg1 = n – 1;
jal fact #chama fact(n-1);

#Restaurando registradores.
#A próxima instrução é onde fact retorna.
lw $a0, 0($sp) #retorna de jal: restaura n
lw $ra, 4($sp) #restaura endereço de retorno
addi $sp, $sp, 8 #ajusta stack pointer

mul $v0, $a0, $v0 # retorna n * fact( n - 1)
jr $ra # retorna para o procedimento que o chamou











