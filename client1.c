#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
	#include <winsock2.h>
	#include <dirent.h>
	#define close(s) closesocket(s);
	#define SOCKADDR_IN sockaddr_in;
	#define SOCKET int;
	#define SOCKADDR sockaddr;
	#define IN_ADDR in_addr;
#else

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <sys/stat.h>
	#include <sys/sendfile.h>
	#include <fcntl.h>
	#include <dirent.h>
#endif

int main(){
	char host[255];
	char choice[500];
	char buf[500];
	printf("Client$ : open ");
	scanf("%s", host);

	//Create a socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	//specify an address and port for the socket
	struct sockaddr_in cli_addr;
	cli_addr.sin_addr.s_addr =inet_addr(host);
	cli_addr.sin_port = htons(1337);
	cli_addr.sin_family = AF_INET;

	int connection_status = connect(sock, (struct sockaddr*) &cli_addr, sizeof(cli_addr));
	//Check for connection

	if (connection_status == -1){
		printf("Connection was unsuccessful \n");
		exit(1); }

	//Receive data from server
	char server_message[500];
	char server_message1[500];
	recv(sock, server_message, sizeof(server_message),0);
	recv(sock, server_message1, sizeof(server_message1),0);

	//print response from server
	printf("Server# : %s\n", server_message);
	printf("Server# : %s\n", server_message1);

	//send message to server
	int i = 1;
	while(1){
	printf("Client$ : ");
	scanf("%s", choice);
	send(sock, choice, sizeof(choice),0);

	//comparing the user input with the commands.
	int help = strcmp ("help", choice);
	int cd = strcmp ("cd", choice);
	int get = strcmp ("get", choice);
	int put = strcmp ("put", choice);
	int quit = strcmp ("quit", choice);
	int ls = strcmp ("ls", choice);
	int del = strcmp ("delete", choice);
	int status = strcmp ("status", choice);
	int mkdir = strcmp ("mkdir", choice);

	//when user input is equal to help
	if (help == 0) {

        char message[500];
        char message1[500];
        char message2[500];
        char message3[500];
        char message4[500];
        char message5[500];
        char message6[500];
        char message7[500];
        char message8[500];
        char message9[500];

        // recv means the client will receive message form the server
        recv(sock, message, sizeof(message),0);
        recv(sock, message1, sizeof(message1),0);
        recv(sock, message2, sizeof(message2),0);
        recv(sock, message3, sizeof(message3),0);
        recv(sock, message4, sizeof(message4),0);
        recv(sock, message5, sizeof(message5),0);
        recv(sock, message6, sizeof(message6),0);
        recv(sock, message7, sizeof(message7),0);
        recv(sock, message8, sizeof(message8),0);
        recv(sock, message9, sizeof(message9),0);

        printf("Server# : %s\n", message);
        printf("Server# : %s\n", message1);
        printf("Server# : %s\n", message2);
        printf("Server# : %s\n", message3);
        printf("Server# : %s\n", message4);
        printf("Server# : %s\n", message5);
        printf("Server# : %s\n", message6);
        printf("Server# : %s\n", message7);
        printf("Server# : %s\n", message8);
        printf("Server# : %s\n", message9);

}
	//when user input is equal to cd

	else if (cd == 0){
        int status;
        char directory[500];
        char s[500];
        printf("Client$ : Enter the path to change the remote directory: ");
        scanf("%s", directory);
        //path input by the user will be assigned to the variable directory
        send (sock, directory, sizeof(directory), 0);
        recv(sock, &status, sizeof(int), 0);
            if (status == 1) {
                printf("Directory changed successfully \n");
                }
            else	    {
                printf("Directory failed to be changed \n"); }

}
    //when user input is equal to get
	else if (get == 0){
		char filename[500];
		FILE *fp;
		FILE *f;
		char buffer[500];
		char path[500] = "/home/supinfo/project/"; // this path is that of the client
		int ch;
		printf ("Put the name of the file to download: ");
		scanf ("%s", filename);
        //send the name of the file to the server
		write(sock, filename, sizeof(filename));
        //merging the path and the filename to obtain the overall path
		strcat (path, filename);
        //open the file, the parameter "w" will create the file if it does not exist
		fp = fopen(path, "w");
		read (sock, buffer, sizeof(buffer));
		fputs(buffer, fp);
		fclose(fp);
		// open the file, the parameter "r" ensures that the file created above is not empty (received data successfully from server)
		f = fopen(path, "r");
        if (f != NULL){
            printf ("File has been downloaded successfully \n");}
		else {
            printf("Unable to download file");}
		fclose(f);

}
    //when user input is equal to put
	else if (put == 0)
	{
		char filename[500];
		FILE *fp;
		int c;
		char buffer[500];
		char path[500] = "/home/supinfo/project/"; // this path is that of the client
		printf ("Put the name of the file to send to the server: ");
		scanf ("%s", filename);
		write(sock, filename, sizeof(filename));
		strcat (path, filename);
		//ensure that the filename enter by the user do exist on the server
		fp = fopen(path, "r");
		if (fp != NULL)
		{
			//fgets will read the content of the file line by line
			fgets(buffer, sizeof(buffer), fp)
;			write(sock, buffer, sizeof(buffer));
			fclose(fp);
		}
		else {
			printf("File does not exist \n"); }
		read(sock, &c, sizeof(int));
		if (c == 1)
			printf("File sent successfully \n");
		else
			printf("Failure in sending file \n");


	}

    //when user input is equal to ls (listing)
	else if (ls ==0)
	{
		char directory[500];
		DIR *dir;
		struct dirent *file;
		char fileName[500];
        // will read data from server
		read(sock, directory, sizeof(directory));
		// will open the directory e
		dir = opendir(directory);
		while ((file = readdir(dir)) != NULL)
		{
            // will copy the content of file->d_name into the variable filename
			strncpy(fileName, file->d_name, 500);
			printf("Server# : %s\n", fileName);
		}

		closedir(dir);
	}

	else if (del == 0)
	{
		char message[100];
		char identifier[500];
		char filename[500];
		char path[500];
		int status;
		printf("Do you want to delete a FILE or a DIRECTORY(write in capital) ?:");
		scanf ("%s", identifier);
		// send whether the user wants to delete a file or directory to the server
		write (sock, identifier, sizeof(identifier));
		// if the user wants to delete a file
		if (strcmp (identifier, "FILE") == 0) {
			printf ("Enter the name of the file: ");
			scanf ("%s", filename);
			write (sock, filename, sizeof(filename));
			read (sock, &status, sizeof(int));
			if (status == 0){
                printf ("Server#: File deleted successfully \n");}
			else {
				printf("File not found "); }
		}
        // if the user wants to delete a directory
		else if (strcmp (identifier, "DIRECTORY") == 0 ){
			printf ("Enter the name of the directory: ");
			scanf ("%s", filename);
			write (sock, filename, sizeof(filename));
			read (sock, &status, sizeof(int));

			if (status == 0){
                printf ("Server#: Directory deleted successfully \n");}
			else {
				printf("Directory not found "); } }

	}

     //when user input is equal to status
	else if (status == 0){
		int clients;
		read (sock, &clients, sizeof(int));
		printf("Server# : Number of client connected to me is  %d\n", clients);
	}

    //when user input is equal to mkdir
	else if (mkdir == 0){
		char fileName[500];
		int check;
		printf("Enter the name of the directory: ");
		scanf("%s", fileName);
		// the write function will send the name of the directory to the server
		write(sock, fileName, sizeof(fileName));
		read(sock, &check, sizeof(check));
		if (check == 0){
			printf("Server#: Directory created successfully \n");
		}
		else {
			printf("Server#: Error in the creation of the directory");
		}
	}

    //when user input is equal to quit
	else if (quit == 0)
	{
		char message[100];
		read(sock, message, sizeof(message));
		printf("Server#: %s\n", message);
		break;
	}
 }
	close(sock);
	return 0;
}
