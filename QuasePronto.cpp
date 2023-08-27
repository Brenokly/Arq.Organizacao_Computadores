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

bitset<32> convertTypeR(string str, instruction * ins, registradores * regis) {
    bitset<32> bit;
    return bit;
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
    else {      // se for uma label, pega o endereco da label e subtrai do endereco da instrucao atual
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

bitset<32> convertTypeJ(string str, instruction* ins, registradores* regis) {
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

int main() {
    registradores* regis = new registradores[32]{
    {"$zero", "$0", "00000"}, {"$at", "$1", "00001"}, {"$v0", "$2", "00010"}, {"$v1", "$3", "00011"},
    {"$a0", "$4", "00100"}, {"$a1", "$5", "00101"}, {"$a2", "$6", "00110"}, {"$a3", "$7", "00111"},
    {"$t0", "$8", "01000"}, {"$t1", "$9", "01001"}, {"$t2", "$10", "01010"}, {"$t3", "$11", "01011"},
    {"$t4", "$12", "01100"}, {"$t5", "$13", "01101"}, {"$t6", "$14", "01110"}, {"$t7", "$15", "01111"},
    {"$s0", "$16", "10000"}, {"$s1", "$17", "10001"}, {"$s2", "$18", "10010"}, {"$s3", "$19", "10011"},
    {"$s4", "$20", "10100"}, {"$s5", "$21", "10101"}, {"$s6", "$22", "10110"}, {"$s7", "$23", "10111"},
    {"$t8", "$24", "11000"}, {"$t9", "$25", "11001"}, {"$k0", "$26", "11010"}, {"$k1", "$27", "11011"},
    {"$gp", "$28", "11100"}, {"$sp", "$29", "11101"}, {"$fp", "$30", "11110"}, {"$ra", "$31", "11111"}
    };

    instruction * inst = new instruction[31]{
    {"beq", convertTypeI, "000100"}, {"bne", convertTypeI, "000101"}, {"addi", convertTypeI, "001000"}, {"addiu", convertTypeI, "001001"},
    {"slti", convertTypeI, "001010"}, {"sltiu", convertTypeI, "001011"}, {"andi", convertTypeI, "001100"}, {"ori", convertTypeI, "001101"},
    {"lui", convertTypeI, "001111"}, {"lw", convertTypeI, "100011"}, {"sw", convertTypeI, "101011"}, {"j", convertTypeJ, "000010"}, {"jal", convertTypeJ, "000011"},
    {"sll", convertTypeR, "000000"}, {"srl", convertTypeR, "000010"}, {"jr", convertTypeR, "001000"}, {"mfhi", convertTypeR, "010000"},
    {"mflo", convertTypeR, "010010"}, {"mult", convertTypeR, "011000"}, {"multu", convertTypeR, "011001"}, {"div", convertTypeR, "011010"},
    {"divu", convertTypeR, "011011"}, {"add", convertTypeR, "100000"}, {"addu", convertTypeR, "100001"}, {"sub", convertTypeR, "100010"},
    {"subu", convertTypeR, "100011"}, {"and", convertTypeR, "100100"}, {"or", convertTypeR, "100101"}, {"slt", convertTypeR, "101010"},
    {"sltu", convertTypeR, "101011"}, {"mul", convertTypeR, "000010"}
    };

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
    char line[50];
    int endereco{}, saida{1};
    while (fin.getline(line, 50)) {
        if (line[0] != '\0') {
            string str = line, ins{};
            if (str[0] != '#') {
                size_t pos = str.find('\t');
                while (pos != string::npos) {
                    str.erase(pos, 1);
                    pos = str.find('\t');
                }
                pos = str.find(':');
                if (pos != string::npos) {
                    str.assign(str, pos + 1);
                }
                while (str.front() == ' ') {
                    str.erase(0, 1);
                }
            }
            ins.assign(str, 0, str.find(' '));
            bitset<32> codigo;
            for (int i = 0; i < 39; i++) {
                if (inst[i].name == ins) {
                    inst[i].endereco = ++endereco;
                    codigo = inst[i].type(str, &inst[i], regis);
                    /*cout << str << endl;
                    cout << codigo << endl;*/
                    fout.width(8);
                    fout.fill('0');
                    if (saida++ % 4 == 0)
                        fout << hex << codigo.to_ullong() << endl;
                    else
                        fout << hex << codigo.to_ullong() << ' ';
                    i = 39;
                }
            }
        }
    }
    fout << endl;
    fin.close();
    fout.close();
    delete[] regis;
    delete[] inst;
    delete[] labels;
}