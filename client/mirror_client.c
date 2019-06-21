// Project 2 syspro
#include <stdio.h>
#include "functions.h"


int main(int argc, char *argv[]){
 int id, b, wpid, status = 0;
 char *common_dir, *input_dir, *mirror_dir, *log_file;

 check_arguments(argc, argv, &id, &b, &common_dir, &input_dir, &mirror_dir, &log_file);    // take arguments in command line
 check_directory(id, common_dir, input_dir, mirror_dir, log_file);			   // check for all director (1,2)

 g_stop = 0;						// global varible - stop loop
 g_my_id = id;						// global varible - id client


 signal(SIGINT, f_handler);				// g_stop = 1 stop loop
 signal(SIGQUIT, f_handler);				// g_stop = 1 stop loop
 signal(SIGUSR1 , sig_from_childs);			// parent print complete transport *id_to_*id (4.vi)

 struct sigaction s;					// reader child send this signal in parent for kill writer proccess
 s.sa_sigaction = &sig_handler;
 s.sa_flags = SA_SIGINFO;
 sigaction(SIGUSR2, &s, NULL);

 struct sigaction s2;					// writer child send this signal in parent for kill reader proccess
 s2.sa_sigaction = &sig_handler2;
 s2.sa_flags = SA_SIGINFO;
 sigaction(SIGTERM, &s2, NULL);

 info_deikti list = NULL;				// Create list for saving clients id
 list = LIST_dimiourgia(&list);


 while(g_stop == 0){
     char *new_id = NULL;
     new_id = check_my_list(&list,id,common_dir);		// check for new client (3)

     if(new_id != NULL){
	 printf("Find new client  %s  \n", new_id);
	 strcpy(g_sen_id, new_id);
         eisagogi(&list,new_id);
         antallagi_arxeiwn(id,new_id,common_dir,mirror_dir,input_dir,log_file,b); // (4)
     }
     else printf("I did not find a new client. \n");

      sleep(5);							// repeat each 5 second (3)
      check_for_leaves_ids(&list, common_dir, mirror_dir);      // check if a clinet leave from common_dir (5)
 }

 delete_my_id_and_director(id, common_dir, mirror_dir);		// client delete mirror_dir and id-file in common_dir (6)

 while ((wpid = wait(&status)) > 0);				// wait all the children to end

 FILE *fp = fopen(log_file, "a");				// write -1 in log_file if leave from common_dir
 fprintf(fp, "-1\n");
 fclose(fp);

 lista_diagrafi(&list);
 free(common_dir);
 free(input_dir);
 free(mirror_dir);
 free(log_file);
 exit(0);
}
