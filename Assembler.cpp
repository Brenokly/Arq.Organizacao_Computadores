#include <iostream>  //
#include <string>    //
#include <bitset>    // BIBLIOTECAS USADAS
#include <fstream>   //
using namespace std; //

struct registradores { // Struct que representa um registrador
    string nome;
    string numero;
    string codigo;
};

struct instruction { // Struct que representa uma instrução
    string name;
    bitset<32>(*type)(string, instruction*, registradores*);
    string opcode;
    int endereco;
};

struct label {      // Struct para guardar as labels
    string name;
    int address;
};

label* labels;
int tam;

string codRegistrador(string r, registradores* regis);  // Função para retornar o código binário de um registrador
bitset<32> convertTypeR(string linha, instruction* ins, registradores* regis); // função para converter instrucoes tipo R
bitset<32> convertTypeJ(string str, instruction* ins, registradores* regis); // função para converter instrucoes tipo J
bitset<32> convertTypeI(string str, instruction* ins, registradores* regis); // função para converter instrucoes tipo I

int main() {
    registradores* regis = new registradores[32]{
    {"$zero", "$0","00000"}, {"$at", "$1", "00001"}, {"$v0", "$2", "00010"}, {"$v1", "$3", "00011"},
    {"$a0", "$4", "00100"}, {"$a1", "$5", "00101"}, {"$a2", "$6", "00110"}, {"$a3", "$7", "00111"},
    {"$t0", "$8", "01000"}, {"$t1", "$9", "01001"}, {"$t2", "$10", "01010"}, {"$t3", "$11", "01011"},
    {"$t4", "$12", "01100"}, {"$t5", "$13", "01101"}, {"$t6", "$14", "01110"}, {"$t7", "$15", "01111"},
    {"$s0", "$16", "10000"}, {"$s1", "$17", "10001"}, {"$s2", "$18", "10010"}, {"$s3", "$19", "10011"},
    {"$s4", "$20", "10100"}, {"$s5", "$21", "10101"}, {"$s6", "$22", "10110"}, {"$s7", "$23", "10111"},
    {"$t8", "$24", "11000"}, {"$t9", "$25", "11001"}, {"$k0", "$26", "11010"}, {"$k1", "$27", "11011"},
    {"$gp", "$28", "11100"}, {"$sp", "$29", "11101"}, {"$fp", "$30", "11110"}, {"$ra", "$31", "11111"}
    };

    instruction* inst = new instruction[31]{
    {"beq", convertTypeI, "000100"}, {"bne", convertTypeI, "000101"},
    {"addi", convertTypeI, "001000"}, {"addiu", convertTypeI, "001001"},
    {"slti", convertTypeI, "001010"}, {"sltiu", convertTypeI, "001011"},
    {"andi", convertTypeI, "001100"}, {"ori", convertTypeI, "001101"},
    {"lui", convertTypeI, "001111"}, {"lw", convertTypeI, "100011"},
    {"sw", convertTypeI, "101011"},{"j", convertTypeJ, "000010"},
    {"jal", convertTypeJ, "000011"},{"sll", convertTypeR, "000000"},
    {"srl", convertTypeR, "000010"}, {"jr", convertTypeR, "001000"},
    {"mfhi", convertTypeR, "010000"}, {"mflo", convertTypeR, "010010"},
    {"mult", convertTypeR, "011000"}, {"multu", convertTypeR, "011001"},
    {"div", convertTypeR, "011010"}, {"divu", convertTypeR, "011011"},
    {"add", convertTypeR, "100000"}, {"addu", convertTypeR, "100001"},
    {"sub", convertTypeR, "100010"}, {"subu", convertTypeR, "100011"},
    {"and", convertTypeR, "100100"}, {"or", convertTypeR, "100101"},
    {"slt", convertTypeR, "101010"},{"mul", convertTypeR, "000010"},
    {"sltu", convertTypeR, "101011"}
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
    string str, s;
    int linha{};
    while (fin >> str) {
        if (str.back() == ':') {                            // verifica o ultimo caractere
            labels[tam].name.assign(str, 0, str.find(':')); // guarda a label sem o ':'
            labels[tam++].address = linha++;                // guarda o endereco da label
        }
        getline(fin, s);     // pula para a proxima linha
    }

    for (int i = 0; i < tam; i++) {
        cout << labels[i].address << ' ' << labels[i].name << endl;
    }

    fin.close();
    fin.open("main.asm");

    ofstream fout;
    fout.open("assembler.hex");
    fout << "v2.0 raw\n";

    // segunda leitura
    int endereco{}, saida{ 1 };
    while (getline(fin, str)) {
        if (str[0] != '\0') {
            size_t inv = string::npos;

            if (str.find('#') != inv)           // apaga o comentario
                str.erase(str.find('#'));

            if (str.find(':') != inv)           // apaga a label
                str.erase(0, str.find(':') + 1);

            while (str.find('\t') != inv)       // substitui todos os '\t' por ' '
                str.replace(str.find('\t'), 1, " ");

            while (str.front() == ' ')          // remove espacos extras antes da instrucao
                str.erase(0, 1);

            while (str.back() == ' ')           // remove espacos extras depois da instrucao
                str.pop_back();

            if (str != "") {                     // apos todas as modificacoes, verifica se a string esta vazia
                s.assign(str, 0, str.find(' ')); // atribui nome da instrucao para s
                bitset<32> codigo;               // codigo binario de 32 bits da instrucao
                for (int i = 0; i < 39; i++) {   // laco para buscar a instrucao no vetor de instrucoes
                    if (inst[i].name == s) {
                        inst[i].endereco = ++endereco; // quando achar a instrucao, atribui o endereco atual
                        codigo = inst[i].type(str, &inst[i], regis); // chama a funcao apontada pela instrucao
                        fout.width(8);
                        fout.fill('0');
                        if (saida++ % 4 == 0)   // escreve de 4 em 4 valores hexa no arquivo de saida
                            fout << hex << codigo.to_ullong() << endl;
                        else
                            fout << hex << codigo.to_ullong() << ' ';
                        i = 39;
                    }
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

bitset<32> convertTypeR(string linha, instruction* ins, registradores* regis)
{
    // Variaveis para guardar os valores do registradores, da constante, do opcode e do funct.
    string binario{},constante{}, rs{}, rt{}, rd{}, nome{ ins->name }, funct{ ins->opcode };
    bitset<6> opcode(0);
    bitset<6> opcodeMul(28);

    size_t pos = linha.find(','); // Procura a primeira vírgula na linha recebida

    // Enquanto houver vírgulas na string, remove-as | add $8,$9,$10 ou add $8, $9, $10
    while (pos != std::string::npos) {
        linha.erase(pos, 1); // Remove a vírgula na posição 'pos'
        pos = linha.find(','); // Encontra a próxima vírgula
    }

    // Encontra a posição do caractere ' '
    size_t vazio = linha.find(' ');  // encontra o primeiro espaço vazio e pega a posição dele

    // Se o caractere ' ' for encontrado, remove tudo antes dele
    if (vazio != string::npos) {
        linha.erase(0, vazio + 1); // Limpa a string até o primeiro espaço vazio

        while (!linha.empty() && isspace(linha[0])) { // enquanto tiver espaço antes da string, apaga
            linha.erase(0, 1);
        }
    }

    for (int j = 0; linha[j]; j++) { // percorre cada caracter da linha 
        if (isdigit(linha[j]) && linha[j - 1] != '$' && linha[j - 2] != '$') { // evita de pegar constante de instruções que tem número mesmo o tipo R não tendo
            constante = linha[j]; // pega a constante
            linha[j] = '\0'; // coloca o caracter nulo no lugar da constante

            if (isdigit(linha[j + 1])) // verifica se o próximo também não é uma constante caso tenha tipo 14
            {
                constante += linha[j + 1]; // concatena a constante
                linha[j] = '\0'; // coloca o caracter nulo no lugar da constante
                break; // sai do laço
            }
            break;
        }
    }

    // Se a instrução for do tipo R e for jr, mfhi ou mflo, não precisa pegar o registrador rd
    if (nome != "jr" && nome != "mult" && nome != "multu" && nome != "div" && nome != "divu") {

        rd.assign(linha, 0, linha.find(' '));

        linha.erase(0, linha.find(' ') + 1);

        while (!linha.empty() && isspace(linha[0])) {
            linha.erase(0, 1);
        }
    }

    // Se a instrução for do tipo R e for mfhi ou mflo, não precisa pegar os registradores rs e rt
    if (nome != "mfhi" && nome != "mflo" && nome != "sll" && nome != "srl") {
        rs.assign(linha, 0, linha.find(' '));

        linha.erase(0, linha.find(' ') + 1);

        while (!linha.empty() && isspace(linha[0])) {
            linha.erase(0, 1);
        }
    }

    // Se a instrução for do tipo R e for sll ou srl, não precisa pegar o registrador rs
    if (nome != "jr" && nome != "mfhi" && nome != "mflo") {
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
    else {
        binario.append(codRegistrador(rs, regis));
    }

    if (rt.empty())
    {
        binario.append("00000");
    }
    else {
        binario.append(codRegistrador(rt, regis));
    }

    if (rd.empty())
    {
        binario.append("00000");
    }
    else {
        binario.append(codRegistrador(rd, regis));
    }

    if (constante.empty()) { // se não tiver constante, coloca 0
        bitset<5> immediate(0);
        binario.append(immediate.to_string());
    }
    else { // se tiver constante, coloca ela
        int immediateValue = stoi(constante);
        bitset<5> immediate(immediateValue);
        binario.append(immediate.to_string());
    }

    binario.append(funct); // coloca o funct

    bitset<32>binary(binario); // converte a string binario para bitset de 32 bits

    return binary;
}

bitset<32> convertTypeI(string str, instruction* ins, registradores* regis) {
    string name{ ins->name }, opcode{ ins->opcode }, rs{ "00000" }, rt{ "00000" };
    str.erase(0, str.find('$'));    // apaga o nome da instrucao completa

    // verifica qual instrucao a ser codificada
    if (name == "beq" || name == "bne") {
        rs.assign(str, str.find('$'), str.find(',') - str.find('$'));
        str.erase(0, str.find(',') + 1);
        rt.assign(str, str.find('$'), str.find(',') - str.find('$'));
        str.erase(0, str.find(',') + 1);
    }
    else if (name == "lui") {
        rt.assign(str, str.find('$'), str.find(',') - str.find('$'));
        str.erase(0, str.find(',') + 1);
    }
    else if (name == "lw" || name == "sw") {
        rt.assign(str, str.find('$'), str.find(',') - str.find('$'));
        str.erase(0, str.find(',') + 1);
        rs.assign(str, str.find('$'), str.find(')') - str.find('$'));
        str.erase(str.find('('), str.find(')') + 1);
    }
    else {
        rt.assign(str, str.find('$'), str.find(',') - str.find('$'));
        str.erase(0, str.find(',') + 1);
        rs.assign(str, str.find('$'), str.find(',') - str.find('$'));
        str.erase(0, str.find(',') + 1);
    }

    while (str.front() == ' ') {    // elimina os espacos
        str.erase(0, 1);
    }

    bitset<16> immediate;   // declara o valor imediato de 16 bits
    if (str.front() == '-' || isdigit(str.front())) {     // converte para inteiro se for um numero
        immediate = stoi(str);
    }
    else {      // se for uma label, pega o endereco da label e subtrai do endereco da instrucao atual
        for (int i = 0; i < tam; i++) {
            if (labels[i].name == str) {
                immediate = labels[i].address - ins->endereco;
            }
        }
    }

    str.clear();                            // limpa string
    str.append(opcode);                     // concatena o opcode
    str.append(codRegistrador(rs, regis));  // concatena o endereco do registrador rs
    str.append(codRegistrador(rt, regis));  // concatena o endereco do registrador rt
    str.append(immediate.to_string());      // concatena o valor imediato, seja um numero ou endereco de label

    bitset<32> bit(str);
    return bit;
}

bitset<32> convertTypeJ(string str, instruction* ins, registradores* regis) {
    string endereco{}, opcode{ ins->opcode }, pc{ "000001" };
    bitset<20> endLabel;    // endereco da label do jump

    endereco.append(opcode);
    endereco.append(pc);

    str.erase(0, str.find(' '));
    while (str.front() == ' ') {    // elimina os espacos
        str.erase(0, 1);
    }

    for (int i = 0; i < tam; i++)   // busca a label correspondente e pega seu endereco
        if (labels[i].name == str)
            endLabel = labels[i].address;

    endereco.append(endLabel.to_string());  // converte o endereco da label para string e concatena em endereco

    bitset<32> bit(endereco);   // converte string endereco em binario de 32 bits
    return bit;
}

string codRegistrador(string r, registradores* regis) {
    // procura e retorna o codigo binario do registrador
    if (r != "00000") {
        for (int i = 0; i < 32; i++) {
            if (r == regis[i].nome) {   // procura por nome ($t0, $t1...)
                return regis[i].codigo;
            }
            else if (r == regis[i].numero) {    // procura por numero ($8, $9...)
                return regis[i].codigo;
            }
        }
    }
    return r;
}