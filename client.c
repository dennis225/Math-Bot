#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <netdb.h>

// Open connection to server at <hostname, port>

int open_clientfd(char *hostname, int port) {
  // The client's socket file descriptor.
  int clientfd;
  // The hostent struct is used to get the IP address of the server
  // using DNS.
  struct hostent *hp;
  // serveraddr is used to record the server information (IP address
  // and port number).
  struct sockaddr_in serveraddr;
  // First, we create the socket file descriptor with the given
  // protocol and protocol family.
  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
  // Query DNS for the host (server) information.
  if ((hp = gethostbyname(hostname)) == NULL) return -2;
  // The socket API requires that you zero out the bytes!
  bzero((char *)&serveraddr, sizeof(serveraddr));
  // Record the protocol family we are using to connect.
  serveraddr.sin_family = AF_INET;
  // Copy the IP address provided by DNS to our server address
  // structure.
  bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr,
        hp->h_length);
  // Convert the port from host byte order to network byte order and
  // store this in the server address structure.     
  serveraddr.sin_port = htons(port);
   // Establish a connection with the server.
  if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    return -1;

  // Return the connected file descriptor.
  return clientfd;
}

// This function performs the math problem in the server's status message

int math_func(char buff[50]){

  // Split string by delimiter
  char* arg_split = strtok(buff," ");
  int i=0; // count
  int num1 = 0; // first number
  int num2 = 0; // second number 
  char oper='\0';
  while(i<=3){
   arg_split = strtok(NULL, " ");
   if(i==1){
     num1 = atoi(arg_split);
   }
   if(i==2){
    // Check 'add' argument 
    if(strcmp(arg_split, "+")==0){
      oper = '+'; 
    }
    // Check 'subtract' argument
    else if(strcmp(arg_split, "-")==0){
      oper = '-';
    }
    // Check 'multiply' argument
    else if(strcmp(arg_split, "*")==0){
      oper = '*';
    }
    // Check 'divide' argument
    else if(strcmp(arg_split, "/")==0){
      oper = '/';
    }
   }
   if(i==3){
     num2 = atoi(arg_split);
   }
   i++;
  }

  // Return the math operations
  if(oper=='+'){
    return num1+num2;
  }
  else if(oper=='-'){
    return num1-num2;
  }
  else if(oper=='*'){
    return num1*num2;
  }
  else if(oper=='/'){
    return num1/num2;
  }
  return 0;
}


int main(int argc, char** argv) {

  // check if valid number of arguments passed
  if(argc!=4){
    printf("Invalid number of arguments");
    exit(0);
  }
  // store the host domain name
  char* host = argv[3];
  // initialize port to second 
  int port = atoi(argv[2]);
  // open the client socket file descriptor
  int client_fd = open_clientfd(host,port);
  // buff to receiver the message from the server
  char buff[50];
  sprintf(buff,"cs230 HELLO %s\n",argv[1]);
  //sending request to server by passing clientfd.
  int sendReq =  send(client_fd,buff,strlen(buff),0);
  if(sendReq<0){
    printf("error while sending message");
    exit(0);
  }
  
  while(1){
    // receiving from the server
    int receiveCount = recv(client_fd,buff,50,0);
    buff[receiveCount]='\0';
    printf("%s",buff);
    //checking if it has STATUS after cs230 in the buff text sent by server
    if(buff[6]=='S'){
      // calling math_func to get the end result after operation
      int result = math_func(buff);
      sprintf(buff, "cs230 %i\n",result);
      printf("%s",buff);
      sendReq = send(client_fd,buff,strlen(buff),0); //sending response to server
      if(sendReq<0){
         printf("error while sending message");
         exit(0);
      }
    }
    else{
      printf("\nServer disconnected");
      break;
    }
  }
  //close file descriptors
  close(client_fd);
  return 0;
}