#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<sys/types.h>
#include<signal.h>
#define size 1024
char home[size]="/home/mukul";
void sig_handler(int signo)
{
     if (signo == SIGINT)
	  printf("received SIGINT\n");
     else if (signo == SIGQUIT)
	  printf("received SIGINT\n");
     else if (signo == SIGUSR1)
	  printf("received SIGUSR1\n");
     else if (signo == SIGTSTP)
     {
	  printf("received SIGTSTP\n");
//	  printf("%d\n",fore_id);
//	  if(fore_id!=-1)
//	       kill(fore_id,SIGTSTP);  
     }
     else if(signo==SIGCHLD);
     // printf("received SIGCHLD\n");
}

void after_every_command(){
	char *username=getenv("USER");
        char host[size];
        gethostname(host,size);
        printf("%s@",username);
        printf("%s:",host);
	char cwdirectory[size];
        getcwd(cwdirectory,size);
        printf("%s~$",cwdirectory);
	int ret=strcmp(home,cwdirectory);
	//printf("%d",ret);
	}
void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv)
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
}
int execpipe (char ** argv1, char ** argv2) {
    int fds[2];
    pipe(fds);
    int i;
    pid_t pid = fork();
    if (pid == -1) { //error
        char *error = strerror(errno);
        printf("error fork!!\n");
        return 1;
    } 
    if (pid == 0) { // child process
        close(fds[1]);
        dup2(fds[0], 0);
        //close(fds[0]);
        execvp(argv2[0], argv2); // run command AFTER pipe character in userinput
        char *error = strerror(errno);
        printf("unknown command\n");
        return 0;
    } else { // parent process
        close(fds[0]);
        dup2(fds[1], 1);
        //close(fds[1]);
        execvp(argv1[0], argv1); // run command BEFORE pipe character in userinput
        char *error = strerror(errno);
        printf("unknown command\n");
        return 0;
    }
}
int main(void){
     char  line[1024];             /* the input line                 */
     char  *argv[64];              /* the command line argument      */

     while (1) {
	  if (signal(SIGINT, sig_handler) == SIG_ERR)
	       printf("\ncan't catch SIGINT\n");
	  if (signal(SIGQUIT, sig_handler) == SIG_ERR)
	       printf("\ncan't catch SIGQUIT\n");
	  if (signal(SIGUSR1, sig_handler) == SIG_ERR)
	       printf("\ncan't catch SIGUSR1\n");
	  if (signal(SIGTSTP, sig_handler) == SIG_ERR)
	       printf("\ncan't catch SIGTSTP\n");
	  if(signal(SIGCHLD,sig_handler)==SIG_ERR)
		printf("\ncan't catch SIGCHLD\n");                   /* repeat until done ....         */
          after_every_command();     /*   display a prompt             */
          gets(line);              /*   read in the command line     */
          printf("\n");
          parse(line, argv);       /*   parse the line               */
          if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
               exit(0);            /*   exit if it is                */
          execute(argv);           /* otherwise, execute the command */
     }

	return(0);
}

