# 🖥️ Programa de Montador em C++

Este é um programa de montador desenvolvido na disciplina de **Arquitetura de Organizações de Computadores** na **UFERSA** (Universidade Federal Rural do Semi-Árido). O objetivo do programa é converter um código assembly em um formato binário que pode ser processado por um computador.

## 📜 Funcionalidades

- **Leitura de Código Assembly**: O programa lê um arquivo `.asm` que contém instruções em código assembly.
- **Conversão de Instruções**: Converte instruções de diferentes tipos (R, I, J) para o formato binário apropriado.
- **Geração de Arquivo de Saída**: O resultado da conversão é salvo em um arquivo chamado `assembler.hex`.

## ⚙️ Estruturas de Dados

### Registradores

```cpp
struct registradores {
    string nome;
    string numero;
    string codigo;
};
```

### Instruções

```cpp
struct instruction {
    string name;
    bitset<32>(*type)(string, instruction*, registradores*);
    string opcode;
    int endereco;
};
```

### Labels

```cpp
struct label {
    string name;
    int address;
};
```

## 📂 Como Usar

1. **Clone o repositório**:

   ```bash
   git clone https://github.com/BrenoKly/Arq.Organizacao_Computadores.git
   cd AssemblerMips.git
   ```

2. **Prepare o arquivo de entrada**:

   - Crie um arquivo chamado `main.asm` no diretório do projeto com as instruções em assembly.

3. **Compile o programa**:

   ```bash
   g++ -o assembler main.cpp
   ```

4. **Execute o programa**:

   ```bash
   ./assembler
   ```

5. **Verifique a saída**:
   - O arquivo `assembler.hex` será criado com o código binário.

## 🛠️ Bibliotecas Usadas

- `<iostream>`: Para operações de entrada e saída.
- `<string>`: Para manipulação de strings.
- `<bitset>`: Para manipulação de números binários.
- `<fstream>`: Para operações de arquivo.

---

# Biblioteca de Manipulação de Strings para MIPS em Assembler

Este repositório também contém uma biblioteca de manipulação de strings em MIPS, que está localizada no package `BibliotecaString`. As funções criadas são:

### Funções Implementadas

- **Strlen**: Recebe um ponteiro de uma string e retorna a quantidade de caracteres.
- **Strcmp**: Compara duas strings e retorna um inteiro.
  - Se for negativo, a primeira string é menor que a segunda.
  - Se for positivo, a primeira string é maior que a segunda.
  - Se for zero, as strings são idênticas.
- **Strcat**: Concatena a string `src` ao final da string `dest`.

- **Strncat**: Concatena uma determinada quantidade da string `src` no final da string `dest`.

- **Strncpy**: Faz a mesma coisa que a `strcpy`, mas copia apenas os primeiros `count` elementos da `src` para a `dest`.

## 🙏 Agradecimentos

Agradeço ao meu professor e colegas da UFERSA pela orientação e suporte durante o desenvolvimento deste projeto!
