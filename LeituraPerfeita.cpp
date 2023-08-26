#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

int main() {
    
    ifstream fin2;
	fin2.open("main.asm", ios_base::in);

	if (!fin2.is_open()) {
		cout << "Nao foi possivel abrir o arquivo.\n";
		exit(1);
	}

	string ins{},r{};
    int i = 1;

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

        cout << i++ << " " << r << endl;
    }
    

    return 0;
}
