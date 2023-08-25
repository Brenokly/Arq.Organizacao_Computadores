#include <iostream>
#include <string>
#include <bitset>
using namespace std;

struct instruction {
	string name;
	char type;
	int funct;
};

struct registradores
{
	string nome;
	string numero;
	int codigo;
};

int main(){

    instruction * tipos = new instruction[39]{
    {"beq", 'i', 0b000100}, {"bne", 'i', 0b000101}, {"addi", 'i', 0b001000}, {"addiu", 'i', 0b001001},
    {"slti", 'i', 0b001010}, {"sltiu", 'i', 0b001011}, {"andi", 'i', 0b001100}, {"ori", 'i', 0b001101},
    {"lui", 'i', 0b001111}, {"lw", 'i', 0b100011}, {"sw", 'i', 0b101011},
    {"j", 'j', 0b000010}, {"jal", 'j', 0b000011}, {"sll", 'r', 0b000000},
    {"srl", 'r', 0b000010}, {"jr", 'r', 0b001000}, {"mfhi", 'r', 0b010000},
    {"mflo", 'r', 0b010010}, {"mult", 'r', 0b011000}, {"multu", 'r', 0b011001}, {"div", 'r', 0b011010},
    {"divu", 'r', 0b011011}, {"add", 'r', 0b100000}, {"addu", 'r', 0b100001},
    {"sub", 'r', 0b100010}, {"subu", 'r', 0b100011}, {"and", 'r', 0b100100},
    {"or", 'r', 0b100101}, {"slt", 'r', 0b101010}, {"sltu", 'r', 0b101011},
    {"mul", 'r', 0b000010}
	};

	registradores * regis = new registradores[32]{
    {"$zero", "$0", 0b00000}, {"$at", "$1", 0b00001}, {"$v0", "$2", 0b00010}, {"$v1", "$3", 0b00011},
    {"$a0", "$4", 0b00100}, {"$a1", "$5", 0b00101}, {"$a2", "$6", 0b00110}, {"$a3", "$7", 0b00111},
    {"$t0", "$8", 0b01000}, {"$t1", "$9", 0b01001}, {"$t2", "$10", 0b01010}, {"$t3", "$11", 0b01011},
    {"$t4", "$12", 0b01100}, {"$t5", "$13", 0b01101}, {"$t6", "$14", 0b01110}, {"$t7", "$15", 0b01111},
    {"$s0", "$16", 0b10000}, {"$s1", "$17", 0b10001}, {"$s2", "$18", 0b10010}, {"$s3", "$19", 0b10011},
    {"$s4", "$20", 0b10100}, {"$s5", "$21", 0b10101}, {"$s6", "$22", 0b10110}, {"$s7", "$23", 0b10111},
    {"$t8", "$24", 0b11000}, {"$t9", "$25", 0b11001}, {"$k0", "$26", 0b11010}, {"$k1", "$27", 0b11011},
    {"$gp", "$28", 0b11100}, {"$sp", "$29", 0b11101}, {"$fp", "$30", 0b11110}, {"$ra", "$31", 0b11111}
};

    string linha = "sll $t0, $t1, 4";
    string copia = linha;

    int binario = 0;
    int constante = 0;

    for (int z = 14; z < 39; ++z) {
        if (linha.find(tipos[z].name) == 0) {
		binario = tipos[z].funct;
        break;
		}
	}
		for (int j = 0; linha[j]; j++){
			if (isdigit(linha[j]) && linha[j - 1] != '$' && linha[j - 2] != '$'){ // evita de pegar constante de instruções que tem número mesmo o tipo R não tendo
                constante = stoi(string(1, linha[j]));

                cout << constante << endl;

				if (isdigit(linha[j + 1]))
				{
					constante *= 10;
					constante += stoi(string(1, linha[j + 1])); // verifica se o próximo também não é uma constante caso tenha tipo 14
					break;	
				}
				break;
			}
	    }

        binario = ((constante << 6) | binario); // colocando os bits da constante

        cout << linha << endl;
        bitset<32> binario11(binario);
        cout << "binario final:" << binario11 << endl;
        cout << linha << endl;

	
    return 0;
}