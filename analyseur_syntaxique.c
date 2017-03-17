#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"


char yytext[100];
FILE *yyin;
FILE *sortie_xml;
int uniteCourante,activated;
int activated = 1;


void takeToScreen(int entier){
	char name[20],value[25];
	nom_token(entier,name,value);
	affiche_element(name,value,activated);
	uniteCourante=yylex();
}

void programme(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_optDecVariables_) || est_premier(uniteCourante,_listeDecFonctions_)){
		optDecVariables();
		listeDecFonctions();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}else if(est_suivant(uniteCourante,_programme_)){
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
	}
	erreur(__FUNCTION__);
}

void optDecVariables(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_listeDecVariables_)){
		listeDecVariables();
		if(uniteCourante==POINT_VIRGULE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
		}
	}else if(est_suivant(uniteCourante,_optDecVariables_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	  erreur(__FUNCTION__);
}


void listeDecVariables(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	  declarationVariable();
		listeDecVariablesBis();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;

	erreur(__FUNCTION__);
}

void listeDecVariablesBis(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante == VIRGULE){
			takeToScreen(uniteCourante);
			declarationVariable();
			listeDecVariablesBis();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
}
	else if(est_suivant(uniteCourante, _listeDecVariablesBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void declarationVariable(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante == ENTIER){
		takeToScreen(uniteCourante);
		if(uniteCourante == ID_VAR){
			takeToScreen(uniteCourante);
				optTailleTableau();
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
		}
	}
	if(est_suivant(uniteCourante,_declarationVariable_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void instructionFaire(){
affiche_balise_ouvrante(__FUNCTION__,activated);
		if(uniteCourante==FAIRE){
			takeToScreen(uniteCourante);
				instructionBloc();
				if(uniteCourante==TANTQUE){
					takeToScreen(uniteCourante);
						expression();
						if(uniteCourante==POINT_VIRGULE){
							takeToScreen(uniteCourante);
							affiche_balise_fermante(__FUNCTION__,activated);
						  return;

					}
				}
			}
	erreur(__FUNCTION__);
}


void optTailleTableau(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante == CROCHET_OUVRANT){
		takeToScreen(uniteCourante);
		if(uniteCourante == NOMBRE){
			takeToScreen(uniteCourante);
			if(uniteCourante == CROCHET_FERMANT){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
			}
		}
		erreur(__FUNCTION__);
	}
	if(est_suivant(uniteCourante, _optTailleTableau_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void listeDecFonctions(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante, _declarationFonction_)){
		  declarationFonction();
		  listeDecFonctions();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
		}
	if(est_suivant(uniteCourante, _listeDecFonctions_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}
void declarationFonction(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante == ID_FCT){
		takeToScreen(uniteCourante);
		listeParam();
		optDecVariables();
		instructionBloc();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}

	erreur(__FUNCTION__);
}
void listeParam(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		takeToScreen(uniteCourante);
		optListeDecVariables();
		if(uniteCourante == PARENTHESE_FERMANTE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
		}
	}

	erreur(__FUNCTION__);
}

void optListeDecVariables(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_listeDecVariables_)){
		listeDecVariables();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_suivant(uniteCourante,_optListeDecVariables_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
		erreur(__FUNCTION__);
}



void instruction(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_instructionAffect_)){
		instructionAffect();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante, _instructionFaire_)){
			instructionFaire();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
		}
	if(est_premier(uniteCourante,_instructionBloc_)){
		instructionBloc();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante,_instructionSi_)){
		instructionSi();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante,_instructionTantque_)){
		instructionTantque();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante,_instructionAppel_)){
		instructionAppel();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante,_instructionRetour_)){
		instructionRetour();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante,_instructionEcriture_)){
		instructionEcriture();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante,_instructionVide_)){
		instructionVide();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
}

void instructionAffect(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
		var();
		if(uniteCourante == EGAL){
			takeToScreen(uniteCourante);
			expression();
			if(uniteCourante==POINT_VIRGULE){
				takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
		}
	}
	erreur(__FUNCTION__);
}

void listeInstructions(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_instruction_)){
		instruction();
		listeInstructions();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;

	}else if(est_suivant(uniteCourante,_listeInstructions_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
		erreur(__FUNCTION__);
}

void instructionBloc(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ACCOLADE_OUVRANTE){
		takeToScreen(uniteCourante);
			listeInstructions();
			if(uniteCourante==ACCOLADE_FERMANTE){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
			}

	}
	erreur(__FUNCTION__);
}
void instructionSi(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==SI){
		takeToScreen(uniteCourante);
			expression();
			if(uniteCourante==ALORS){
				takeToScreen(uniteCourante);
				instructionBloc();
				optSinon();
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
				}
	}
	erreur(__FUNCTION__);
}

void optSinon(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==SINON){
		takeToScreen(uniteCourante);
		instructionBloc();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
}
	if(est_suivant(uniteCourante,_optSinon_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}

	erreur(__FUNCTION__);
}

void instructionTantque(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==TANTQUE){
		takeToScreen(uniteCourante);
			expression();
			if(uniteCourante==FAIRE){
				takeToScreen(uniteCourante);
					instructionBloc();
					affiche_balise_fermante(__FUNCTION__,activated);
					return;
		}
	}
	erreur(__FUNCTION__);
}

void instructionAppel(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
		appelFct();
		if(uniteCourante== POINT_VIRGULE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
	}
 erreur(__FUNCTION__);
}

void instructionRetour(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==RETOUR){
		takeToScreen(uniteCourante);
			expression();
			if(uniteCourante==POINT_VIRGULE){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
		}
	}
	erreur(__FUNCTION__);
}

void instructionEcriture(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ECRIRE){
		takeToScreen(uniteCourante);
		if(uniteCourante==PARENTHESE_OUVRANTE){
			takeToScreen(uniteCourante);
					expression();
					if(uniteCourante==PARENTHESE_FERMANTE){
						takeToScreen(uniteCourante);
						 if(uniteCourante==POINT_VIRGULE){
							 takeToScreen(uniteCourante);
							 affiche_balise_fermante(__FUNCTION__,activated);
							 return;
					 }
				}
		}
	}
	erreur(__FUNCTION__);
}

void instructionVide(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==POINT_VIRGULE){
		takeToScreen(uniteCourante);
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void expression(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
   	conjonction();
		expressionBis();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
erreur(__FUNCTION__);
}

void expressionBis(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==OU){
		takeToScreen(uniteCourante);
			conjonction();
			expressionBis();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
	}
	if(est_suivant(uniteCourante,_expressionBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
}


void conjonction(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
		comparaison();
		conjonctionBis();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;

	erreur(__FUNCTION__);
}

void conjonctionBis(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ET){
		takeToScreen(uniteCourante);
			comparaison();
			conjonctionBis();
				affiche_balise_fermante(__FUNCTION__,activated);
				return;

	}
	if(est_suivant(uniteCourante,_conjonctionBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void comparaison(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
			expArith();
		  comparaisonBis();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
	erreur(__FUNCTION__);
}

void expArith(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	terme();
	expArithBis();
	affiche_balise_fermante(__FUNCTION__,activated);
	return;
}

void expArithBis(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==PLUS || uniteCourante==MOINS ){
		takeToScreen(uniteCourante);
	  terme();
		expArithBis();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	else if (est_suivant(uniteCourante,_expArithBis_)){
	affiche_balise_fermante(__FUNCTION__,activated);
	return;
	}
erreur(__FUNCTION__);
}

void comparaisonBis(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==EGAL || uniteCourante==INFERIEUR){
		takeToScreen(uniteCourante);
			expArith();
			comparaisonBis();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
		}
	if(est_suivant(uniteCourante,_comparaisonBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}



void terme() {
	affiche_balise_ouvrante(__FUNCTION__,activated);
		negation();
		termeBis();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;

	erreur(__FUNCTION__);
}

void termeBis(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==FOIS || uniteCourante==DIVISE){
		takeToScreen(uniteCourante);
			negation();
			termeBis();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;

	}
	if(est_suivant(uniteCourante,_termeBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void negation(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_facteur_)){
		facteur();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
}
if(uniteCourante==NON){
	takeToScreen(uniteCourante);
			negation();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
	}else{
	erreur(__FUNCTION__);
	}
}

void facteur(){
	affiche_balise_ouvrante(__FUNCTION__,activated);

	if(uniteCourante==PARENTHESE_OUVRANTE){
		takeToScreen(uniteCourante);
			expression();
			if(uniteCourante==PARENTHESE_FERMANTE){
					takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
			}
	}
	if(uniteCourante==NOMBRE){
		takeToScreen(uniteCourante);
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(est_premier(uniteCourante,_appelFct_)){
		appelFct();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;

	}
	if(est_premier(uniteCourante,_var_)){
		var();
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	if(uniteCourante==LIRE){
		takeToScreen(uniteCourante);
		if(uniteCourante==PARENTHESE_OUVRANTE){
			takeToScreen(uniteCourante);
			if(uniteCourante==PARENTHESE_FERMANTE){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
			}
		}
	}
	erreur(__FUNCTION__);
}

void var(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ID_VAR){
		takeToScreen(uniteCourante);
			optIndice();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;
	}
	erreur(__FUNCTION__);
}

void optIndice(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==CROCHET_OUVRANT){
		takeToScreen(uniteCourante);
			expression();
			if(uniteCourante==CROCHET_FERMANT){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
		}
	}
	else if(est_suivant(uniteCourante,_optIndice_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void appelFct(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ID_FCT){
		takeToScreen(uniteCourante);
		if(uniteCourante==PARENTHESE_OUVRANTE){
			takeToScreen(uniteCourante);
				listeExpressions();
				if(uniteCourante==PARENTHESE_FERMANTE){
					takeToScreen(uniteCourante);
					affiche_balise_fermante(__FUNCTION__,activated);
					return;
			}
		}
	}
	erreur(__FUNCTION__);
}

void listeExpressions(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
		if(est_premier(uniteCourante,_expression_)){
			expression();
			listeExpressionsBis();
			affiche_balise_fermante(__FUNCTION__,activated);
			return;

	}
	if(est_suivant(uniteCourante,_listeExpressions_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}

void listeExpressionsBis(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==VIRGULE){
		takeToScreen(uniteCourante);
			expression();
			listeExpressionsBis();
				affiche_balise_fermante(__FUNCTION__,activated);
				return;
		}
	if(est_suivant(uniteCourante,_listeExpressionsBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return;
	}
	erreur(__FUNCTION__);
}


void Axiome() {
	initialise_premiers();
	initialise_suivants();
	//printf( "Axiome\n" );
	uniteCourante = yylex();
	//fprintf(stdout,"%d\n",uniteCourante);
	programme();
  //close(sortie_xml);
  if(uniteCourante!=FIN){
		erreur("Pas de fin détectée");
	}
}
