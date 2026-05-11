 # Practica de Procesos /EPN/FIS

## Descripcion

Este proyecto demuestra la creacion de procesos en Linux para visualizarlos procesos de un sistema mediante `system ().`


# Utilizaciòn de las siguientes acciones

Para mantener un correccto flujo de informaciòn y entender cual es el proceso <<PADRE>> y cual el proceso <<HIJO>>.

1. - En primer lugar se obtiene el PID del proceso original. 

2. -Crear un proceso hijo mediante `fork()`.

3. -El **hijo** utiliza ***system()*** para:

 ***

- Listar procesos con  `ps -f --forest`.

** ps -f --forest*: Es ideal para obtener detalles tècnicos como (UID,PID,PPID), o el uso de la CPU. su visualizaciòn depende de 
`forest ()`para mostrar los procesos del computador en foma de **jerarquìas**.

- Generar un àrbol jeràrquico dinàmico con `pstree`.

**pstree** es superior para la comprensiòn de los procesos que ocurren en PC ya que permite la observaciòn de los procesos del sistema en forma de ramificaciòn mostrando la dependencia de forma gràfica y compacta.

4. - El **padre** espera la finalizaciòn del hijo para evitar procesos zombies usando `wait`.

## Requisitos

-----Sistema Operativo---- : Ubuntu (Linux).

-----Compilador----------- : GCC
## Compilaciòn y ejecuciòn:

Para compilar el programa
```bash
gcc ejemplosystem.c -o practica3
```
## Conclusiones ----

-Se logrò dominar la creaciòn de procesos mediante `fork ()`, ademàs se comprendiò la utilidad del comando `sprintf ()` para construir comandos dinàmicos en tiempo de ejecuciòn. 
Con ellos se logrò verificar la gestiòn de procesos en Linux para el desarrollo de sistemas concurrentes 
