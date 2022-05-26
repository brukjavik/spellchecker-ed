#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#define WORD_SIZE 50
#define BUCKETS 50000

#define SPECIALCHARACTERS "[]{}()â€“@;Â§/â€?»:;¿ï½“ _*,.!\n"
#define buffersize 1000

typedef struct node { 
	char word[WORD_SIZE]; 
	struct node* next; 
} node;

// Declaração das variáveis globais;

node* hash_table[BUCKETS]; 
node* notFoundWords = NULL; 
unsigned int counterHowManyWords=0; 
unsigned int counterNotFoundWords=0; 
int numberOfCollisions=0;

// Declaração das funções;

unsigned int Hash_Function(const char* word);
double readText();
int verify(char* buffer);
void Load_Table(); 
void GenerateReport(double totalTime);
void PercorreBuckets();

int main(void) {

	setlocale(LC_ALL, "Portuguese");

	double totalTime;

	Load_Table(); 

	totalTime = readText(); 

	GenerateReport(totalTime); 
	
	PercorreBuckets();

	return 0;
}

void PercorreBuckets(){ // Função adicional para calcular as colisões em cada bucket
	
	int i=0, j=0;	
	FILE *bucketreport; 
	bucketreport = fopen("ColisoesporBucket.txt", "w"); 
	
	for(i=0; i<BUCKETS; i++){
		
		while(hash_table[i]->word!=NULL){
			
			if(j==0){ // Caso seja o primeiro elemento do bucket
				hash_table[i] = hash_table[i]->next;
			}
			else{ // Caso não seja o primeiro elemento do bucket
				numberOfCollisions++;
				hash_table[i] = hash_table[i]->next;
			}
			j++;
		}
		fprintf(bucketreport, "%d %d \n", i, numberOfCollisions);
		numberOfCollisions=0;
		j=0;
	}
	fclose(bucketreport);
}

unsigned int Hash_Function(const char* word) { // Função que calculará o valor hash de uma palavra recebida;
	int size = strlen(word); 
	int i=0;
	unsigned int value = 5381;

	for(i = 0; i < size; ++i) {
		value += (value << 5)  + word[i];
	}

	return value % BUCKETS; // Retorna o valor hash da palavra em questão;
}

double readText() { // Responsável pela leitura do texto e comparação contra o dicionário (com auxílio de verify)

	clock_t ini, end; 
	FILE* text; 
	char buffer[buffersize];
	char *CmpTxt;
	unsigned int hash;
	int i=0;

	text = fopen("text.txt", "r");

	if(text==NULL) printf("Erro na leitura do texto!\n"); 

	ini = clock();

	while(!feof(text)) {
		
		memset(buffer, buffersize, '\n');
		if(!fgets(buffer, buffersize, text)) break;
		
		CmpTxt = strtok(buffer, SPECIALCHARACTERS);
		
		while(CmpTxt!=NULL){		
			counterHowManyWords++;
		
			if(verify(CmpTxt)==0) {
				counterNotFoundWords++;
				node* no = malloc(sizeof(node));
				no->next=NULL;

				strcpy(no->word, CmpTxt);
				if(notFoundWords == NULL) { 
					notFoundWords=no; 
				} else { 
					no->next=notFoundWords;
					notFoundWords=no;
				}
				no = NULL;
				free(no);	
			}	
		CmpTxt = strtok(NULL, SPECIALCHARACTERS);
		}
	}
	
	end = clock();
	
	fclose(text);

	return ((double) (end - ini) ) / (CLOCKS_PER_SEC/1000);

}

int verify(char* buffer) { // Verifica se a palavra está no dicionário ou não

	int callFunction=0;
	node* aux;

	callFunction = Hash_Function(buffer);

	aux = hash_table[callFunction];
	while(aux!=NULL) {

		if(strcmp(buffer, aux->word)==0) {
			return 1;
		}

		aux = aux->next; 
	}
	return 0; 
}

void Load_Table() { // Responsável pela leitura do dicionário e geração da tabela hash;
	char buffer[WORD_SIZE];
	int i=0;
	FILE* dicio; 
	unsigned int bucket; 

	dicio = fopen("dicionario.txt", "r"); 

	if(dicio==NULL) {  
		printf("Erro ao carregar dicionário!\n");
	}

	for(i=0; i < BUCKETS; i++) {
		hash_table[i] = NULL; 
	}

	while(fscanf(dicio, "%s", buffer) > 0) { // Lê o dicionário palavra por palavra enquanto houver palavras não lidas;

		node* no = malloc(sizeof(node)); 
		no->next=NULL; 

		strcpy(no->word, buffer); 

		bucket = Hash_Function(no->word); // Calcula-se o valor do bucket com base no retorno da função hash da palavra;

		if(hash_table[bucket]->word == NULL) { 
			hash_table[bucket]=no; 
		} else { 
			no->next=hash_table[bucket]; 
			hash_table[bucket]=no; 
		}
	}

	fclose(dicio); 

}

void GenerateReport(double totalTime) { // Gera o relatório de saída com dados acerca do programa;

	int i;
	FILE *finalfile; 

	finalfile = fopen("Relatorio.txt", "w"); 

	fprintf(finalfile, "Número total de palavras do texto: %d\n", counterHowManyWords);
	fprintf(finalfile, "Tempo total da verificação: %.1lf ms\n", totalTime);
	fprintf(finalfile, "Número de palavras que falharam no spell check: %d\n", counterNotFoundWords);
	fprintf(finalfile, "Lista de palavras que falharam no spell check: \n\n");
	fprintf(finalfile, "Num. Ocorrencia - Palavra\n");
	fprintf(finalfile, "-----------------------------------------\n");

	for(i=1; i<=counterNotFoundWords; i++) { // Laço responsável por percorrer todas as palavras não encontradas;
		fprintf(finalfile, "%d - %s\n", i, notFoundWords->word);
		notFoundWords = notFoundWords->next;
	}

	fclose(finalfile); 

}
