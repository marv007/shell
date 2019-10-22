#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define prompt "grupo7$> "
#define mensaje_error "Se produjo un error de sintaxis\n"
#define MAX 50
#define compara " "


int Comparar(char texto1[MAX],char texto2[MAX]);
void Ejecutar(char texto[MAX], int plano);
void cambiarEntrada(char texto[MAX]);
void cambiarSalida(char texto[MAX]);
void tuberias(char* parametro1[MAX], char* parametro2[MAX]);
void NuevoProceso(char* parametro[MAX], int plano);


/*La función Main() se ejecutará hasta que el usuario escriba la palabra salir*/
int main(int argc, char *argv[]){ 
char texto[MAX];  
char textFinal[]="salir";
int fin=0,i,segundoPlano=0, guardaStdout = dup(1), guardaStdin = dup(0);

while(fin==0){
        close(1); 
        dup(guardaStdout); 
        close(0); 
        dup(guardaStdin); 
	printf(prompt); // Imprimir prompt
	scanf("\n%[^\n]",texto); // Entrada
	segundoPlano=0;
	for(i=0;texto[i] != '\0'; i++){
		if(texto[i] == '&') {
					texto[i] = '\0';
					segundoPlano = 1;
					}
						}
	fin=Comparar(texto,textFinal); 
	Ejecutar(texto,segundoPlano); 
	}
		
exit(0);
	
return(0);
} 

void NuevoProceso(char* parametro[MAX],int plano){	
	int estado=0;
	pid_t hijo; 
	hijo=fork();

	if (hijo==-1) printf("ERROR Creación de proceso"); 
	else if (hijo==0) {			
			execvp(parametro[0],parametro);
			perror("ErrorShell");
			exit(estado);
		}else if (plano == 0) hijo=wait(&estado);
		

} 

int Comparar(char texto1[],char texto2[]){
int i=0;
int igual=0;

while ((texto1[i]==texto2[i])&&(texto1[i]!='\0')&&(texto2[i]!='\0')) i++;	
if ((texto1[i]==texto2[i])&&(texto1[i]=='\0')&&(texto2[i]=='\0')) exit(0);				
return(igual);
}

void Ejecutar(char texto[MAX], int plano){
int i,j,k,flag_tuberia;		
char parametroInd[MAX][MAX]; 				
char parametroInd2[MAX][MAX];
char redirec[MAX];
char entrada[MAX];
char *parametro[MAX]; 
char *parametro2[MAX];
int ejecucion=0;
parametro[0] = NULL;					
parametro2[0] = NULL;
flag_tuberia = 0;

	 	k = 0;
	    i = 0;
		while(texto[i] != '\0' && texto[i] != '|' && texto[i] != '>'){		
			for(j=0; texto[i] != ' ' && texto[i] != '\0' && texto[i] != '|'  && texto[i] != '>' && texto[i] != '<';j++) { 
				parametroInd[k][j] = texto[i];
				i++;
			}

			if (texto[i] == ' ') i++;
					
				parametro[k] = parametroInd[k];
				k++;
				if (texto[i] == '<') { 
					i++;
					if (texto[i] != ' ') ejecucion=1;
					else { i++;
						for(j=0; texto[i] != '\0' && texto[i] != ' ' && texto [i] != '|' && texto [i] != '>'; j++){
							entrada[j] = texto[i];
							i++;
						}
						entrada[j] = '\0';
						if (texto[i] != '\0') i++;
							cambiarEntrada(entrada);
						}
					}
					
			} 

		parametro[k] = NULL;

		if (texto[i] == '|') {
			k=0;
			i++;
			if (texto[i] != ' ') ejecucion=1;
			else {
			i++;
			flag_tuberia = 1;
			while(texto[i] != '\0' && texto[i] != '>'){	 
					for(j=0; texto[i] != ' ' && texto[i] != '\0' && texto[i] != '>';j++) {
								parametroInd2[k][j] = texto[i];
								i++;}
					if (texto[i] == ' ' ) i++;
			
					parametroInd2[k][j] = '\0';
					parametro2[k] = parametroInd2[k];
					k++;
				}
			parametro2[k] = NULL;
			}	
		}


		
		if (texto[i] == '>') {
					i++;
					if (texto[i] != ' ') ejecucion=1;
					else {
					i++;
					for(j=0; texto[i] != '\0';j++) {
								redirec[j] = texto[i];
								i++;}
					redirec[j] = '\0';
					cambiarSalida(redirec);	
					}
		}
		if(ejecucion == 0) {
					if (flag_tuberia==0) NuevoProceso(parametro,plano);
			
					else tuberias(parametro,parametro2);
				  }
		else printf( mensaje_error );
		
}
void cambiarSalida(char texto[MAX]){ 
  char *textoPtr;
  textoPtr = texto; // Puntero al texto
  close (1); 	
  open (textoPtr,O_CREAT | O_WRONLY,0777); // Asigno a la salida el fichero

}

void cambiarEntrada(char texto[MAX]){
  char *textoPtr;
  int fd;  
  textoPtr = texto; // Puntero a la texto
  fd = open (textoPtr,O_RDONLY); // Asigno a la salida el fichero
  close (0); 
  dup(fd);	
} 



void tuberias(char* parametro1[MAX], char* parametro2[MAX]){ 
  int fd[2],estado;
	pid_t hijo; 
	hijo=fork();

	if (hijo==-1) printf("ERROR Creacion de proceso"); 
	else if (hijo==0){
		pipe(&fd[0]); 
   		if (fork()!=0){
      		close(fd[0]);
      		close(1);
      		dup(fd[1]);
      		close(fd[1]);
      		execvp(parametro1[0],parametro1);
   		}else {
      		close(fd[1]);
      		close(0);
      		dup(fd[0]);
      		close(fd[0]);
     		execvp(parametro2[0],parametro2);
      
  		}
	}else  hijo=wait(&estado);
} 
