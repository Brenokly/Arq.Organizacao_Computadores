#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
using namespace std;

struct registradores {
    string nome;
    string numero;
    string codigo;
};

struct instruction {
    string name;
    bitset<32>(*type)(string, instruction*, registradores*);
    string opcode;
    int endereco;
};

struct label {
    string name;
    int address;
};

label* labels;
int tam;

string codRegistrador(string r, registradores* regis);
bitset<32> convertTypeR(string linha, instruction * ins, registradores * regis);
bitset<32> convertTypeJ(string str, instruction * ins, registradores * regis);
bitset<32> convertTypeI(string str, instruction * ins, registradores * regis);

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

	instruction* inst = new instruction[31]{
    {"beq", convertTypeI, "000100", 0}, {"bne", convertTypeI, "000101", 0},
    {"addi", convertTypeI, "001000", 0}, {"addiu", convertTypeI, "001001", 0},
    {"slti", convertTypeI, "001010", 0}, {"sltiu", convertTypeI, "001011", 0},
    {"andi", convertTypeI, "001100", 0}, {"ori", convertTypeI, "001101", 0},
    {"lui", convertTypeI, "001111", 0}, {"lw", convertTypeI, "100011", 0},
    {"sw", convertTypeI, "101011", 0},{"j", convertTypeJ, "000010", 0},
    {"jal", convertTypeJ, "000011", 0},{"sll", convertTypeR, "000000", 0},
    {"srl", convertTypeR, "000010", 0}, {"jr", convertTypeR, "001000", 0},
    {"mfhi", convertTypeR, "010000", 0}, {"mflo", convertTypeR, "010010", 0},
    {"mult", convertTypeR, "011000", 0}, {"multu", convertTypeR, "011001", 0},
    {"div", convertTypeR, "011010", 0}, {"divu", convertTypeR, "011011", 0},
    {"add", convertTypeR, "100000", 0}, {"addu", convertTypeR, "100001", 0},
    {"sub", convertTypeR, "100010", 0}, {"subu", convertTypeR, "100011", 0},
    {"and", convertTypeR, "100100", 0}, {"or", convertTypeR, "100101", 0},
    {"slt", convertTypeR, "101010", 0},{"mul", convertTypeR, "000010", 0},
    {"sltu", convertTypeR, "101011", 0}};

    ifstream fin;
    fin.open("main.asm");
    if (!fin.is_open()) {
        cout << "Nao foi possivel abrir o arquivo.\n";
        exit(1);
    }

    // vetor de struct label para guardar nome e endereço da label
    labels = new label[50];

    // primeira leitura
    string str{};
    int linha{};
    char c[50];
    while (fin >> str) {
        if (str.back() == ':') {                // verifica o ultimo caractere
            str.erase(str.find(str.back()), 1); // apaga ':', se houver
            labels[tam].name = str;             // guarda a label
            labels[tam++].address = linha++;      // guarda o endereco da label
        }
        fin.getline(c, 50);     // pula para a proxima linha
        fin.clear();            // limpa o buffer
    }

    /*cout << "Labels: \n";
    for (int i = 0; i < tam; i++) {
        cout << labels[i].address << ' ' << labels[i].name << endl;
    }
    cout << endl;*/

    fin.close();
    fin.open("main.asm");
    ofstream fout;
    fout.open("assembler.hex");
    fout << "v2.0 raw\n";

    // segunda leitura
    
	string ins{},line{};
    int endereco{}, saida{1};
    while (getline(fin, line)) {
        // Encontra a posição do caractere '#'
        size_t posComentario = line.find('#'); 

        if (posComentario != std::string::npos) {
            line.erase(posComentario); // Remove tudo a partir da posição do caractere '#'
        }

        // Remove espaços em branco no final da linha
        size_t posUltimoNaoEspaco = line.find_last_not_of(" \t\r\n");
        if (posUltimoNaoEspaco != std::string::npos) {
            line.erase(posUltimoNaoEspaco + 1);
        }

        // Encontra a posição do caractere ' '
        size_t posDoisPontos = line.find(':');
        
        // Se o caractere ':' for encontrado, remove tudo antes dele, ou seja, remove a label e o ":" se tiver
        if (posDoisPontos != string::npos) {
            line.erase(0, posDoisPontos + 1);

            while (!line.empty() && isspace(line[0])) {
            line.erase(0, 1);
            }
        }

        while (line[0] == '.'|| line.empty())
        {
            getline(fin, line);

            // Encontra a posição do caractere '#'
            size_t posComentario = line.find('#'); 

            if (posComentario != std::string::npos) {
                line.erase(posComentario); // Remove tudo a partir da posição do caractere '#'
            }

            // Remove espaços em branco no final da linha
            size_t posUltimoNaoEspaco = line.find_last_not_of(" \t\r\n");
            if (posUltimoNaoEspaco != std::string::npos) {
                line.erase(posUltimoNaoEspaco + 1);
            }

            // Encontra a posição do caractere ' '
            size_t posDoisPontos = line.find(':');
            
            // Se o caractere ':' for encontrado, remove tudo antes dele, ou seja, remove a label e o ":" se tiver
            if (posDoisPontos != string::npos) {
                line.erase(0, posDoisPontos + 1);

                while (!line.empty() && isspace(line[0])) {
                line.erase(0, 1);
                }
            }
        }

        ins.assign(line, 0, line.find(' '));
        bitset<32> codigo;


        for (int i = 0; i < 31; i++) {
            if (inst[i].name == ins) {
                codigo = inst[i].type(line, &inst[i], regis);

                i = 31;
            }
        }
    }

    fout << endl;
    fin.close();
    fout.close();
    delete[] regis;
    delete[] inst;
    delete[] labels;

    return 0;
}
bitset<32> convertTypeR(string linha, instruction * ins, registradores * regis)
{
    string binario{}, constante{}, rs{}, rt{}, rd{}, nome{ ins->name },funct{ins->opcode};
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
bitset<32> convertTypeI(string str, instruction * ins, registradores * regis) {
    string name{ ins->name }, opcode{ ins->opcode }, rs{}, rt{}, im{};
    str.erase(0, str.find(' ') + 1);    // apaga o nome da instrucao completa

    // verifica qual instrucao a ser codificada
    if (name == "beq" || name == "bne") {
        rs.assign(str, 0, str.find(','));
        str.erase(0, str.find(' ') + 1);
        rt.assign(str, 0, str.find(','));
        str.erase(0, str.find(' ') + 1);
        im = str;
    }
    else if (name == "lui") {
        rt.assign(str, 0, str.find(','));
        str.erase(0, str.find(' ') + 1);
        im = "00000";
    }
    else if (name == "lw" || name == "sw") {
        rt.assign(str, 0, str.find(','));
        str.erase(0, str.find(' ') + 1);
        rs.assign(str, str.find('$'), str.length() - str.find('$') - 1);
        str.erase(str.find('('), str.find(')') + 1);
        im = str;
    }
    else {
        rt.assign(str, 0, str.find(','));
        str.erase(0, str.find(' ') + 1);
        rs.assign(str, 0, str.find(','));
        str.erase(0, str.find(' ') + 1);
        im = str;
    }

    bitset<16> immediate;   // declara o valor imediato de 16 bits
    if (isdigit(im[0])) {   // se o imediato for um digito, converte para binario
        immediate = stoi(im);
    }
    else {    
        for (int i = 0; i < tam; i++) {
            if (labels[i].name == im) {
                immediate = labels[i].address - ins->endereco;
            }
        }
    }

    str.clear();
    str.append(opcode);
    str.append(codRegistrador(rs, regis));
    str.append(codRegistrador(rt, regis));
    str.append(immediate.to_string());

    bitset<32> bit(str);
    return bit;
}

bitset<32> convertTypeJ(string str, instruction * ins, registradores * regis) {
    string endereco{}, opcode{ins->opcode}, pc{"000001"};
    str.erase(0, str.find(' ') + 1);
    endereco.append(opcode);
    endereco.append(pc);
    bitset<20> endLabel;
    for (int i = 0; i < tam; i++) {
        if (labels[i].name == str) {
            endLabel = labels[i].address;
        }
    }
    endereco.append(endLabel.to_string());
    
    bitset<32> bit(endereco);
    return bit;
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