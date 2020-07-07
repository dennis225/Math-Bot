# Math-Bot

# Socket Requirements
Your client is expected to be written in C, not a high-level language such as python (no matter how tempting that might be). In order to write a networking program in C, you must use various system calls provided by the socket API. In particular, the system calls you might find useful are:

socket(int domain, int type, int protocol);
ssize_t send(int socket, const void * buffer, size_t length, int flags);
ssize_t recv(int socket, void * buffer, size_t length, int flags);
ssize_t connect(int socket, const struct sockaddr *address, socklen_t address_len);
int close(int fildes);

If you are unfamiliar with any of these, you should read up on it, review the course material and example code, look at the man pages. Reviewing the man pages will be very helpful during the completion of this assignment. Please do not neglect this valuable resource. Note: These system calls are functions, and just like any function they can possibly return errors. Be sure to check the return values of these functions to make your client more robust.

To make it a little easier on you we provide you with the minimum list of header files that you need to include in your client in order to produce a working client:

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

Notice that connect takes as an argument: struct sockaddr *address. This is one of the challenges of the assignment. You will have to populate the struct with the IP address and port number as well as other information. An extremely helpful link for this is found in our code example with slides. Please read that entire code closely! It is very short, but dense. Hint: The network requires big endian format, so you will have to convert between the two. Helpful functions for this are htons and inet_pton. They do have man pages available to anyone with a Linux (vagrant) or Mac distribution!

# Protocol Requirements
To capture the flag from the math bot server you must combine your understanding of the socket API functions and an application-layer protocol known as math speak which is as follows:

When you first connect to the server you must identify yourself. In particular, you must send the following string:

cs230 HELLO <NETID>@umass.edu\n

This string must be exactly as we describe above - network protocols are very specific. You must replace <NETID> with your UMass NetID. By sending the identification string it will initiate the math bot server to start sending you math problems. You will immediately receive a "status" message with the following format:

cs230 STATUS NUM OP NUM\n

This status message includes a simple arithmetic operation. An example of an actual message is:

cs230 STATUS 505 * 700\n

You will need to implement functionality in your client that will compute the math problem provided in the server's status message. After you do that you need to send a response back to the math bot server that is formatted like this:

cs230 <ANSWER>\n

You must replace <ANSWER> with the answer to the math problem. Here is an example:

cs230 353500\n

With your response back to the server, the server will then repeatedly send you hundreds of math problems that your client must solve. Your client will need to solve each of the status math problems until you receive the response with the flag:

cs230 <FLAG> BYE\n

The <FLAG> is a long hash value. Here is an example of the final message you will receive from the server before it disconnects from your client:

cs230 7c5ee45183d657f5148fd4bbabb6615128ec32699164980be7b8b451fd9ac0c3 

If you are able to "capture the flag" you have completed this assignment successfully. You still need to submit it to Gradescope though.

# Program Requirements
Your client program must accept the following command line arguments in this order:

Identification
Port
Host IP address

The first argument (Identification) must be a UMass email address of the form "NetID@umass.edu". The port and host IP address are as they are defined by the socket API.

We will be running a test server at address 128.119.243.147 on port 27993. You are welcome to test your client by running it like so:

$ ./client netid@umass.edu 27993 128.119.243.147

And see if you can capture your flag! Note: it took 22 seconds for our solution client to complete over a 5 Mbps connection. If it seems to take a while, it may not be wrong. If it never ends you should then add debugging output to see where it is getting blocked.
