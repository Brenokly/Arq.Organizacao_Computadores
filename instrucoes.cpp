#include <iostream>
#include <string>
#include <bitset>
using namespace std;

int main() {
    std::string linha = "add $8, $9, 3", rs{}, rt{}, rd{}, constante{};

	// Encontra a posição do caractere ' '
    size_t vazio = linha.find('$');
    
    // Se o caractere '$' for encontrado, remove tudo antes dele
    if (vazio != string::npos) {
        linha.erase(0, vazio);

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

	cout << "linha inicial:" << linha << endl;

    rd.assign(linha, 0, linha.find('$')+2);

	cout << "RD:" << rd << endl;



    linha.erase(0, linha.find_first_not_of('$')+2); 

	while (!linha.empty() && isspace(linha[0])) {
    linha.erase(0, 1);
	}


	cout << "depois:" << linha << endl;

	rs.assign(linha, 0, linha.find('$')+2);

	cout << "RS:" << rs << endl;



	linha.erase(0, linha.find_first_not_of('$')+2); 

	while (!linha.empty() && isspace(linha[0])) {
    linha.erase(0, 1);
	}


	cout << "depois:" << linha << endl;

	rt.assign(linha, 0, linha.find_first_not_of('$')+2);

	cout << "RT:" << rt << endl;
	
	bitset<6> opcodeMul(28);
    
	cout << opcodeMul << endl;



    return 0;
}
