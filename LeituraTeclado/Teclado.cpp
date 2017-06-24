/*
Automação em Tempo Real - 2017/1
Lucas Aeraf -
Luciana Botelho - 2012016736
*/

// Include
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h> 
#include <conio.h> //_getch
// Define
#define ESC 0x1B

// Handles globais
HANDLE hEscEvent;

DWORD wait;


int main() {
	char tecla;		// Caractere digitado	

	// Handle para entrada do teclado
	HANDLE hstdin;

	// Handle para bloqueio dos CLPs
	HANDLE hBloqueioCLPs[2];

	SetConsoleTitle("Tratamento de entrada do teclado");

	BOOL status;

	//Variaveis de Controle Para o Status
	int Status_CLP1 = 0, Status_CLP2 = 0;

	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	if (!hstdin) {
		printf("Erro na criacao do handle para ler teclado. Codigo de erro = %d.\n", GetLastError());
		exit(0);
	}

	// Evento ativado pela tecla ESC
	hEscEvent = CreateEvent(NULL, TRUE, FALSE, "hEscEvent");
	if (!hEscEvent) {
		printf("Erro na criacao do Evento para finalizacao das tarefas. Codigo de erro = %d.\n", GetLastError());
		exit(0);
	}

	hBloqueioCLPs[0] = CreateEvent(NULL, FALSE, FALSE, "hBloqueioCLP1");
	if (!hBloqueioCLPs[0]) {
		printf("Erro na criacao do Evento para bloqueio do CLP1. Codigo de erro = %d.\n", GetLastError());
		exit(0);
	}
	hBloqueioCLPs[1] = CreateEvent(NULL, FALSE, FALSE, "hBloqueioCLP2"); //se quiser que comece sinalizado, 2 false é true
	if (!hBloqueioCLPs[1]) {
		printf("Erro na criacao do Evento para bloqueio do CLP2. Codigo de erro = %d.\n", GetLastError());
		exit(0);
	}
	
	// Criando os processos
	STARTUPINFO si;				
	PROCESS_INFORMATION pi;		
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	status = CreateProcess("..\\Debug\\CLPs.exe",			// Caminho do executável
		NULL,														// Parâmetros da linha de comando
		NULL,														// Descritor de segurança
		NULL,														// Apontador para threads do processo
		FALSE,														// Herança dos Handles do processo 
		CREATE_NEW_CONSOLE,											//TODO: trocar para Sem janela de console
		NULL,														// Herança ambiente
		"..\\Debug\\",												// Diretório do executável
		&si,														// lpStartUpInfo
		&pi);												// lpProcessInfo
	if (!status) {
		printf("Erro na criacao do Processo \"CLPs\". Codigo de erro: %d.\n", GetLastError());
		_getch();
		exit(0);
	}

	// Definição da tela de comandos
	printf("*****************************************************************");
	printf("\n*\t\tTRABALHO FINAL: AUTOMACAO EM TEMPO REAL 2017-1\t\t*\n");
	printf("\n*\t\t\tLucas Aeraf\t\t\t*\n");
	printf("\n*\t\t\tLuciana Botelho\t\t\t*\n");
	printf("*****************************************************************\n\n");
	printf("Digite o comando desejado:\n\n");
	printf("'1': Bloqueia/desbloqueia a Tarefa de Leitura do CLP #1\n");
	printf("'2': Bloqueia/desbloqueia a Tarefa de Leitura do CLP #2\n");
	printf("'u': Bloqueia/desbloqueia a Tarefa de Leitura de Mensagens #1\n");
	printf("'d': Bloqueia/desbloqueia a Tarefa de Leitura de Mensagens #2\n");
	printf("'a': Bloqueia/desbloqueia a Tarefa de Exibição de Alarmes\n");
	printf("'p': Bloqueia/desbloqueia a Tarefa de Exibicao de Dados do Processo\n");
	printf("'c': Notifica Tareda de Exibição de Alarmes para limpar sua janela de console\n");
	printf("'ESC': Encerra a execucao do programa.\n");

	do {
		wait = WaitForSingleObject(hstdin, INFINITE);
		if (wait == WAIT_OBJECT_0) {
			tecla = _getch();
			tecla = tolower(tecla);
			switch (tecla) {
			case '1': 
				status = SetEvent(hBloqueioCLPs[0]);
				if (!status) {
					printf("Erro ao tentar bloquear/desbloquear o CLP #1. Codigo de erro: %d.\n", GetLastError());
					exit(0);
				}
				Status_CLP1 = (Status_CLP1 + 1) % 2;
				if (Status_CLP1 == 1) printf("\nTarefa de Leitura do CLP #1 bloqueada.\n");
				else printf("\nTarefa de Leitura do CLP #1 desbloqueada.\n");
				break;
			case '2': 
				status = SetEvent(hBloqueioCLPs[1]);
				if (!status) {
					printf("Erro ao tentar bloquear/desbloquear o CLP #2. Codigo de erro: %d.\n", GetLastError());
					exit(0);
				}
				Status_CLP2 = (Status_CLP2 + 1) % 2;
				if (Status_CLP2 == 1) printf("\nTarefa de Leitura do CLP #2 bloqueada.\n");
				else printf("\nTarefa de Leitura do CLP #2 desbloqueada.\n");
				break;
			case 'u': 
				printf("apertei u");
				break;
			case 'd': 
				printf("apertei d");
				break;
			case 'a': 
				printf("apertei a");
				break;
			case 'p': 
				printf("apertei p");
				break;
			case 'c': 
				printf("apertei c");
				break;
			case ESC: 
				status = SetEvent(hEscEvent);
				if (!status) {
					printf("Erro ao tentar sinalizar o encerramento das tarefas. Codigo de erro: %d.\n", GetLastError());
					//_getch();
					exit(0);
				}
				printf("\nEncerrando Tarefas...\n\n");
				break;

			default:								// Caso não seja digitada uma tecla válida
				printf("\n\n\a*Comando invalido.*\n\n");
				printf("*********************************************************\n\n");
				printf("Digite o comando desejado:\n\n");
				printf("'1': Bloqueia/desbloqueia a Tarefa de Leitura do CLP #1\n");
				printf("'2': Bloqueia/desbloqueia a Tarefa de Leitura do CLP #2\n");
				printf("'u': Bloqueia/desbloqueia a Tarefa de Leitura de Mensagens #1\n");
				printf("'d': Bloqueia/desbloqueia a Tarefa de Leitura de Mensagens #2\n");
				printf("'a': Bloqueia/desbloqueia a Tarefa de Exibição de Alarmes\n");
				printf("'p': Bloqueia/desbloqueia a Tarefa de Exibicao de Dados do Processo\n");
				printf("'c': Notifica Tareda de Exibição de Alarmes para limpar sua janela de console\n");
				printf("'ESC': Encerra a execucao do programa.\n");
				break;
			}
		}
	} while (tecla != ESC);

	// Fechando os Handles abertos
	CloseHandle(hEscEvent);
	int i;
	for (i = 0; i<2; i++) CloseHandle(hBloqueioCLPs[i]);

	printf("Programa encerrado com sucesso.\n");
	return EXIT_SUCCESS;


}