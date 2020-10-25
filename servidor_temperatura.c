#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9090);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0)
		printf("Error en el Listen");
	
	int i;
	// Atenderemos solo 5 peticione
	for(;;){
		printf ("Escuchando\n");
		
		//1)ESPERANDO CONEXION
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexi?n\n");
		//sock_conn es el socket que usaremos para este cliente
		
		//Bucle de atencion al cliente 
		int terminar = 0;
		while (terminar ==0)
		{
			
			// Ahora recibimos su nombre, que dejamos en buff
			//2)RECOGE LA PETICION
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			//Escribimos el nombre en la consola
			
			printf ("Se ha conectado: %s\n",peticion);
			
			//3)DETERMINA QUE ME PIDEN
			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			float temp;
			
			if (codigo !=0)
			{
				p = strtok( NULL, "/");
				temp = atoi(p);
				printf ("Codigo: %d, Temp: %f\n", codigo, temp);
			}
			
			//4)PREPARA LA RESPUESTA
			
			if (codigo == 0)
			{
				terminar = 1;
			}
			else if (codigo == 1)
			{
				// Pasamos de celsius a fahrenheit
				float fahrenheit;
				
	
				// Convert the temperature from celsius to fahrenheit 
				fahrenheit = ((temp * 9)/5) + 32;
				// fahrenheit = ((9/5) * celsius) + 32;
				// fahrenheit = ((1.8 * celsius) + 32;
				sprintf(respuesta,"\n %.2f Celsius = %.2f Fahrenheit",temp,fahrenheit);
				
			}
			else if (codigo == 2)
			{
				// Pasamos de fahrenheit a celsius
				float celsius;
				
				
				// Convert the temperature from celsius to fahrenheit 
				celsius = (temp - 32) * 5 / 9;
				// fahrenheit = ((9/5) * celsius) + 32;
				// fahrenheit = ((1.8 * celsius) + 32;
				sprintf(respuesta,"\n %.2f Fahrenheit = %.2f Celsius",temp,celsius);
			}
			if (codigo !=0)
			{
				printf ("%s\n", respuesta);
					
				//5)ENVIA LA RESPUESTA
				// Y lo enviamos
				write (sock_conn,respuesta, strlen(respuesta));
			}
		}
		
		//6)CIERRA LA CONEXION
		// Se acabo el servicio para este cliente
		close(sock_conn); 
	}
}
