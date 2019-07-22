#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct dlist
{
    // tiene traccia di quanti nodi sono presenti all'interno della lista
    int count;

    // puntatore al primo nodo della lista
    struct node *header;

    // puntatore all'ultimo nodo della lista
    struct node *tailer;
} t_dlist;

typedef struct dlistrel
{
    // tiene traccia di quanti nodi sono presenti all'interno della lista
    int count;

    // puntatore al primo nodo della lista
    struct noderel *header;

    // puntatore all'ultimo nodo della lista
    struct noderel *tailer;
} t_dlistrel;


typedef struct node
{
    // informazioni contenute nel nodo
    char persona[30];

    //tipi di relazioni
    t_dlistrel rel1;
    t_dlistrel rel2;
    t_dlistrel rel3;
    t_dlistrel rel4;



    // puntatore al nodo precedente
    struct node *prev;

    // puntatore al nodo successivo
    struct node *next;
} t_node;



typedef struct noderel
{
    // identificativo dest
    char persona[30];

    //identificativo relazione
    char trel[30];


    // puntatore al nodo precedente
    struct noderel *prev;

    // puntatore al nodo successivo
    struct noderel *next;
} t_noderel;


/*PROTOTIPI FUNZIONI*/
void init_list(t_dlist *list);
void insert(t_dlist *list, char *new_data);
void print_list(t_dlist *list);
t_node *controllo(t_dlist *list, char *ent);
void init_listrel(t_dlistrel *list);
void insertrel(t_dlistrel *list, char *org, char *id_rel);
void print_listrel(t_dlistrel *list);
int crel1(t_dlist *list, char *id_rel);
int crel2(t_dlist *list, char *id_rel);

/*INIZIO DEL MAIN*/
int main(){
  /*definizioni*/
  char comando[7];//dove salverò il tipo di comando

  char casofine[4]="end";//ci sono 6 tipi di comando
  char caso1[7]="addent";
  char caso2[7]="delent";
  char caso3[7]="addrel";
  char caso4[7]="delrel";
  char caso5[7]="report";

  char id_ent[30];
  char id_rel[30];//relazione
  char id_dest[30];//destinazione

  t_node *temp;

  printf("\nCreo e inizializzo la lista\n");
    t_dlist list_ent;
    init_list(&list_ent);


/*codice esecuzione*/
 //salvo comando
      scanf("%s",comando);


      //a questo punto inizio l'analisi
  while(strcmp(comando,casofine)!=0){

      if(strcmp(comando,caso1)==0){//caso addent
        scanf("%s", id_ent);
        if(controllo(&list_ent, id_ent)==NULL){
          insert(&list_ent, id_ent);

        }
      }//fine caso addent

      if(strcmp(comando,caso2)==0){//caso delent
        scanf("%s", id_ent);
      }//fine caso delent

      if(strcmp(comando,caso3)==0){//addrel
        scanf("%s", id_ent);//origine
        scanf("%s", id_dest);//destinazione
        scanf("%s", id_rel);//relazione

       //devo cercare nella lista delle entità quella di destinazione
       temp = controllo(&list_ent, id_dest);
         if(temp!=NULL){//se trovo l'entità allora inserisco la nuova relazione
           if(controllo(&list_ent, id_ent)!=NULL){//se origine non è monitorata non aggiunge la rel
             if(crel1(&list_ent,id_rel)==1){//entra in rel1
               insertrel(&(temp->rel1), id_ent, id_rel);
             }
             else{
               if(crel2(&list_ent,id_rel)==1){//entra in rel2
                 insertrel(&(temp->rel2), id_ent, id_rel);
               }

             }
          }
        }
      }//fine caso addrel

      if(strcmp(comando,caso4)==0){//caso delrel

      }//fine caso delrel

      if(strcmp(comando,caso5)==0){//caso report

      }//fine caso report

    //fine pezzo di selezione

    scanf("%s",comando);//lettura del comando successivo
  }

  
}//FINE DEL MAIN


/*DEFINIZIINE FUNZIONI*/

void init_list(t_dlist *list){
  // la lista inizialmente non contiene elementi
   list->count = 0;

   // sia la testa che la coda puntano inizialmente a NULL
   list->header = list->tailer = NULL;
}

void init_listrel(t_dlistrel *list){
  // la lista inizialmente non contiene elementi
   list->count = 0;

   // sia la testa che la coda puntano inizialmente a NULL
   list->header = list->tailer = NULL;
}

void insert(t_dlist *list, char *new_data){
  // creo il nuovo nodo e gli alloco uno spazio di memoria
    t_node *new_node;
    new_node = malloc(sizeof(t_node));

    // inizializzo il nuovo nodo e gli inserisco il nuovo dato
      strcpy(new_node->persona, new_data);
      new_node->next = NULL;
      init_listrel(&(new_node->rel1));
      init_listrel(&(new_node->rel2));
      init_listrel(&(new_node->rel3));
      init_listrel(&(new_node->rel4));


    // inserisco il nodo all'interno della lista: due casi possibili

    // CASO 1: la lista e' vuota (count e' 0)
    // il nuovo nodo fara' sia da header che da tailer
    if (list->count == 0)
    {
        new_node->prev = NULL;
        list->header = list->tailer = new_node;
    }

    // CASO 2: la lista contiene gia' almeno un elemento
    // aggancio il nuovo nodo alla fine della lista
    // dopo l'inserimento, il nuovo nodo sara' quindi il tailer della lista
    else
    {
        new_node->prev = list->tailer;
        list->tailer->next = new_node;
        list->tailer = new_node;
    }
    // aumento il contatore dei nodi della lista
    list->count++;
}

void print_list(t_dlist *list){
  t_node *tmp;
    tmp = list->header;
    int i;

    for (i = 1; i <= list->count; i++)
    {
        printf("nodo %d: %s\n", i, tmp->persona);
        tmp = tmp->next;
    }

    return;
}

t_node *controllo(t_dlist *list, char *ent){
  t_node *tmp;
    tmp = list->header;

    // scorro la lista cercando l'entità
    // ritorno la sua posizione se trovo l'entità
    // altrimenti continuo a scorrere la lista
    while (tmp != NULL)
    {
      if (strcmp(tmp->persona, ent)==0)
            return tmp;

        tmp = tmp->next;
    }

    // se non trovo nessun nodo contenente personam, ritorno null
    return NULL;
}

void insertrel(t_dlistrel *list, char *org, char *id_rel){
  // creo il nuovo nodo e gli alloco uno spazio di memoria
    t_noderel *new_node;
    new_node = malloc(sizeof(t_noderel));

    // inizializzo il nuovo nodo e gli inserisco il nuovo dato
      strcpy(new_node->persona, org);
      strcpy(new_node->trel, id_rel);
      new_node->next = NULL;



    // inserisco il nodo all'interno della lista: due casi possibili

    // CASO 1: la lista e' vuota (count e' 0)
    // il nuovo nodo fara' sia da header che da tailer
    if (list->count == 0)
    {
        new_node->prev = NULL;
        list->header = list->tailer = new_node;
    }

    // CASO 2: la lista contiene gia' almeno un elemento
    // aggancio il nuovo nodo alla fine della lista
    // dopo l'inserimento, il nuovo nodo sara' quindi il tailer della lista
    else
    {
        new_node->prev = list->tailer;
        list->tailer->next = new_node;
        list->tailer = new_node;
    }
    // aumento il contatore dei nodi della lista
    list->count++;
}

void print_listrel(t_dlistrel *list){
  t_noderel *tmp;
    tmp = list->header;
    int i;

    for (i = 1; i <= list->count; i++)
    {
        printf("nodo %d:\norigine :%s\nrel: %s\n", i, tmp->persona, tmp->trel);
        tmp = tmp->next;
    }

    return;
}

int crel1(t_dlist *list, char *id_rel){
  t_node *tmp;
  tmp = list->header;
  while(tmp != NULL){
    if(tmp->rel1.count==0){//se è vuoto passo a quello dopo
      tmp=tmp->next;
    }
    else{
      if( strcmp(tmp->rel1.header->trel, id_rel)==0){
            return 1;//ho trovato rel
          }
      else return 0;//rel 1 è gia occupato
      }
  }
  //il ramo rel è vuoto
  return 1;
}

int crel2(t_dlist *list, char *id_rel){
  t_node *tmp;
  tmp = list->header;
  while(tmp != NULL){
    if(tmp->rel2.count==0){ //se è vuoto passo a quello dopo
      tmp=tmp->next;
    }
    else{
      if( strcmp(tmp->rel2.header->trel, id_rel)==0){
            return 1;//ho trovato rel
          }
      else return 0;//rel 1 è gia occupato
      }
  }
  //il ramo rel è vuoto
  return 1;
}
