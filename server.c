#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
	#include <winsock2.h>
	#define close(s) closesocket(s);
	#define SOCKET int;
	#define SOCKADDR_IN sockaddr_in;
	#define SOCKADDR sockaddr;
	#define IN_ADDR in_addr;
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <sys/stat.h>
	#include <sys/sendfile.h>
	#include <fcntl.h>
	#include <ctype.h>
	#include <dirent.h>


#endif
#include <string.h>

int main(){

	char server_message[500] = "You have connected successfully \n";
	char server_message1[500] = "You can use help to list the command \n";
	char buf[500];
	char command[500];
	int numberOfClients = 0;

	//create a socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//define the server address
	struct sockaddr_in server_address, client;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(1337);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind the socket into our specified IP and port
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	//listen to the socket
	listen(server_socket, 5);

	//accept the connection
	int click_socket;
	click_socket = accept(server_socket, NULL, NULL);
	if (click_socket >= 0){
	  numberOfClients++; }

	//send the message
	send(click_socket, server_message, sizeof(server_message), 0);
	send(click_socket, server_message1, sizeof(server_message1), 0);

	//read message from client
	while(1){
    // receive command from the client.
	recv(click_socket, buf, sizeof(buf), 0);
	int help = strcmp ("help", buf);
	int cd = strcmp ("cd", buf);
	int get = strcmp ("get", buf);
	int put = strcmp ("put", buf);
	int ls = strcmp ("ls", buf);
	int quit = strcmp ("quit", buf);
	int delete = strcmp ("delete", buf);
	int status = strcmp ("status", buf);
	int makedir = strcmp ("mkdir", buf);

	//when user input is equal to help
	if (help == 0){
		char message[500] = "list of commands: \n" ;
		char message1[500] = "help: list all available commands \n" ;
		char message2[500] = "cd: change the current directory \n" ;
		char message3[500] = "get: download a file \n" ;
		char message4[500] = "ls: list files and directories \n" ;
		char message5[500] = "delete: delete a file or directory \n" ;
		char message6[500] = "mkdir: create a directory \n" ;
		char message7[500] = "put: send a file \n" ;
		char message8[500] = "status: list numbers of connected users \n" ;
		char message9[500] = "quit: logout the client \n" ;

		send(click_socket, message, sizeof(message), 0) ;
		send(click_socket, message1, sizeof(message1), 0) ;
		send(click_socket, message2, sizeof(message2), 0) ;
		send(click_socket, message3, sizeof(message3), 0) ;
		send(click_socket, message4, sizeof(message4), 0) ;
		send(click_socket, message5, sizeof(message5), 0) ;
		send(click_socket, message6, sizeof(message6), 0) ;
		send(click_socket, message7, sizeof(message7), 0) ;
		send(click_socket, message8, sizeof(message8), 0) ;
		send(click_socket, message9, sizeof(message9), 0) ;
}
	//when user input is equal to cd
	else if (cd == 0){
		int c;
		char directory[500];
		recv(click_socket, directory, sizeof(directory), 0);
        // function to change the directory
		if(chdir(directory) == 0) {
			c = 1; }
		else	{
			c = 0; }
		send(click_socket, &c, sizeof(int), 0);
}
     //when user input is equal to get
	else if (get == 0)
	{
		char filename[500];
		char path[500] = "/home/supinfo/project/storageServer/";
		char buffer[500];
		FILE *f;
		int c = 0;
		read (click_socket, filename, sizeof(filename));
		strcat(path, filename);
		f = fopen(path, "r");
		// will open the file and will put the content of the file into the variable buffer and send it to the client
		if (f != NULL)
		{
			fgets(buffer, sizeof(buffer), f);
			write(click_socket, buffer, sizeof(buffer));
		}
		if (f ==NULL){
		printf ("File does not exist");}
		fclose (f);
	}

	else if (put == 0)
	{
		char filename[500];
		char path[500] = "/home/supinfo/project/storageServer/";
		char buffer[500];
		FILE *f;
		FILE *fp;
		int c;
		read (click_socket, filename, sizeof(filename));
		strcat(path, filename);
		//will create a file and put the content of the file in the server in that found on the client by using fputs
		f = fopen(path, "w");
		read (click_socket, buffer, sizeof(buffer));
		fputs(buffer, f);
		fclose(f);
		fp = fopen(path, "r");
		if (fp ==NULL)
			c = 0;
		else
			c = 1;
		fclose(fp);
		write (click_socket, &c, sizeof(int));
	}

    // when the command put by the user is equal to delete
	else if (delete == 0)
	{
		char identifier[500];
		char filename[500];
		char fileName[500];
		char directory[500];
		int status;
		struct dirent *file;
		char *ptr;
		DIR *dir;
		ptr = getcwd(directory, sizeof(directory));
		strcat (ptr, "/");
		read (click_socket, identifier, sizeof(identifier));
		if (strcmp (identifier, "FILE") == 0){
			read(click_socket, filename, sizeof(filename));
			strcat (ptr, filename);
			status = remove(ptr); // remove() will delete a file.
			write (click_socket, &status, sizeof(int));
		}

		if (strcmp (identifier, "DIRECTORY") == 0) {
			read(click_socket, filename, sizeof(filename));
			strcat (ptr, filename);
			status = rmdir(ptr); // rmdir() will delete a directory.
			write (click_socket, &status, sizeof(int));
		}
	}

	else if (ls == 0)
	{
		char directory[500];
		char *ptr;
		ptr = getcwd(directory, sizeof(directory)); // get the current directory of the server and assign it to a variable.
		write (click_socket, ptr, 500);

	}

	else if (status == 0){
		write (click_socket, &numberOfClients, sizeof(int));// send the number
	}

	else if (makedir == 0){
		char fileName[500];
		char *path;
		char directory[500];
		int check;
		read(click_socket, fileName, sizeof(fileName));
		path = getcwd(directory, sizeof(directory));
		strcat (path, "/");
		strcat (path, fileName);
		check = mkdir(path); // mkdir() will make the directory choosen by the user
		write(click_socket, &check, sizeof(int));
	}

    // when the user the command entered by the user is equal to quit.
	else if (quit == 0)
	{
		char message[100] = "Bye ;)";
		write (click_socket, message, sizeof(message));
		break;
	}
}
	close(click_socket);
	close(server_socket);
	return 0;
}
