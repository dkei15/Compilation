#ifndef __ANALYSE_SYNT_CMP__
#define __ANALYSE_SYNT_CMP__

#include "stdio.h"
#include "premiers.h"
#include "suivants.h"
#include "util.h"


void programme();
void optDecVariables();
void listeDecVariables();
void listeDecVariablesBis();
void declarationVariable();
void listeDecFonctions();
void declarationFonction();
void listeParam();
void optListeDecVariables();
void instruction();// à revoir
void instructionAffect();
void instructionBloc();
void instructionSi();
void instructionTantque();
void instructionAppel();
void instructionRetour();
void instructionEcriture();
void instructionVide();
void var();
void expression();
void appelFct();
void conjonction();
void negation();
void comparaison();
void expArith();
void expArithBis();
void terme();
void facteur();
void argumentsEffectifs();
void listeExpressions();
void listeExpressionsBis();
void conjonctionBis();
void optTailleTableau();
void optSinon();
void comparaisonBis();
void optIndice();
void listeDecVariablesBis();
void termeBis();
void expressionBis();
void instructionFaire();
void listreInstructions();
void Axiome();

#endif
