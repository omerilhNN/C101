#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 512

int main() {
	FILE* file;
	char c;
	char *input;
	int i = 0;
	char* filename = "test.txt";
	int wordCtr = 0, punctCtr = 0, lineCtr = 1,sentenceCtr = 0 ;
	int sWordCtr = 0;
	int inputLen = 0;


	input = (char*)malloc(sizeof(char)* 101);
	printf("Enter a string that you want to search:");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = '\0';
	inputLen = strlen(input);
	

	if (fopen_s(&file, filename, "r") != 0) {
		printf("Error opening file\n");
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char*)malloc(sizeof(char) * (length + 2)); //sentenceCtr -> buffer[i+2]kontrolü için +3
	if (buffer == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	fread(buffer, sizeof(char), length, file);
	buffer[length-1] = '\0';

	for (int i =0; i< length; i++) {

		if (buffer[i] == ' ' || buffer[i] == '\0') {
			wordCtr++;
		}
		if (ispunct(buffer[i]) && buffer[i] != '.')
			punctCtr++;

		if (buffer[i] == '\n') {
			lineCtr++;
			wordCtr++;
		}
		if (strncmp(&buffer[i], input, inputLen) == 0 && (buffer[i + inputLen] == ' ' || buffer[i + inputLen] == '\0')) {
			sWordCtr++;
		}
		if (buffer[i] == '.') {
			punctCtr++;
			if (isupper(buffer[i + 2]) && buffer[i + 1] == ' ') {
				sentenceCtr++;
			}

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