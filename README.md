# VMTranslator

## Objective

The project aims to create a translator that translate Virtual Machine (VM) language into Hack assembly language.

## Project Structure

The project is divided into the following main files:

- **main.c**: The main program that manages file input/output and coordinates the translation.
- **setup.c**: Functions for handling input files, including extension verification and output file name creation.
- **translator.c**: Contains functions for translating VM instructions into assembly code, including arithmetic commands, flow control, and stack operations.
- **setup.h**: Function declarations for file management.
- **translator.h**: Function declarations for VM-to-ASM translation.
- **Makefile**: The build file for compiling the project.

## How It Works

1. The program takes a `.vm` file as input, containing Virtual Machine language code.
2. The file is read line by line, and each instruction is interpreted and translated into Hack assembly language.
3. The result is written to an output file with a `.asm` extension.
4. The translator handles various types of instructions, including:
   - Arithmetic and logical operations (`add`, `sub`, `and`, `or`, `neg`, etc.)
   - Flow control commands (`label`, `goto`, `if-goto`)
   - Function calls and returns (`function`, `call`, `return`)
   - Stack operations (`push`, `pop`)

## Compilation and Execution

### Compilation using Makefile

To compile the project using the provided Makefile, simply run:

```sh
make main
```

This will generate the `VMtranslator` executable.

### Manual Compilation

If you prefer to compile manually, run the following command:

```sh
gcc -o VMtranslator main.c setup.c translator.c
```

## Usage

To use the translator, execute the compiled program with a `.vm` file as input:

```sh
./VMtranslator filename.vm
```

------

# VMTranslator

## Obiettivo

Il progetto ha come obiettivo la creazione di un traduttore che converte il linguaggio di macchina virtuale (VM) in linguaggio di assemblaggio Hack.

## Struttura del Progetto

Il progetto è suddiviso nei seguenti file principali:

- **main.c**: Il programma principale che gestisce l'input/output dei file e coordina la traduzione.
- **setup.c**: Funzioni per la gestione dei file di input, inclusa la verifica dell'estensione e la creazione del nome del file di output.
- **translator.c**: Contiene le funzioni per la traduzione delle istruzioni VM in codice assembly, inclusi i comandi aritmetici, di controllo del flusso e le operazioni di stack.
- **setup.h**: Dichiarazioni delle funzioni di gestione dei file.
- **translator.h**: Dichiarazioni delle funzioni di traduzione VM-to-ASM.
- **Makefile**: Il file di build per la compilazione del progetto.

## Funzionamento

1. Il programma prende in input un file `.vm` che contiene il codice del linguaggio di macchina virtuale.
2. Il file viene letto riga per riga, e ogni istruzione viene interpretata e tradotta in linguaggio di assembly Hack.
3. Il risultato viene scritto in un file di output con estensione `.asm`.
4. Il traduttore gestisce vari tipi di istruzioni, tra cui:
   - Operazioni aritmetiche e logiche (`add`, `sub`, `and`, `or`, `neg`, ecc.)
   - Comandi di flusso di controllo (`label`, `goto`, `if-goto`)
   - Chiamate di funzione e ritorni (`function`, `call`, `return`)
   - Operazioni di stack (`push`, `pop`)

## Compilazione ed Esecuzione

### Compilazione con Makefile

Per compilare il progetto utilizzando il Makefile fornito, eseguire:

```sh
make main
```

Questo genererà l'eseguibile `VMtranslator`.

### Compilazione Manuale

Se si preferisce compilare manualmente, eseguire il seguente comando:

```sh
gcc -o VMtranslator main.c setup.c translator.c
```

### Utilizzo

Per utilizzare il traduttore, eseguire il programma compilato con un file `.vm` come input:

```sh
./VMtranslator filename.vm
```
