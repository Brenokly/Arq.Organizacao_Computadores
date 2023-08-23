#include <iostream>
using namespace std;

struct instruction {
	string name;
	char type;
};

void bin_i(string ins) {
	cout << "convertendo tipo i...";
}

void bin_j(string ins) {
	cout << "convertendo tipo j...";
}

int main() {
	instruction* tipos = new instruction[13]{
		{"beq", 'i'}, {"bne", 'i'}, {"addi", 'i'}, {"addiu", 'i'}, {"slti", 'i'},
		{"sltiu", 'i'}, {"andi", 'i'}, {"ori", 'i'}, {"lui", 'i'}, {"lw", 'i'}, {"sw", 'i'},
		{"j", 'j'}, {"jal", 'j'}
	};
	
	string inst = "bne";
	for (int i = 0; i < 13; i++) {
		if (tipos[i].name == inst) {
			switch (tipos[i].type) {
			case 'i':
				bin_i(inst);
				break;
			case 'j':
				bin_j(inst);
				break;
			}
			break;
		}
	}

}