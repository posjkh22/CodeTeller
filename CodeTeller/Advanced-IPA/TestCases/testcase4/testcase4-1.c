
#include <stdlib.h>
#include <stdio.h>

void fnc1(FILE* fin){

	fclose(fin);
	fclose(fin);

}

int main(){

	FILE *fin  = fopen("file.in", "r");

	fnc1(fin);


	return 0;

}
