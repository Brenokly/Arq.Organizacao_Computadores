#include <iostream>
#include <fstream>
using namespace std;

struct label {
	string name;
	int address;
};

int main() {
	ifstream fin("main.asm");

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
		if (*ch == ':') {					// se o último caractere for ':' guarda a label e o endereço
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

	// segunda leitura


	for (int i = 0; i < tam; i++) {
		cout << "Label " << labels[i].name;
		cout << " Address: " << labels[i].address << endl;
	}

}