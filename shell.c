#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define compara " "
#define MSJ_ERROR "Error de sintaxis\n"
#define MAX 51
#define PROMPT "grupo7$> "

int CadIguales(char cadena1[MAX],char cadena2[MAX]);
void Comando(char cadena[MAX], int plano);
void redirecEntrada(char cadena[MAX]);
void redirecSalida(char cadena[MAX]);
void tuberias(char* argumento1[MAX], char* argumento2[MAX]);
void CrearProceso(char* argumento[MAX], int plano);



int main(int argc, char *argv[])
{ 
char cadena[MAX];  
char cadFin[]="FIN";
int fin=0,i,segplano=0, guardaStdout = dup(1), guardaStdin = dup(0);

while(fin==0) 
	{
        close(1); 
        dup(guardaStdout); 
        close(0); 
        dup(guardaStdin); 
	printf(PROMPT); // Imprimir prompt
	scanf("\n%[^\n]",cadena); // Entrada
	segplano=0;
	for(i=0;cadena[i] != '\0'; i++){
		if(cadena[i] == '&') {
					cadena[i] = '\0';
					segplano = 1;
					}
						}
	fin=CadIguales(cadena,cadFin); 
	Comando(cadena,segplano); 
	}
		
exit(0);
	
return(0);
} 

int CadIguales(char cadena1[],char cadena2[])
{
int i=0;
int igual=0;

while ((cadena1[i]==cadena2[i])&&(cadena1[i]!='\0')&&(cadena2[i]!='\0')) i++;	
if ((cadena1[i]==cadena2[i])&&(cadena1[i]=='\0')&&(cadena2[i]=='\0')) exit(0);				
return(igual);
}

void Comando(char cadena[MAX], int plano){
int i,j,k,flag_tuberia;		
char argumentoInd[MAX][MAX]; 				
char argumentoInd2[MAX][MAX];
char redirec[MAX];
char entrada[MAX];
char *argumento[MAX]; 
char *argumento2[MAX];
int ejecutar=0;
argumento[0] = NULL;					
argumento2[0] = NULL;
flag_tuberia = 0;

	 	k = 0;
	    i = 0;
		while(cadena[i] != '\0' && cadena[i] != '|' && cadena[i] != '>'){		
			for(j=0; cadena[i] != ' ' && cadena[i] != '\0' && cadena[i] != '|'  && cadena[i] != '>' && cadena[i] != '<';j++) { 
				argumentoInd[k][j] = cadena[i];
				i++;
			}

			if (cadena[i] == ' ') i++;
					
				argumento[k] = argumentoInd[k];
				k++;
				if (cadena[i] == '<') { 
					i++;
					if (cadena[i] != ' ') ejecutar=1;
					else { i++;
						for(j=0; cadena[i] != '\0' && cadena[i] != ' ' && cadena [i] != '|' && cadena [i] != '>'; j++){
							entrada[j] = cadena[i];
							i++;
						}
						entrada[j] = '\0';
						if (cadena[i] != '\0') i++;
							redirecEntrada(entrada);
						}
					}
					
			} 

		argumento[k] = NULL;

		if (cadena[i] == '|') {
			k=0;
			i++;
			if (cadena[i] != ' ') ejecutar=1;
			else {
			i++;
			flag_tuberia = 1;
			while(cadena[i] != '\0' && cadena[i] != '>'){	 
					for(j=0; cadena[i] != ' ' && cadena[i] != '\0' && cadena[i] != '>';j++) {
								argumentoInd2[k][j] = cadena[i];
								i++;}
					if (cadena[i] == ' ' ) i++;
			
					argumentoInd2[k][j] = '\0';
					argumento2[k] = argumentoInd2[k];
					k++;
				}
			argumento2[k] = NULL;
			}	
		}


		
		if (cadena[i] == '>') {
					i++;
					if (cadena[i] != ' ') ejecutar=1;
					else {
					i++;
					for(j=0; cadena[i] != '\0';j++) {
								redirec[j] = cadena[i];
								i++;}
					redirec[j] = '\0';
					redirecSalida(redirec);	
					}
		}
		if(ejecutar == 0) {
					if (flag_tuberia==0) CrearProceso(argumento,plano);
			
					else tuberias(argumento,argumento2);
				  }
		else printf( MSJ_ERROR );
		
}

void redirecEntrada(char cadena[MAX])

{
  char *cadenaPtr;
  int fd;  
  cadenaPtr = cadena; // Puntero a la cadena
  fd = open (cadenaPtr,O_RDONLY); // Asigno a la salida el fichero
  close (0); 
  dup(fd);	
  

} 

void redirecSalida(char cadena[MAX])

{ /* INICIO DE LA FUNCI�N redirecSalida */
  char *cadenaPtr;
  cadenaPtr = cadena; // Puntero a la cadena
  close (1); 	
  open (cadenaPtr,O_CREAT | O_WRONLY,0777); // Asigno a la salida el fichero

} 

void tuberias(char* argumento1[MAX], char* argumento2[MAX])

{ 
  int fd[2],estado;
	pid_t hijo; 
	hijo=fork();
	

	if (hijo==-1) printf("ERROR Creacion de proceso"); 
	else if (hijo==0) {
		   pipe(&fd[0]); 
   			if (fork()!=0) {
      				close(fd[0]);
      				close(1);
      				dup(fd[1]);
      				close(fd[1]);
      				execvp(argumento1[0],argumento1);
   				}
   			else {
      				close(fd[1]);
      				close(0);
      				dup(fd[0]);
      				close(fd[0]);
     				execvp(argumento2[0],argumento2);
      
  				 }
		   }
	else  hijo=wait(&estado);
} 


void CrearProceso(char* argumento[MAX],int plano)

{	
	int estado=0;
	pid_t hijo; 
	hijo=fork();

if (hijo==-1) printf("ERROR Creacion de proceso"); 
else if (hijo==0) {
			
			execvp(argumento[0],argumento);
			perror("JASHELL");
			exit(estado);
		   }
else  { 
		if (plano == 0) hijo=wait(&estado);
		}

} 

