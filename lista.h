#ifndef __LISTA__
#define __LISTA__

typedef struct info_node * info_deikti;
typedef struct typos_komvou * typos_deikti;


info_deikti LIST_dimiourgia(info_deikti*);
void eisagogi(info_deikti* linfo,char *);

void emfanisi(info_deikti* linfo);
void lista_diagrafi(info_deikti* linfo);

void diagrafi_komvou(info_deikti* linfo, int komvos);
int exist_in_list(info_deikti* linfo, char *id);

char * take_elem(info_deikti* linfo,int elem);

#endif
