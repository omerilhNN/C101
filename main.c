#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

const char* turkish_characters = "çÇþÞÝöÖðÐüÜý";

int isTurkishChar(char c) {
	for (int i = 0; turkish_characters[i] != '\0'; i++) {
		if (c == turkish_characters[i]) {
			return 1;
		}
	}
	return 0;
}

int main() {
	FILE* file;
	char c;
	char *input;
	char* filename = "test.txt";
	int wordCtr = 0, punctCtr = 0, lineCtr = 0, sentenceCtr = 0, sWordCtr = 0, inputLen = 0;

	input = (char*)malloc(sizeof(char)* 101);
	printf("Enter a string that you want to search:");
	fgets(input, 101, stdin);
	input[strcspn(input, "\n")] = '\0';
	inputLen = strlen(input);

	if (fopen_s(&file, filename, "r") != 0) {
		printf("Error opening file\n");
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	//file'ý tutmak için buffer
	char* buffer = (char*)malloc(sizeof(char) * (length +1 )); 
	if (buffer == NULL) {
		printf("Memory allocation failed\n");
		return -1;
	}
	//Buffer'ý dosyadaki charlardan oku
	int k = 0;
	while ((c = fgetc(file)) != EOF && k < length) {
		buffer[k++] = c;
		if (c == '\n') {
			lineCtr++;
		}
	}
	buffer[k] = '\0'; // Son char null-terminated
	char* temp_buffer = strdup(buffer); //Temp -> strtok_s() buffer'ýn orjinal deðerine etki ediyordu for döngüsünü bozuyordu

	
	char** sentences = (char**)malloc(sizeof(char*) * BUFFER_SIZE);
	if (sentences == NULL) {
		printf("Memory allocation failed for sentences\n");
		return -1;
	}

	char* context = NULL;
	char* token = strtok_s(temp_buffer, ".!?", &context);
	int sentenceIndex = 0;
	while (token != NULL && sentenceIndex < BUFFER_SIZE) {
		// 1. durum: (\0 ile baþlama + new line ya da boþluk + 1.indeksteki eleman büyük mü deðil mi)  2.durum: ilk cümle için büyük harfle baþlýyor mu kontrolü
		if ((*token != '\0' && ((*token == '\n' || isspace(*token))) && isupper(*(token+1))) || (sentenceIndex ==0 && isupper(*token)) ) {
			sentences[sentenceIndex] = (char*)malloc(strlen(token) + 1);
			if (sentences[sentenceIndex] == NULL) {
				printf("Memory allocation failed for sentence\n");
				return -1;
			}
			strcpy_s(sentences[sentenceIndex], strlen(token) + 1, token);
			sentenceIndex++;
		}
		token = strtok_s(NULL, ".!?", &context);//iterate next sentence
	}
	sentenceCtr = sentenceIndex;

	for (int i =0; i< length && buffer[i] != '\0'; i++) {
		//Turkce karakter iceren kelimeyi pas gececek
		if (isTurkishChar(buffer[i])) {
			while (buffer[i] != ' ' && buffer[i] != '\0') {
				i++;
			}
			continue;
		}
		// Boþluklarý, noktalama iþaretlerini ve yeni satýrlarý kontrol et
		if (isspace(buffer[i]) || ispunct(buffer[i]) || buffer[i] == '\n') {
			if (i> 0 && !isspace(buffer[i - 1]) && !ispunct(buffer[i - 1])) {
				wordCtr++; // Kelime sayýsýný artýr
			}
			if (ispunct(buffer[i])) {
				punctCtr++; // Noktalama iþareti sayýsýný artýr
			}
		}
		

		if (strncmp(&buffer[i], input, inputLen) == 0 && (buffer[i + inputLen] == ' ' || ispunct(buffer[i + inputLen]) || buffer[i + inputLen] == '\0' || buffer[i + inputLen] == '\n')) {
			sWordCtr++;
		} 
		if (i == length - 1) {
			//son karakter eðer harf ise
			if (!isspace(buffer[i]) && !ispunct(buffer[i])) {
				wordCtr++;
			}

			// Son karakter noktalama ise
			if (ispunct(buffer[i])) {
				punctCtr++;
			}

		}
		
	}
	
	
	printf("Word Count: %d \n", wordCtr);
	printf("Punctuation Count: %d\n", punctCtr);
	printf("Line count: %d\n", lineCtr +1 );
	printf("Searched word count:%d\n", sWordCtr);
	printf("Sentence count: %d\n", sentenceCtr);

	fclose(file);
	free(input);
	free(buffer);
	free(temp_buffer);
}