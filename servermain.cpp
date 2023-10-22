#include <stdio.h> 
#include <string.h>  
#include <unistd.h>    
#include <arpa/inet.h>  
#include <sys/time.h> 
#include <calcLib.h>    
#include <math.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#define TRUE   1 
#include <signal.h>
#include<errno.h>
#define PORT 5000

void handler(int signo)
{
  return;
}


// Helper function you can use:

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
  switch (sa->sa_family)
  {
  case AF_INET:
    inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
              s, maxlen);
    break;

  case AF_INET6:
    inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
              s, maxlen);
    break;

  default:
    strncpy(s, "Unknown AF", maxlen);
    return NULL;
  }

  return s;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}


int main(int argc , char *argv[])  
{   
	
	char delim[]=":";
  	char *Desthost=strtok(argv[1],delim);
  	char *Destport=strtok(NULL,delim);
  	//struct addrinfo hints; 
	struct addrinfo* res=0;
	struct addrinfo *bob;
	char addrstring[100];
  	
  	int port=atoi(Destport);
	 
 	printf("Host %s, and port %d.\n",Desthost,port);	

	int master_socket , addrlen , new_socket ; 
 	struct sockaddr_in address;  
	struct addrinfo hints, *servinfo, *p;
    char *ptr;
	int n, rv;
	  
	char buffer[1024];  // buffer set to 1K 
         
    //socket descriptors 
	char message[] = "TEXT TCP 1.0\n\n";      
  	bzero(&hints, sizeof(hints));
  	hints.ai_family = 0; // AF_INET6;
  	hints.ai_socktype=0; //SOCK_STREAM;
  	hints.ai_protocol=0;
  	hints.ai_flags = AI_ALL; //AI_CANONNAME;
	//Check Arguments
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s IP server:Port", argv[0]);
    exit(1);
  }

 if ((rv = getaddrinfo(Desthost, Destport, &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    if ((master_socket = socket(p->ai_family, p->ai_socktype,
                         p->ai_protocol)) == -1)
    {
      perror("check socket");
      exit(1);
    }

//Binding
    if (bind(master_socket, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(master_socket);
      perror("check bind");
      continue;
    }

    break;
  }

  if (p == NULL)
  {
    fprintf(stderr, "failed to bind socket\n");
    return 2;
  }

  freeaddrinfo(servinfo);


////////////

	   	 #ifdef DEBUG
	   	 printf("Listening on port %d \n", port);  	
		 #endif
	    	//Setting up maximum connections for the master socket 
	    if (listen(master_socket, 5) < 0)  
	    	{  
		perror("reject the sixth client");  
		exit(1);  
	    	}
	    	else
	    	{	  	   
    		//Incoming connection 
				addrlen = sizeof(address);  
				#ifdef DEBUG
				printf("Waiting for connections ...");  
				#endif 
			}   
			while(1)  
				{ 
					struct sigaction sa;
					sa.sa_handler = handler;
					sigemptyset(&sa.sa_mask);
					sa.sa_flags = 0;
					sigaction(SIGALRM, &sa, NULL);
					char ERROR[]="ERROR TO\n";    					                       
					if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
						{  
						perror("accept error realised...");  
						exit(6);  
						}              
					#ifdef DEBUG							//Telling socket name to the user 
					printf("New connection....\n");                    
					#endif								
					//sending protocol message to the server                        
							int sen= send(new_socket, message, sizeof(message), 0);
							if(sen ==-1)  
						{  
						perror("sending protocol failure...");
						exit(1);   
						}
						else
							{
							#ifdef DEBUG		                       
							printf("%s\n",message);
							#endif
							}
							bzero(buffer, sizeof(buffer));          		 					
							
						alarm(5);
						if(recv(new_socket, &buffer, sizeof(buffer), 0) > 0)          						
						{
						#ifdef DEBUG
						printf("%s\n", buffer);	//printing the current received message in the buffer.	
						#endif
						alarm(0);	      
						}
						else
							{
							perror("no OK message from client, possibly protocol mismatch...");
							printf("ERROR receiving from client, 5 sec delays\n");
							send(new_socket, ERROR, sizeof(ERROR), 0);
							exit(1);
							}	

					/* Initializing the library */
					initCalcLib();
					char *op; 
					char opr_val[256];
					op=randomType(); // Getting a random arithmetic operator. 
					double fv1,fv2,fresult;
					int iv1,iv2,iresult;
					iv1=randomInt();
					iv2=randomInt();
					fv1=randomFloat();
					fv2=randomFloat();
							
					if(op[0]=='f'){
						
						if(strcmp(op,"fadd")==0){
						fresult=fv1+fv2;
						} else if (strcmp(op, "fsub")==0){
						fresult=fv1-fv2;
						} else if (strcmp(op, "fmul")==0){
						fresult=fv1*fv2;
						} else if (strcmp(op, "fdiv")==0){
						fresult=fv1/fv2;
						}   
							#ifdef DEBUG	
							printf("%s %8.8g %8.8g = %8.8g\n",op,fv1,fv2,fresult);
							#endif
							
							double fr = fresult;     	
						fr = round(fr * 10000.0)/10000.0;
						
						bzero(opr_val, sizeof(opr_val));
						sprintf(opr_val, "%s %8.8g %8.8g\n",op,fv1,fv2);
						printf("%s %8.8g %8.8g\n",op,fv1,fv2);
								//sending operation and random float values to the client
								if(send(new_socket, opr_val, sizeof(opr_val), 0)<-1)
											{
											perror("failure sending operation and float values to client...");
											exit(1);
											}
											else
											{
											#ifdef DEBUG
											printf("sending operation and values successful\n");
											#endif
											}						

												char client_float_result[256];
												bzero(client_float_result, sizeof(client_float_result)); 
												alarm(5);			
												if(recv(new_socket, client_float_result, sizeof(client_float_result),0) >0)					 	
													{
																					
													printf("%s", client_float_result); 
													
													alarm(0);
													}
													else
														{
														perror("no message received from client..."); 
														send(new_socket, ERROR, sizeof(ERROR), 0);
														exit(1);
														}							 	
															//Comparing the results from the client side with the ones on the server
													
														char server_response[] = "OK\n" ;
														#ifdef DEBUG
														printf("rounded off server answer : %8.8g\n",fr);						 		
														#endif							 		
														//using an if function, compare the result from client and server
														
														char fr_result[20];
														//double fr;
														sprintf(fr_result,"%8.8g",fr);
														#ifdef DEBUG	
														printf("%s\n", fr_result);
														printf("%s\n",client_float_result);
														#endif
															if (strncmp(fr_result, client_float_result, 0)==0)  
																{
																send(new_socket, server_response, sizeof(server_response), 0);
																printf("%s\n",server_response);
																} 
															else
															{										
															printf("Strings are unequal, hence no server response to the client\n"); 
															exit(1);   									
															}
																																																					
									}  
								else 
									{		  		
								if(strcmp(op,"add")==0){
								iresult=iv1+iv2;
								} else if (strcmp(op, "sub")==0){
								iresult=iv1-iv2;
								} else if (strcmp(op, "mul")==0){
								iresult=iv1*iv2;
								} else if (strcmp(op, "div")==0){
								iresult=iv1/iv2;
								}
								#ifdef DEBUG
								printf("%s %d %d = %d \n",op,iv1,iv2,iresult);
								#endif
								//round of iresult to 5 digit number.
								int ir = iresult;
									ir = round(ir * 10000.0)/10000.0;
					
								bzero(opr_val, sizeof(opr_val));
								sprintf(opr_val, "%s %d %d\n",op,iv1,iv2);
								printf("%s %d %d\n",op,iv1,iv2);
									//sending operation and random float values to client
									if(send(new_socket, opr_val, sizeof(opr_val), 0)==-1)
													{
													perror("failure sending operation and interger values to client\n");
													exit(1);
													}
													char client_int_result[256];
													bzero(client_int_result, sizeof(client_int_result));
													alarm(5);
													if(recv(new_socket, client_int_result, sizeof(client_int_result),0) >0)				 	
															{
																									
															printf("%s", client_int_result); 
															
															alarm(0);
															}
															else
															{
															perror("no message received from client//client result\n"); 
															send(new_socket, ERROR, sizeof(ERROR), 0);
															exit(1);
															}	
															//Comparing the results
																					
																char server_response[] ="OK\n" ;
																#ifdef DEBUG								 	
																printf("rounded off server answer: %d\n",ir);
																#endif
																char ir_result[20];
																#ifdef DEBUG
																sprintf(ir_result,"%d",ir);	
																printf("%s\n", ir_result);
																printf("%s\n",client_int_result);	
																#endif
																	if (strncmp(ir_result, client_int_result, 0)==0)  
																			{
																		send(new_socket, server_response, sizeof(server_response), 0);
																		printf("%s\n",server_response);
																		} 
																	else
																	{
																	perror("Strings are unequal, hence no server response to the client\n"); 
																	exit(1);
																	}																   										  										    		    
								} //end of arithmetic operation
				}  
				printf("**************\n");
				close(new_socket);							
			//}//Closing while loop
		close(master_socket);
		} //end of main function
