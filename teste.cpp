#include <iostream>
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
    {"zero", "$0", 0b00000}, {"at", "$1", 0b00001}, {"v0", "$2", 0b00010}, {"v1", "$3", 0b00011},
    {"a0", "$4", 0b00100}, {"a1", "$5", 0b00101}, {"a2", "$6", 0b00110}, {"a3", "$7", 0b00111},
    {"t0", "$8", 0b01000}, {"t1", "$9", 0b01001}, {"t2", "$10", 0b01010}, {"t3", "$11", 0b01011},
    {"t4", "$12", 0b01100}, {"t5", "$13", 0b01101}, {"t6", "$14", 0b01110}, {"t7", "$15", 0b01111},
    {"s0", "$16", 0b10000}, {"s1", "$17", 0b10001}, {"s2", "$18", 0b10010}, {"s3", "$19", 0b10011},
    {"s4", "$20", 0b10100}, {"s5", "$21", 0b10101}, {"s6", "$22", 0b10110}, {"s7", "$23", 0b10111},
    {"t8", "$24", 0b11000}, {"t9", "$25", 0b11001}, {"k0", "$26", 0b11010}, {"k1", "$27", 0b11011},
    {"gp", "$28", 0b11100}, {"sp", "$29", 0b11101}, {"fp", "$30", 0b11110}, {"ra", "$31", 0b11111}
    };


    string linha = "add $8, $9, $10";

	unsigned int binario{0};
	int constante{0};

    for (int i = 14; i < 39; ++i) {
        if (linha.find(tipos[i].name) == 0) {
		binario = tipos[i].funct;

        bitset<32> binario1(binario);
        cout << "funct: " << binario1 << endl;
        break;
		}
	}
		for (int j = 0; linha[j]; j++)
		{
			if (isdigit(linha[j]) && linha[j - 1] == '$'){ // evita de pegar constante de instruções que tem número mesmo o tipo R não tendo
				constante = int(linha[j]); // se encontrar um inteiro na linha ele guarda em constante.

				if (isdigit(linha[j + 1]))
				{
					constante *= 10;
					constante += int(linha[j + 1]); // verifica se o próximo também não é uma constante caso tenha tipo 14
					break;	
				}
				break;
			}
		}

			binario = ((constante << 6) | binario); // colocando os bits da constante
            bitset<32> binario2(constante);
            cout << "constante: " <<  binario2 << endl;

            bitset<32> binario3(binario);
            cout << "binario com constante:" << binario3 << endl;


            // Encontra os registradores presentes na linha
            string rd_i,rt_i,rs_i;

            for (int j = 0; j < 32; ++j) {
                if (linha.find(regis[j].nome) != string::npos || linha.find(regis[j].numero) != string::npos) {
				int rd = regis[j].codigo;
                rd_i = regis[j].nome;

                bitset<32> binario4(rd);
                cout << "normal rd:" << binario4 << endl;
                cout << "deslocado 11 rd:" << (binario4 << 11) << endl;

                binario = ((rd << 11) | binario);
                bitset<32> binario10(binario);
                cout << "binario com o rd:" << binario10 << endl;

				break;
                }
            }

            size_t rd = linha.find(rd_i);
            linha.replace(rd,1,"X");

            for (int j = 0; j < 32; ++j) {
                if (linha.find(regis[j].nome) != string::npos || linha.find(regis[j].numero) != string::npos) {
				int rs = regis[j].codigo;
                rs_i = regis[j].nome;
                bitset<32> binario6(rs);
                cout << "normal rt:" << binario6 << endl;
                cout << "deslocado 21 rs:" << (binario6 << 21) << endl;

                binario = ((rs << 21) | binario);

                bitset<32> binario12(binario);
                cout << "binario com o rs:" << binario12 << endl;

				break;
                }
            }

            size_t rs = linha.find(rs_i);
            linha.replace(rs,1,"X");

			for (int j = 0; j < 32; ++j) {
                if (linha.find(regis[j].nome) != string::npos || linha.find(regis[j].numero) != string::npos) {
				int rt = regis[j].codigo;
                rt_i = regis[j].nome;
                bitset<32> binario5(rt);
                cout << "normal rt:" << binario5 << endl;
                cout << "deslocado 16 rt:" << (binario5 << 16) << endl;

                binario = ((rt << 16) | binario);

                 bitset<32> binario11(binario);
                cout << "binario com o rt:" << binario11 << endl;

				break;
                }
            }

            size_t rt = linha.find(rt_i);
            linha.replace(rt,1,"X");

			bitset<32> binario11(binario);
            cout << "binario final:" << binario11 << endl;
            cout << binario << endl;

            cout << linha << endl;


return 0;
}