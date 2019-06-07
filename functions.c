/* finction.c */
#include "functions.h"



 /*
  Elegxei oti dothikan ola ta orismata sti grammi entolon.
  Vazei ta orismata stis metablites
 */
 void check_arguments(int argc,char *argv[],int *id,int *b,char **common_dir,char **input_dir,
		      char **mirror_dir,char **log_file)
 {
   char *arg;
   if(argc != 13 ){
       printf("\nError in arguments command line. \n\n");
       exit(EXIT_FAILURE);
   }

   while(--argc){
        arg = *++argv;
        if(!strcmp(arg, "-n")){
	  *id = atoi(*++argv);
        }else if(!strcmp(arg, "-c")){
	    *common_dir = malloc((strlen(*++argv) + 1) * sizeof(char));
	    strcpy(*common_dir,*argv);
        }else if(!strcmp(arg, "-i")){
	    *input_dir = malloc((strlen(*++argv) + 1) * sizeof(char));
	    strcpy(*input_dir,*argv);
        }else if(!strcmp(arg, "-m")){
	    *mirror_dir = malloc((strlen(*++argv) + 1) * sizeof(char));
	    strcpy(*mirror_dir,*argv);
	}else if(!strcmp(arg, "-b")){
	    *b = atoi(*++argv);
	}else if(!strcmp(arg, "-l")){
	    *log_file = malloc((strlen(*++argv) + 1) * sizeof(char));
	    strcpy(*log_file,*argv);
	}
        if(argc > 1) argc--;
   }

    return;
 }



 /*
  Elegxei an yparxei to input_dir.An den yparxei termatizei me lathos.
  An yparxei mirror_dir me to idio onoma termatizei me lathos.
  An den yparxei to mirror_dir to dimiourgei kai an den yparxei to common_dir to dimiourgei
 */
 void check_directory(int id,char *common_dir, char *input_dir, char *mirror_dir, char *log_file){

     DIR *dir, *dir2;
     if((dir = opendir(input_dir)) == NULL){
         perror("input_dir does not exist! \n");
         exit(1);
     }
     else closedir(dir);

     if((dir2 = opendir(mirror_dir)) != NULL){
         perror("mirror_dir exist! \n");
         exit(1);
     }
     mkdir(mirror_dir,0700);

     struct stat st = {0};
     if (stat(common_dir, &st) == -1) {
         mkdir(common_dir, 0700);
     }
     char path[150]; // path for common
     sprintf(path, "%s/%d.id", common_dir, id);


     if(open(path,O_RDWR|0644) != -1){
         printf("file %d.id exist \n",id);
         exit(1);
     }
     int filedes;
     if((filedes = open(path,O_CREAT|O_RDWR|O_APPEND,0644)) == -1){
         printf("file %d.id exist\n",id);
         exit(1);
     }

     char str_id[20], *strId = NULL;
     sprintf(str_id, "%ld\n", (long)getpid());
     strId = malloc((strlen(str_id)+1) * sizeof(char));
     strcpy(strId,str_id);

     if(write(filedes, strId, strlen(strId)) == -1) printf("\nERROR write \n");
     close(filedes);

     FILE *fp = fopen(log_file, "w");
     if(fp == NULL){
	 printf("Error open logfile \n");
	 exit(1);
     }
     fprintf(fp, "-30\n%d\n", id);
     fclose(fp);

     free(strId);
     return;
 }



 /*
  To vasiko loop sti main trexei oso h global metavliti g_stop
  einai 0.Otan parei sima SIGINT h SIGQUIT h g_stop ginetai 1
  kai to loop spaei.Meta to loop paei gia termatismo o client.
 */
 void f_handler(int signal){

     g_stop = 1;
 }



 /*
  Signal apo to paidi reader pros ton patera oti elabe ta arxeia
  kai o pateras ektipwnei to minima
 */
 void sig_from_childs(int sig){

     if(sig == SIGUSR1) printf("\nTransport %s to %d.id completed. \n\n", g_sen_id, g_my_id);
 }



 /*
  To paidi reader termatise anomala hte epeidi perasn 30 sec xwris na diavasei hte
  h read() epestrepse -1 ara lathos.
  Efoson to paidi reader termatise prepei na skotvthei kai o apostoleas
 */
 void sig_handler(int sig, siginfo_t *siginfo, void *con){

     long id_receiver = siginfo->si_pid;
     kill(id_receiver-1, SIGKILL);
 }



 /*
  To paidi writer termatise anomala
  Prepei o pateras na skotvsei kai to paidi reader
 */
 void sig_handler2(int sig, siginfo_t *siginfo, void *con){

     long id_sender = siginfo->si_pid;
     kill(id_sender+1, SIGKILL);
 }



 /*
  Elegxei ton common dir gia neous client
  An vrei kainourgio client tote epistrefei to id tou neou client
  Diaforetika NULL
 */
 char * check_my_list(info_deikti *lista,int my_id,char *common_dir){

     char my[30];
     sprintf(my, "%d", my_id);
     strcat(my,".id");


     DIR *dir = opendir(common_dir);
     struct dirent *dirEntry;
     while((dirEntry = readdir(dir)) != 0){
	if(strlen(dirEntry->d_name) < 3 || strlen(dirEntry->d_name) > 13 || strcmp(dirEntry->d_name,my) == 0) continue;

        if(exist_in_list(&lista[0],dirEntry->d_name) == 0){
	    closedir(dir);
	    return dirEntry->d_name;
	}
     }
     closedir(dir);
     return NULL;
 }



 /*
  Pernei ena to onoma enos arxeiou kai vriksei to egethos tou
  Sto telow to epistrefei.
 */
 int leng_file(char *input_dir, char *f){

     char file[50];
     strcpy(file,input_dir);
     strcat(file,"/");
     strcat(file,f);

     FILE *fp = fopen(file,"r");

     fseek(fp, 0, SEEK_END);
     int size = ftell(fp);
     fseek(fp, 0, SEEK_SET);

     fclose(fp);
     return size;
 }



 /*
  Antallagi arxeiwn metaxi paidiwn
  Dimiourgountai 2 paidia,o reader kai o writer

  Yparxoun sxolia dipla apo tis entoles
 */
 void antallagi_arxeiwn(int id,char *new_id,char *common_dir,char *mirror_dir,char *input_dir,char *log_file,int size_b){

    int pid, i;
    char *buffer = NULL;
    buffer = malloc(size_b * sizeof(char));
    if(buffer == NULL){
        printf("Error malloc buffer. \n");
	return;
    }

    for(i = 0 ; i < 2 ; i++){
        pid = fork();
	if(pid) pid = 0;
	else{
	    pid = i+1;
 	    break;
	}
    }


    if(pid){
        if(pid == 1){ /**************************    writer    *******************************/

	    char fifo_name[50];
            char A[20];
	    sprintf(A, "%d", id);
	    strcpy(fifo_name,A);
	    strcat(fifo_name,".id_to_");
	    strcat(fifo_name,new_id);
	    strcat(fifo_name,".fifo");				// fifo name

	    char path_fifo[50];
	    strcpy(path_fifo,common_dir);
	    strcat(path_fifo,"/");
	    strcat(path_fifo,fifo_name);			// fifo in common_dir


	    int fd = open(path_fifo, O_WRONLY);			// open if exist with blocking
	    if(fd < 0){
		mkfifo(path_fifo, 0666);			// Create fifo if doesn t exist
	        fd = open(path_fifo, O_WRONLY);			// Open fifo with blocking
	    }

	    DIR *dir;
	    struct dirent *dirEntry;
	    dir = opendir(input_dir);				// open input_dir


	    int sum_bytes = 0;					// sum of all bytes it sent
	    int sum_files = 0;					// sum of all files it sent

	    while((dirEntry = readdir(dir)) != 0){
	        if(strcmp(dirEntry->d_name,".") == 0 || strcmp(dirEntry->d_name,"..") == 0) continue;


		short length_name_file = strlen(dirEntry->d_name);			// length name file
		if( write(fd, &length_name_file, sizeof(length_name_file)) == -1){      // write name length in fifo
		    printf("Error writer. 1\n");
		    printf("errno : %d\n", errno);
		    kill(getppid(), SIGTERM);						// if exist error then writer exit with 1
		    exit(1);								// and sent signal to the father
		}									// father must kill reader child

		char name_file[50];
		strcpy(name_file,dirEntry->d_name);					// file name
	 	if( write(fd, name_file, sizeof(name_file)+1) == -1){			// write file name in fifo
		    printf("Error writer. 2\n");
		    printf("errno : %d\n", errno);
		    kill(getppid(), SIGTERM);
		    exit(1);
		}

		int length_file = leng_file(input_dir, dirEntry->d_name);		// file size
		if( write(fd, &length_file, sizeof(length_file)) == -1){		// write file size in fifo
		    printf("Error writer. 3\n");
		    printf("errno : %d\n", errno);
		    kill(getppid(), SIGTERM);
		    exit(1);
		}

		sum_bytes += length_file;
		sum_bytes += 2 + 4 + strlen(name_file);					// sum of all bytes
		sum_files++;								// suum of all files


		int rep = length_file / size_b;
  		int ypo = length_file % size_b;

		char sen_file[50];
		strcpy(sen_file, input_dir);
		strcat(sen_file, "/");
		strcat(sen_file, dirEntry->d_name);
		int file_sen = open(sen_file, O_RDWR);					// open file in input_dir

		while( rep-- > 0){							// length_file div size_buffer repetitions
		    read(file_sen, buffer, size_b);					// read bytes from input_dir
		    if( write(fd, buffer, size_b) == -1){				// write file in fifo
			printf("Error writer. 4\n");
			printf("errno : %d\n", errno);
			kill(getppid(), SIGTERM);
			exit(1);
		    }
		}

		if(ypo > 0){								// length_file mod size_buffer
		    read(file_sen, buffer, ypo);					// read (length_file mod size_buffer) bytes
		    if( write(fd, buffer, ypo) == -1){					// write (length_file mod size_buffer) bytes
			printf("Error writer. 5\n");
			printf("errno : %d\n", errno);
			kill(getppid(), SIGTERM);
		        exit(1);
		    }
		}
		close(file_sen);
	    }
	    short end = 0;
	    write(fd, &end, sizeof(end));						// end - write 00 bytees


	    closedir(dir);
	    close(fd);
	    free(buffer);
	    FILE *fp = fopen(log_file, "a");						// write in the log_file sum_bytes and sum_bytes
	    fprintf(fp, "-10\n%d\n%d\n", sum_bytes, sum_files);
	    fclose(fp);

        }else{ /* ******************************************     reader    ******************************************************************************* */

	    char fifo_name[50];
	    strcpy(fifo_name,new_id);
	    strcat(fifo_name,"_to_");
	    char A[20];
	    sprintf(A, "%d", id);
	    strcat(fifo_name,A);
	    strcat(fifo_name,".id.fifo");				// fifo name

	    char path_fifo[50];
            strcpy(path_fifo,common_dir);
            strcat(path_fifo,"/");
            strcat(path_fifo,fifo_name);				// fifo in common_dir


	    int fd = open(path_fifo, O_RDWR );         			// open fifo if exist with blocking
            if(fd < 0){
                mkfifo(path_fifo, 0666);                	   	// create fifo if doesn t exist
                fd = open(path_fifo, O_RDWR );         			// open fifo with blocking
            }


	    char kat_newid[50];
	    strcpy(kat_newid, mirror_dir);
	    strcat(kat_newid,"/");
	    strcat(kat_newid,new_id);
	    mkdir(kat_newid,0700);					// create director with name new client in mirro_dir


	    DIR *dir;
            struct dirent *dirEntry;
            dir = opendir(kat_newid);					// open new client director

	    struct pollfd poll_array[1];				// for waiting read until 30 second
	    int pol;
	    while(1){
	 	int sum_bytes = 0;

		char new_file[50];
                strcpy(new_file, kat_newid);
                strcat(new_file,"/");

		poll_array[0].fd = fd;
                poll_array[0].events = POLLIN;
		pol = poll(poll_array, 1, 30000);			// read wait 30 sec

		short length_name_file;
		if(pol == 0){						// reader wait for length_name_file 30 second
		    printf("No data available for 30 sec. 1\n");
		    kill(getppid(), SIGUSR2);				// reader exit and sent signal to the parent
		    exit(1);						// parent must kill writer child
		}else if( (pol == 1) && (poll_array[0].revents == POLLIN) && (poll_array[0].fd == fd) ){
	    	    if( read(fd, &length_name_file, sizeof(length_name_file)) == -1){	// read length_name_file from fifo
		    	printf("Error reader. 1\n");
			kill(getppid(), SIGUSR2);
			exit(1);
		    }
		}

		if(length_name_file == 0) break;					// stop when you get 00

		char name_file[50];
		pol = poll(poll_array, 1, 30000);
		if(pol == 0){
		    printf("No data available for 30 sec. 2\n");
		    kill(getppid(), SIGUSR2);
                    exit(1);
		}else if( (pol == 1) && (poll_array[0].revents == POLLIN) && (poll_array[0].fd == fd) ){
		    if( read(fd, name_file, sizeof(name_file)+1) == -1){		// read file name from fifo
			 printf("Error reader. 2\n");
			 kill(getppid(), SIGUSR2);
			 exit(1);
		    }
		}


		int length_file;
		pol = poll(poll_array, 1, 30000);
		if(pol == 0){
		    printf("No data available for 30 sec. 3\n");
		    kill(getppid(), SIGUSR2);
		    exit(1);
		}else if( (pol == 1) && (poll_array[0].revents == POLLIN) && (poll_array[0].fd == fd) ){
		    if( read(fd, &length_file, sizeof(length_file)) == -1){		// read file size from fifo
			printf("Error reader. 3\n");
		        kill(getppid(), SIGUSR2);
			exit(1);
		    }
		}

		sum_bytes += 2 + 4 + strlen(name_file);
		sum_bytes += length_file;
		FILE *fp;
                fp = fopen(log_file, "a");						// print to the lof_file crowd bytes and file name
                fprintf(fp, "-20\n%d\n%s\n", sum_bytes, name_file);
                fclose(fp);

		strcat(new_file,name_file);
		int filedes = open(new_file ,O_CREAT|O_RDWR|O_APPEND ,0644);		// create the file

		int rep = length_file / size_b;
                int ypo = length_file % size_b;
                while(rep-- > 0){							// read bytes and write in new file
		    pol = poll(poll_array, 1, 30000);
		    if(pol == 0){
		        printf("No data available for 30 sec. 4\n");
			kill(getppid(), SIGUSR2);
			exit(1);
		    }else if( (pol == 1) && (poll_array[0].revents == POLLIN) && (poll_array[0].fd == fd) ){
		        if( read(fd, buffer, size_b) == -1){				// read bytes
			    printf("Error reader. 4\n");
			    printf("errno : %d\n", errno);
			    kill(getppid(), SIGUSR2);
			    exit(1);
			}
		    }
                    write(filedes, buffer, size_b);					// write in the file
                }

		if(ypo > 0){
		    pol = poll(poll_array, 1, 30000);
		    if(pol == 0){
			printf("No data available for 30 sec. 5\n");
			kill(getppid(), SIGUSR2);
                        exit(1);
		    }else if( (pol == 1) && (poll_array[0].revents == POLLIN) && (poll_array[0].fd == fd) ){
		        if( read(fd, buffer, ypo) == -1){				// read bytes
			    printf("Error reader. 5\n");
			    printf("errno : %d\n", errno);
			    kill(getppid(), SIGUSR2);
			    exit(1);
			}
		    }
		    write(filedes, buffer, ypo);					// write in the file
		}
		close(filedes);
	    }

	    closedir(dir);
	    close(fd);
	    unlink(path_fifo);					// remove fifo
	    free(buffer);
	    kill(getppid(), SIGUSR1);				// sent signal to the parent
        }							// parent must print that the trasport complete
        exit(0);
    }
    free(buffer);
    return;
 }



 /*
  Elegxei na kapoios client efyge apo to sistima.
  An vrei kapoion tote o client ton afairei apo tin lista tou kai kanei 1 paidi
  to opoio analamvanei na digrapsei apo ton miiror_dir ta arxeia pou exei parei apo
  ton client pou efyge
 */
 void check_for_leaves_ids(info_deikti *lista, char *common_dir, char *mirror_dir){

    /* Delete clients from list */
    char *id;
    int i = 1;
    while( (id = take_elem(&lista[0],i)) != NULL){
        DIR *dir;
	dir = opendir(common_dir);
	struct dirent *dirEntry;

	int del = 1;
	while( (dirEntry = readdir(dir)) != 0){
	    if(!strcmp(id,dirEntry->d_name)){ del = 0; break; }
	}
	closedir(dir);

	if(del){
	    diagrafi_komvou(&lista[0],i);
	     printf("\nDelete from my mirror_dir client  %s \n\n", id);
	    i--;
	}
        i++;
    }


    int pid;
    pid = fork();
    if(pid < 0) printf("Error fork \n");


    /* child delete clients files from mirror_dir */
    if(!pid){	// child proccess

        DIR *dir1, *dir2;
	dir1 = opendir(mirror_dir);
	struct dirent *dirEntry1;

	while( (dirEntry1 = readdir(dir1)) != 0){
	    if( strcmp(dirEntry1->d_name,".") == 0 || strcmp(dirEntry1->d_name,"..") == 0) continue;


	    dir2 = opendir(common_dir);
	    struct dirent *dirEntry2;
	    int del = 1;
	    while( (dirEntry2 = readdir(dir2)) != 0){
		if( strcmp(dirEntry2->d_name, dirEntry1->d_name) == 0 ) del = 0;
	    }
	    closedir(dir2);

	    if(del){
		char katal[50];
	  	strcpy(katal,mirror_dir);
		strcat(katal, "/");
		strcat(katal, dirEntry1->d_name);

		DIR *dir3;
		dir3 = opendir(katal);
		struct dirent *dirEntry3;
		while( (dirEntry3 = readdir(dir3)) != 0){
		    char file[50];
		    strcpy(file, katal);
		    strcat(file, "/");
		    strcat(file, dirEntry3->d_name);

		    remove(file);
		}
		closedir(dir3);
		remove(katal);
	    }
	}
	closedir(dir1);
    }
    else return;	// parent proccess

    exit(0);
 }



 /*
  Diagrafei to id tou apo to common_dir.
  Kai ton topiko tou katalogo mirror_dir
 */
 void delete_my_id_and_director(int my_id, char *common_dir, char *mirror_dir){

     /* Delete id from common_dir */
     char id[50];
     sprintf(id, "%d", my_id);
     strcat(id,".id");

     DIR *dir, *dir2;
     dir = opendir(common_dir);
     struct dirent *dirEntry;
     while( (dirEntry = readdir(dir)) != 0){
	 if( !strcmp(id, dirEntry->d_name) ){
	     char A[50];
	     strcpy(A, common_dir);
	     strcat(A, "/");
	     strcat(A, dirEntry->d_name);
	     remove(A);
	     break;
	 }
     }
     closedir(dir);

     printf("\nDelete my id in common_dir and my mirror_dir. \n");


     /* Delete mirror dir */
     dir = opendir(mirror_dir);
     while( (dirEntry = readdir(dir)) != 0){
	 if(strcmp(dirEntry->d_name,".") == 0 || strcmp(dirEntry->d_name,"..") == 0) continue;
	 char sub_dir[50];
	 strcpy(sub_dir, mirror_dir);
	 strcat(sub_dir, "/");
	 strcat(sub_dir, dirEntry->d_name);

	 dir2 = opendir(sub_dir);
         struct dirent *dirEntry2;
	 while( (dirEntry2 = readdir(dir2)) != 0){
	     if(strcmp(dirEntry2->d_name,".") == 0 || strcmp(dirEntry2->d_name,"..") == 0) continue;
	     char file[100];
	     strcpy(file, sub_dir);
	     strcat(file, "/");
	     strcat(file, dirEntry2->d_name);

	     remove(file);
  	 }

	 closedir(dir2);
	 remove(sub_dir);
     }

     closedir(dir);
     remove(mirror_dir);
     return;
 }
