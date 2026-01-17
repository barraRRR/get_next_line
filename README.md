Compruebo si tengo lista
leo el buffer
voy copiando frases enteras
si stash->next == NULL sigo leyendo hasta que stash->str[0] == 0


problemas de buf grande:
	perder buf leido entre llamadas

problemas de buf pequeño
	leer varias veces hasta completar un string


split style
	funciona mejor con buf grande


doble struc


conclusiones
	- hay que guardar los datos guardados del bufer entre llamadas
	- hay que controlar cuantos datos llevamos leidos del buffer.
	- hay que leer solo una linea.


YouTube | [Learn How to Use the read() System Call in C to Read Files](https://www.youtube.com/watch?v=jYQPGkJHFr8)
YouTube | [Static Variables In C Programming Language](https://www.youtube.com/watch?v=MVeN3GDwjUg)
YouTube | [C structs 🏠](https://youtu.be/oKXP1HZ8xIs?si=UcOuFMv0SPqruzh7)
YouTube | [System Programming with C - Opening and Reading Files](https://www.youtube.com/watch?v=scXWLP8uhDU)