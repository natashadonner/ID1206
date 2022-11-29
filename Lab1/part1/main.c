/* 
** @author Natasha Donner, ndonner@kth.se
** @date 2022.11.12
** @compile gcc -o main main.c
** @run /.main
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

main(int argc, char const *argv[])
{
    char *ls_argv[] = {"ls", "/", NULL};
    char *wc_argv[] = {"wc", "-l", NULL};

    int pfd[2];

    pipe(pfd);  // pfd[0] read
                // pfd[1] write

    pid_t pid = fork();

    if (pid == -1) // error handling for fork
    {   
        perror("fork"); 
        return -1;
    }


    /* 
    ** child process
    ** pid = 0
    */
    else if (pid == 0)  
    {   
        close(pfd[0]); //close unused read pipe-end

        //dup2(X,Y) Y => std_in = 0, std_out = 1, stderr = 2
        dup2(pfd[1],1); //redirect std_out to write pipe-end 

        int ret = execvp(ls_argv[0],ls_argv); //execvp is pathname
        if (ret == -1)
        {
            perror("execv");
            exit(1);
        }
    }

    /* 
    ** parent process 
    */
    else 
    {
        close(pfd[1]);  //close unused write pipe-end
        dup2(pfd[0],0); //redirect std_in to read pipe-end 

        // Execute wc command
        execvp(wc_argv[0],wc_argv);
        printf("Done with execv");

    }

    return 0; 
}
