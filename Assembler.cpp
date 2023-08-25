#include <iostream>
#include <fstream>
#include <cctype>
#include <bitset>
using namespace std;

struct label {
	string name;
	int address;
};

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

int Tipo_R(string & linha, instruction * tipos, registradores * regis);

int main() {

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


	ifstream fin;
	fin.open("main.asm", ios_base::in);

	if (!fin.is_open()) {
		cout << "Nao foi possivel abrir o arquivo.\n";
		exit(1);
	}

	// vetor de struct label para guardar nome e endereço da label
	label* labels = new label[50];

	// primeira leitura
	string str;
	int linha{1}, tam{};	// variaveis para contagem de linhas do arquivo e quantidade de labels
	// faz a leitura de cada palavra ate o fim do arquivo
	while (fin >> str) {
		char* ch = &str[str.length() - 1];	// ponteiro para o último caractere de cada palavra
		if (*ch == ':') {					// se o último caractere for ':' guarda a label e o endere�o
			*ch = '\0';						// substitui ':' por '\0'
			labels[tam].name = str;			// cada struct vai corresponder a uma label do arquivo
			labels[tam++].address = linha;	// endereço da label vai ser o valor da linha atual
		}
		linha++;
		char* c = new char[20];
		fin.getline(c, 20);					// pula para a proxima linha
		fin.clear();
		delete[] c;
	}

	fin.close();

	// segunda leitura

    ifstream fin2;
	fin2.open("main.asm", ios_base::in);

	if (!fin2.is_open()) {
		cout << "Nao foi possivel abrir o arquivo.\n";
		exit(1);
	}

	string cleitin;
	
	getline(fin2, cleitin);

    int binario = Tipo_R(cleitin,tipos,regis);
    bitset<32> binario2(binario);
	
    cout << binario << ": " << binario2 << endl;

	delete[] tipos;
	delete[] regis;
}

int Tipo_R(string & linha, instruction * tipos, registradores * regis) {
int binario = 0;
int constante = 0;
string instrucao;

    for (int z = 14; z < 39; ++z) {
        if (linha.find(tipos[z].name) == 0) {
		binario = tipos[z].funct;
        instrucao = tipos[z].name;

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

    binario = ((constante << 6) | binario); // colocando os bits da constante

    if (instrucao != "jr" && instrucao != "mult" && instrucao != "multu" && instrucao != "div" && instrucao != "divu"){
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
                                binario = ((regis[i].codigo << 11) | binario);
                                break;
                        }
                    }

                    break;
                }
            }
        }

        if (instrucao != "mfhi" && instrucao != "mfhlo"){
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
                                binario = ((regis[i].codigo << 21) | binario);
                                break;
                        }
                    }

                    break;
                }
            }
        }

        if (instrucao != "jr" && instrucao != "mfhi" && instrucao != "mfhlo"){
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
                                binario = ((regis[i].codigo << 16) | binario);
                        }
                    }

                    break;
                }
            }
        }

       return binario;
}