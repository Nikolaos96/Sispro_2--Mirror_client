//File: lista.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"



 typedef struct info_node{
 	int size;
	typos_deikti arxi;
	typos_deikti telos;
 }info_node;


 typedef struct typos_komvou{
 	char dedomena[30];
	typos_deikti epomenos;
	typos_deikti proigoumenos;
 }typos_komvou;




 info_deikti LIST_dimiourgia(info_deikti* lista){

 	info_deikti linfo;
 	linfo = malloc(sizeof(info_node));
 	linfo->size = 0;
 	linfo->arxi = NULL;
	linfo->telos = NULL;
 	return linfo;
 }



 void eisagogi(info_deikti* linfo,char *dedom){

 	typos_deikti prosorinos;
 	prosorinos = malloc(sizeof(typos_komvou));
 	strcpy(prosorinos->dedomena,dedom);

 	if((*linfo)->size == 0){
 		(*linfo)->arxi = prosorinos;
		(*linfo)->telos = prosorinos;
		prosorinos->epomenos = NULL;
		prosorinos->proigoumenos = NULL;

 	}
 	if((*linfo)->size > 0){
		prosorinos->proigoumenos = (*linfo)->telos;
		prosorinos->epomenos = NULL;
 		(*linfo)->telos->epomenos = prosorinos;
		(*linfo)->telos = prosorinos;
 	}
 	(*linfo)->size++;
 }


 void emfanisi(info_deikti* linfo){

	typos_deikti mapas = (*linfo)->arxi;
	printf(" \n ");
 	while(mapas != NULL){
 		printf("%s\n", mapas->dedomena);
		mapas = mapas->epomenos;
 	}
	printf(" \n ");
 }


 void lista_diagrafi(info_deikti* linfo){

 	typos_deikti mapas = (*linfo)->arxi;
	typos_deikti mapas2;

 	while(mapas != NULL){
		mapas2 = mapas;
		mapas = mapas->epomenos;
		free(mapas2);
	}
	(*linfo)->arxi = NULL;
	(*linfo)->telos = NULL;
	(*linfo)->size = 0;
	free(*linfo);
	(*linfo) = NULL;
 }

 void diagrafi_komvou(info_deikti* linfo, int komvos){
	int k;

 	if(komvos > (*linfo)->size || komvos <= 0 || (*linfo)->size == 0){
		printf("Den yparxei aytos o komvos \n");
		return;
	}else{
		typos_deikti mapas = (*linfo)->arxi;
		typos_deikti mapas2;
		while(komvos-- > 0){
			mapas2 = mapas;
			mapas = mapas->epomenos;
		}
		if(mapas2 == (*linfo)->arxi && mapas2 == (*linfo)->telos){
			free(mapas2);
			(*linfo)->arxi = NULL;
			(*linfo)->telos = NULL;
			(*linfo)->size--;
		}else if(mapas2 == (*linfo)->arxi){
			(*linfo)->arxi = (*linfo)->arxi->epomenos;
			(*linfo)->arxi->proigoumenos = NULL;
			free(mapas2);
		}else if(mapas2 == (*linfo)->telos){
			(*linfo)->telos = (*linfo)->telos->proigoumenos;
			(*linfo)->telos->epomenos = NULL;
			free(mapas2);
		}else{
			mapas2->proigoumenos->epomenos = mapas2->epomenos;
			mapas2->epomenos->proigoumenos = mapas2->proigoumenos;
			free(mapas2);
		}
		(*linfo)->size--;
	}
 }



 int exist_in_list(info_deikti* linfo, char *id){

    typos_deikti mapas = (*linfo)->arxi;
    while(mapas != NULL){
	if(!strcmp(id,mapas->dedomena)) return 1;
        mapas = mapas->epomenos;
    }
    return 0;





 }


 char * take_elem(info_deikti* linfo,int elem){

     typos_deikti mapas = (*linfo)->arxi;
     while(mapas != NULL && --elem > 0){
	mapas = mapas->epomenos;
     }

     if(mapas != NULL) return mapas->dedomena;
     else return NULL;
 }
