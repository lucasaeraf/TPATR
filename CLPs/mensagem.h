#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>
//gera numero inteiro aleatorio dentro do intervalo
int RandomInt(int low, int high);
//gera string de mensagem de dados
void GeraMensagemDados(char* msg, int nCLP);
//gera string de mensagem de alarme
void GeraMensagemAlarmes(char* msg, int nCLP);
//gera uma tag de processo aleatoria, no formato AAA-000
void GetTAG(char* tag);
//gera um numero aleatorio para o campo valor, 1 numero em notação de ponto flutuante 00000.0
void GetVALOR(char* prod);
//AINDA TEM QUE REFAZER COM OPÇOES QUE EXISTEM
void GetTEXTO(char* texto);
//gera Nseq para os Dados
void GetNseqDados(char *nseq, int nCLP);
//gera Nseq para os Alarmes
void GetNseqAlarmes(char *nseqm, int nCLP);
//obtem Time Stamp
void GetTimeStamp(char *time_stamp);
#pragma once
