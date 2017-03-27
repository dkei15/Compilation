#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "tabsymboles.h"
#include "analyseur_lexical.h"
#include <stdlib.h>
#include "parcours_arbre_abstrait.h"
 int label = 0;
//trace_abs = 0;
 portee=1;
 adresseLocaleCourante;
 adresseArgumentCourant;
int adresseGlobaleCourante=0;
/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n)
{
  tabsymboles.base=0;
  tabsymboles.sommet=0;
  printf("%include io.asm\n");
  printf("section \t.bss\n");
  parcours_l_dec(n->variables);

  printf("section \t.txt\n");
  printf("_start : call main\n");

  printf("mov eax, 1\n");
  printf("int 0x80\n");

  printf("main :\n")
  parcours_l_dec(n->fonctions);


  if(rechercheExecutable("main")==-1){
    printf("Absence de fonction de la fonction main.\n");
    error(0);
  }
  printf("ret:\n");
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void parcours_l_instr(n_l_instr *n)
{
  char *fct = "l_instr";
  if(n){
  parcours_instr(n->tete);
  parcours_l_instr(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) parcours_l_instr(n->u.liste);
    else if(n->type == affecteInst) parcours_instr_affect(n);
    else if(n->type == siInst) parcours_instr_si(n);
    else if(n->type == tantqueInst) parcours_instr_tantque(n);
    else if(n->type == faireInst) parcours_instr_faire(n);
    //else if(n->type == pourInst) parcours_instr_pour(n);
    else if(n->type == appelInst) parcours_instr_appel(n);
    else if(n->type == retourInst) parcours_instr_retour(n);
    else if(n->type == ecrireInst) parcours_instr_ecrire(n);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr_si(n_instr *n)
{
  char *fct = "instr_si";

  parcours_exp(n->u.si_.test);
  parcours_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    parcours_instr(n->u.si_.sinon);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n)
{
  char *fct = "instr_tantque";

  parcours_exp(n->u.tantque_.test);
  parcours_instr(n->u.tantque_.faire);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
  char *fct = "instr_faire";                  /* MODIFIE POUR EVAL */
  //affiche_balise_ouvrante(fct, trace_abs);    /* MODIFIE POUR EVAL */
  parcours_instr(n->u.faire_.faire);           /* MODIFIE POUR EVAL */
  parcours_exp(n->u.faire_.test);              /* MODIFIE POUR EVAL */
  //affiche_balise_fermante(fct, trace_abs);    /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/


//void affiche_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
//{                                                  /* MODIFIE POUR EVAL */
  //char *fct = "instr_pour";                        /* MODIFIE POUR EVAL */
  //affiche_balise_ouvrante(fct, trace_abs);         /* MODIFIE POUR EVAL */
  //affiche_instr(n->u.pour_.init);                  /* MODIFIE POUR EVAL */
  //affiche_exp(n->u.pour_.test);                    /* MODIFIE POUR EVAL */
  //affiche_instr(n->u.pour_.faire);                 /* MODIFIE POUR EVAL */
  //affiche_instr(n->u.pour_.incr);                  /* MODIFIE POUR EVAL */
  //affiche_balise_fermante(fct, trace_abs);         /* MODIFIE POUR EVAL */
//}                                                  /* MODIFIE POUR EVAL */



/*-------------------------------------------------------------------------*/

void parcours_instr_affect(n_instr *n)
{

  parcours_var(n->u.affecte_.var);
  parcours_exp(n->u.affecte_.exp);
  }

/*-------------------------------------------------------------------------*/

int countArgs(n_l_exp* args){
  int returnValue;
  if(args){
      returnValue = 1 + countArgs(args->queue);
    }
  else
    returnValue = 0;
  return returnValue;
}
/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n)
{
  char *fct = "instr_appel";

  int exec=rechercheExecutable(n->u.appel->fonction);
	int nbreArg;
	if(exec != -1)
	{
		nbreArg = countArgs(n->u.appel->args);
  //  printf("resultat de countArgs : %d \n",nbreArg);
		if(tabsymboles.tab[exec].complement == nbreArg)
		{
			parcours_appel(n->u.appel);
		}
		else{
			printf("Erreur : Le nombre d'argument n'est pas acceptable \n");
			printf ("Erreur : Le nombre d'argument de \"%s\" est %d au lieu de %d.\n",n->u.appel->fonction,nbreArg, tabsymboles.tab[exec].complement);
		}
	}
	else{
		printf("Erreur : La fonction \"%s\" n'est pas déclarée.\n", n->u.appel->fonction);
		//fprintf (fichier,"Erreur : La fonction \"%s\" n'est pas déclarée.\n", n->u.appel->fonction);
	}
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n)
{
  char *fct = "appel";
//  affiche_texte( n->fonction, trace_abs);
  parcours_l_exp(n->args);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n)
{
  char *fct = "instr_retour";
  parcours_exp(n->u.retour_.expression);

}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n)
{
  char *fct = "instr_ecrire";
  parcours_exp(n->u.ecrire_.expression);
}

/*-------------------------------------------------------------------------*/

void parcours_l_exp(n_l_exp *n)
{
  char *fct = "l_exp";

  if(n){
    parcours_exp(n->tete);
    parcours_l_exp(n->queue);
  }
}


/*-------------------------------------------------------------------------*/

void parcours_exp(n_exp *n)
{

  if(n->type == varExp) parcours_varExp(n);
  else if(n->type == opExp) parcours_opExp(n);
  else if(n->type == intExp) parcours_intExp(n);
  else if(n->type == appelExp) parcours_appelExp(n);
  else if(n->type == lireExp) parcours_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void parcours_varExp(n_exp *n)
{
  if(rechercheExecutable(n->u.var->nom)!=-1){
  //  char *fct = "varExp";
    parcours_var(n->u.var);
  }else {
      printf("erreur : Absence de la variable \n");

  }
}
/*-------------------------------------------------------------------------*/
void parcours_opExp(n_exp *n)
{
  char *fct = "opExp";
  if(n->u.opExp_.op == plus){
    if( n->u.opExp_.op1 != NULL ) {
      parcours_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    parcours_exp(n->u.opExp_.op2);
    }
  }
}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n)
{
  char texte[ 50 ]; // Max. 50 chiffres
  sprintf(texte, "%d", n->u.entier);

}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n)
{
  char *fct = "lireExp";

}

/*-------------------------------------------------------------------------*/

void parcours_appelExp(n_exp *n)
{
  int t=rechercheExecutable(n->u.appel->fonction);
	int nbreAgrs;
	if(t!=-1)
	{
		nbreAgrs= countArgs(n->u.appel->args);
		if(tabsymboles.tab[t].complement != nbreAgrs){
			printf("Erreur : Le nombre d'argument(s)n'est pas bo \n");
			//fprintf (fichier,"Erreur : Le nombre d'argument(s) de \"%s\" est de %d au lieu de %d.\n", n->u.appel->fonction,dico.tab[t].complement, count_n_l_exp(n->u.appel->args));
		}
		else
			parcours_appel(n->u.appel);
	}
	else{
		printf("Erreur : La fonction \"%s\" n'est pas déclarée.\n", n->u.appel->fonction);
		//fprintf (fichier,"Erreur : La fonction \"%s\" n'est pas déclarée.\n", n->u.appel->fonction);
	}
  /*
  char *fct = "appelExhfp";
  parcours_appel(n->u.appel);*/
}

/*-------------------------------------------------------------------------*/

void parcours_l_dec(n_l_dec *n)
{
  char *fct = "l_dec";
  ;
  if( n ){
    parcours_dec(n->tete);
    parcours_l_dec(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_dec(n_dec *n)
{

  if(n){
    if(n->type == foncDec) {
      parcours_foncDec(n);
    }
    else if(n->type == varDec) {
      parcours_varDec(n);
    }
    else if(n->type == tabDec) {
      parcours_tabDec(n);
    }
  }
}

/*-------------------------------------------------------------------------*/

int nombreParam(n_l_dec * param){ // A MODFIER ##DAN
  if(param!=NULL)
     return 1+nombreParam(param->queue);
   return 0;
}
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/

void parcours_foncDec(n_dec *n)
{

	if(rechercheExecutable(n->nom)==-1)
	{

	int nbreArgs=nombreParam(n->u.foncDec_.param);
	ajouteIdentificateur(n->nom,portee,T_FONCTION,0,nbreArgs);
  tabsymboles.base++;
  entreeFonction();
  int ret=0;
  	portee = P_ARGUMENT;
		parcours_l_dec(n->u.foncDec_.param);
		portee = P_VARIABLE_LOCALE;
		parcours_l_dec(n->u.foncDec_.variables);
		parcours_instr(n->u.foncDec_.corps);
	}
	afficheTabsymboles();
	sortieFonction();
  /*
  char *fct = "foncDec";
  if(rechercheExecutable(n->nom)) {
    printf("Déjà déclaré");
    tabsymboles.base++;
    nombreParametre=;

  char * identifiant = malloc(sizeof(char)*YYTEXT_MAX);
  ajouteIdentificateur(n->nom,P_VARIABLE_GLOBALE,);
  entreeFonction();
  portee = P_ARGUMENT;
  parcours_l_dec(n->u.foncDec_.param);
  portee = P_VARIABLE_LOCALE;
  // nombredeVarLocal = taille_n_l_dec(n->u.foncDec_.variables);
  parcours_l_dec(n->u.foncDec_.variables);
  parcours_instr(n->u.foncDec_.corps);

  sortieFonction();
}*/
}
/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n)
{
  int declaree = rechercheDeclarative(n->nom);
  if((rechercheExecutable(n->nom)!=-1)&&(portee==P_VARIABLE_GLOBALE)){
    printf("variable déjà déclarée");
  }
 if(rechercheDeclarative(n->nom)==-1){
    if(portee==P_VARIABLE_GLOBALE || portee==P_VARIABLE_LOCALE){
			ajouteIdentificateur(n->nom,portee,T_ENTIER,adresseLocaleCourante,1);
      if(portee==P_VARIABLE_GLOBALE){
        tabsymboles.base++;
        printf("%s\t resw 2",n->nom);


      }
       adresseLocaleCourante += 4;
  	}	else if(portee == P_ARGUMENT){
			ajouteIdentificateur(n->nom,portee,T_ENTIER,adresseArgumentCourant,/*tabsymboles.tab[rechercheExecutable(n->nom)].complement++*/1);
       adresseArgumentCourant += 4;
  }
  else{
    printf("Variable déjà déclarée\n " );
  }
}
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n)
{

if(portee==P_VARIABLE_GLOBALE)// si contexte vaut P_VARIABLE_GLOBALE
	{
		if(rechercheExecutable(n->nom)<0)
		{
			ajouteIdentificateur(n->nom,portee,T_TABLEAU_ENTIER,adresseGlobaleCourante,n->u.tabDec_.taille);
			adresseGlobaleCourante=adresseGlobaleCourante+n->u.tabDec_.taille;
      int tailleTab = 2 * n->u.tabDec_.taille;
      printf("%s\t resw %d",n->nom, tailleTab);
		}
	}
	else{
		fprintf(stderr,"Action interdite : \n");
}
/*
  if(rechercheDeclarative(n->nom)) {
    printf("Déjà déclaré");
  }
  char texte[100]; // Max. 100 chars nom tab + taille
  ajouteIdentificateur(n->nom,,T_TABLEAU_ENTIER,P_VARIABLE_GLOBALE,$fp,n->u.tabDec_.taille);
  sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);*/
}

/*-------------------------------------------------------------------------*/

void parcours_var(n_var *n)
{
  if(n->type == simple) {
    parcours_var_simple(n);
  }
  else if(n->type == indicee) {
    parcours_var_indicee(n);
  }
}

/*-------------------------------------------------------------------------*/
void parcours_var_simple(n_var *n)
{
  int indice =rechercheExecutable(n->nom);
	if(indice<0)
	{
		printf("Erreur : Variable \"%s\" non déclarée.\n",n->nom);

	}

}

/*-------------------------------------------------------------------------*/
void parcours_var_indicee(n_var *n)
{
  int declaree = rechercheExecutable(n->nom);
  if(declaree==-1){
    printf(": Variable \"%s\" non déclarée.\n",n->nom);
    }
  else{
      if(n->u.indicee_.indice->u.entier >=tabsymboles.tab[declaree].complement){
//        printf("Dépassement de mémoire. Indice trop élevé");

      } else{
    parcours_exp(n->u.indicee_.indice);

}
}
}
/*-------------------------------------------------------------------------*/
