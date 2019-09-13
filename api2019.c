#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  t_dlistrel rel[4];

  // puntatore al nodo precedente
  struct node *prev;

  // puntatore al nodo successivo
  struct node *next;
} t_node;

typedef struct noderel
{
  // identificativo ori
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

t_node *controllo(t_dlist *list, char *ent);
void init_listrel(t_dlistrel *list);
void insertrel(t_dlistrel *list, char *org, char *id_rel);

int crel(t_dlist *list, char *id_rel, int i);

void report1(t_dlist *list);
void scambia_righe(int prima, int seconda);
void stampa_maggiore(t_dlist *list);

void delete_org(t_dlist *list, char *id);
void delete_ent(t_dlist *list, char *id);
void delete_list(t_dlistrel *list);

void delrel(t_dlist *list, char *org, char *dst, char *rel);

void chop(char* str);
void exec_command(char *command[4], t_dlist *list);

/*VARIABILI GLOBALI*/
char reportrel[4][30];

/*INIZIO DEL MAIN*/
int main() {


    t_dlist list_ent;
    init_list(&list_ent);

    FILE *fp = stdin;

    char *line = NULL;
    size_t len = 0;

    char *token;
    char *separator = " ";

    while(getline(&line, &len, fp) != -1) {
        char *command[4] = {NULL,NULL,NULL,NULL};
        char* rest = line;
        int i = 0;
        while ((token = strtok_r(rest, separator, &rest))) {
            chop(token);
            command[i] = malloc(strlen(token) + 1);
            strcpy(command[i], token);
            i++;
        }
        exec_command(command, &list_ent);
    }

    free(line);

    return 0;

}


/*DEFINIZIONE FUNZIONI*/
void chop(char* str) {
    int len = strlen(str);
    if( str[len-1] == '\n' ) {
        str[len-1] = 0;
    }
}

void exec_command(char *command[4], t_dlist *list) {

    t_node *temp;
    int r, b,a, vuoto;

  //  printf("cmd: %s arg1: %s arg2: %s arg3: %s\n", command[0], command[1], command[2], command[3]);

    if (strcmp(command[0], "end") == 0) {
      //  printf("--------- END ---------\n");
        exit(0);
        return;
    }

    if(strcmp(command[0], "delent")==0){
      //elimino le origini
       delete_org(list, command[1]);
       //elimino le destinazioni e l'entità
       delete_ent(list, command[1]);
    }

    if(strcmp(command[0], "delrel")==0){
       delrel(list, command[1], command[2], command[3]);
    }

    if (strcmp(command[0], "report") == 0) {
        // inserire quì solo la chiamata alla funzione di reporting
      //  printf("-------- REPORT -------\n");
        report1(list);//popolo la matrice

        a=1;
        while(a==1){
          a=0;
          for(b=0; b<3; b++){
            if(strcmp(reportrel[b],reportrel[b+1])>0){
             a=1;
             scambia_righe(b, b+1);
            }
          }
        }//riordina le relezioni nella matrice

        vuoto = 1;
        for(int i=0;i<4;i++){
          if(strlen(reportrel[i])!=0)
            vuoto = 0;
        }

        if(vuoto==0){
          stampa_maggiore(list);
        }
        else printf("none");

        printf("\n");

        for(b=0;b<4;b++){//"pulisco la matrice"
          for(int j=0;j<30;j++){
              reportrel[b][j] = 0;
            }
        }
        return;
    }

    if (strcmp(command[0], "addent") == 0) {
        // inserire quì solo la chiamata alla funzione di aggiunta delle entità
      //  printf("-------- ADDENT -------\n");
        if(controllo(list, command[1])==NULL){
          insert(list, command[1]);
        }
        return;
    }

    if (strcmp(command[0], "addrel") == 0) {
        // inserire quì solo la chiamata alla funzione di aggiunta delle relazioni tra le entità
      //  printf("-------- ADDREL -------\n");
        //devo cercare nella lista delle entità quella di destinazione
       temp = controllo(list, command[2]);
         if(temp!=NULL){//se trovo l'entità allora inserisco la nuova relazione
           if(controllo(list, command[1])!=NULL){//se origine non è monitorata non aggiunge la rel
             for(int i=0;i<4;i++){
               r = crel(list,command[3], i);
               if(r==1+i){
                 insertrel(&(temp->rel[i]), command[1], command[3]);
                 i=5;
               }
             }
          }
        }
        return;
    }
}

void init_list(t_dlist *list)
{
  // la lista inizialmente non contiene elementi
  list->count = 0;

  // sia la testa che la coda puntano inizialmente a NULL
  list->header = list->tailer = NULL;
}

void init_listrel(t_dlistrel *list)
{
  // la lista inizialmente non contiene elementi
  list->count = 0;

  // sia la testa che la coda puntano inizialmente a NULL
  list->header = list->tailer = NULL;
}

void insert(t_dlist *list, char *new_data)
{
  // creo il nuovo nodo e gli alloco uno spazio di memoria
  t_node *tmp;
  t_node *new_node;
  new_node = malloc(sizeof(t_node));

  // inizializzo il nuovo nodo e gli inserisco il nuovo dato
  strcpy(new_node->persona, new_data);
  new_node->next = NULL;
  for (int i = 0; i < 4; i++)
  {
    init_listrel(&(new_node->rel[i]));
  }

  // inserisco il nodo all'interno della lista: due casi possibili

  // CASO 1: la lista e' vuota (count e' 0)
  // il nuovo nodo fara' sia da header che da tailer
  if (list->count == 0)
  {
    new_node->prev = NULL;
    list->header = list->tailer = new_node;
  }

  // CASO 2: la lista contiene gia' almeno un elemento

  else /*devo trovare la posizione di dove va inserito l'elemento per mantenerla ordinata*/
  {

    if (strcmp(new_node->persona, list->header->persona) <= 0)
    {
      //inserirò in testa
      new_node->prev = NULL;
      new_node->next = list->header;
      list->header->prev = new_node;
      list->header = new_node;
    }
    else
    {
      if (strcmp(new_node->persona, list->tailer->persona) > 0)
      {
        //inserisco in coda
        new_node->prev = list->tailer;
        list->tailer->next = new_node;
        new_node->next = NULL;
        list->tailer = new_node;
      }
      else
      { //inserisco nel mezzo
        /*non devo modificare tailer ed header */
        tmp = list->header->next;
        while (tmp != NULL)
        {
          if (strcmp(new_node->persona, tmp->persona) <= 0)//se la nuova persona è minore di tmp
          {
            new_node->prev = tmp->prev;
            new_node->next = tmp;
            tmp->prev->next = new_node;
            tmp->prev = new_node;
            list->count++;
            tmp = NULL;
          }
          else tmp = tmp->next;
        }
      }
    }
  }

  // aumento il contatore dei nodi della lista
  list->count++;

}


t_node *controllo(t_dlist *list, char *ent)
{
  t_node *tmp;
  tmp = list->header;

  // scorro la lista cercando l'entità
  // ritorno la sua posizione se trovo l'entità
  // altrimenti continuo a scorrere la lista
  while (tmp != NULL)
  {
    if (strcmp(tmp->persona, ent) == 0)
      return tmp;

    tmp = tmp->next;
  }

  // se non trovo nessun nodo contenente personam, ritorno null
  return NULL;
}

t_noderel *contrel(t_dlistrel *list, char *ent)
{
  t_noderel *tmp;
  tmp = list->header;

  // scorro la lista cercando l'entità
  // ritorno la sua posizione se trovo l'entità
  // altrimenti continuo a scorrere la lista
  while (tmp != NULL)
  {
    if (strcmp(tmp->persona, ent) == 0){
      return tmp;
    }

    else tmp = tmp->next;
  }

  // se non trovo nessun nodo contenente personam, ritorno null
  return NULL;
}

void insertrel(t_dlistrel *list, char *org, char *id_rel)
{
  t_noderel *temp;
  temp = contrel(list, org);
  if(temp==NULL){//vedo se la relazione non è già monitorata
    // creo il nuovo nodo e gli alloco uno spazio di memoria
    t_noderel *new_node;
    new_node = malloc(sizeof(t_noderel));

    // inizializzo il nuovo nodo e gli inserisco il nuovo dato
    strcpy(new_node->persona, org);
    strcpy(new_node->trel, id_rel);
    new_node->next = NULL;
    new_node->prev = NULL;

    // inserisco il nodo all'interno della lista: due casi possibili

    // CASO 1: la lista e' vuota (count e' 0)
    // il nuovo nodo fara' sia da header che da tailer
    if (list->count == 0)
    {
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
}

int crel(t_dlist *list, char *id_rel, int i)
{
  t_node *tmp;
  tmp = list->header;

  while (tmp != NULL)
  {
    if (tmp->rel[i].count == 0)
    { //se è vuoto passo a quello dopo
      tmp = tmp->next;
    }
    else
    {
      if (strcmp(tmp->rel[i].header->trel, id_rel) == 0)
      {
        return 1 + i; //ho trovato rel
      }
      else
        return 0; //rel [i] è gia occupato
    }
  }
  //il ramo rel è vuoto
  return 1 + i;
}

void report1(t_dlist *list)
{ //crea la marice
  t_node *tmp;
  tmp = list->header;

  for (int i = 0; i < 4; i++)
  {
    while (tmp != NULL)
    {
      if (i == 0)
      {
        if (tmp->rel[i].count != 0)
        {
          strcpy(reportrel[i], tmp->rel[i].header->trel);
          tmp = NULL;
        }
        else
        {
          tmp = tmp->next;
        }
      }
      else
      {
        if (tmp->rel[i].count != 0)
        {
          if (strcmp(reportrel[i - 1], tmp->rel[i].header->trel) != 0)
          {
            strcpy(reportrel[i], tmp->rel[i].header->trel);
            tmp = NULL; //ho trovato
          }             //fine if
          else
          {
            tmp = tmp->next;
          } //fine else
        }   //fine if
        else
        {
          tmp = tmp->next;
        } //fine else
      }   //fine else
    }     //fine while
    tmp = list->header;
  } //fine for
  return;
} //fine funzione

void scambia_righe(int prima, int seconda)
{
  char appoggio[30];

  strcpy(appoggio, reportrel[prima]);
  strcpy(reportrel[prima], reportrel[seconda]);
  strcpy(reportrel[seconda], appoggio);
}

void stampa_maggiore(t_dlist *list)
{
  int contatore_maggiore = 0;
  int i, j;

  t_node *tmp;
  tmp = list->header;

  for (i = 0; i < 4; i++)
  {
    if (strcmp(reportrel[i + 1], reportrel[i]) != 0)
    {
      printf("%s", reportrel[i]);
      while (tmp != NULL)
      {
        for (j = 0; j < 4; j++)
        {
          if (tmp->rel[j].count != 0)
          {
            if (strcmp(reportrel[i], tmp->rel[j].header->trel) == 0)
            {
              if (contatore_maggiore < tmp->rel[j].count)
              {
                contatore_maggiore = tmp->rel[j].count;
              }
            }
          }
        }
        tmp = tmp->next;
      }                   //usciti dal while so quanto vale cont maggiore, devo quindi stamapare i nomi
      tmp = list->header; //torno all'inizio
      while (tmp != NULL)
      {
        for (j = 0; j < 4; j++)
        {
          if (tmp->rel[j].count != 0)
          {
            if (strcmp(reportrel[i], tmp->rel[j].header->trel) == 0)
            {
              if (contatore_maggiore == tmp->rel[j].count)
              {
                printf(" %s", tmp->persona);
              }
            }
          }
        }
        tmp = tmp->next;
      } //alla fine del secondo while ho stampato i nomi per la reliìazione i-esima;

      //rimane da stamapre il contatore
      if (contatore_maggiore > 0)
      {
        printf(" %d; ", contatore_maggiore);
        contatore_maggiore = 0;
      }
      tmp = list->header;
    }
  } //fine for

  return;
}

void delete_org(t_dlist *list, char *id)
{
  t_node *tmp;
  t_noderel *tempr = NULL, *n;
  tmp = list->header;
  int i;

  while (tmp != NULL)
  {
    for (i = 0; i < 4; i++)
    {
      if (tmp->rel[i].count != 0)
      {
        tempr = (t_noderel *)tmp->rel[i].header;
        while (tempr != NULL)
        {
          if (strcmp(tempr->persona, id) == 0)
          {
            n = tempr;
            if (n->prev != NULL)
            {
              n->prev->next = n->next;
            }
            else
            { //siamo in testa
              tmp->rel[i].header = n->next;
            }
            if (n->next != NULL)
            { // previene possibili Segmentation Fault
              n->next->prev = n->prev;
            }
            else
            { //siamo in coda
              tmp->rel[i].tailer = n->prev;
            }
            tempr = tempr->next;
            //dealloco la memoria occupata
            free(n);
            tmp->rel[i].count--;
          } //fine if
          else
          {
            tempr = tempr->next;
          } //fine else
        }   //fine while (quando tempr è null ho finito di percorrere la relazione)
      }     //fine if (se ==0 allora non serve percorrere la lista delle rel passo all'altra lista)
    }       //fine for
    //controllate le relazioni della persona passo ad un'altra
    tmp = tmp->next;
  } //fine while
  //ho cancellato le origini

} //fine funzione

void delete_ent(t_dlist *list, char *id)
{
  t_node *tmp;

  while ((tmp = controllo(list, id)) != NULL)
  {
    // sgancio il nodo puntato da n dalla lista
    if (tmp->prev != NULL)
    {
      tmp->prev->next = tmp->next;
    }
    else
    { //siamo in testa
      list->header = tmp->next;
    }
    if (tmp->next != NULL)
    { // previene possibili Segmentation Fault
      tmp->next->prev = tmp->prev;
    }
    else
    { //siamo in coda
      list->tailer = tmp->prev;
    }
    for (int i = 0; i < 4; i++)
    {
      delete_list(&tmp->rel[i]);
    }

    // libero la memoria allocata
    free(tmp);
    list->count--;
  }
  return;
} //fine

void delete_list(t_dlistrel *list)
{
  t_noderel *n1, *n2;
  n1 = list->header;

  // scorro i nodi della lista e li cancello
  // tramite la funzione free() libero la memoria da essi occupata
  while (n1 != NULL)
  {
    n2 = n1->next;
    free(n1);
    n1 = n2;
  }
  return;
} //fine funzione

void delrel(t_dlist *list, char *org, char *dst, char *rel)
{
  t_node *tmp;
  t_noderel *tmpr, *n;
  int i;
  tmp = controllo(list, dst);
  if (tmp != NULL)
  { //se non trovo la destinazione non faccio nulla
    for (i = 0; i < 4; i++)
    { //scorro le relazioni
      if (tmp->rel[i].count != 0)
      { //se la lista e vuota passo avanti
        if (strcmp(tmp->rel[i].header->trel, rel) == 0)
        { //se gli id relazioni sono diversi passo avanti
          //trovata la rel devo scorrere la lista fino a trovare org
          tmpr = tmp->rel[i].header;
          while (tmpr != NULL)
          {
            if (strcmp(tmpr->persona, org) == 0)
            { //se trovo id org procedo alla cancellazione
              n = tmpr;
              if (n->prev != NULL)
              {
                n->prev->next = n->next;
              }
              else
              { //siamo in testa
                tmp->rel[i].header = n->next;
              }
              if (n->next != NULL)
              { // previene possibili Segmentation Fault
                n->next->prev = n->prev;
              }
              else
              { //siamo in coda
                tmp->rel[i].tailer = n->prev;
              }
              tmpr = tmpr->next;
              //dealloco la memoria occupata
              free(n);
              tmp->rel[i].count--;
              return;
            } //fine if
            else
            {
              tmpr = tmpr->next;
            }
          } //fine while
        }
      }
    } //fine for
  }
} //fine funzione
