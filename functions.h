#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <poll.h>
#include "lista.h"

int g_stop;

int g_my_id;
char g_sen_id[100];


void check_arguments(int argc,char *argv[],int *id,int *b,char **common_dir,char **input_dir,
                     char **mirror_dir,char **log_file);

void check_directory(int id,char *common_dir, char *input_dir, char *mirror_dir, char *log_file);


void f_handler(int signal);
void sig_from_childs(int sig);

void sig_handler(int sig, siginfo_t *siginfo, void *con);
void sig_handler2(int sig, siginfo_t *siginfo, void *con);

char * check_my_list(info_deikti *lista,int my_id,char *common_dir);

int leng_file(char *input_dir, char *f);
void antallagi_arxeiwn(int id,char *new_id,char *common_dir,char *mirror_dir,char *input_dir,char *log_file,int size_b);

void check_for_leaves_ids(info_deikti *lista, char *common_dir, char *mirror_dir);
void delete_my_id_and_director(int my_id, char *common_dir, char *mirror_dir);
