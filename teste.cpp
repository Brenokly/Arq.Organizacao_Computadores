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
    {"$zero", "$0", 0b00000}, {"$at", "$1", 0b00001}, {"$v0", "$2", 0b00010}, {"$v1", "$3", 0b00011},
    {"$a0", "$4", 0b00100}, {"$a1", "$5", 0b00101}, {"$a2", "$6", 0b00110}, {"$a3", "$7", 0b00111},
    {"$t0", "$8", 0b01000}, {"$t1", "$9", 0b01001}, {"$t2", "$10", 0b01010}, {"$t3", "$11", 0b01011},
    {"$t4", "$12", 0b01100}, {"$t5", "$13", 0b01101}, {"$t6", "$14", 0b01110}, {"$t7", "$15", 0b01111},
    {"$s0", "$16", 0b10000}, {"$s1", "$17", 0b10001}, {"$s2", "$18", 0b10010}, {"$s3", "$19", 0b10011},
    {"$s4", "$20", 0b10100}, {"$s5", "$21", 0b10101}, {"$s6", "$22", 0b10110}, {"$s7", "$23", 0b10111},
    {"$t8", "$24", 0b11000}, {"$t9", "$25", 0b11001}, {"$k0", "$26", 0b11010}, {"$k1", "$27", 0b11011},
    {"$gp", "$28", 0b11100}, {"$sp", "$29", 0b11101}, {"$fp", "$30", 0b11110}, {"$ra", "$31", 0b11111}
};



    string linha = "add $t0, $s1, $s2";

	unsigned int binario{0};
	int constante{0};

    for (int t = 14; t < 39; ++t) {
        if (linha.find(tipos[t].name) == 0) {
		binario = tipos[t].funct;
        cout << "funct: " << tipos[t].funct << endl;

        bitset<32> binario1(binario);
        cout << "funct: " << binario1 << endl;
        break;
		}
	}
		for (int j = 0; linha[j]; j++){
			if (isdigit(linha[j]) && linha[j - 1] != '$' && linha[j - 2] != '$'){ // evita de pegar constante de instruções que tem número mesmo o tipo R não tendo
                constante = stoi(string(1, linha[j]));

				if (isdigit(linha[j + 1]))
				{
					constante *= 10;
					constante += stoi(string(1, linha[j + 1])); // verifica se o próximo também não é uma constante caso tenha tipo 14
					break;	
				}
				break;
			}
	    }
            cout << constante << endl;
			binario = ((constante << 6) | binario); // colocando os bits da constante
           
            bitset<32> binario3(binario);
            cout << "binario com constante:" << binario3 << endl;
        
        
        for (int j = 0; j < 32; ++j) {
            if (linha.find("$") != string::npos) {
                int rd = linha.find("$"); // procura o $
                string rd_i;

                rd_i += linha[rd]; // guarda o  $

                linha.replace(rd,1,"X"); // coloca um X no lugar o $rd para não aparecer novamente

                rd_i += linha[rd+1];

                linha.replace(rd+1,1,"X"); // coloca um X no lugar do número do registrador para não aparecer novamente XX

                if (isdigit(linha[rd+2]))
                {
                    rd_i += linha[rd+2];
                    linha.replace(rd+2,1,"X"); // coloca um X no lugar do número do registrador para não aparecer novamente XXX
                }

                for (int i = 0; i < 32; i++) {
                    if (rd_i == regis[i].nome || rd_i == regis[i].numero){
                            cout << "RD: " << regis[i].nome;
                            binario = ((regis[i].codigo << 11) | binario);
                            cout << "binario: " << binario << endl; 
                            break;
                    }
                }

                break;
            }
        }

        for (int j = 0; j < 32; ++j) {
            if (linha.find("$") != string::npos) {
                int rs = linha.find("$"); // procura o $
                string rs_i;

                string copia = linha;

                rs_i += linha[rs]; // guarda o  $

                linha.replace(rs,1,"X"); // coloca um X no lugar o $rd para não aparecer novamente

                rs_i += linha[rs+1];
                
                linha.replace(rs+1,1,"X"); // coloca um X no lugar do número do registrador para não aparecer novamente XX

                if (isdigit(linha[rs+2])){
                    rs_i += linha[rs+2];
                    linha.replace(rs+2,1,"X"); // coloca um X no lugar do número do registrador para não aparecer novamente XXX
                }   

                for (int i = 0; i < 32; i++) {
                    if (rs_i == regis[i].nome || rs_i == regis[i].numero){
                            cout << "RS: " << regis[i].nome;
                            binario = ((regis[i].codigo << 21) | binario);
                            cout << "binario: " << binario << endl; 
                            break;
                    }
                }

                break;
            }
        }

		for (int j = 0; j < 32; ++j) {
            if (linha.find("$") != string::npos) {
                int rt = linha.find("$"); // procura o $
                string rt_i;

                string copia = linha;

                rt_i += linha[rt]; // guarda o  $

                linha.replace(rt,1,"X"); // coloca um X no lugar o $rd para não aparecer novamente

                rt_i += linha[rt+1];
                
                linha.replace(rt+1,1,"X"); // coloca um X no lugar do número do registrador para não aparecer novamente XX

                if (isdigit(linha[rt+2])){
                    rt_i += linha[rt+2];
                    linha.replace(rt+2,1,"X"); // coloca um X no lugar do número do registrador para não aparecer novamente XXX
                }   

                for (int i = 0; i < 32; i++) {
                    if (rt_i == regis[i].nome || rt_i == regis[i].numero){
                            cout << "RT: " << regis[i].nome;
                            binario = ((regis[i].codigo << 16) | binario);
                            cout << "binario: " << binario << endl; 
                            break;
                    }
                }

                break;
            }
        }

		bitset<32> binario11(binario);
        cout << "binario final:" << binario11 << endl;
        cout << binario << endl;

           
        return 0;
}