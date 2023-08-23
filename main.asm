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

move $s0, $v0 # Move o valor de retorno da função strlen para $s0