#define _CRT_SECURE_NO_WARNINGS
#include "mensagem.h"

int nseq_dados_CLP[2] = { 0,0 };

int nseq_alarme[2] = { 0,0 };

//	Função para gerar números aleatórios inteiros dentro de um intervalo definido 
int RandomInt(int low, int high)
{
	return (rand() % (high - low + 1)) + low;
}

void GeraMensagemDados(char* msg, int nCLP)//o \0 final jah eh adicionado pelo sprintf do timestamp
{
	//Init char array
	strcpy(msg, "");
	//NSEQ
	char nseqDados[7];
	GetNseqDados(nseqDados, nCLP);
	strcat(msg, nseqDados);
	strcat(msg, "|");
	//TAG
	char tag[8];
	GetTAG(tag);
	strcat(msg, tag);
	strcat(msg, "|");
	//TIPO
	char tipo[3];
	tipo[0] = '0';
	tipo[1] = '0';
	tipo[2] = '\0';
	strcat(msg, tipo);
	strcat(msg, "|");
	//LINHA
	char linha[2];
	linha[0] = '0' + nCLP;
	linha[1] = '\0';
	strcat(msg, linha);
	strcat(msg, "|");
	//VALOR
	char valor[8];
	GetVALOR(valor);
	strcat(msg, valor);
	strcat(msg, "|");
	//Time Stamp
	char time_stamp[9];
	GetTimeStamp(time_stamp);
	strcat(msg, time_stamp);
}

void GeraMensagemAlarmes(char* msg, int nCLP)
{
	//Init char array
	strcpy(msg, "");
	//NSEQ
	char nseqAlarme[7];
	GetNseqAlarmes(nseqAlarme, nCLP);
	strcat(msg, nseqAlarme);
	strcat(msg, "|");
	//TAG
	char tag[8];
	GetTAG(tag);
	strcat(msg, tag);
	strcat(msg, "|");
	//TIPO
	char tipo[3];
	tipo[0] = '1';
	tipo[1] = '1';
	tipo[2] = '\0';
	strcat(msg, tipo);
	strcat(msg, "|");
	//LINHA
	char linha[2];
	linha[0] = '0' + nCLP;
	linha[1] = '\0';
	strcat(msg, linha);
	strcat(msg, "|");
	//TEXTO
	char texto[31];
	GetTEXTO(texto);
	strcat(msg, texto);
	strcat(msg, "|");
	//Time Stamp
	char time_stamp[9];
	GetTimeStamp(time_stamp);
	strcat(msg, time_stamp);
}


void GetTAG(char* tag)
{
	tag[0] = 'A' + rand() % 24;
	tag[1] = 'A' + rand() % 24;
	tag[2] = 'A' + rand() % 24;
	tag[3] = '-';
	tag[4] = '0' + RandomInt(0, 9);
	tag[5] = '0' + RandomInt(0, 9);
	tag[6] = '0' + RandomInt(0, 9);
	tag[7] = '\0';

}

void GetVALOR(char* prod)
{
	prod[0] = '0' + RandomInt(0, 9);
	prod[1] = '0' + RandomInt(0, 9);
	prod[2] = '0' + RandomInt(0, 9);
	prod[3] = '0' + RandomInt(0, 9);
	prod[4] = '0' + RandomInt(0, 9);
	prod[5] = '.';
	prod[6] = '0' + RandomInt(0, 9);
	prod[7] = '\0';
}

void GetTEXTO(char* texto)
{
	int i;
	for (i = 0; i < 30; i++) {
		texto[i] = 'A' + rand() % 24;
	}
	texto[30] = '\0';
}


void GetNseqDados(char* nseq, int nCLP)
{
	int indice = nCLP - 1;

	sprintf(nseq, "%06d", nseq_dados_CLP[indice]);

	nseq_dados_CLP[indice]++;

	if (nseq_dados_CLP[indice] > 999999)
		nseq_dados_CLP[indice] = 0;
}

void GetNseqAlarmes(char* nseq, int nCLP)
{
	int indice = nCLP - 1;

	sprintf(nseq, "%06d", nseq_dados_CLP[indice]);

	nseq_dados_CLP[indice]++;

	if (nseq_dados_CLP[indice] > 999999)
		nseq_dados_CLP[indice] = 0;
}

void GetTimeStamp(char *time_stamp)
{
	SYSTEMTIME tempo;	//Variável para obtenção da hora corrente do sistema 
	GetLocalTime(&tempo); //obtém a hora atual do sistema

						  /*	Define o campo Time_Stamp da mensagem	*/

	sprintf(time_stamp, "%02d:%02d:%02d", tempo.wHour, tempo.wMinute, tempo.wSecond);
}