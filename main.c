#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

const char* turkish_characters = "çÇþÞÝöÖðÐüÜý";

int isTurkishChar(char c) {
	for (int i = 0; turkish_characters[i] != '\0'; i++) {
		if (c == turkish_characters[i]) {
			return 1;
		}
		return 0;
	}
}

int main() {
	FILE* file;
	char c;
	char *input;
	char* filename = "test.txt";
	int wordCtr = 0, punctCtr = 0, lineCtr = 1,sentenceCtr = 0 ;
	int sWordCtr = 0;
	int inputLen = 0;

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

	char* buffer = (char*)malloc(sizeof(char) * (length)); //sentenceCtr -> buffer[i+2]kontrolü için +3
	if (buffer == NULL) {
		printf("Memory allocation failed\n");
		return -1;
	}
	//Buffer'ý dosyadaki charlardan oku
	int k = 0;
	while ((c = fgetc(file)) != EOF) {
		buffer[k] = c;
		k++;
	}
	buffer[k] = '\0';

	for (int i =0; i< length && buffer[i] != '\0'; i++) {
		if (isTurkishChar(buffer[i])) {
			//Turkce karakter iceren kelimeyi pas gececek
			while (buffer[i] != ' ' && buffer[i] != '\0') {
				i++;
			}
			continue;
		}
		else if (buffer[i] == ' ') {
			wordCtr++;
		}
		else if (ispunct(buffer[i])) {
			punctCtr++;
			
		}
		else if (buffer[i] == '\n') {
			lineCtr++;
			wordCtr++;
		}
		else if (i>= 2 && buffer[i - 2] == '.' && isupper(buffer[i])) {
			sentenceCtr++;
		}
		else if (i == 0 && isupper(buffer[i])) {
			sentenceCtr++;

		}

		if (strncmp(&buffer[i], input, inputLen) == 0 && (buffer[i + inputLen] == ' ' || buffer[i + inputLen] == '.' || buffer[i + inputLen] == '\0' || buffer[i + inputLen] == '\n')) {
			sWordCtr++;
		} 
	}
	
	printf("Word Count: %d \n", wordCtr);
	printf("Punctuation Count: %d\n", punctCtr);
	printf("Line count: %d\n", lineCtr);
	printf("Searched word count:%d\n", sWordCtr);
	printf("Sentence count: %d\n", sentenceCtr);
	fclose(file);
	free(input);
	free(buffer);
}