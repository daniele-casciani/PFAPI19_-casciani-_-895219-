/*CALCOLATORE PUNTA-BANCA*/

#include <stdio.h>
#include <stdlib.h>
/*PROTOTIPI*/
float differenza(float a, float b);

int main(){
  float puntata_banca, quota_punta, quota_banca;
  float vincita_punta, responsabilita_banca;
  float  puntata_punta;
  int reating1,reating2;

  printf("Inserisci la puntata  \n");
  scanf("%f", &puntata_punta);
  printf("Inserisci la quota della puntata \n");
  scanf("%f", &quota_punta);
  printf("Inserisci la quota della bancata \n");
  scanf("%f", &quota_banca);

  vincita_punta=(puntata_punta*quota_punta)-puntata_punta;

  responsabilita_banca=vincita_punta;
  puntata_banca=responsabilita_banca/(quota_banca-1);

  /*sono soddisfatto quando la differenza1( tra la puntata vincente e la bancata perdente)
  è molto vicina alla differenza2 ( tra bancata vincente e alla puntata perdente)
  nota che quando perde la bancata sottrai la responsabilità, mentre quando la bancata vince sommi
  la bancata e togli la puntata */

  while(differenza(differenza(vincita_punta,responsabilita_banca),differenza(puntata_banca,puntata_punta))>0.0001){
      responsabilita_banca= responsabilita_banca+0.0001;
      puntata_banca=responsabilita_banca/(quota_banca-1);
  }

  reating1=100*(puntata_punta+differenza(vincita_punta,responsabilita_banca))/puntata_punta;
  reating2=100*(puntata_punta+differenza(puntata_banca,puntata_punta))/puntata_punta;

  printf("\n");
  printf("Caso vince la puntata:\t+%.2f\t-%.2f\t%.2f\n",vincita_punta,responsabilita_banca,differenza(vincita_punta,responsabilita_banca));
  printf("Caso vince la bancata:\t-%.2f\t+%.2f\t%.2f\n",puntata_punta,puntata_banca,differenza(puntata_banca,puntata_punta));
  printf("Dovrai bancare:\t%.2f\n", puntata_banca);
  printf("Reating:\t%d-%d\n", reating1,reating2);

  return 0;
}

float differenza(float a, float b){
  float diff;
  diff=a-b;
  return diff;
}
