#include <iostream>
#include <fstream>
#include <cctype>
#include <bitset>
using namespace std;

struct label {
	string name;
	int address;
};

struct registradores {
    string nome;
    string numero;
    string codigo;
};

struct instruction {
    string name;
    bitset<32>(*type)(string, string nome, string linha, registradores*);
    string funct;
};

bitset<32> convertTypeR(string, string funct, string linha, registradores*);
bitset<32> convertTypeI(string, string funct, string linha, registradores*);
bitset<32> convertTypeJ(string, string funct, string linha, registradores*);
string codRegistrador(string , registradores*);

int main() {

	registradores* regis = new registradores[32]{
    {"$zero", "$0","00000"}, {"$at", "$1", "00001"}, {"$v0", "$2", "00010"}, {"$v1", "$3", "00011"},
    {"$a0", "$4", "00100"}, {"$a1", "$5", "00101"}, {"$a2", "$6", "00110"}, {"$a3", "$7", "00111"},
    {"$t0", "$8", "01000"}, {"$t1", "$9", "01001"}, {"$t2", "$10", "01010"}, {"$t3", "$11", "01011"},
    {"$t4", "$12", "01100"}, {"$t5", "$13", "01101"}, {"$t6", "$14", "01110"}, {"$t7", "$15", "01111"},
    {"$s0", "$16", "10000"}, {"$s1", "$17", "10001"}, {"$s2", "$18", "10010"}, {"$s3", "$19", "10011"},
    {"$s4", "$20", "10100"}, {"$s5", "$21", "10101"}, {"$s6", "$22", "10110"}, {"$s7", "$23", "10111"},
    {"$t8", "$24", "11000"}, {"$t9", "$25", "11001"}, {"$k0", "$26", "11010"}, {"$k1", "$27", "11011"},
    {"$gp", "$28", "11100"}, {"$sp", "$29", "11101"}, {"$fp", "$30", "11110"}, {"$ra", "$31", "11111"}};

	instruction* inst = new instruction[39]{
    {"beq", convertTypeI, "000100"}, {"bne", convertTypeI, "000101"}, {"addi", convertTypeI, "001000"}, {"addiu", convertTypeI, "001001"},
    {"slti", convertTypeI, "001010"}, {"sltiu", convertTypeI, "001011"}, {"andi", convertTypeI, "001100"}, {"ori", convertTypeI, "001101"},
    {"lui", convertTypeI, "001111"}, {"lw", convertTypeI, "100011"}, {"sw", convertTypeI, "101011"},
    {"j", convertTypeJ, "000010"}, {"jal", convertTypeJ, "000011"},
    {"sll", convertTypeR, "000000"}, {"srl", convertTypeR, "000010"}, {"jr", convertTypeR, "001000"},
    {"mfhi", convertTypeR, "010000"}, {"mflo", convertTypeR, "010010"},
    {"mult", convertTypeR, "011000"}, {"multu", convertTypeR, "011001"}, {"div", convertTypeR, "011010"},
    {"divu", convertTypeR, "011011"}, {"add", convertTypeR, "100000"}, {"addu", convertTypeR, "100001"},
    {"sub", convertTypeR, "100010"}, {"subu", convertTypeR, "100011"}, {"and", convertTypeR, "100100"},
    {"or", convertTypeR, "100101"}, {"slt", convertTypeR, "101010"}, {"sltu", convertTypeR, "101011"},
    {"mul", convertTypeR, "000010"}
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

    bitset<32> codigo;

	string ins{},r{};

    while (getline(fin2,r))
    {
        // Encontra a posição do caractere '#'
        size_t posComentario = r.find('#'); 

        if (posComentario != std::string::npos) {
            r.erase(posComentario); // Remove tudo a partir da posição do caractere '#'
        }

        // Remove espaços em branco no final da linha
        size_t posUltimoNaoEspaco = r.find_last_not_of(" \t\r\n");
        if (posUltimoNaoEspaco != std::string::npos) {
            r.erase(posUltimoNaoEspaco + 1);
        }

        // Encontra a posição do caractere ' '
        size_t posDoisPontos = r.find(':');
        
        // Se o caractere ':' for encontrado, remove tudo antes dele, ou seja, remove a label e o ":" se tiver
        if (posDoisPontos != string::npos) {
            r.erase(0, posDoisPontos + 1);

            while (!r.empty() && isspace(r[0])) {
            r.erase(0, 1);
            }
        }

        while (r[0] == '.'|| r.empty())
        {
            getline(fin2,r);

            // Encontra a posição do caractere '#'
            size_t posComentario = r.find('#'); 

            if (posComentario != std::string::npos) {
                r.erase(posComentario); // Remove tudo a partir da posição do caractere '#'
            }

            // Remove espaços em branco no final da linha
            size_t posUltimoNaoEspaco = r.find_last_not_of(" \t\r\n");
            if (posUltimoNaoEspaco != std::string::npos) {
                r.erase(posUltimoNaoEspaco + 1);
            }

            // Encontra a posição do caractere ' '
            size_t posDoisPontos = r.find(':');
            
            // Se o caractere ':' for encontrado, remove tudo antes dele, ou seja, remove a label e o ":" se tiver
            if (posDoisPontos != string::npos) {
                r.erase(0, posDoisPontos + 1);

                while (!r.empty() && isspace(r[0])) {
                r.erase(0, 1);
                }
            }
        }

        ins.assign(r, 0, r.find(' '));

        cout << r << ": ";

        for (int i = 0; i < 39; i++) {

            if (inst[i].name == ins) {
                codigo = inst[i].type(inst[i].funct,inst[i].name, r, regis);
                i = 39;
            }
        }

        cout << codigo << endl;
    }

   

	delete[] inst;
	delete[] regis;
}
bitset<32> convertTypeR(string funct, string nome, string linha, registradores* regis)
{
    string binario{}, constante{}, rs{}, rt{}, rd{};
    bitset<6> opcode(0);
    bitset<6> opcodeMul(28);

    size_t pos = linha.find(',');

    // Enquanto houver vírgulas na string, remove-as
    while (pos != std::string::npos) {
        linha.erase(pos, 1); // Remove a vírgula na posição 'pos'
        pos = linha.find(','); // Encontra a próxima vírgula
    }

    // Encontra a posição do caractere ' '
    size_t vazio = linha.find(' ');
    
    // Se o caractere ':' for encontrado, remove tudo antes dele
    if (vazio != string::npos) {
        linha.erase(0, vazio + 1);

        while (!linha.empty() && isspace(linha[0])) {
        linha.erase(0, 1);
        }
    }

	for (int j = 0; linha[j]; j++){
		if (isdigit(linha[j]) && linha[j - 1] != '$' && linha[j - 2] != '$'){ // evita de pegar constante de instruções que tem número mesmo o tipo R não tendo
            constante = linha[j];
            linha[j] = '\0';

			if (isdigit(linha[j + 1]))
			{
				constante += linha[j + 1]; // verifica se o próximo também não é uma constante caso tenha tipo 14
                linha[j] = '\0';    
				break;	
			}
			break;
		}
	}

    if (nome != "jr" && nome != "mult" && nome != "multu" && nome != "div" && nome != "divu"){

        rd.assign(linha, 0, linha.find(' '));

        linha.erase(0, linha.find(' ')+1); 

	    while (!linha.empty() && isspace(linha[0])) {
        linha.erase(0, 1);
	    }
    }
    
    if (nome != "mfhi" && nome != "mflo" && nome != "sll" && nome != "srl"){
        rs.assign(linha, 0, linha.find(' '));

        linha.erase(0, linha.find(' ')+1); 

        while (!linha.empty() && isspace(linha[0])) {
        linha.erase(0, 1);
        }      
    }

    if (nome != "jr" && nome != "mfhi" && nome != "mflo"){
        rt.assign(linha, 0, linha.find(' '));    
    }

    if (nome == "mul")
    {
        binario.append(opcodeMul.to_string());
    }
    else
    {
       binario.append(opcode.to_string());
    }

	if (rs.empty())
	{
        binario.append("00000");
		
	}
	else{
		binario.append(codRegistrador(rs, regis));
	}

	if (rt.empty())
	{
		binario.append("00000");
	}
	else{
        binario.append(codRegistrador(rt, regis));
	}
	
	if (rd.empty())
	{
		binario.append("00000");
	}
	else{
        binario.append(codRegistrador(rd, regis));
	}

    if (constante.empty()) {
    bitset<5> immediate(0); 
    binario.append(immediate.to_string());
    } else {
    int immediateValue = stoi(constante);
    bitset<5> immediate(immediateValue);
    binario.append(immediate.to_string());
    }

    binario.append(funct);

    bitset<32>binary(binario);

    return binary;
}
bitset<32> convertTypeI(string opcode, string nome, string ins, registradores* regis) {
    string str{opcode}, rs{}, rt{};
    cout << str << endl;
    // pega o primeiro registrador rs
    ins.erase(0, ins.find(' ') + 1);
    rs.assign(ins, 0, ins.find(','));

    // pega o segundo registrador rt
    ins.erase(0, ins.find(' ') + 1);
    rt.assign(ins, 0, ins.find(','));

    // pega o valor imediato
    ins.erase(0, ins.find(' ') + 1);
    bitset<16> immediate(stoi(ins));
    
    cout << "rs: " << rs << endl;
    cout << "rt: " << rt << endl;
    cout << "im: " << ins << endl;
    
    // procura o codigo binario do registrador rs e concatena na string
    str.append(codRegistrador(rt, regis));

    // procura o codigo binario do registrador rt e concatena na string
    str.append(codRegistrador(rs, regis));

    // concatena o valor imediato
    str.append(immediate.to_string());

    bitset<32> bit(str);
    return bit;
}
bitset<32> convertTypeJ(string funct, string nome, string linha, registradores* regis)
{
}
string codRegistrador(string r, registradores* regis) {
    // procura e retorna o codigo binario do registrador
    for (int i = 0; i < 32; i++) {
        if (r == regis[i].nome) {
            return regis[i].codigo;
        }
        else if (r == regis[i].numero) {
            return regis[i].codigo;
        }
    }
    return "";
}