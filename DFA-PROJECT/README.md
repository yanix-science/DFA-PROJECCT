# DFA Project
Este proyecto implementa un Autómata Finito Determinista (DFA) en lenguaje C, diseñado para evaluar cadenas de entrada y determinar si son aceptadas o rechazadas con base en un conjunto de estados, un alfabeto, y transiciones especificadas en un archivo de configuración (configuracion.txt) o (configuracion2.txt). El DFA se procesa de manera que, a partir de un estado inicial, sigue las transiciones definidas hasta llegar a un estado de aceptación o rechazo.

Instrucciones de Ejecución:
1. Abrir terminal.
1.1 Clonar el repositorio:
    git clone https://github.com/yanix-science/DFA-PROJECCT.git
    cd DFA-PROJECT
   
1.2 Compilar el programa con el siguente comando:
    gcc -o dfaa dfaa.c
   
1.3 Ejecutar el programa:
    ./dfaa configuracion.txt
   o
    ./dfaa configuracion2.txt
    
1.4 Procesar la cadena de entrada:
    El programa solicitará una cadena de entrada. Ingresa la cadena que deseas evaluar según las reglas del DFA (.txt)
    
1.5 Obtener resultado:
    cat resultado.txt
    
1.5 Resultado:
    El resultado se guardará en un archivo resultado.txt, que indicará si la cadena fue aceptada o rechazada.
