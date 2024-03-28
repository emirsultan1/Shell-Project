# Our custom shell project

Students involved in this project: Emir Sultan , Mersiha Baltic
Module: Operating Systems



## Files/Directories
README.md: Information about our project
shell.c: the full c program in one c file



## An outline of our project

NOTE -- To run and compile our custom shell you need to go to the terminal and write

gcc shell.c -lreadline 

./a.out

We have created a simple shell program that can run on linux systems. I will now go into all the functionalities in detail.
- initialization --> the init_shell() function is called to clear the screen and display a message when the shell starts up. It also shows the current user. Also, I have put a sleep function so the text on the display remains for a second then disappears with the clear function after that.
- Input Handling --> the takeInput() function is esponsible for reading input from the user using the readline() function provided by the readline library. It also maintains command history. 
- Colouring and styling of the shell --> At the top I have pre-defined all the colours and their related codes that I wish to implement in our shell project. I have also used them in the places that I found were best suited for them.
- Directory Display --> The printDir() function retrieves and prints the current working directory.
- Command Execution --> The execArgs() function is used to execute single commands entered by the user. It forks a child process and uses execvp() to execute the command.
- Piped Command Execution --> The execArgsPiped() function handles the execution of commands separated by a pipe symbol |. It creates a pipe using pipe() and forks two child processes to execute each command on either side of the pipe.
- Something I created differently --> I also made a different help command that will be called when the user writes “help” in the terminal. This command works the same with the original bash shell help but with my own twist and where a different text is being shown. This can be seen with the openHelp() function that I have created. This function is also residing in the function ownCmdHandler() where its functions are coming into play. Also we can find some other commands in the ownCmdHandler() function such as “exit” , “hello” , and “cd”. These are a bit different but also very similar to the commands in the basic bash shell. Of course all other general commands are available.
-Parsing --> The parsePipe() and parseSpace() functions are used to parse the input command string into individual commands and arguments.

- showHistory --> We also created a showHistory function that basically shows a list of all the commands that we have used. They are saved in memory due to the readline library that was implemented.

- Main Loop--> The main loop continuously prompts the user for input, processes the input string, and executes the commands accordingly.



## Challenges that we have encoutered during the creation of this custom shell

The main challenges of creating a project like this was really trying to understand the fork() and forkbomb() functions. Also we had a difficult time with analysing and creating functions with anything that's related to piping. We found that a bit more advanced but we managed to pull through with some extra work. Some other challenges involved trying to learn C as it is a new programming language for us. Also we did not now how to put all these different functions such as main and execute into different C files and still make the shell work effeciently. To counter this we just made a massive c file with everything included in it. I hope you understand.



## Main sources that we used for the project

i) https://www.youtube.com/watch?
v=cIBmeEpsMj0&list=LL&index=13&t=1s&ab_channel=Jess-Inspired https://www.youtube.com/watch?v=cIBmeEpsMj0&list=LL&index=13&t=1s&ab_channel=Jess-Inspired
 
ii) https://www.geeksforgeeks.org/

iii) https://medium.com/@winfrednginakilonzo/guide-to-code-a-simple-shell-in-c-bd4a3a4c41cd

iv) https://brennan.io/2015/01/16/write-a-shell-in-c/

v) We also used GPT a lot and other ai for help. This was a bit more advanced project and we found it somewhat difficult so you will probably find a lot of our code being like that of online examples, but we tried to change and implement our own one as much as possible.



## Answers to the questions of task 1.5

1.5.1 --> What is the purpose of the fork() system call?
The purpose of fork() is to allow a process to create a copy of itself, resulting in two separate and independent processes called the parent process and the child process. The primary purpose of fork() is to enable multitasking and parallel execution of processes. Once fork() is called, the child process is created as an exact copy of the parent process, including its memory space, file descriptors, and program state. However, the child process has its own unique process identifier (PID) and runs independently of the parent process. 

1.5.2 (A) How does the process obtain the address of the kernel instruction to jump to?

When a process initiates a system call to execute kernel code, it doesn't directly know the address of the kernel instruction to jump to. Instead, it triggers a software interrupt or executes a specific system call instruction (like syscall on x86 or svc on ARM), which is predefined to signal the CPU to switch from user mode to kernel mode. This action causes the processor to refer to an interrupt vector table, a data structure maintained by the kernel that maps each type of interrupt to its handling routine's address. The entry for the system call interrupt in this table points to the system call dispatcher in the kernel. The dispatcher then uses the system call number, provided by the process, to look up and jump to the correct kernel function that implements the requested system call. This mechanism abstracts the actual kernel instruction addresses, providing a secure and standardized method for user-space applications to request services from the kernel.

1.5.2 (B)

During the transition from user mode to kernel mode for a system call, the userspace context of the process, including the program counter and other CPU registers, is stored on the process's kernel stack. This dedicated area within the kernel space is reserved for each process to save its execution state before the kernel begins executing the system call. This mechanism ensures that once the system call is completed, and control is returned to the user-space program, the process's original context can be restored accurately, allowing it to continue execution seamlessly from where it left off. This context-saving step is crucial for maintaining the integrity and continuity of the process's execution across the user-kernel boundary.


1.5.3 (coding question)

I will explain the code itself first. 

1) the integer i is initialized to 1
2) the first while loop increments i until it reaches 100. This loop increases i by 1 in each iteration until i is no longer less that 100
3) using the printf command the program outputs the value of i, which would be 100 at this point followed by a space.
4) another while loop is seen in the code. This while loop decrements i until it reaches 0. i is starting at 100 now and it eventually reaches 0.
5) the program again uses printf to output the value of i followed by a space. i is now 0. 

Now assuming that this is the only process that the CPU is executing, the following process state transitions would occur during the execution of the program:


1) Ready State: The process starts in the ready state, waiting for CPU allocation.
2) Running State: Once the CPU is allocated, the process enters the running state to execute its code, including loops and printf statements.
3) Sleep/Wait State: During execution, the process may implicitly enter the wait state for I/O operations like printf.
4) Running State: The process resumes the running state after waiting is completed.
5) Terminated State: After completing its execution, the process moves to the terminated state, where it is cleaned up by the operating system.

## Some issues we had with the project

To be honest we were discouraged to actually implement the forkbomb into our shell project as we were afraid it would destroy our pc's. Instead i will provide the code for the forkbomb if I were to implement it. Code it below------>

I would have to set a max_processes above next to my libraries. I have to set a reasonable number like maybe 5 or 10. I would also define a forkbomb delay where there is a delay between forks to ease the system.



#define MAX_FORKBOMB_PROCESSES 5  // Limit the maximum number of processes
#define FORKBOMB_DELAY 1  // Delay in seconds between forks, to ease the system load

void controlledForkbomb() {
    printf("Launching a controlled forkbomb... Please be cautious.\n");
    int createdProcesses = 0;

    for (createdProcesses = 0; createdProcesses < MAX_FORKBOMB_PROCESSES; ++createdProcesses) {
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            printf("Child process created: %d\n", getpid());
            sleep(FORKBOMB_DELAY);  // Sleep to simulate work and slow down forking
            exit(0);  // Child exits
        } else if (pid > 0) {
            // Parent process
            printf("Parent process, created a child: %d\n", pid);
            sleep(FORKBOMB_DELAY);  // Parent waits before potentially forking again
        } else {
            // Fork failed
            perror("Fork failed");
            break;
        }
    }

    printf("Controlled forkbomb completed. Created %d processes.\n", createdProcesses);
}
