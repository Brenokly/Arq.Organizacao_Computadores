.macro done
	li $v0,10
	syscall
.end_macro	

.macro print_int (%valor)
	add $a0, $0 ,%valor
	li $v0,1
	syscall
.end_macro

.macro print_char (%caracter)
	move $a0, %caracter
	li $v0,11
	syscall
.end_macro

.macro print_string (%string)
	move $a0, %string
	li $v0,4
	syscall
.end_macro

.macro read_int  	
	li $v0,5
	syscall
.end_macro

.macro read_char
	li $v0,12
	syscall
.end_macro

.macro read_string (%endereco,%tamanho)
	move $a0, %endereco
	li $a1, %tamanho
	li $v0,8
	syscall
.end_macro


	
