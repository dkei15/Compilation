#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"
#include <string.h>

FILE * yyin;

int main(int argc, char * argv[]){
	if(argc !=3){
		printf("Usage: %s -option fichier.txt",argv[0]);
	}
	yyin = fopen(argv[2], "r");
	if(yyin == NULL)
	{
		printf("Le fichier n'existe pas\n");
		exit(1);
	}

	if(!strcmp(argv[1],"-l")){ // execute l'analyseur lexical
		test_yylex_internal(yyin);

	}
	else if(!strcmp(argv[1],"-s")){ //execute l'analyseur syntaxique
 Axiome();

}
	/*else if(!strcmp(argv[1],"-a")){ //execute l'analyseur syntaxique
	n_prog* p=Axiome();
	affiche_n_prog(p);
}*/
	else{
		printf("Commande inconnue");
		exit(1);
		}
	return 0;
}
