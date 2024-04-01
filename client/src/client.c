#include "client.h"
#include "utils.h"

//HOLA GABI ESO ESTA EN LA NUEVA BRANCH

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'	

	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "VALOR");

	// Loggeamos el valor de config	

	log_info(logger, "Lei la IP %s y puerto %s\n", ip, puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	if ((nuevo_logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO)) == NULL)
	{
		printf("No pude crear el logger\n");
		exit(1);
	}
	

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	if((nuevo_config = config_create("./cliente.config")) == NULL){
		printf("No pude leer la config\n");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

    while(strcmp(leido, ""))
    {
        //Primero, logeame lo leido
        log_info(logger, leido);

        //ahora que ya usamos el string, liberamelo con free
        free(leido);

        //volvemos a poner el readline para poder seguir loggeando.
        leido = readline(">");
    }

    
	
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
    //Ahora toca lo divertido!
    t_paquete* paquete = crear_paquete();

    //De nuevo vas a tener que usar readline hasta que te llegue un nulo igual que antes, solo que ahora en lugar de logear, vas a agregar esa cadena al paquete!
    //Recomiendo revisar bien el enunciado del TP ya que ahi está explicado con detalles
    char* leido = readline("> ");

    while(strcmp(leido, ""))
    {
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);

        free(leido);

        leido = readline("");
    }

    //Una vez hayamos terminado de ingresar valores, toca enviarlo
    enviar_paquete(paquete, conexion);

    //Y no nos olvidemos de borrar el paquete con eliminar_paquete
    eliminar_paquete(paquete);
    free(leido);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}

	if (config != NULL)
	{
		config_destroy(config);
	}
	
	liberar_conexion(conexion);
}
