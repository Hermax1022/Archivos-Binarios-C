#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estructura de empleados
struct empleados{
  int legajo;
  char nombre[30];
  char categoria;
  double sueldo;
  int activo;
};

//funcion menu
void menu(){
  printf("\n\t=========MENU=========");
  printf("\n1. Crear el archivo empleados.dat");
  printf("\n2. Cargar datos de un empleado");
  printf("\n3. Listar los datos por pantalla");
  printf("\n4. Buscar datos de un empleado");
  printf("\n5. Modificar categoria y sueldo de un empleado");
  printf("\n6. Actualizar sueldos");
  printf("\n7. dar de baja logica a un empleado");
  printf("\n8. dar la baja fisica de los empleados inactivos");
  printf("\n9. Salir del programa");
  printf("\nIngrese una opcion [ ]\b\b");
}

//Prototipos de las funciones
void crearArch(FILE* pA,char* );
void altaDeDatos(FILE* pA,char* nom);
void mostrarDatos(FILE* pA, char* nom);
void buscarEmpleado(FILE* pA, char* nom);
void modificarEmpleado(FILE* pA,char *nom);
void actualizarSueldos(FILE* pA,char* nom);
void bajaLogica(FILE* pA,char*nom);
void bajaFisica(FILE* pA,char* nom);

//Funciones adicionales
int busqueda(FILE* pA,int leg);
void cabeceras();;

int main(void) {
  char nom[30];
  int opc = 0;
  FILE* pArch;
  strcpy(nom,"empleados.dat");
  do{
    system("cls");
    menu();
    scanf("%d",&opc);
    switch(opc){
      case 1: crearArch(pArch,nom); break;
      case 2: altaDeDatos(pArch,nom);  break;
      case 3: mostrarDatos(pArch,nom); break;
      case 4: buscarEmpleado(pArch,nom); break;
      case 5: modificarEmpleado(pArch,nom); break;
      case 6: actualizarSueldos(pArch,nom); break;
      case 7: bajaLogica(pArch,nom); break;
      case 8: bajaFisica(pArch,nom); break;
      case 9: break;
      default: printf("\nNumero mal ingresado!!"); break;
    }


  }while(opc != 9);
}
//punto a, crear el archivo
void crearArch(FILE* pA,char* nom){
  pA=fopen(nom,"wb");
  struct empleados emp = {0,"-",'-',0,0};
  if(pA != NULL){
    printf("\nArchivo creado correctamente!!");
    fseek(pA,0,SEEK_SET);
    for(int i=0;i<10;i++){
      fwrite(&emp,sizeof(struct empleados),1,pA);
    }
    fclose(pA);
  }
  else{
    printf("No se ha podido crear el archivo\n");
  }
  printf("\n\nPresione una tecla para continuar.. ");
  getch();
}

void altaDeDatos(FILE* pA,char* nom){
  struct empleados emp;
  struct empleados aux = {0,"-",'-',0,0};
  char opc;
  pA = fopen(nom, "r+b");
  if(pA != NULL){
    do{
        system("cls");
        printf("\nIntroduzca el numero de legajo: ");
        scanf("%d",&emp.legajo);
        if(emp.legajo > 0 && busqueda(pA,emp.legajo) == 1){
          printf("Ingrese el nombre: ");
          fflush(stdin);
          gets(emp.nombre);
          printf("Ingrese categoria: ");
          fflush(stdin);
          scanf("%c",&emp.categoria);
          printf("Ingrese el sueldo: ");
          scanf("%lf",&emp.sueldo);
          printf("¿Esta activo? 1-si / 0-no : ");
          scanf("%d",&emp.activo);
          if(emp.legajo == 1){
            fseek(pA,0,SEEK_SET);
            fwrite(&emp,sizeof(struct empleados),1,pA);
          }
          else{
            fseek(pA,0,SEEK_SET);
            fseek(pA,(emp.legajo-1)*sizeof(struct empleados),SEEK_SET);
            fwrite(&emp,sizeof(struct empleados),1,pA);
          }
        }
        else if(emp.legajo>0){
          printf("\nEl registro %d ya esta creado..",emp.legajo);
        }else printf("\nEl legajo tiene que ser mayor a 0");

    printf("\nSi desea cargar otro empleado marque 1, de lo contrario presione cualquier tecla.. ");
    fflush(stdin);
    scanf("%c",&opc);
    }while(opc == '1');

  }else{
    printf("\nError al abrir el archivo %s",nom);
  }

  fclose(pA);

}

void mostrarDatos(FILE* pA, char* nom){
  int opc=0,i=0;
  pA = fopen(nom,"rb");
  if(pA != NULL){
    fseek(pA,0,SEEK_END);
    int cantReg = ftell(pA)/sizeof(struct empleados),i=0;
    printf("\n1. Mostrar todos los empleados\n2.Mostrar los empleados activos\nElija una opcion [ ]\b\b");
    scanf("%d",&opc);
    if(opc == 1 || opc == 2){
      struct empleados emp;
      fseek(pA,0,SEEK_SET);
      cabeceras();
      //
      while(i<cantReg){
        fread(&emp,sizeof(struct empleados),1,pA);
        if(opc == 1 && emp.legajo >0){
          printf("\n%6d | %14s | %9c | %9.1f | %5d",emp.legajo,emp.nombre,emp.categoria,emp.sueldo,emp.activo);
        }else if(opc == 2 && emp.activo == 1){
          printf("\n%6d | %14s | %9c | %9.1f | %5d",emp.legajo,emp.nombre,emp.categoria,emp.sueldo,emp.activo);
        }
       i++;
      }
    }else{
      printf("\nopcion mal incorrecta!!");
    }
    fclose(pA);
  }else{
    printf("\nError al abrir el archivo");
  }
  printf("\n\nPresione una tecla para continuar.. ");
  getch();
}

void buscarEmpleado(FILE* pA, char* nom){
  pA = fopen(nom,"rb");
  struct empleados emp;
  int opc;
  if(pA != NULL){
     fseek(pA,0,SEEK_END);
     int cantReg = ftell(pA)/sizeof(struct empleados),i=0;
     printf("\n1. Buscar un empleado por legajo\n2. buscar empleado por nombre\nElija una opcion [ ]\b\b");
     scanf("%d",&opc);
     if(opc == 1){
       int legajo;
       printf("\nIntroduzca el legajo que quiere buscar: ");
       scanf("%d",&legajo);
       fseek(pA,0,SEEK_SET);
       if(legajo>0){
         fseek(pA,(legajo-1)*sizeof(struct empleados),SEEK_SET);
         fread(&emp,sizeof(struct empleados),1,pA);
         if(emp.legajo >0){
           cabeceras();
           printf("\n%6d | %14s | %9c | %9.1f | %5d",emp.legajo,emp.nombre,emp.categoria,emp.sueldo,emp.activo);
         }else printf("\nNo existe el legajo solicitado!");
       }else printf("Legajo mal ingresado!");

     } else if(opc == 2){
       char nombre[30];
       int flag = 0,i=0;
       printf("\nIntroduzca un nombre: ");
       fflush(stdin);
       gets(nombre);
       fseek(pA,0,SEEK_SET);
       while(i<cantReg){
         fread(&emp,sizeof(struct empleados),1,pA);
         if(strcmp(emp.nombre,nombre)==0){
           cabeceras();
           printf("\n%6d | %14s | %9c | %9.1f | %5d",emp.legajo,emp.nombre,emp.categoria,emp.sueldo,emp.activo);
           flag = 1;
         }
         i++;
       }
       if(flag == 0){
         printf("\nNo existe en el registro el nombre solicitado!");
       }

     } else printf("\nOpcion mal incorrecta!");

    fclose(pA);
  }else printf("\nError al abrir el archivo");
  printf("\n\nPresione una tecla paa continuar.. ");
  getch();
}

void modificarEmpleado(FILE* pA,char *nom){
  pA = fopen(nom,"r+b");
  struct empleados emp;
  int opc;
  char categoria;
  double sueldo;
  if(pA != NULL){
    fseek(pA,0,SEEK_END);
    int nReg = ftell(pA)/(sizeof(struct empleados));
    fseek(pA,0,SEEK_SET);
    printf("\n1. Buscar empleado por Legajo\n2.Buscar empleado por nombre\nElija una opcion [ ]\b\b");
    scanf("%d",&opc);
    if(opc == 1){
      char opc2;
      int legajo;
      printf("\nIntroduzca el nro de legajo del empleado: ");
      scanf("%d",&legajo);
      printf("\nSi realmente desea modificar el registro %d presione 1, de lo contrario presione cualquier tecla: ",legajo);
      fflush(stdin);
      scanf("%c",&opc2);
      if(opc2 == '1'){
          fseek(pA,0,SEEK_SET);
          fseek(pA,(legajo-1)*sizeof(struct empleados),SEEK_SET);
          fread(&emp,sizeof(struct empleados),1,pA);
          printf("\nIntroduzca una categoria: ");
          fflush(stdin);
          scanf("%c",&categoria);
          emp.categoria = categoria;
          printf("Introduzca el sueldo: ");
          scanf("%lf",&sueldo);
          emp.sueldo = sueldo;
          fseek(pA,-sizeof(struct empleados),SEEK_CUR);
          fwrite(&emp,sizeof(struct empleados),1,pA);
          printf("\nSe ha modifica los datos del empleado correctamente");
      }
    }
    else if(opc == 2){
      char nombre[30];
      int flag = 0,i=0;
      char opc2;
      printf("\nIngrese el nombre del empleado: ");
      fflush(stdin);
      gets(nombre);
      fseek(pA,0,SEEK_SET);
      while(i<nReg){
        fread(&emp,sizeof(struct empleados),1,pA);
        if(strcmp(emp.nombre,nombre)==0){
          flag = 1;
          break;
        }
      }
      if(flag == 1){
        printf("\nSi realmente desea modificar el registro %s presione 1, de lo contrario presione cualquier tecla: ",emp.nombre);
        scanf("%c",&opc2);
        if(opc2 == '1'){
            printf("\nIntroduzca una categoria: ");
            fflush(stdin);
            scanf("%c",&categoria);
            emp.categoria = categoria;
            printf("Introduzca el sueldo: ");
            scanf("%lf",&sueldo);
            emp.sueldo = sueldo;
            fseek(pA,0,SEEK_SET);
            fseek(pA,(emp.legajo-1)*sizeof(struct empleados),SEEK_SET);
            fwrite(&emp,sizeof(struct empleados),1,pA);
            printf("\nSe ha modificado los datos del empleado correctamente");
            flag = 1;
          }
        }else
        printf("\nNo se encontro en el registro el nombre solicitado!");

    }else printf("\nOpcion mal incorrecta!");

    fclose(pA);
  }else printf("\nError al abrir el archivo!");

  printf("\n\nPresione una tecla para continuar.. ");
  getch();
}

void actualizarSueldos(FILE* pA,char* nom){
  pA = fopen(nom,"r+b");
  char categoria[10];
  double porcentaje[10];
  int nReg, cont=0,opc,i=0;
  struct empleados emp;
  if(pA != NULL){
    do{
      printf("\nIngrese la categoria: ");
      fflush(stdin);
      scanf("%c",&categoria[cont]);
      printf("\nIngrese el porcentaje de paritaria: ");
      scanf("%lf",&porcentaje[cont]);
      cont++;
      printf("\nPresione cualquier tecla si desea seguir cargando mas datos, 0 para finalizar: ");
      scanf("%d",&opc);

    }while (opc != 0);

    fseek(pA,0,SEEK_END);
    nReg = ftell(pA)/(sizeof(struct empleados));


    while(i<nReg){
      fseek(pA,0,SEEK_SET);
      fseek(pA,i*sizeof(struct empleados),SEEK_SET);
      fread(&emp,sizeof(struct empleados),1,pA);
      for(int j=0;j<cont;j++){
        if(emp.categoria == categoria[j]){
          double adicional = emp.sueldo * porcentaje[j] / 100;
          emp.sueldo += adicional;
        }
      }

      fseek(pA,0,SEEK_SET);
      fseek(pA,i*sizeof(struct empleados),SEEK_SET);
      fwrite(&emp,sizeof(struct empleados),1,pA);
      i++;
    }
    printf("\nSueldos actualizados correctamente!");
    fclose(pA);
  }else printf("\nError al abrir el archivo");
  printf("\n\nPresione una tecla para continuar.. ");
  getch();
}

void bajaLogica(FILE* pA,char*nom){
  pA = fopen(nom,"r+b");
  int legajo;
  struct empleados emp;
  char opc;
  if(pA != NULL){
    printf("\nIngrese el legajo del empleado que quiere dar la baja logica: ");
    scanf("%d",&legajo);
    fseek(pA,0,SEEK_SET);
    fseek(pA,(legajo-1)*sizeof(struct empleados),SEEK_SET);
    fread(&emp,sizeof(struct empleados),1,pA);
    if(emp.legajo > 0){
        if(emp.activo == 1){
            printf("\nLegajo %d\nNombre: %s\nCategoria: %c\nSueldo: %.2lf\nActivo: %d\nDesea dar de baja presione 1, para cancelar presione cualquier tecla [ ]\b\b",emp.legajo,emp.nombre,emp.categoria,emp.sueldo,emp.activo);
        fflush(stdin);
        scanf("%c",&opc);
        if(opc == '1'){
          emp.activo = 0;
          fseek(pA,-sizeof(struct empleados),SEEK_CUR);
          fwrite(&emp,sizeof(struct empleados),1,pA);
          printf("\nEl empleado %s fue dado de baja..",emp.nombre);
          printf("\n\n\t==Listado de Empleados Activos==");
          fseek(pA,0,SEEK_END);
          int nReg = ftell(pA)/(sizeof(struct empleados));
          int i=0;
          cabeceras();
          fseek(pA,0,SEEK_SET);
          while(i<nReg){
            fread(&emp,sizeof(struct empleados),1,pA);
            if(emp.activo == 1){
              printf("\n%6d | %14s | %9c | %9.1f | %5d",emp.legajo,emp.nombre,emp.categoria,emp.sueldo,emp.activo);
            }
            i++;
          }
          printf("\n");

        }else printf("\nModificacion cancelada..");
        }else printf("\nEl empleado seleccionado ya esta dado de baja");

    }else printf("\nEse legajo no existe");
    fclose(pA);
  }else printf("\nError al abrir el archivo");
  printf("\n\nPresione una tecla para continuar.. ");
  getch();
}

void bajaFisica(FILE* pA,char* nom){
  pA = fopen(nom,"rb");
  struct empleados emp;
  if(pA != NULL){
    FILE* pAux = fopen("temp.dat","wb");
    if(pAux != NULL){
      fseek(pA,0,SEEK_END);
      int nReg = ftell(pA)/(sizeof(struct empleados));
      int i=0;
      fseek(pA,0,SEEK_SET);
      while(i<nReg){
        fread(&emp,sizeof(struct empleados),1,pA);
        if(emp.activo == 1){
          fseek(pAux,0,SEEK_SET);
          fseek(pAux,(emp.legajo-1)*sizeof(struct empleados),SEEK_SET);
          fwrite(&emp,sizeof(struct empleados),1,pAux);
        }
        i++;
      }
      printf("\nSe ha creado el archivo temp.dat con los empleados activos exitosamente..");
      fclose(pA);
      fclose(pAux);
    }else printf("\nError al abrir el archivo temp.dat");
  }else printf("\nError al abrir el archivo %s",nom);
  printf("\n\nPresione una tecla para continuar.. ");
  getch();
}

int busqueda(FILE* pA,int leg){
  struct empleados aux;
  fseek(pA,0,SEEK_SET);
  fread(&aux, sizeof(struct empleados),1,pA);
  if(aux.legajo == leg){
    return -1;
  }
  else{
    while(!feof(pA)){
      fread(&aux, sizeof(struct empleados),1,pA);
      if(aux.legajo == leg){
        return -1;
      }
    }
    return 1;
  }
}

void cabeceras(){
  char headers[5][30] = {"Legajo","Nombre","categoria","sueldo","activo"};
  printf("\n\n%3s | %10s\t| %7s |  %7s  | %7s",headers[0],headers[1],headers[2],headers[3],headers[4]);
    printf("\n-------+----------------+-----------+-----------+---------");
}
