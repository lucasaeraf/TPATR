/*
Automação em Tempo Real - 2017 / 1
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
#include "mensagem.h"

// Casting para terceiro e sexto parâmetros da função _beginthreadex
typedef unsigned (WINAPI *CAST_FUNCTION)(LPVOID);
typedef unsigned *CAST_LPDWORD;

// Define
#define NThreads 2							// Número de Threads criadas por este processo

HANDLE hEscEvent;
HANDLE hBloqueioCLPs[2];

// Declaração da função
DWORD WINAPI CLP(int);


int nTecla;

int main() {
	HANDLE hThreadsCLPs[NThreads];

	DWORD dwThreadsCLPsId;
	DWORD dwExitCode = 0;
	int i;
	BOOL status;

	hEscEvent = OpenEvent(
		EVENT_ALL_ACCESS,				//	_In_ DWORD   dwDesiredAccess,
		FALSE,							//	_In_ BOOL    bInheritHandle,
		"hEscEvent"						//	_In_ LPCTSTR lpName
	);
	if (!hEscEvent) {
		printf("Erro na criacao do Evento para finalizacao das tarefas. Codigo de erro = %d.\n", GetLastError());
		exit(0);
	}

	hBloqueioCLPs[0] = OpenEvent(
		EVENT_ALL_ACCESS,				//	_In_ DWORD   dwDesiredAccess,
		FALSE,							//	_In_ BOOL    bInheritHandle,
		"hBloqueioCLP1"					//	_In_ LPCTSTR lpName
	);
	if (!hBloqueioCLPs[0]) {
		printf("Erro na criacao do Evento para bloqueio do CLP1. Codigo de erro = %d.\n", GetLastError());
		exit(0);
	}

	hBloqueioCLPs[1] = OpenEvent(
		EVENT_ALL_ACCESS,				//	_In_ DWORD   dwDesiredAccess,
		FALSE,							//	_In_ BOOL    bInheritHandle,
		"hBloqueioCLP2"					//	_In_ LPCTSTR lpName
	);
	if (!hBloqueioCLPs[1]) {
		printf("Erro na criacao do Evento para bloqueio do CLP2. Codigo de erro = %d.\n", GetLastError());
		exit(0);
	}

	for (i = 0; i<NThreads; i++) {
		hThreadsCLPs[i] = (HANDLE)_beginthreadex(
			NULL,
			0,
			(CAST_FUNCTION)CLP,	//Casting necessário
			(LPVOID)i,
			0,
			(CAST_LPDWORD)&dwThreadsCLPsId);		//Casting necessário
		if (hThreadsCLPs[i])
			continue;
		else {
			printf("Erro na criacao da thread CLP N = %d Erro = %d\n", i, errno);
			exit(0);
		}
	}

	status = WaitForSingleObject(hEscEvent, INFINITE);
	CloseHandle(hEscEvent);
	for (i = 0; i<NThreads; i++) {
		GetExitCodeThread(hThreadsCLPs[i], &dwExitCode);
		CloseHandle(hThreadsCLPs[i]);
		CloseHandle(hBloqueioCLPs[i]);
	}
	return 0;
}//main

DWORD WINAPI CLP(int i) {
	//DWORD status;
	HANDLE processoTimer;
	DWORD Wait, Wait2;	//Variável para retorno do WaitForMultipleObjects 
	int AlarmeDados;

	processoTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	LARGE_INTEGER preset;
	HANDLE VetorHandles_Bloqueio[3] = {hEscEvent, hBloqueioCLPs[i], processoTimer};
	HANDLE VetorHandles_Bloqueio2[2] = {hEscEvent, hBloqueioCLPs[i]};

	char msgAlarme[59];
	char msgDados[36];

	preset.QuadPart = -(1000 * 10000 * (i+1));
	printf("Timeout %d %d \n", i, 1000 * 10000 * (i+1));
	SetWaitableTimer(processoTimer, &preset, 1000 * (i+1), NULL, NULL, FALSE);

	while (1) {
		Wait = WaitForMultipleObjects(3, VetorHandles_Bloqueio, FALSE, INFINITE);

		if (Wait == WAIT_OBJECT_0)
			break;				// Encerrar foi sinalizado

		else if (Wait == WAIT_OBJECT_0 + 1) { //Bloqueio foi sinzalizado
			Wait2 = WaitForMultipleObjects(2, VetorHandles_Bloqueio2, FALSE, INFINITE);
			if (Wait2 == WAIT_OBJECT_0)
				break;				// Encerrar foi sinalizado
			else if (Wait2 == WAIT_OBJECT_0 + 1) {		// Bloqueio_CLP foi sinalizado
				continue;
			}
		}

		else if (Wait == WAIT_OBJECT_0 + 2) {		//Temporizador sinalizado
			AlarmeDados = rand() % 2;
			if (AlarmeDados == 0) {// Mensagem de alarme
				GeraMensagemAlarmes(msgAlarme, i + 1);
				printf("Sou o CLP %d e escrevi uma mensagem de alarme\n", i + 1);
				printf(msgAlarme);
				printf("\n");
			}
			else if (AlarmeDados == 1) {// Mensagem de dados
				GeraMensagemDados(msgDados, i + 1);
				printf("Sou o CLP %d e escrevi uma mensagem de dados\n", i + 1);
				printf(msgDados);
				printf("\n");
			}
		}
	}

	CloseHandle(processoTimer);
	_endthreadex(0);
	return(0);


}