#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h> 
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/wait.h>

int PORT = 5060;
int sock, connfd;
struct sockaddr_in servaddr, cli;
typedef enum {F, T} boolean;


int main()
{
    boolean send_to_server = F;
    char inp[1024];
    char inp2[20];
    printf("do you to continue:Y/N");
    scanf("%s",inp2);
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("current working directory is: %s\n", cwd);
     // 2 - getcwd
         if (getcwd(cwd, sizeof(cwd)) != NULL) 
        {
       printf("Current working dir: %s\n", cwd);
        } 
         else 
        {
       perror("getcwd() error");
        }

    while (1)
    {
        // 1 -EXIT
        printf("enter commend:");
        scanf("%s",inp);
        if (strcmp(inp,"EXIT") == 0)
        {
            exit(0);
        }

        // 3 -ECHO
       else if (strcmp(inp,"ECHO") == 0){
            // char inp1[20];
            // scanf("%s",inp1);
            // puts(inp1);
            //this is implemetion with system commend
            // setenv("ShellVar", inp1, 1) ;  
            // system("echo $ShellVar") ;  
             getchar();
            fgets(inp, 1024, stdin); 
            if(send_to_server) {
                send(sock , inp , strlen(inp) , 0);
            }
             else 
                printf("%s", inp);
        }

        // 4 - TCP PORT
       else if (strcmp(inp, "TCP") == 0)
       {
           scanf("%s",inp);
           if(strcmp(inp, "PORT") == 0){
               char inp1[20];
                scanf("%s",inp1);
                // socket create and verification
                int sock = socket(AF_INET, SOCK_STREAM, 0);
                if (sock == -1) 
                {
                    printf("socket creation failed...\n");
                    exit(0);
                }
                else 
                    printf("Socket successfully created..\n");
                    bzero(&servaddr, sizeof(servaddr));
        
                    // assign IP, PORT
                    servaddr.sin_family = AF_INET;
                    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
                    servaddr.sin_port = htons(PORT);
        
                // connect the client socket to server socket
                if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) 
                {
                    printf("connection with the server failed...\n");
                    exit(0);
                }
                else{
                    printf("connected to the server..\n");
                    int bytesSent = send(sock, inp1, strlen(inp1), 0); 
                    if (-1 == bytesSent){
                        printf("Error in sending file: %d", errno);
                    }
                    else if (0 == bytesSent){
                        printf("peer has closed the TCP connection prior to send().\n");
                    }
                    else if (1 > bytesSent)
                    {
                        printf("sent only %d bytes from the required %d.\n", bytesSent, 1); 
                    }
                    else {
                        printf("Message was successfully sent. Send %d bytes: \n", bytesSent);
                    }
                    close(sock);
                }
            }    
        }  

        // 5 - LOCAL
        else if (strcmp(inp,"LOCAL") == 0)
        {
            close(sock);
            return 0;
            // this is implemetion with system commend
            setenv("ShellVar", inp, 1) ;  
            system("local") ;  
        } 

        // 6 - DIR
       else if (strcmp(inp,"DIR") == 0){
           struct dirent *de;  
            DIR *dr = opendir(".");
            if (dr == NULL) { // opendir returns NULL if couldn't open directory
                printf("Could not open current directory" );
                break;
            }
            while((de = readdir(dr)) != NULL) 
            {
                if(de->d_name[0]!='.')
                    printf("%s\n", de->d_name);
            }
            closedir(dr);
            //this is implemetion with system commend
            // system("dir");
        } 

        // 7 - CD
        else if (strcmp(inp,"CD") == 0)
        {
            char cd[256];
            getchar();
            scanf("%s",cd);
            if (chdir(cd) != 0) 
            {
             perror("chdir() failed");
            //this is implemetion with system commend
            // setenv("ShellVar", inp1, 1) ;  
            // system("cd $ShellVar") ;  
        }
        }

         // DELETE 
        else if(!strcmp (inp,"DELETE")) 
        {
            char filename[256];
            scanf("%s",filename);
            if(unlink(filename) == 0)
                printf("succses!!");
            else 
             perror("failed - File does not exist");

            //this is implemetion with system commend
            // setenv("ShellVar", inp1, 1) ;  
            // system("delete $ShellVar") ;  
        } 

        // COPY
         else if(!strcmp(inp,"COPY"))
        {
            char src[256];
            char dst[256];
            scanf("%s",src);
            scanf("%s",dst);           
	        FILE *fp1, *fp2;  // declaring file pointers
	        fp1 = fopen(src, "a+");
	        fp2 = fopen(dst, "a+");
	        if (!fp1 && !fp2) // If file is not found then return.
            { 
		        printf("Unable to open/""detect file(s)\n");
		        break;
            }            
            char buf[100];
	        fprintf(fp2, "\n"); // explicitly writing "\n" to the destination file so to enhance readability.
	        while (!feof(fp1)) {
		        fgets(buf, sizeof(buf), fp1);
		        fprintf(fp2, "%s", buf);
            }
            rewind(fp2);
        }
          

        //system
        // else if(!strcmp (inp,"system"))
        //  system(inp);
       

        //******* without system : run by - fork, exec, wait
        else 
        { 
          int pid = fork();
          if(pid == 0)
          {
              char* arr[] = {inp,NULL};
              execvp(arr[0],arr);
              exit(0);
          }
          else
          {
              wait(0);
          }
        }
    }
    return 0;
}

