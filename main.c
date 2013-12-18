#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define BNEWSUDOKU   1001
#define BRESET	     1002
#define BLOAD        1003
#define BSAVE        1004
#define BCHECK       1005
#define BLOGOUT      1006
#define BEXIT        1007

#define USERNAME     2001
#define BNEWUSER     2002
#define BLOGIN       2003
#define LOGINTITLE   2004

#define INFOBOX      3000

#define PLAYER       4001
#define TIMESPLAYED  4002
#define TIMESSUCCESS 4003
#define LASTGAME     4004

const int IDTXT[81]= {11,12,13,14,15,16,17,18,19,
                      21,22,23,24,25,26,27,28,29,
                      31,32,33,34,35,36,37,38,39,
                      41,42,43,44,45,46,47,48,49,
                      51,52,53,54,55,56,57,58,59,
                      61,62,63,64,65,66,67,68,69,
                      71,72,73,74,75,76,77,78,79,
                      81,82,83,84,85,86,87,88,89,
                      91,92,93,94,95,96,97,98,99};

typedef enum {false=0,true=1} bool;

typedef struct{
	int id; 
    int inicial[9][9];
    int guardado[9][9];
}board;

typedef struct{
	board partida;
	char nombre[50];
	char ultima_jugada[15];
	int veces_jugadas;
	int veces_ganadas;
}reg;

reg player = {0};
int tablero[9][9] = {0};
bool  blog = false;
FILE *user_db;

HWND hwnd;

HWND casilla11, casilla12, casilla13, casilla14, casilla15, casilla16, casilla17, casilla18, casilla19;
HWND casilla21, casilla22, casilla23, casilla24, casilla25, casilla26, casilla27, casilla28, casilla29;
HWND casilla31, casilla32, casilla33, casilla34, casilla35, casilla36, casilla37, casilla38, casilla39;
HWND casilla41, casilla42, casilla43, casilla44, casilla45, casilla46, casilla47, casilla48, casilla49;
HWND casilla51, casilla52, casilla53, casilla54, casilla55, casilla56, casilla57, casilla58, casilla59;
HWND casilla61, casilla62, casilla63, casilla64, casilla65, casilla66, casilla67, casilla68, casilla69;
HWND casilla71, casilla72, casilla73, casilla74, casilla75, casilla76, casilla77, casilla78, casilla79;
HWND casilla81, casilla82, casilla83, casilla84, casilla85, casilla86, casilla87, casilla88, casilla89;
HWND casilla91, casilla92, casilla93, casilla94, casilla95, casilla96, casilla97, casilla98, casilla99;

HWND login_title,username,Bnewuser,Blogin;//para el logueo de los xses

HWND Bnewsudoku, Breset, BLoad, BSave, Blogout, Bexit; 
HWND jugador1,veces_jugadas1,veces_ganadas1,last_jugada1,infobox1;
HWND jugador,veces_jugadas,veces_ganadas,last_jugada,infobox;

bool equal_boards(int matriz1[9][9],int matriz2[9][9]);
void enable_disable(BOOL a);
void actualizar_tablero();
bool completo();
void date_updater();
void disable_clues(HWND hwnd);
void user_database_access();
void new_user(HWND hwnd,char username[50]);
void login(HWND hwnd,char username[50]);
void logout(HWND hwnd);
void cargar(board *reg, FILE *sudo);
void generador(int matriz[9][9]);
void variacion_0(int matriz[9][9]);
void variacion_1(int matriz[9][9]);
void variacion_fil(int matriz[9][9]);
void variacion_col(int matriz[9][9]);
void imprimir(int matriz[9][9]);
void new_sudoku(HWND hwnd);
void db_updater();
void user_info_show(HWND hwnd);
void user_info_hide(HWND hwnd);
/*muestra en pantalla el tablero guardado*/
void load(HWND hwnd){
    char a[3];
    int i,j,k;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			tablero[i][j]=player.partida.guardado[i][j];
			k = 10*(i+1) + j + 1;
			if(tablero[i][j]!=0){
                SetDlgItemText(hwnd,k,itoa(tablero[i][j],a,10));
            }else{
                SetDlgItemText(hwnd,k," ");
            }
		}
	}
	enable_disable(TRUE);
	disable_clues(hwnd);
}
	
/*muestra en pantalla el sudoku inicial de la partida actual*/
void reset(HWND hwnd){
	int i,j,k;
	char a[3];
    for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			tablero[i][j]=player.partida.inicial[i][j];
			k = 10*(i+1) + j + 1;
			if(tablero[i][j]!=0){
                SetDlgItemText(hwnd,k,itoa(tablero[i][j],a,10));
            }else{
                SetDlgItemText(hwnd,k," ");
            }
		}
	}
}
	
/*guarda en el registro de la partida actual los cambios realizados por el usario en el sudoku activo*/
void save(){
	int i,j;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			player.partida.guardado[i][j]=tablero[i][j];
		}
	}
	db_updater();
}

/*se carga la cuenta correspondiente, en caso de errores se dan las indicaciones correspondientes*/
void login(HWND hwnd,char user_name[50]){
	bool bandera = false; 
	
	user_db = fopen("users.raf","rb");
	if(strcmp(user_name,"") != 0){
	
		if(user_name[0]!= ' '){
			while( !feof(user_db) && bandera == false){        
				fread(&player,sizeof(reg),1,user_db);
				if( strcmp(user_name,player.nombre) == 0 )
					bandera = true;
			}
		}
		if(bandera == true){
            SetDlgItemText(hwnd,USERNAME,"");
            player.veces_jugadas++;
			enable_disable(TRUE);
			user_info_show(hwnd);
            load(hwnd);
            SetDlgItemText(hwnd,INFOBOX,"\n\nSudoku cargado");
        }else{
			MessageBox(hwnd,"El Nombre solicitado no existe o no es válido.\nAsegúrese de haber ingresado el nombre correctamente o cree una nueva cuenta.\nSe distingen mayúsculas y minúsculas.\nEl nombre no debe comenzar con espacios.","Login Error",MB_OK);
		    user_info_hide(hwnd);
        }
    }else{
		MessageBox(hwnd,"Debe ingresar un nombre de usuario","Error",MB_OK);
	}
}

/*se verifica que dos tableros sean iguales*/
bool equal_boards(int matriz1[9][9],int matriz2[9][9]){
	int i,j;
    for (i=0;i<9;i++){
		for(j=0;j<9;j++){
			if(matriz1[i][j]!=matriz2[i][j])
				return false;
		}
	}
	return true;
}

/*cierra la sesion del jugador actual, dándole previamente la opción de guardar o descartar los cambios hechos en caso de que los haya*/
void logout(HWND hwnd){
	int saveoptions;
    if (equal_boards(tablero, player.partida.guardado)==true){
		enable_disable(FALSE);
		date_updater();
		db_updater();
		user_info_hide(hwnd);
	}else{
		saveoptions = MessageBox(hwnd, "Esta abandonando la partida, ¿desea guardarla?", "Logout", MB_YESNOCANCEL );
		
		switch(saveoptions){
			case IDYES: save();enable_disable(FALSE);date_updater();db_updater();user_info_hide(hwnd);break;
			case IDNO: enable_disable(FALSE);date_updater();db_updater();user_info_hide(hwnd);/*cerado de tablero de winapi*/;break;
		}
	}
	 blog = true;
}

/*Pregunta al usuario si desea guardar los cambios de su partida actual en caso de que hayan cambios, luego cierra el programa*/
void bexit(HWND hwnd){
	int saveoptions;
	if( blog == false){
		if (equal_boards(tablero, player.partida.guardado)==true){
			date_updater();
			db_updater();
			PostQuitMessage(0);
		}else{
			saveoptions = MessageBox(hwnd, "Esta abandonando el juego, ¿desea guardar la partida?", "Salir", MB_YESNOCANCEL );
				switch(saveoptions){
				case IDYES: save();date_updater();db_updater();PostQuitMessage(0);break;
				case IDNO: date_updater();db_updater();PostQuitMessage(0);break;
			}
		}
	}else{
		PostQuitMessage(0);
	}
}


/*genera un archivo binario con una lista de registros de sudokus completos a partir de un archivo secuencial,
de esta base de datos binaria se recuperarán más tarde los sudokus iniciales a utilizarse para el juego*/
void dbbinaria_access(HWND hwnd){
	int i,j,x;                  //variables para navegar la matriz y asignar la id del sudoku
	FILE *base_bin, *base_sec;  //punteros a las bases de datos de sudokus completos, respectivamente a la base de datos binaria y secuencial
    board sudoku_aux = {0};     //representa a cada sudoku, con su identificador y su respectiva solucion
    
	if((base_sec = fopen("dbsecuencial.saf","r"))==NULL){
		MessageBox(hwnd,"No se puede acceder a la base de datos del juego, no se puede continuar con la ejecución del programa","FATAL ERROR",MB_OK);
		PostQuitMessage(0);
	}else{
			base_bin = fopen("dbbinaria.raf","wb");
			
			for(x=0;x<26;x++){
				for (i=0;i<9;i++){
					for (j=0;j<9;j++){
						fscanf(base_sec,"%d", &sudoku_aux.inicial[i][j]);
					}
				}
				fwrite(&sudoku_aux,sizeof(board),1,base_bin);
            }
    }
	fclose(base_sec);
	fclose(base_bin);
}

/*verifica el acceso al archivo que contiene la información de los usuarios registrados, en caso de que no exista, lo crea*/
void users_access(){
	if((user_db = fopen("users.raf","rb+"))==NULL){
		user_db = fopen("users.raf","ab");
		fclose(user_db);
	}else{
		fclose(user_db);	
	}
}

/*añade al archivo de registro de usuarios una nueva entrada, con el nombre de usuario proporcionado por el usuario,
en caso de que exista una cuenta con el nombre especificado da las indicaciones correspondientes*/
void new_user(HWND hwnd,char user_name[50]){
	int i,j;
	bool bandera;	
	bandera = false;
	user_db = fopen("users.raf","ab+");
	if(strcmp(user_name,"") != 0){
	
		if(user_name[0]!= ' '){
			while( !feof(user_db) && bandera == false){
				fread(&player,sizeof(reg),1,user_db);
				if( strcmp(user_name,player.nombre) == 0 )
					bandera = true;
			}
			fclose(user_db);
		}else bandera = true;
		
		if(bandera == true){
			MessageBox(hwnd,"El nombre de usuario introducido no es válido o no está disponible.\n\nIntroduzca un nuevo nombre de usuario para crear una cuenta.\nSe distingen mayúsculas y minúsculas.\nEl nombre no debe comenzar con espacios.","Error de Registro",MB_OK);
		    user_info_hide(hwnd);
        }else{
			
			strcpy(player.nombre, user_name);
			sprintf(player.ultima_jugada,"Primera Vez");
			player.veces_jugadas       =  1;
			player.veces_ganadas       =  0;
			player.partida.id          =  0;
			new_sudoku(hwnd);
			SetDlgItemText(hwnd,USERNAME,"");
			enable_disable(TRUE);
			user_info_show(hwnd);
            load(hwnd);
            SetDlgItemText(hwnd,INFOBOX,"\n\nNuevo Sudoku");
        }
		fclose(user_db);
	}else{
		MessageBox(hwnd,"Debe ingresar un nombre de usuario","Error",MB_OK);
	}
}
	
/*genera un sudoku inicial (no completo) aplicando diversas variaciones a los sudokus de la base de datos,
actualiza el registro del juego activo asignando el sudoku generado al registro de sudoku inicial correspondiente*/
void new_sudoku(HWND hwnd){
	int i,j;
	board reg_aux;
	FILE *base; //apuntador a la base de datos de sudokus completos
	
    if((base = fopen("dbbinaria.raf","rb"))==NULL){
		dbbinaria_access(hwnd);
		new_sudoku(hwnd);
	}else{
        /*se carga un sudoku al azar*/
    	reg_aux.id = rand()%25;
		fseek(base,(reg_aux.id)*sizeof(board),SEEK_SET);
		fread(&reg_aux, sizeof(board),1,base);
		
		generador(reg_aux.inicial);
		for(i=0;i<9;i++){
			for(j=0;j<9;j++){
				player.partida.inicial[i][j]=reg_aux.inicial[i][j];
				player.partida.guardado[i][j]=reg_aux.inicial[i][j];
				tablero[i][j]=reg_aux.inicial[i][j];
			}
		}
		fclose(base);
		db_updater();
		
	}
}

/*selecciona y aplica variaciones a los sudokus completos*/
void generador(int matriz[9][9]){
	int x,y,i,j,z,pista;		
	variacion_fil(matriz);
	variacion_col(matriz);
	x = rand()%4;
	switch(x){
		case 1:variacion_0(matriz);break;
		case 2:variacion_1(matriz);break;
		case 3:variacion_0(matriz);variacion_1(matriz);break;
	}
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			pista=rand()%4 + 4;
			z=0;
			while(z!=pista){
				x=rand()%3 + i*3;
				y=rand()%3 + j*3;
				if(matriz[x][y]!=0){
					matriz[x][y]=0;
					z++;
				}
			}
		}
	}
}
	
/*INICIO DE FUNCIONES DE VARIACION DE SUDOKU*/
		
//traspuesta de la diagonal principal
void variacion_1(int matriz[9][9]){
	int i,j,aux;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
		aux=matriz[i][j];
		matriz[i][j]=matriz[j][i];
		matriz[j][i]=aux;
		}
	}
}
	
//traspuesta de la diagonal secundaria
void variacion_0(int matriz[9][9]){
	int i,j,aux;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			aux=matriz[i][j];
			matriz[i][j]=matriz[8-j][8-i];
			matriz[8-j][8-i]=aux;
		}
	}
}
	
//se toman las cuadrillas formando filas de cuadrillas, éstas filas se intercambian aleatoriamente
void variacion_fil(int matriz[9][9]){
	int z,q,i,j,w,aux;
	for(w=0;w<2;w++){
		z=rand()%3;
		q=rand()%3;
		if(z!=q){
			for(i=0;i<3;i++){
				for(j=0;j<9;j++){
					aux=matriz[i+3*z][j];
					matriz[i+3*z][j]=matriz[i+3*q][j];
					matriz[i+3*q][j]=aux;
				}
			}
		}
	}
}
	
//se toman las cuadrillas formando columnas de cuadrillas, éstas columnas se intercambian aleatoriamente
void variacion_col(int matriz[9][9]){
	int z,q,i,j,k,aux;
	for(k=0;k<2;k++){
		z=rand()%3;
		q=rand()%3;
		if(z!=q){
			for(j=0;j<3;j++){
				for(i=0;i<9;i++){
					aux=matriz[i][j+3*z];
					matriz[i][j+3*z]=matriz[i][j+3*q];
					matriz[i][j+3*q]=aux;
				}
			}
		}
	}
}
		
/*FIN DE FUNCIONES DE VARIACION DE SUDOKU*/
	
	
/*verifica si el numero se repetite en la fila, columna, y cuadrilla correspondiente
asignando valores de la siguiente manera:      

0 = no se repite
1 = se repite en columna
2 = se repite en fila
4 = se repite en cuadrilla

retorna la suma de los valores
*/
int repetido(int x, int y, int num){
	int resul,i,j;
	
	resul=0;
	for(i=0;i<9;i++){
		if(i!=x && (resul==0 || resul==2)){
			if(tablero[i][y]==num)	resul=resul+1;
		}
		if(i!=y && (resul==0 || resul==1)){
			if(tablero[x][i]==num)	resul=resul+2;
		}
	}
	for(i=0;i<=2;i++){
		for(j=0;j<=2;j++){
			if((tablero[(x/3)*3+i][(y/3)*3+j]==num) && ((x/3)*3+i)!=x && ((y/3)*3+j)!=y){	
				resul=resul+4;
				return resul;
			}
		}
	}
	return resul;
}

/*verifica si el numero intrudicido por el usuario es válido, los números considerados válidos son digitos enteros de 1 a 9*/
void valido(HWND hwnd,int x,int y,int num,HWND casilla){
    int k,len;
    char a[5];
    k=10*(x+1) + (y+1);
    BOOL b;
    len = GetWindowTextLength(casilla) + 1;
    GetWindowText(casilla, a, len);
    if(strcmp(a," ")!=0 && strcmp(a,"")!=0){
		num = GetDlgItemInt(hwnd,k,&b,FALSE);
		if(0<num && num<10){
			
			actualizar_tablero(hwnd,x,y,k);
		  
			if(completo()!=true){
				switch (repetido(x,y,num)){
					case 0: SetDlgItemText(hwnd,INFOBOX,"\n\nEl número ingresado\nno se repite");break;
					case 1: SetDlgItemText(hwnd,INFOBOX,"\nEl número ingresado se\nrepite en la columna correspondiente");break;
					case 2: SetDlgItemText(hwnd,INFOBOX,"\nEl número ingresado se\nrepite en la fila correspondiente");break;
					case 4: SetDlgItemText(hwnd,INFOBOX,"\nEl número ingresado se\nrepite en la cuadrilla correspondiente");break;
					case 3: SetDlgItemText(hwnd,INFOBOX,"\nEl número ingresado se\nrepite en la fila y columna correspondientes");break;
					case 5: SetDlgItemText(hwnd,INFOBOX,"\nEl número ingresado se\nrepite en la columna y cuadrilla correspondientes");break;
					case 6: SetDlgItemText(hwnd,INFOBOX,"\nEl número ingresado se\nrepite en la fila y cuadrilla correspondientes");break;
					case 7: SetDlgItemText(hwnd,INFOBOX,"\nEl número ingresado se\nrepite en la fila, columna y cuadrilla correspondientes");break;
				}
					
		   }else check(hwnd);
		   
		}else{
		    MessageBox(hwnd,"Debe ingresar un número entre 1 y 9","Numero no válido",MB_OK);
			SetDlgItemText(hwnd,k," ");
		    SetDlgItemText(hwnd,INFOBOX,"");
		}
	}else{
		actualizar_tablero(hwnd,x,y,k);
	}
}

/*verifica si esta completo el sudoku*/
bool completo(){
	int i,j;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			if(tablero[i][j]==0){
                return false;
			}
		}
	}
	return true;
}

/*verifica si la matriz se completo correctamente*/
int check(HWND hwnd){
    bool incorrecto = false;
	int i,j;
	    for(i=0;i<9;i++){
			for(j=0;j<9;j++){
				if(repetido(i,j,tablero[i][j])!= 0){
					SetDlgItemText(hwnd,INFOBOX, "\nSudoku Incorrecto\nEl sudoku no esta correctamente completo");
					incorrecto = true;
                    i=j=9;
				}
			}
		}
		if(incorrecto == false){
        player.veces_ganadas++;
		MessageBox(hwnd, "\n\n\nFelicidades!! Ha completado el sudoku\n\n\n", "Victoria!", MB_OK );
		new_sudoku(hwnd);
		load(hwnd);
        }
}

/*actualiza la base de datos de registro de usuarios, salvando los datos de la partida actual además de las estadisticas del jugador*/
void db_updater(){
	bool bandera = false;
	reg user_reg_aux;
	
    user_db = fopen("users.raf","rb+");
	
	while( !feof(user_db) && bandera == false){
		fread(&user_reg_aux,sizeof(reg),1,user_db);
		if( strcmp(player.nombre,user_reg_aux.nombre) == 0 )
			bandera = true;
	}
	
	if (bandera == true){
        fseek(user_db,-1*sizeof(reg),SEEK_CUR);
	    fwrite(&player,sizeof(reg),1,user_db);
    }else{
        fwrite(&player,sizeof(reg),1,user_db);
    }
    fclose(user_db);
}

/*actualiza la fecha de la ultima partida del jugador y la guarda en el registro de juego activo*/
void date_updater(){
	time_t tiempo;
	char cad[15];
	struct tm *tmPtr;

	tiempo = time(NULL);
	tmPtr = localtime(&tiempo);
	strftime( cad, 80, "%d/%B/%Y", tmPtr );

	strcpy(player.ultima_jugada,cad);
}

/*muestra en pantalla la informacion detallada sobre el usuario*/
void user_info_show(HWND hwnd){
   SetDlgItemText(hwnd,PLAYER       ,player.nombre);
   SetDlgItemInt (hwnd,TIMESPLAYED  ,player.veces_jugadas,FALSE);
   SetDlgItemInt (hwnd,TIMESSUCCESS ,player.veces_ganadas,FALSE);
   SetDlgItemText(hwnd,LASTGAME     ,player.ultima_jugada);
}

/*deja de mostrar en pantalla la informacion detallada sobre el usuario*/
void user_info_hide(HWND hwnd){
   int i,j,k;
   SetDlgItemText(hwnd,PLAYER       ,"");
   SetDlgItemText(hwnd,TIMESPLAYED  ,"");
   SetDlgItemText(hwnd,TIMESSUCCESS ,"");
   SetDlgItemText(hwnd,LASTGAME     ,"");
   SetDlgItemText(hwnd,INFOBOX      ,"\n\nINFOBOX");
   for(i=0;i<9;i++){
       for(j=0;j<9;j++){
           k = 10*(i+1) + (j+1);
           SetDlgItemText(hwnd,k," ");
           player.partida.guardado[i][j] = 0;
           //tablero[i][j]=0;
       }
   }
}

/*desabilita la edicion de las casillas que son pistas iniciales de la partida actual*/
void disable_clues(HWND hwnd){
	int i,j,k;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			if(player.partida.inicial[i][j]!=0){
				k = 10*(i+1) + (j+1);
				switch(k){
					case 11: EnableWindow(casilla11,FALSE);break; case 12: EnableWindow(casilla12,FALSE);break; case 13: EnableWindow(casilla13,FALSE);break; case 14: EnableWindow(casilla14,FALSE);break; case 15: EnableWindow(casilla15,FALSE);break; case 16: EnableWindow(casilla16,FALSE);break; case 17: EnableWindow(casilla17,FALSE);break; case 18: EnableWindow(casilla18,FALSE);break; case 19: EnableWindow(casilla19,FALSE);break;
					case 21: EnableWindow(casilla21,FALSE);break; case 22: EnableWindow(casilla22,FALSE);break; case 23: EnableWindow(casilla23,FALSE);break; case 24: EnableWindow(casilla24,FALSE);break; case 25: EnableWindow(casilla25,FALSE);break; case 26: EnableWindow(casilla26,FALSE);break; case 27: EnableWindow(casilla27,FALSE);break; case 28: EnableWindow(casilla28,FALSE);break; case 29: EnableWindow(casilla29,FALSE);break;
					case 31: EnableWindow(casilla31,FALSE);break; case 32: EnableWindow(casilla32,FALSE);break; case 33: EnableWindow(casilla33,FALSE);break; case 34: EnableWindow(casilla34,FALSE);break; case 35: EnableWindow(casilla35,FALSE);break; case 36: EnableWindow(casilla36,FALSE);break; case 37: EnableWindow(casilla37,FALSE);break; case 38: EnableWindow(casilla38,FALSE);break; case 39: EnableWindow(casilla39,FALSE);break;
					case 41: EnableWindow(casilla41,FALSE);break; case 42: EnableWindow(casilla42,FALSE);break; case 43: EnableWindow(casilla43,FALSE);break; case 44: EnableWindow(casilla44,FALSE);break; case 45: EnableWindow(casilla45,FALSE);break; case 46: EnableWindow(casilla46,FALSE);break; case 47: EnableWindow(casilla47,FALSE);break; case 48: EnableWindow(casilla48,FALSE);break; case 49: EnableWindow(casilla49,FALSE);break;
					case 51: EnableWindow(casilla51,FALSE);break; case 52: EnableWindow(casilla52,FALSE);break; case 53: EnableWindow(casilla53,FALSE);break; case 54: EnableWindow(casilla54,FALSE);break; case 55: EnableWindow(casilla55,FALSE);break; case 56: EnableWindow(casilla56,FALSE);break; case 57: EnableWindow(casilla57,FALSE);break; case 58: EnableWindow(casilla58,FALSE);break; case 59: EnableWindow(casilla59,FALSE);break;
					case 61: EnableWindow(casilla61,FALSE);break; case 62: EnableWindow(casilla62,FALSE);break; case 63: EnableWindow(casilla63,FALSE);break; case 64: EnableWindow(casilla64,FALSE);break; case 65: EnableWindow(casilla65,FALSE);break; case 66: EnableWindow(casilla66,FALSE);break; case 67: EnableWindow(casilla67,FALSE);break; case 68: EnableWindow(casilla68,FALSE);break; case 69: EnableWindow(casilla69,FALSE);break;
					case 71: EnableWindow(casilla71,FALSE);break; case 72: EnableWindow(casilla72,FALSE);break; case 73: EnableWindow(casilla73,FALSE);break; case 74: EnableWindow(casilla74,FALSE);break; case 75: EnableWindow(casilla75,FALSE);break; case 76: EnableWindow(casilla76,FALSE);break; case 77: EnableWindow(casilla77,FALSE);break; case 78: EnableWindow(casilla78,FALSE);break; case 79: EnableWindow(casilla79,FALSE);break;
					case 81: EnableWindow(casilla81,FALSE);break; case 82: EnableWindow(casilla82,FALSE);break; case 83: EnableWindow(casilla83,FALSE);break; case 84: EnableWindow(casilla84,FALSE);break; case 85: EnableWindow(casilla85,FALSE);break; case 86: EnableWindow(casilla86,FALSE);break; case 87: EnableWindow(casilla87,FALSE);break; case 88: EnableWindow(casilla88,FALSE);break; case 89: EnableWindow(casilla89,FALSE);break;
					case 91: EnableWindow(casilla91,FALSE);break; case 92: EnableWindow(casilla92,FALSE);break; case 93: EnableWindow(casilla93,FALSE);break; case 94: EnableWindow(casilla94,FALSE);break; case 95: EnableWindow(casilla95,FALSE);break; case 96: EnableWindow(casilla96,FALSE);break; case 97: EnableWindow(casilla97,FALSE);break; case 98: EnableWindow(casilla98,FALSE);break; case 99: EnableWindow(casilla99,FALSE);break;
				}
			}
		}
	}
}

/*habilita y desabilita las opciones de juego e inicio de sesion respectivamente*/
void enable_disable(BOOL a){
	
	EnableWindow(casilla11,a);EnableWindow(casilla12,a);EnableWindow(casilla13,a);EnableWindow(casilla14,a);EnableWindow(casilla15,a);EnableWindow(casilla16,a);EnableWindow(casilla17,a);EnableWindow(casilla18,a);EnableWindow(casilla19,a);
	EnableWindow(casilla21,a);EnableWindow(casilla22,a);EnableWindow(casilla23,a);EnableWindow(casilla24,a);EnableWindow(casilla25,a);EnableWindow(casilla26,a);EnableWindow(casilla27,a);EnableWindow(casilla28,a);EnableWindow(casilla29,a);
	EnableWindow(casilla31,a);EnableWindow(casilla32,a);EnableWindow(casilla33,a);EnableWindow(casilla34,a);EnableWindow(casilla35,a);EnableWindow(casilla36,a);EnableWindow(casilla37,a);EnableWindow(casilla38,a);EnableWindow(casilla39,a);
	EnableWindow(casilla41,a);EnableWindow(casilla42,a);EnableWindow(casilla43,a);EnableWindow(casilla44,a);EnableWindow(casilla45,a);EnableWindow(casilla46,a);EnableWindow(casilla47,a);EnableWindow(casilla48,a);EnableWindow(casilla49,a);
	EnableWindow(casilla51,a);EnableWindow(casilla52,a);EnableWindow(casilla53,a);EnableWindow(casilla54,a);EnableWindow(casilla55,a);EnableWindow(casilla56,a);EnableWindow(casilla57,a);EnableWindow(casilla58,a);EnableWindow(casilla59,a);
	EnableWindow(casilla61,a);EnableWindow(casilla62,a);EnableWindow(casilla63,a);EnableWindow(casilla64,a);EnableWindow(casilla65,a);EnableWindow(casilla66,a);EnableWindow(casilla67,a);EnableWindow(casilla68,a);EnableWindow(casilla69,a);
	EnableWindow(casilla71,a);EnableWindow(casilla72,a);EnableWindow(casilla73,a);EnableWindow(casilla74,a);EnableWindow(casilla75,a);EnableWindow(casilla76,a);EnableWindow(casilla77,a);EnableWindow(casilla78,a);EnableWindow(casilla79,a);
	EnableWindow(casilla81,a);EnableWindow(casilla82,a);EnableWindow(casilla83,a);EnableWindow(casilla84,a);EnableWindow(casilla85,a);EnableWindow(casilla86,a);EnableWindow(casilla87,a);EnableWindow(casilla88,a);EnableWindow(casilla89,a);
	EnableWindow(casilla91,a);EnableWindow(casilla92,a);EnableWindow(casilla93,a);EnableWindow(casilla94,a);EnableWindow(casilla95,a);EnableWindow(casilla96,a);EnableWindow(casilla97,a);EnableWindow(casilla98,a);EnableWindow(casilla99,a);
     
    EnableWindow(Bnewsudoku,a);EnableWindow(Breset,a);EnableWindow(BLoad,a);EnableWindow(BSave,a);EnableWindow(Blogout,a);EnableWindow(Bexit,a);
    
    EnableWindow(jugador,a);EnableWindow(veces_jugadas,a);EnableWindow(veces_ganadas,a);EnableWindow(last_jugada,a);EnableWindow(infobox,a);
    
    EnableWindow(login_title,!a);EnableWindow(username,!a);EnableWindow(Bnewuser,!a);EnableWindow(Blogin,!a);
	 };

/*llena la matriz de juego activo con los números introducidos por el usuario*/
void actualizar_tablero(HWND hwnd,int x, int y, int ID){
	BOOL num;
	
    tablero[x][y] = GetDlgItemInt(hwnd, ID, &num, FALSE);
    
    
};

/*WINAPI - dibuja dos lineas verticales en el diseño de la ventana*/
void vertical(HDC hDC){
    MoveToEx(hDC,340,10,NULL);
    LineTo(hDC, 340, 153);
    MoveToEx(hDC,340,158,NULL);
    LineTo(hDC, 340, 345);
}

/*WINAPI - dibuja dos lineas horizontales en el diseño de la ventana*/
void horizontal(HDC hDC){
    MoveToEx(hDC,10,155,NULL);
    LineTo(hDC, 337, 155);
    MoveToEx(hDC,344,155,NULL);
    LineTo(hDC, 575, 155);
}

/*WINAPI - construye la interfaz gráfica del programa, indicando las funciones a ejecutarse al presionar cada boton y al actualizar las casillas*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    
	HDC hDC;
    PAINTSTRUCT ps;
    static TCHAR user_name[50];
    int len,option;
	switch(msg)
	{
		case WM_PAINT:{
            users_access();
            hDC = BeginPaint(hwnd, &ps);
            vertical(hDC);
            horizontal(hDC);            
            break;
        }	
		case WM_CREATE:
       	
		casilla11 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 166, 20, 20, hwnd, (HMENU) 11 ,NULL, NULL);
		casilla12 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 166, 20, 20, hwnd, (HMENU) 12 ,NULL, NULL);
		casilla13 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 166, 20, 20, hwnd, (HMENU) 13 ,NULL, NULL);
		casilla14 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 166, 20, 20, hwnd, (HMENU) 14 ,NULL, NULL);
		casilla15 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 166, 20, 20, hwnd, (HMENU) 15 ,NULL, NULL);
		casilla16 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 166, 20, 20, hwnd, (HMENU) 16 ,NULL, NULL);
		casilla17 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 166, 20, 20, hwnd, (HMENU) 17 ,NULL, NULL);
		casilla18 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 166, 20, 20, hwnd, (HMENU) 18 ,NULL, NULL);
		casilla19 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 166, 20, 20, hwnd, (HMENU) 19 ,NULL, NULL);
		
		casilla21 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 185, 20, 20, hwnd, (HMENU) 21 ,NULL, NULL);
		casilla22 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 185, 20, 20, hwnd, (HMENU) 22 ,NULL, NULL);
		casilla23 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 185, 20, 20, hwnd, (HMENU) 23 ,NULL, NULL);
		casilla24 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 185, 20, 20, hwnd, (HMENU) 24 ,NULL, NULL);
		casilla25 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 185, 20, 20, hwnd, (HMENU) 25 ,NULL, NULL);
		casilla26 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 185, 20, 20, hwnd, (HMENU) 26 ,NULL, NULL);
		casilla27 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 185, 20, 20, hwnd, (HMENU) 27 ,NULL, NULL);
		casilla28 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 185, 20, 20, hwnd, (HMENU) 28 ,NULL, NULL);
		casilla29 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 185, 20, 20, hwnd, (HMENU) 29 ,NULL, NULL);
		
		casilla31 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 204, 20, 20, hwnd, (HMENU) 31 ,NULL, NULL);
		casilla32 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 204, 20, 20, hwnd, (HMENU) 32 ,NULL, NULL);
		casilla33 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 204, 20, 20, hwnd, (HMENU) 33 ,NULL, NULL);
		casilla34 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 204, 20, 20, hwnd, (HMENU) 34 ,NULL, NULL);
		casilla35 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 204, 20, 20, hwnd, (HMENU) 35 ,NULL, NULL);
		casilla36 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 204, 20, 20, hwnd, (HMENU) 36 ,NULL, NULL);
		casilla37 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 204, 20, 20, hwnd, (HMENU) 37 ,NULL, NULL);
		casilla38 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 204, 20, 20, hwnd, (HMENU) 38 ,NULL, NULL);
		casilla39 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 204, 20, 20, hwnd, (HMENU) 39 ,NULL, NULL);
		
		casilla41 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 224, 20, 20, hwnd, (HMENU) 41 ,NULL, NULL);
		casilla42 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 224, 20, 20, hwnd, (HMENU) 42 ,NULL, NULL);
		casilla43 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 224, 20, 20, hwnd, (HMENU) 43 ,NULL, NULL);
		casilla44 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 224, 20, 20, hwnd, (HMENU) 44 ,NULL, NULL);
		casilla45 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 224, 20, 20, hwnd, (HMENU) 45 ,NULL, NULL);
		casilla46 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 224, 20, 20, hwnd, (HMENU) 46 ,NULL, NULL);
		casilla47 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 224, 20, 20, hwnd, (HMENU) 47 ,NULL, NULL);
		casilla48 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 224, 20, 20, hwnd, (HMENU) 48 ,NULL, NULL);
		casilla49 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 224, 20, 20, hwnd, (HMENU) 49 ,NULL, NULL);
		
		casilla51 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 243, 20, 20, hwnd, (HMENU) 51 ,NULL, NULL);
		casilla52 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 243, 20, 20, hwnd, (HMENU) 52 ,NULL, NULL);
		casilla53 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 243, 20, 20, hwnd, (HMENU) 53 ,NULL, NULL);
		casilla54 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 243, 20, 20, hwnd, (HMENU) 54 ,NULL, NULL);
		casilla55 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 243, 20, 20, hwnd, (HMENU) 55 ,NULL, NULL);
		casilla56 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 243, 20, 20, hwnd, (HMENU) 56 ,NULL, NULL);
		casilla57 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 243, 20, 20, hwnd, (HMENU) 57 ,NULL, NULL);
		casilla58 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 243, 20, 20, hwnd, (HMENU) 58 ,NULL, NULL);
		casilla59 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 243, 20, 20, hwnd, (HMENU) 59 ,NULL, NULL);
		
		casilla61 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 262, 20, 20, hwnd, (HMENU) 61 ,NULL, NULL);
		casilla62 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 262, 20, 20, hwnd, (HMENU) 62 ,NULL, NULL);
		casilla63 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 262, 20, 20, hwnd, (HMENU) 63 ,NULL, NULL);
		casilla64 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 262, 20, 20, hwnd, (HMENU) 64 ,NULL, NULL);
		casilla65 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 262, 20, 20, hwnd, (HMENU) 65 ,NULL, NULL);
		casilla66 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 262, 20, 20, hwnd, (HMENU) 66 ,NULL, NULL);
		casilla67 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 262, 20, 20, hwnd, (HMENU) 67 ,NULL, NULL);
		casilla68 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 262, 20, 20, hwnd, (HMENU) 68 ,NULL, NULL);
		casilla69 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 262, 20, 20, hwnd, (HMENU) 69 ,NULL, NULL);
		
		casilla71 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 282, 20, 20, hwnd, (HMENU) 71 ,NULL, NULL);
		casilla72 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 282, 20, 20, hwnd, (HMENU) 72 ,NULL, NULL);
		casilla73 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 282, 20, 20, hwnd, (HMENU) 73 ,NULL, NULL);
		casilla74 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 282, 20, 20, hwnd, (HMENU) 74 ,NULL, NULL);
		casilla75 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 282, 20, 20, hwnd, (HMENU) 75 ,NULL, NULL);
		casilla76 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 282, 20, 20, hwnd, (HMENU) 76 ,NULL, NULL);
		casilla77 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 282, 20, 20, hwnd, (HMENU) 77 ,NULL, NULL);
		casilla78 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 282, 20, 20, hwnd, (HMENU) 78 ,NULL, NULL);
		casilla79 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 282, 20, 20, hwnd, (HMENU) 79 ,NULL, NULL);
		
		casilla81 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 301, 20, 20, hwnd, (HMENU) 81 ,NULL, NULL);
		casilla82 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 301, 20, 20, hwnd, (HMENU) 82 ,NULL, NULL);
		casilla83 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 301, 20, 20, hwnd, (HMENU) 83 ,NULL, NULL);
		casilla84 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 301, 20, 20, hwnd, (HMENU) 84 ,NULL, NULL);
		casilla85 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 301, 20, 20, hwnd, (HMENU) 85 ,NULL, NULL);
		casilla86 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 301, 20, 20, hwnd, (HMENU) 86 ,NULL, NULL);
		casilla87 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 301, 20, 20, hwnd, (HMENU) 87 ,NULL, NULL);
		casilla88 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 301, 20, 20, hwnd, (HMENU) 88 ,NULL, NULL);
		casilla89 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 301, 20, 20, hwnd, (HMENU) 89 ,NULL, NULL);
		
		casilla91 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 50 , 320, 20, 20, hwnd, (HMENU) 91 ,NULL, NULL);
		casilla92 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 69 , 320, 20, 20, hwnd, (HMENU) 92 ,NULL, NULL);
		casilla93 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 88 , 320, 20, 20, hwnd, (HMENU) 93 ,NULL, NULL);
		casilla94 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 108, 320, 20, 20, hwnd, (HMENU) 94 ,NULL, NULL);
		casilla95 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 127, 320, 20, 20, hwnd, (HMENU) 95 ,NULL, NULL);
		casilla96 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 146, 320, 20, 20, hwnd, (HMENU) 96 ,NULL, NULL);
		casilla97 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 166, 320, 20, 20, hwnd, (HMENU) 97 ,NULL, NULL);
		casilla98 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 185, 320, 20, 20, hwnd, (HMENU) 98 ,NULL, NULL);
		casilla99 = CreateWindow( TEXT("Edit"), NULL, ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, 204, 320, 20, 20, hwnd, (HMENU) 99 ,NULL, NULL);
		
		Bnewsudoku = CreateWindow( TEXT("button"), TEXT("Nuevo")    , WS_VISIBLE | WS_CHILD, 250, 166, 80 , 25, hwnd, (HMENU) BNEWSUDOKU, NULL, NULL);
		Breset     = CreateWindow( TEXT("button"), TEXT("Reiniciar"), WS_VISIBLE | WS_CHILD, 250, 196, 80 , 25, hwnd, (HMENU) BRESET    , NULL, NULL);  
		BLoad      = CreateWindow( TEXT("button"), TEXT("Cargar")   , WS_VISIBLE | WS_CHILD, 250, 226, 80 , 25, hwnd, (HMENU) BLOAD     , NULL, NULL); 
		BSave      = CreateWindow( TEXT("button"), TEXT("Guardar")  , WS_VISIBLE | WS_CHILD, 250, 256, 80 , 25, hwnd, (HMENU) BSAVE     , NULL, NULL); 
		Blogout    = CreateWindow( TEXT("button"), TEXT("Logout")   , WS_VISIBLE | WS_CHILD, 250, 286, 80 , 25, hwnd, (HMENU) BLOGOUT   , NULL, NULL);          
		Bexit      = CreateWindow( TEXT("button"), TEXT("Salir")    , WS_VISIBLE | WS_CHILD, 250, 316, 80 , 25, hwnd, (HMENU) BEXIT     , NULL, NULL);          
		
    	jugador         = CreateWindow( TEXT("static"), TEXT("Jugador: ")      , WS_CHILD | WS_VISIBLE | SS_LEFT, 50 , 30 , 80 , 25, hwnd,  NULL                , NULL, NULL);
		jugador1        = CreateWindow( TEXT("static"), NULL                   , WS_CHILD | WS_VISIBLE | SS_LEFT, 111, 30 , 220 , 25, hwnd,  (HMENU)PLAYER       , NULL, NULL);
        veces_jugadas   = CreateWindow( TEXT("static"), TEXT("Juego N°: ")     , WS_CHILD | WS_VISIBLE | SS_LEFT, 50 , 60 , 150, 25, hwnd,  NULL                , NULL, NULL);
        veces_jugadas1  = CreateWindow( TEXT("static"), NULL                   , WS_CHILD | WS_VISIBLE | SS_LEFT, 117, 60 , 150, 25, hwnd,  (HMENU)TIMESPLAYED  , NULL, NULL);
        veces_ganadas   = CreateWindow( TEXT("static"), TEXT("Veces Ganadas: "), WS_CHILD | WS_VISIBLE | SS_LEFT, 50 , 90 , 150, 25, hwnd,  NULL                , NULL, NULL);
        veces_ganadas1  = CreateWindow( TEXT("static"), NULL                   , WS_CHILD | WS_VISIBLE | SS_LEFT, 160, 90 , 150, 25, hwnd,  (HMENU)TIMESSUCCESS , NULL, NULL);
        last_jugada     = CreateWindow( TEXT("static"), TEXT("Ultimo Juego: ") , WS_CHILD | WS_VISIBLE | SS_LEFT, 50 , 120, 150, 25, hwnd,  NULL                , NULL, NULL);
        last_jugada1    = CreateWindow( TEXT("static"), NULL                   , WS_CHILD | WS_VISIBLE | SS_LEFT, 145, 120, 150, 25, hwnd,  (HMENU)LASTGAME     , NULL, NULL);
        
        infobox       = CreateWindow( TEXT("static"), TEXT("\n\nINFOBOX") , WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER , 365, 210, 200, 100, hwnd,  (HMENU)INFOBOX, NULL, NULL);
        
        login_title = CreateWindow( TEXT("static"), TEXT(" Introducir Nombre de Usuario"), WS_CHILD | WS_VISIBLE | SS_LEFT, 365, 30 , 200, 25, hwnd, (HMENU) LOGINTITLE, NULL, NULL); 
        username    = CreateWindow( TEXT("Edit")  , NULL, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER                     , 365, 50 , 200, 25, hwnd, (HMENU) USERNAME ,NULL, NULL);
		Blogin      = CreateWindow( TEXT("button"), TEXT("Login")                       , WS_VISIBLE | WS_CHILD           , 405, 80 , 120, 25, hwnd, (HMENU) BLOGIN, NULL, NULL);
		Bnewuser    = CreateWindow( TEXT("button"), TEXT("Nuevo Jugador")               , WS_VISIBLE | WS_CHILD           , 405, 110, 120, 25, hwnd, (HMENU) BNEWUSER, NULL, NULL);
        enable_disable(FALSE);
        break;      
		
		case WM_COMMAND:	
			switch(wParam){
                case BLOGIN:{
                    len = GetWindowTextLength(username) + 1;
                    GetWindowText(username, user_name, len);
                    login(hwnd,user_name);
                break;}
                case BNEWUSER:{
                    len = GetWindowTextLength(username) + 1;
                    GetWindowText(username, user_name, len);
                    new_user(hwnd,user_name);
                break;}
                case BNEWSUDOKU:{
					option = MessageBox(hwnd,"Al realizar esta accion perderá su partida guardada, ¿Desea continuar?","Advertencia",MB_YESNO);
                    if(option == IDYES){
			            new_sudoku(hwnd);
						load(hwnd);
						SetDlgItemText(hwnd,INFOBOX,"\n\nNuevo Sudoku");
					}
                break;}
                case BRESET:{
                    reset(hwnd);
                    SetDlgItemText(hwnd,INFOBOX,"\n\nSudoku Inicial Cargado");
                break;}
                case BLOAD:{
                    load(hwnd);
                    SetDlgItemText(hwnd,INFOBOX,"\n\nSudoku Cargado");
                break;}
                case BSAVE:{
                    save();
                    SetDlgItemText(hwnd,INFOBOX,"\n\nSudoku Guardado Exitosamente");
                break;}
                case BLOGOUT:{
                    logout(hwnd);
                break;}
                case BCHECK:{
                    check(hwnd);
                break;}
                case BEXIT:{
                    bexit(hwnd);
                break;}
			}
			switch (LOWORD(wParam)){
                case 11:{ 
					if(HIWORD(wParam)==EN_UPDATE){
                        valido(hwnd,0,0,LOWORD(wParam),casilla11);
					}
				break;}
				case 12:{ 
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,1,LOWORD(wParam),casilla12);
					}
				break;}
				case 13:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,2,LOWORD(wParam),casilla13);
					}
				break;}
				case 14:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,3,LOWORD(wParam),casilla14);
					}
				break;}
				case 15:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,4,LOWORD(wParam),casilla15);
					}
				break;}
				case 16:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,5,LOWORD(wParam),casilla16);
					}
				break;}
				case 17:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,6,LOWORD(wParam),casilla17);
					}
				break;}
				case 18:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,7,LOWORD(wParam),casilla18);
					}
				break;}
				case 19:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,0,8,LOWORD(wParam),casilla19);
					}
				break;}
				
				case 21:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,0,LOWORD(wParam),casilla21);
					}
				break;}
				case 22:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,1,LOWORD(wParam),casilla22);
					}
				break;}
				case 23:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,2,LOWORD(wParam),casilla23);
					}
				break;}
				case 24:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,3,LOWORD(wParam),casilla24);
					}
				break;}
				case 25:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,4,LOWORD(wParam),casilla25);
					}
				break;}
				case 26:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,5,LOWORD(wParam),casilla26);
					}
				break;}
				case 27:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,6,LOWORD(wParam),casilla27);
					}
				break;}
				case 28:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,7,LOWORD(wParam),casilla28);
					}
				break;}
				case 29:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,1,8,LOWORD(wParam),casilla29);
					}
				break;}
				
				case 31:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,0,LOWORD(wParam),casilla31);
					}
				break;}
				case 32:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,1,LOWORD(wParam),casilla32);
					}
				break;}
				case 33:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,2,LOWORD(wParam),casilla33);
					}
				break;}
				case 34:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,3,LOWORD(wParam),casilla34);
					}
				break;}
				case 35:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,4,LOWORD(wParam),casilla35);
					}
				break;}
				case 36:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,5,LOWORD(wParam),casilla36);
					}
				break;}
				case 37:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,6,LOWORD(wParam),casilla37);
					}
				break;}
				case 38:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,7,LOWORD(wParam),casilla38);
					}
				break;}
				case 39:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,2,8,LOWORD(wParam),casilla39);
					}
				break;}
				
				case 41:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,0,LOWORD(wParam),casilla41);
					}
				break;}
				case 42:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,1,LOWORD(wParam),casilla42);
					}
				break;}
				case 43:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,2,LOWORD(wParam),casilla43);
					}
				break;}
				case 44:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,3,LOWORD(wParam),casilla44);
					}
				break;}
				case 45:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,4,LOWORD(wParam),casilla45);
					}
				break;}
				case 46:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,5,LOWORD(wParam),casilla46);
					}
				break;}
				case 47:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,6,LOWORD(wParam),casilla47);
					}
				break;}
				case 48:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,7,LOWORD(wParam),casilla48);
					}
				break;}
				case 49:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,3,8,LOWORD(wParam),casilla49);
					}
				break;}
				
				case 51:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,0,LOWORD(wParam),casilla51);
					}
				break;}
				case 52:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,1,LOWORD(wParam),casilla52);
					}
				break;}
				case 53:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,2,LOWORD(wParam),casilla53);
					}
				break;}
				case 54:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,3,LOWORD(wParam),casilla54);
					}
				break;}
				case 55:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,4,LOWORD(wParam),casilla55);
					}
				break;}
				case 56:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,5,LOWORD(wParam),casilla56);
					}
				break;}
				case 57:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,6,LOWORD(wParam),casilla57);
					}
				break;}
				case 58:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,7,LOWORD(wParam),casilla58);
					}
				break;}
				case 59:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,4,8,LOWORD(wParam),casilla59);
					}
				break;}
				
				case 61:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,0,LOWORD(wParam),casilla61);
					}
				break;}
				case 62:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,1,LOWORD(wParam),casilla62);
					}
				break;}
				case 63:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,2,LOWORD(wParam),casilla63);
					}
				break;}
				case 64:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,3,LOWORD(wParam),casilla64);
					}
				break;}
				case 65:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,4,LOWORD(wParam),casilla65);
					}
				break;}
				case 66:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,5,LOWORD(wParam),casilla66);
					}
				break;}
				case 67:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,6,LOWORD(wParam),casilla67);
					}
				break;}
				case 68:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,7,LOWORD(wParam),casilla68);
					}
				break;}
				case 69:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,5,8,LOWORD(wParam),casilla69);
					}
				break;}
				
				case 71:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,0,LOWORD(wParam),casilla71);
					}
				break;}
				case 72:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,1,LOWORD(wParam),casilla72);
					}
				break;}
				case 73:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,2,LOWORD(wParam),casilla73);
					}
				break;}
				case 74:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,3,LOWORD(wParam),casilla74);
					}
				break;}
				case 75:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,4,LOWORD(wParam),casilla75);
					}
				break;}
				case 76:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,5,LOWORD(wParam),casilla76);
					}
				break;}
				case 77:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,6,LOWORD(wParam),casilla77);
					}
				break;}
				case 78:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,7,LOWORD(wParam),casilla78);
					}
				break;}
				case 79:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,6,8,LOWORD(wParam),casilla79);
					}
				break;}
				
				case 81:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,0,LOWORD(wParam),casilla81);
					}
				break;}
				case 82:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,1,LOWORD(wParam),casilla82);
					}
				break;}
				case 83:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,2,LOWORD(wParam),casilla83);
					}
				break;}
				case 84:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,3,LOWORD(wParam),casilla84);
					}
				break;}
				case 85:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,4,LOWORD(wParam),casilla85);
					}
				break;}
				case 86:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,5,LOWORD(wParam),casilla86);
					}
				break;}
				case 87:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,6,LOWORD(wParam),casilla87);
					}
				break;}
				case 88:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,7,LOWORD(wParam),casilla88);
					}
				break;}
				case 89:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,7,8,LOWORD(wParam),casilla89);
					}
				break;}
				
				case 91:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,0,LOWORD(wParam),casilla91);
					}
				break;}
				case 92:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,1,LOWORD(wParam),casilla92);
					}
				break;}
				case 93:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,2,LOWORD(wParam),casilla93);
					}
				break;}
				case 94:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,3,LOWORD(wParam),casilla94);
					}
				break;}
				case 95:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,4,LOWORD(wParam),casilla95);
					}
				break;}
				case 96:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,5,LOWORD(wParam),casilla96);
					}
				break;}
				case 97:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,6,LOWORD(wParam),casilla97);
					}
				break;}
				case 98:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,7,LOWORD(wParam),casilla98);
					}
				break;}
				case 99:{
					if(HIWORD(wParam)==EN_UPDATE){
						valido(hwnd,8,8,LOWORD(wParam),casilla99);
					}
				break;}
			}
		break;
		case WM_DESTROY:{
			if(blog == false){
				if (equal_boards(tablero, player.partida.guardado)==true){
					date_updater();
					db_updater();
					PostQuitMessage(0);
				}else{
					option = MessageBox(hwnd, "Ha abandonado el juego, ¿desea guardar la partida?", "Salir", MB_YESNO);
					switch(option){
						case IDYES: save();date_updater();db_updater();PostQuitMessage(0);break;
						case IDNO: date_updater();db_updater();PostQuitMessage(0);break;
					}
				}
			}else{
				PostQuitMessage(0);
			}
		break;}
	}
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
    srand(time(0));
	MSG  msg ;    
	WNDCLASS wc = {0};
	wc.lpszClassName = TEXT( "Sudoku" );
	wc.hInstance     = hInstance ;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc   = WndProc ;
	wc.hCursor       = LoadCursor(0,IDC_ARROW);

	RegisterClass(&wc);
	CreateWindow( wc.lpszClassName, TEXT("Sudoku"),
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				220, 220, 600, 385, 0, 0, hInstance, 0);  

	while( GetMessage(&msg, NULL, 0, 0)) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}
