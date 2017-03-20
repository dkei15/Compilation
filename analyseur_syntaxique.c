#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"

char yytext[100];
FILE *yyin;
FILE *sortie_xml;
int uniteCourante,activated;
int activated = 0;


void takeToScreen(int entier){
	char name[20];
	char* value = malloc(sizeof(char) * 100);
	nom_token(entier,name,value);
	affiche_element(name,value,activated);
	uniteCourante=yylex();
}

n_prog* programme(){

	n_prog * toReturn=NULL;
	n_l_dec *variables=NULL;
	n_l_dec *fonctions=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_optDecVariables_) || est_premier(uniteCourante,_listeDecFonctions_)|| est_suivant(uniteCourante, _programme_)){
		variables=optDecVariables();
		fonctions=listeDecFonctions();
		affiche_balise_fermante(__FUNCTION__,activated);
		toReturn=cree_n_prog(variables,fonctions);
		return toReturn;
	}
}

n_l_dec* optDecVariables(){
	n_l_dec * toReturn=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_listeDecVariables_)){
		toReturn=listeDecVariables();
		if(uniteCourante==POINT_VIRGULE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			return toReturn;
		}
	}else if(est_suivant(uniteCourante,_optDecVariables_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}


n_l_dec* listeDecVariables(){
	n_l_dec* toReturn=NULL;
	n_l_dec* listeDecV=NULL;
	n_dec* variable=NULL;
	if(est_premier(uniteCourante,_declarationVariable_)){
		affiche_balise_ouvrante(__FUNCTION__,activated);
		variable=  declarationVariable();
		// printf("ici variable vaut :,%s,----------------------------------------\n", variable->nom);
		listeDecV=	listeDecVariablesBis();
		// printf("là variable vaut :,%s,------------Et elle mange ma virgule !---------\n", variable->nom);
		toReturn=cree_n_l_dec(variable, listeDecV);
		// printf("après elle vaut :,%s,------------------------------------------\n", toReturn->tete->nom);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
}

n_l_dec* listeDecVariablesBis(){

	affiche_balise_ouvrante(__FUNCTION__,activated);
	//printf("On passe là  ....\n");
	if(uniteCourante == VIRGULE){
		n_l_dec* toReturn=NULL;
		n_l_dec* listeDecVariable=NULL;
		n_dec* variable=NULL;
		//	printf("On passe PAS là  ....\n");
		takeToScreen(uniteCourante);
		variable=declarationVariable();
		listeDecVariable=listeDecVariablesBis();
		toReturn=cree_n_l_dec(variable, listeDecVariable);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;

	}

	else if(est_suivant(uniteCourante, _listeDecVariablesBis_)){
		//		printf("......Puis là \n");
		affiche_balise_fermante(__FUNCTION__,activated);
		return NULL;//toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_dec* declarationVariable(){
	n_dec* toReturn=NULL;
	int taille;
	char* nVariable = malloc(sizeof(char) * YYTEXT_MAX);
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante == ENTIER){
		takeToScreen(uniteCourante);

		if(uniteCourante == ID_VAR){

			strcpy(nVariable,yytext);
			takeToScreen(uniteCourante);

			taille=optTailleTableau();
			if(taille < 0){
				toReturn=cree_n_dec_var(nVariable);
			}else {
				toReturn=cree_n_dec_tab(nVariable,taille);
			}
			//				printf("nvar :%s\n", toReturn->nom);
			affiche_balise_fermante(__FUNCTION__,activated);
			return toReturn;
		}
	}
	if(est_suivant(uniteCourante,_declarationVariable_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}


int optTailleTableau(){
	int	entier=-1;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante == CROCHET_OUVRANT){
		takeToScreen(uniteCourante);

		if(uniteCourante == NOMBRE){
			int	entier=atoi(yytext);
			takeToScreen(uniteCourante);
			if(uniteCourante == CROCHET_FERMANT){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				return entier;
			}
		}
		erreur("Erreur");
	}
	if(est_suivant(uniteCourante, _optTailleTableau_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return -1;
	}
	erreur("Erreur");
	return -1;
}


n_l_dec* listeDecFonctions(){
	n_l_dec* toReturn=NULL;
	n_l_dec* listeFonction=NULL;
	n_dec* fonction=NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante, _declarationFonction_)){
		fonction=declarationFonction();
		// printf(" : ,%s,-----------\n",fonction->nom);
		listeFonction=listeDecFonctions();
		// printf("là la fonc est : ,%s,-----------\n",fonction->nom);
		toReturn=cree_n_l_dec(fonction, listeFonction);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_suivant(uniteCourante, _listeDecFonctions_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_dec* declarationFonction(){
	n_dec* toReturn=NULL;
	n_instr* instrBloc=NULL;
	n_l_dec* params=NULL;
	n_l_dec* optVariable=NULL;

	char* name = malloc(sizeof(char) * YYTEXT_MAX);
	strcpy(name, yytext);

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante == ID_FCT){
		takeToScreen(uniteCourante);

		//		printf("NAME : %s \n",name);
		params=listeParam();
		optVariable=optDecVariables();
		instrBloc=instructionBloc();
		toReturn=cree_n_dec_fonc(name,params,optVariable,instrBloc);
		//		printf("NAME : %s \n",toReturn->nom);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;


	}
	erreur("Erreur");
	return NULL;
}

n_l_dec* listeParam(){
	n_l_dec* toReturn=NULL;
	n_l_dec* optListevar=NULL;
	n_dec* tete=NULL;
	n_l_dec* queue=NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if (uniteCourante == PARENTHESE_OUVRANTE) {
		takeToScreen(uniteCourante);
		optListevar=optListeDecVariables();
		if(optListevar != NULL){
			tete = optListevar->tete;
			queue = optListevar->queue;

			toReturn = cree_n_l_dec(tete, queue);
		}
		if(uniteCourante == PARENTHESE_FERMANTE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);

			return toReturn;
		}
	}
	erreur("Erreur");
	return NULL;
}

n_l_dec* optListeDecVariables(){
	n_l_dec* toReturn = NULL;


	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_listeDecVariables_)){
		toReturn=listeDecVariables();
		affiche_balise_fermante(__FUNCTION__,activated);

		return toReturn;
	}
	if(est_suivant(uniteCourante,_optListeDecVariables_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}


n_instr* instruction(){
	n_instr * toReturn = NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_instructionAffect_)){
		toReturn=instructionAffect();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante, _instructionFaire_)){
		toReturn=instructionFaire();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_instructionBloc_)){
		toReturn=instructionBloc();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_instructionSi_)){
		toReturn=instructionSi();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_instructionTantque_)){
		toReturn=instructionTantque();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_instructionAppel_)){
		toReturn=instructionAppel();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_instructionRetour_)){
		toReturn=instructionRetour();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_instructionEcriture_)){
		toReturn=instructionEcriture();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_instructionVide_)){
		toReturn=instructionVide();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	return NULL;
}

n_instr* instructionAffect(){
	n_instr * toReturn = NULL;
	n_var * variable=NULL;
	n_exp * value=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_var_)){
		variable = var();
		if(uniteCourante == EGAL){
			takeToScreen(uniteCourante);
			if(est_premier(uniteCourante,_expression_)){
				value = expression();
				if(uniteCourante==POINT_VIRGULE){
					takeToScreen(uniteCourante);
					affiche_balise_fermante(__FUNCTION__,activated);
					toReturn = cree_n_instr_affect(variable, value);
					return toReturn;
				}
			}
		}
	}

	erreur("Erreur");
	return NULL;
}

n_l_instr* listeInstructions(){
	n_l_instr* toReturn=NULL;
	n_l_instr* listeIntsr=NULL;
	n_instr* instr=NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_instruction_)){
		instr = instruction();
		listeIntsr = listeInstructions();
		affiche_balise_fermante(__FUNCTION__,activated);
		toReturn = cree_n_l_instr(instr, listeIntsr);
		return toReturn;

	}else if(est_suivant(uniteCourante,_listeInstructions_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_instr* instructionBloc(){
	n_instr *toReturn = NULL;
	n_l_instr* listInstr = NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ACCOLADE_OUVRANTE){
		takeToScreen(uniteCourante);
		listInstr = listeInstructions();
		if(uniteCourante==ACCOLADE_FERMANTE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			toReturn = cree_n_instr_bloc(listInstr);
			return toReturn;

		}
	}
	erreur("Erreur");
	return NULL;
}



n_instr* instructionSi(){
	n_instr *toReturn = NULL;
	n_exp *condition = NULL;
	n_instr *alorsInstr = NULL;
	n_instr *sinonInstr = NULL;


	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==SI){
		takeToScreen(uniteCourante);
		condition = expression();
		if(uniteCourante==ALORS){
			takeToScreen(uniteCourante);
			alorsInstr = instructionBloc();
			sinonInstr = optSinon();
			toReturn = cree_n_instr_si(condition, alorsInstr, sinonInstr);
			affiche_balise_fermante(__FUNCTION__,activated);
			return toReturn;
		}
	}
	erreur("Erreur");
	return NULL;
}

n_instr * optSinon(){
	n_instr * toReturn= NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==SINON){
		takeToScreen(uniteCourante);
		toReturn=instructionBloc();
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_suivant(uniteCourante,_optSinon_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_instr* instructionTantque(){
	n_instr* toReturn=NULL;
	n_instr* faire=NULL;
	n_exp* test =NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==TANTQUE){
		takeToScreen(uniteCourante);
		test =	expression();
		if(uniteCourante==FAIRE){
			takeToScreen(uniteCourante);
			faire=instructionBloc();
			affiche_balise_fermante(__FUNCTION__,activated);
			toReturn=cree_n_instr_tantque(test,faire);
			return toReturn;
		}
	}
	erreur("Erreur");
	return NULL;
}

n_instr* instructionAppel(){

	n_instr* toReturn=NULL;
	n_appel* appel=NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_appelFct_)){
		appel=appelFct();
		if(uniteCourante== POINT_VIRGULE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			toReturn=cree_n_instr_appel(appel);
			return toReturn;
		}
	}
	erreur("Erreur");
	return NULL;
}

n_instr* instructionRetour(){
	n_instr* toReturn=NULL;
	n_exp* expr=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==RETOUR){
		takeToScreen(uniteCourante);
		expr= expression();
		if(uniteCourante==POINT_VIRGULE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			toReturn = cree_n_instr_retour(expr);
			return toReturn;
		}

	}
	erreur("Erreur");
	return NULL;
}

n_instr* instructionEcriture(){
	n_instr* toReturn = NULL;
	n_exp* expr = NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ECRIRE){
		takeToScreen(uniteCourante);
		if(uniteCourante==PARENTHESE_OUVRANTE){
			takeToScreen(uniteCourante);
			expr = expression();
			if(uniteCourante==PARENTHESE_FERMANTE){
				takeToScreen(uniteCourante);
				if(uniteCourante==POINT_VIRGULE){
					takeToScreen(uniteCourante);
					affiche_balise_fermante(__FUNCTION__,activated);
					toReturn = cree_n_instr_ecrire(expr);
					return toReturn;

				}	erreur("Erreur");

			}	erreur("Erreur");

		}	erreur("Erreur");

	}
	erreur("Erreur");
	return NULL;
}

n_instr * instructionVide(){
	n_instr* toReturn = NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==POINT_VIRGULE){
		takeToScreen(uniteCourante);
		affiche_balise_fermante(__FUNCTION__,activated);
		toReturn=cree_n_instr_vide();
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_instr * instructionFaire(){// a faire

	n_instr *toReturn = NULL;
	n_instr *instrBloc = NULL;
	n_exp	*express = NULL;

	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==FAIRE){
		takeToScreen(uniteCourante);
		instrBloc=	instructionBloc();
		if(uniteCourante==TANTQUE){
			takeToScreen(uniteCourante);
			express=	expression();
			if(uniteCourante==POINT_VIRGULE){
				takeToScreen(uniteCourante);
				toReturn=cree_n_instr_faire(instrBloc,express);
				affiche_balise_fermante(__FUNCTION__,activated);
				return toReturn;
			}
		}
	}
	erreur("Erreur");
	return NULL;
}

n_exp* expression(){
	n_exp* toReturn=NULL;
	n_exp* conjonc=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_conjonction_)){
		conjonc=conjonction();
		toReturn=expressionBis(conjonc);

		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_exp* expressionBis(n_exp* herite){
	n_exp* toReturn=NULL;
	n_exp* conjonction1=NULL;
	n_exp* fils=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==OU){
		takeToScreen(uniteCourante);
		conjonction1=conjonction();
		fils=cree_n_exp_op(ou, herite, conjonction1);
		toReturn=expressionBis(fils);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;

	}
	if(est_suivant(uniteCourante,_expressionBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		toReturn=herite;
		return toReturn;
	}
	return NULL;
}

n_exp* conjonction(){
	n_exp* toReturn=NULL;
	n_exp* comp=NULL;
	//n_exp* fils=NULL; à suppr
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_comparaison_)){
		comp=comparaison();
		toReturn=conjonctionBis(comp);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_exp* conjonctionBis(n_exp* herite){
	n_exp* toReturn=NULL;
	n_exp* comp=NULL;
	n_exp* fils=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==ET){
		takeToScreen(uniteCourante);
		comp=comparaison();
		fils=cree_n_exp_op(et,herite,comp);
		toReturn=conjonctionBis(fils);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;

	}
	if(est_suivant(uniteCourante,_conjonctionBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		toReturn=herite;
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}

n_exp* comparaison(){
	affiche_balise_ouvrante(__FUNCTION__,activated); //
	n_exp* expAr = expArith();
	n_exp* toReturn = comparaisonBis(expAr);
	affiche_balise_fermante(__FUNCTION__,activated);
	return toReturn;
	//erreur("Erreur");
}

n_exp* comparaisonBis(n_exp* herite){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	n_exp* expAr = NULL;
	n_exp* toReturn = NULL;
	n_exp* herite_f = NULL;

	if(uniteCourante==EGAL || uniteCourante==INFERIEUR){
		takeToScreen(uniteCourante);
		expAr = expArith();
		if(uniteCourante == EGAL)
		 herite_f = cree_n_exp_op(egal, herite, expAr);
		else
			herite_f = cree_n_exp_op(inf, herite, expAr);

		toReturn = comparaisonBis(herite_f);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_suivant(uniteCourante,_comparaisonBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return herite;
	}
	erreur("Erreur");
	return NULL;
}
n_exp* expArith(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	n_exp* term= terme();
	n_exp* toReturn = expArithBis(term);
	affiche_balise_fermante(__FUNCTION__,activated);
	return toReturn;
}

n_exp* expArithBis(n_exp* herite){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	n_exp* toReturn = NULL;
	n_exp* term = NULL;
	n_exp* herite_f = NULL;

	if(uniteCourante==PLUS || uniteCourante==MOINS ){
		takeToScreen(uniteCourante);
		term = terme();
		if(uniteCourante==PLUS)
		herite_f = cree_n_exp_op(plus, herite, term);
		else
		herite_f = cree_n_exp_op(moins, herite, term);

		toReturn = expArithBis(herite_f);
		affiche_balise_fermante(__FUNCTION__,activated);


		return toReturn;
	}
	else if (est_suivant(uniteCourante,_expArithBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return herite;
	}
	return NULL;
	//erreur("Erreur");
}


n_exp* terme() {
	affiche_balise_ouvrante(__FUNCTION__,activated);
	n_exp* toReturn = NULL;
	n_exp* neg = NULL;

	neg = negation();
	toReturn = termeBis(neg);
	affiche_balise_fermante(__FUNCTION__,activated);
	return toReturn;

	//erreur("Erreur");
}

n_exp* termeBis(n_exp* herite){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	n_exp* toReturn = NULL;
	n_exp* neg = NULL;
	n_exp* herite_f = NULL;
	//n_exp* facteur_ = NULL;

	if(uniteCourante==FOIS || uniteCourante==DIVISE){
		takeToScreen(uniteCourante);

		neg = negation();
		if(uniteCourante == FOIS)
		herite_f = cree_n_exp_op(fois, herite, neg);
		else
		herite_f = cree_n_exp_op(divise, herite, neg);


		toReturn = termeBis(herite_f);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;

	}
	if(est_suivant(uniteCourante,_termeBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return herite;
	}
	erreur("Erreur");
	return NULL;
}

n_exp* negation(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_facteur_)){
		n_exp* fact = facteur();
		affiche_balise_fermante(__FUNCTION__,activated);
		return fact;
	}
	if(uniteCourante==NON){
		takeToScreen(uniteCourante);
		n_exp* neg = negation();
		neg = cree_n_exp_op(non, neg, NULL);
		affiche_balise_fermante(__FUNCTION__,activated);
		return neg;
	}
	else{
		erreur("Erreur");
	}
	return NULL;
}

n_exp* facteur(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	n_exp* toReturn = NULL;
	if(uniteCourante==PARENTHESE_OUVRANTE){
		takeToScreen(uniteCourante);
		toReturn = expression();
		if(uniteCourante==PARENTHESE_FERMANTE){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			return toReturn;
		}
	}
	if(uniteCourante==NOMBRE){
		toReturn=cree_n_exp_entier(atoi(yytext));
		takeToScreen(uniteCourante);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_appelFct_)){
		n_appel* fct = appelFct();
		toReturn = cree_n_exp_appel(fct);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_premier(uniteCourante,_var_)){
		n_var* v = var();
		toReturn = cree_n_exp_var(v);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(uniteCourante==LIRE){
		takeToScreen(uniteCourante);
		if(uniteCourante==PARENTHESE_OUVRANTE){
			takeToScreen(uniteCourante);
			if(uniteCourante==PARENTHESE_FERMANTE){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				toReturn=cree_n_exp_lire();
				return toReturn;
			}
		}
	}
	return NULL;
	erreur("Erreur");
}

n_var* var(){
	n_var* toReturn =NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);

	if(uniteCourante==ID_VAR){
		char* name = malloc(sizeof(char) * YYTEXT_MAX);
		strcpy(name, yytext);
		takeToScreen(uniteCourante);
		n_exp* indice =optIndice();
		if(indice == NULL)
		toReturn = cree_n_var_simple(name);
		else{
			toReturn = cree_n_var_indicee(name,indice);
		}
		if(est_suivant(uniteCourante,_var_)){
			affiche_balise_fermante(__FUNCTION__,activated);
			return toReturn;
		}
	}
	return NULL;
	erreur("Erreur");
}

n_exp* optIndice(){
	affiche_balise_ouvrante(__FUNCTION__,activated);
	n_exp* toReturn = NULL;

	if(uniteCourante==CROCHET_OUVRANT){
		takeToScreen(uniteCourante);
		toReturn = expression();
		if(uniteCourante==CROCHET_FERMANT){
			takeToScreen(uniteCourante);
			affiche_balise_fermante(__FUNCTION__,activated);
			return toReturn;
		}
	}
	else if(est_suivant(uniteCourante,_optIndice_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return NULL;
	}
	erreur("Erreur");
	return NULL;
}

n_appel* appelFct(){
	n_appel* toReturn=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	char* id_func=malloc(sizeof(char)*YYTEXT_MAX);
	if(uniteCourante==ID_FCT){
		strcpy(id_func, yytext);
		takeToScreen(uniteCourante);
		if(uniteCourante==PARENTHESE_OUVRANTE){
			takeToScreen(uniteCourante);
			n_l_exp* args = listeExpressions();
			if(uniteCourante==PARENTHESE_FERMANTE){
				takeToScreen(uniteCourante);
				affiche_balise_fermante(__FUNCTION__,activated);
				toReturn= cree_n_appel(id_func, args);
				return toReturn;
			}
		}
	}return NULL;
}

n_l_exp* listeExpressions(){
	n_l_exp* toReturn=NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(est_premier(uniteCourante,_expression_)){
		n_exp* expr= expression();
		toReturn= cree_n_l_exp(expr, listeExpressionsBis(toReturn));
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;

	}
	if(est_suivant(uniteCourante,_listeExpressions_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return NULL;
	}
	return NULL;
}

n_l_exp* listeExpressionsBis(n_l_exp *herite){
	n_l_exp *toReturn = NULL;
	n_exp *expr = NULL;
	n_l_exp *herite_f = NULL;
	affiche_balise_ouvrante(__FUNCTION__,activated);
	if(uniteCourante==VIRGULE){
		takeToScreen(uniteCourante);
		expr = expression();
		herite_f=cree_n_l_exp(expr, herite);
		toReturn = listeExpressionsBis(herite_f);
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	if(est_suivant(uniteCourante,_listeExpressionsBis_)){
		affiche_balise_fermante(__FUNCTION__,activated);
		return toReturn;
	}
	erreur("Erreur");
	return NULL;
}


n_prog* Axiome(int print_xml) {
	activated = print_xml;
	initialise_premiers();
	initialise_suivants();
	///printf( "Axiome\n" );
	uniteCourante = yylex();
	//fprintf(stdout,"%d\n",uniteCourante);
	n_prog* prog = programme();
	//close(sortie_xml);
	if(uniteCourante!=FIN){
		erreur("Pas de fin détectée");
	}
	return prog;
}
