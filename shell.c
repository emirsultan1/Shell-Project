// our c program to create a custom shell in ubuntu virtual box
// included libraries that are required to run the code
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 
  
#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 100 // max number of commands to be supported 
  
// clearing the shell using escape sequences 
#define clear() printf("\033[H\033[J") 


// colours for the shell

#define PRINT_GREEN(message) printf(GREEN "%s" RESET "\n" , message)
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;35m"
#define RESET "\033[0m"
  
// a message that will be shown when shell starts up
void init_shell() 
{ 
    clear(); 
    printf("%s\n\n\n\n******************"
        "************************%s", GREEN , RESET); 
    printf("%s\n\n\n\t WELCOME TO OUR CUSTOM SHELL %s\n" , GREEN , RESET); 
    printf("%s\n\n\n\n*******************"
        "***********************%s" , GREEN , RESET); 
    char* username = getenv("USER"); 
    printf("%s\n\n\nUSER is: @%s%s", BLUE , username , RESET); 
    printf("\n"); 
    sleep(1); 
    clear(); 
} 
  
// Function to take input 
int takeInput(char* str) 
{ 
    char* buf; 
  
    buf = readline("\nenter your command here: "); 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
} 

  
// function to print the Prompt required. 
void printPrompt() 
{ 
    char cwd[1024]; 
    char machineName[64];
    // get current user
    char *username = getenv("USER");
    // get current machinename
	if(gethostname(machineName , sizeof(machineName)) == 0){
	// Print the promt in the format: machinename@username:~$
	printf("%s@%s:~$" , machineName , username);
	} else {
	// Print promt without machine name if retrieval fails
	printf("%s@:~$" , username);
	}
	
} 
  
// function where the system command is executed 
void execArgs(char** parsed) 
{ 
    // forking a child 
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == 0) { 
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        waitpid(pid , NULL , 0);  
        return; 
    } 
} 
  
// Function where the piped system commands is executed 
void execArgsPiped(char** parsed, char** parsedpipe) 
{ 
    // 0 is read end, 1 is write end 
    int pipefd[2];  
    pid_t p1, p2; 
  
    if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork"); 
        return; 
    } 
  
    if (p1 == 0) { 
        // Child 1 executing.. 
        // It only needs to write at the write end 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command 1.."); 
            exit(0); 
        } 
    } else { 
        // Parent executing 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork"); 
            return; 
        } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
        if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(parsedpipe[0], parsedpipe) < 0) { 
                printf("\nCould not execute command 2.."); 
                exit(0); 
            } 
        } else { 
            // parent executing, waiting for two children 
            waitpid(p1 , NULL , 0); 
            waitpid(p2 , NULL , 0); 
        } 
    } 
} 
  
// Help command builtin 
void openHelp() 
{ 
    PRINT_GREEN("\n***WELCOME TO MY SHELL HELP***"
        "\nList of Commands supported:"
        "\n>cd"
        "\n>ls"
        "\n>exit"
        "\n>all other general commands available in UNIX shell"
        "\n>pipe handling"
        "\n>improper space handling"); 
  
    return; 
} 

// function to display command history
void showHistory() {

	HIST_ENTRY **the_history_list;
	int i;

	the_history_list = history_list(); // to get the history list from readline
	if(the_history_list) {
		for(i = 0; the_history_list[i]; i++){
			printf("%d: %s\n" , i + 1, the_history_list[i]->line);
			}
	} else {
	printf("no commands in history.\n");
	}
}

//function that we created that sets reminders
void handleRemindMe(char** parsed){

	if(parsed[1] == NULL){
	
		printf("usage: remindme <time in seconds> \"<reminder message>\"\n");
		return;
		}
	int delay = atoi(parsed[1]);
	if(delay <= 0){
	
		printf("invalid time specified.\n");
		return;
		}
	char message[256] = {0};
	for(int i = 2; parsed[i] != NULL; i++){
		strcat(message , parsed[i]);
		if(parsed[i + 1] != NULL) strcat(message, " ");
	}
			
		
	printf("Reminder set for %d seconds.\n" , delay);
	sleep(delay); // this will cause the shell to wait, not suitable for actual use but good for demonstration.
	
	printf("\nREMINDER: %s\n\n" , message);
	fflush(stdout);
	
}


  
// Function to execute builtin commands 
int ownCmdHandler(char** parsed) 
{ 
    int NoOfOwnCmds = 6, i, switchOwnArg = 0; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username; 
  
    ListOfOwnCmds[0] = "exit"; 
    ListOfOwnCmds[1] = "cd"; 
    ListOfOwnCmds[2] = "help"; 
    ListOfOwnCmds[3] = "hello"; 
    ListOfOwnCmds[4] = "history";
    ListOfOwnCmds[5] = "remind"; 
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchOwnArg) { 
    
    case 1: 
        printf("\n%sHave a great day!!%s\n" , GREEN , RESET); 
        exit(0); 
    
    case 2: 
        chdir(parsed[1]); 
        return 1; 
    
    case 3: 
        openHelp(); 
        return 1; 
    
    case 4: 
        username = getenv("USER"); 
        printf("%s\nHello %s" , BLUE , username);
        printf("%s\nI hope your having a great day."
	"\nUse the help command to know more about me\n%s" , GREEN , RESET); 
        return 1; 
        
    case 5: 
        showHistory(); 
        return 1;
        
    case 6: 
        handleRemindMe(parsed); 
        return 1;
    
    default: 
        break; 
    } 
  
    return 0; 
} 
  
// function for finding pipe 
int parsePipe(char* str, char** strpiped) 
{ 
    int i; 
    for (i = 0; i < 2; i++) { 
        strpiped[i] = strsep(&str, "|"); 
        if (strpiped[i] == NULL) 
            break; 
    } 
  
    if (strpiped[1] == NULL) 
        return 0; // returns zero if no pipe is found. 
    else { 
        return 1; 
    } 
} 
  
// function for parsing command words 
void parseSpace(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 
  
int processString(char* str, char** parsed, char** parsedpipe) 
{ 
  
    char* strpiped[2]; 
    int piped = 0; 
  
    piped = parsePipe(str, strpiped); 
  
    if (piped) { 
        parseSpace(strpiped[0], parsed); 
        parseSpace(strpiped[1], parsedpipe); 
  
    } else { 
  
        parseSpace(str, parsed); 
    } 
  
    if (ownCmdHandler(parsed)) 
        return 0; 
    else
        return 1 + piped; 
}
 
  
int main() 
{ 
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
    char* parsedArgsPiped[MAXLIST]; 
    int execFlag = 0; 
    init_shell(); 
  
    while (1) { 
        // print shell line 
        printPrompt(); 
        // take input 
        if (takeInput(inputString)) 
            continue; 
        // process 
        execFlag = processString(inputString, 
        parsedArgs, parsedArgsPiped); 
        // execflag returns zero if there is no command 
        // or it is a builtin command, 
        // 1 if it is a simple command 
        // 2 if it is including a pipe. 
  
        // execute 
        if (execFlag == 1) 
            execArgs(parsedArgs); 
  
        if (execFlag == 2) 
            execArgsPiped(parsedArgs, parsedArgsPiped); 
    } 
    return 0; 
} 

