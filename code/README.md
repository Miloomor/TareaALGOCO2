# Documentación del Proyecto: AniMarathon

**Autor:** Emilio Moran Valdebenito
**Rol USM:** 202473575-6
**Asignatura:** INF-221 Algoritmos y Complejidad (2026-1)

Este directorio contiene las implementaciones de los algoritmos de resolución para el problema **AniMarathon**, las herramientas para generar casos de prueba, y la infraestructura para medir rendimiento y generar gráficos comparativos.

## Estructura del Directorio

* **`algorithms/`**: Implementaciones individuales de los algoritmos en C++.
  * [brute-force.cpp](file:///home/ejmv70/_cpp/ALGOCO/Tarea2/code/implementation/algorithms/brute-force.cpp): Enfoque exhaustivo con backtracking y podas.
  * [greedy1.cpp](file:///home/ejmv70/_cpp/ALGOCO/Tarea2/code/implementation/algorithms/greedy1.cpp): Heurística Greedy basada en densidad de satisfacción global.
  * [greedy2.cpp](file:///home/ejmv70/_cpp/ALGOCO/Tarea2/code/implementation/algorithms/greedy2.cpp): Heurística Greedy basada en selección marginal por capítulo.
  * [dynamic-programming.cpp](file:///home/ejmv70/_cpp/ALGOCO/Tarea2/code/implementation/algorithms/dynamic-programming.cpp): Solución óptima utilizando Knapsack 2D grupal.
* **`general.cpp`**: Driver principal que coordina la lectura, ejecución secuencial, medición de tiempo (`<chrono>`) y memoria (`wait4` / `getrusage`), y escritura de reportes.
* **`makefile`**: Archivo de automatización de compilación con banderas `-O3` y `-Wall`.
* **`scripts/`**:
  * [testcases_generator.py](file:///home/ejmv70/_cpp/ALGOCO/Tarea2/code/implementation/scripts/testcases_generator.py): Generador de casos de prueba.
  * [plot_generator.py](file:///home/ejmv70/_cpp/ALGOCO/Tarea2/code/implementation/scripts/plot_generator.py): Procesador de métricas y generador de gráficos en PNG.
* **`data/`** *(Creado automáticamente)*:
  * `inputs/`: Casos de prueba generados (`testcases_{n}_{i}.txt`).
  * `outputs/`: Resultados de satisfacción de cada algoritmo (`output_{n}_{i}.txt`).
  * `measurements/`: Métricas de tiempo, memoria y satisfacción en formato CSV (`measurements_{n}_{i}.txt`).
  * `plots/`: Gráficos PNG comparativos de rendimiento, memoria y calidad.

---

## Instrucciones de Compilación y Ejecución

### 1. Compilación

Para compilar el programa principal (`general`) y todos los algoritmos individuales en forma optimizada, ejecuta:

```bash
make
```

Para limpiar los ejecutables compilados:

```bash
make clean
```

### 2. Generación de Casos de Prueba

Para generar la batería completa de casos de prueba (pequeños $n \in \{3, 5, 8\}$, medianos $n \in \{20, 40, 80\}$, y grandes $n \in \{100, 150, 200\}$) con 3 réplicas cada uno:

```bash
python scripts/testcases_generator.py
```

Los archivos de entrada se guardarán en `data/inputs/`.

### 3. Ejecución de Experimentos

Para ejecutar el programa de medición sobre un caso de prueba individual:

```bash
./general data/inputs/testcases_3_1.txt
```

Para procesar de forma automática **todos** los casos de prueba generados en la carpeta `data/inputs/`:

```bash
for file in data/inputs/testcases_*.txt; do
    ./general "$file"
done
```

> [!NOTE]
> El driver `general` ejecutará automáticamente los cuatro algoritmos de forma secuencial. El algoritmo de Fuerza Bruta (`BruteForce`) se omitirá automáticamente para casos con $n > 10$ para evitar tiempos de espera excesivos, marcando sus resultados como `N/A`.

### 4. Generación de Gráficos Comparativos

Una vez que hayas ejecutado todos los casos de prueba y generado los archivos de medición, ejecuta el siguiente script para generar los gráficos comparativos:

```bash
python scripts/plot_generator.py
```

Los gráficos se guardarán en `data/plots/` con los nombres:
* `performance_vs_n.png`: Comparación del tiempo de ejecución (escala logarítmica).
* `memory_vs_n.png`: Comparación del consumo de memoria máxima (RSS en MB).
* `quality_vs_n.png`: Calidad de la solución de los Greedy comparada con el Óptimo (DP).
