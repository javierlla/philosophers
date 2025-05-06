# 🍝 Philosophers

**Philosophers** es un proyecto de concurrencia basado en el clásico problema de los filósofos comensales. Su objetivo es practicar la gestión de **hilos**, **mutexes** y evitar condiciones de carrera y **deadlocks** en entornos multihilo.

---

## 📌 Descripción del proyecto

Cinco filósofos se sientan alrededor de una mesa circular. Cada uno alterna entre pensar, comer y dormir. Para comer, un filósofo necesita dos tenedores (uno a su izquierda y otro a su derecha), pero cada tenedor está compartido con el filósofo de al lado. El reto consiste en gestionar la concurrencia de manera que:

- No se produzca un **deadlock**.
- No se produzcan **condiciones de carrera**.
- Cada filósofo coma sin morir de hambre.

---

🧠 Reglas del problema
Parámetros de entrada:

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

    number_of_philosophers: número de filósofos (≥ 1)

    time_to_die: tiempo máximo (ms) sin comer antes de morir

    time_to_eat: tiempo (ms) que tarda un filósofo en comer

    time_to_sleep: tiempo (ms) que tarda un filósofo en dormir

    number_of_times_each_philosopher_must_eat (opcional): si se proporciona, el programa termina cuando todos los filósofos han comido al menos esa cantidad de veces

⚙️ Compilación

make

Esto genera el ejecutable philo.
🚀 Ejecución

Ejemplo de ejecución con 5 filósofos:

./philo 5 800 200 200

Ejemplo con límite de comidas:

./philo 5 800 200 200 7

🕹️ Comportamiento esperado

    Cada acción (pensar, comer, dormir, morir) se imprime con un timestamp relativo y el ID del filósofo.

    Ejemplo de salida:

timestamp 1 has taken a fork
timestamp 1 has taken a fork
timestamp 1 is eating
timestamp 1 is sleeping
timestamp 1 is thinking
...

    Si un filósofo muere, el mensaje X died debe ser el último en imprimirse y el programa termina inmediatamente.

🔒 Sincronización

Para garantizar un comportamiento correcto en concurrencia, se deben usar:

    Threads (pthread_create, pthread_join)

    Mutexes para proteger:

        Tenedores

        Escritura en pantalla

        Variables compartidas (estado de vida, contador de comidas)

💡 Prevención de deadlocks

El proyecto exige evitar bloqueos entre filósofos. Algunas estrategias válidas son:

    Hacer que los filósofos con ID par tomen primero el tenedor derecho y luego el izquierdo (y los impares al revés).

    Limitar el número de filósofos que pueden intentar comer simultáneamente.

    Usar semáforos (en la versión bonus).

🔍 Consideraciones técnicas

    Evitar condiciones de carrera al acceder a datos compartidos.

    Evitar usleep imprecisos: usar un temporizador más preciso en un bucle si es necesario.

    Manejar correctamente errores de inicialización y liberar recursos (mutexes, memoria).
