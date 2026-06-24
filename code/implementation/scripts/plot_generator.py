import os
import csv
import glob
from collections import defaultdict
import matplotlib.pyplot as plt

# Directorios
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MEASUREMENTS_DIR = os.path.join(SCRIPT_DIR, '..', 'data', 'measurements')
PLOTS_DIR = os.path.join(SCRIPT_DIR, '..', 'data', 'plots')
os.makedirs(PLOTS_DIR, exist_ok=True)

def main():
    # Estructuras para recolectar datos
    # data_by_algo[algo][N] = list of floats/ints
    times = defaultdict(lambda: defaultdict(list))
    memory = defaultdict(lambda: defaultdict(list))
    
    # Para calidad, compararemos a nivel de instancia (N, replica)
    # instance_sat[(N, replica)][algo] = sat_value
    instance_sat = defaultdict(dict)
    
    # Buscar todos los archivos de mediciones
    pattern = os.path.join(MEASUREMENTS_DIR, "measurements_testcases_*.txt")
    files = glob.glob(pattern)
    
    if not files:
        print(f"No se encontraron archivos de mediciones en {MEASUREMENTS_DIR}")
        return
        
    for filepath in files:
        # Extraer n y replica desde el nombre del archivo (ej: measurements_testcases_40_2.txt)
        basename = os.path.basename(filepath)
        parts = basename.replace(".txt", "").split("_")
        if len(parts) >= 3:
            try:
                N_val = int(parts[-2])
                replica_val = int(parts[-1])
            except ValueError:
                continue
        else:
            continue
            
        with open(filepath, 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                algo = row['Algorithm']
                skipped = int(row['Skipped'])
                if skipped:
                    continue
                    
                try:
                    sat = int(row['Satisfaction'])
                    time_ms = float(row['Time_ms'])
                    mem_kb = float(row['Memory_KB'])
                except ValueError:
                    continue
                    
                times[algo][N_val].append(time_ms)
                memory[algo][N_val].append(mem_kb / 1024.0) # Convertir a MB
                instance_sat[(N_val, replica_val)][algo] = sat

    # Calcular promedios por algoritmo y N
    algos = list(times.keys())
    
    # 1. Gráfico de Tiempos de Ejecución
    plt.figure(figsize=(10, 6))
    for algo in algos:
        sorted_N = sorted(times[algo].keys())
        avg_times = [sum(times[algo][n]) / len(times[algo][n]) for n in sorted_N]
        plt.plot(sorted_N, avg_times, marker='o', label=algo, linewidth=2)
        
    plt.title("Tiempo de Ejecución Promedio vs. Cantidad de Animes (N)", fontsize=14, fontweight='bold')
    plt.xlabel("Cantidad de Animes (N)", fontsize=12)
    plt.ylabel("Tiempo de Ejecución (ms) [Escala Log]", fontsize=12)
    plt.yscale('log')
    plt.grid(True, which="both", linestyle="--", alpha=0.6)
    plt.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig(os.path.join(PLOTS_DIR, "performance_vs_n.png"), dpi=150)
    plt.close()
    
    # 2. Gráfico de Consumo de Memoria
    plt.figure(figsize=(10, 6))
    for algo in algos:
        sorted_N = sorted(memory[algo].keys())
        avg_mem = [sum(memory[algo][n]) / len(memory[algo][n]) for n in sorted_N]
        plt.plot(sorted_N, avg_mem, marker='s', label=algo, linewidth=2)
        
    plt.title("Consumo de Memoria Promedio vs. Cantidad de Animes (N)", fontsize=14, fontweight='bold')
    plt.xlabel("Cantidad de Animes (N)", fontsize=12)
    plt.ylabel("Consumo de Memoria (MB)", fontsize=12)
    plt.grid(True, which="both", linestyle="--", alpha=0.6)
    plt.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig(os.path.join(PLOTS_DIR, "memory_vs_n.png"), dpi=150)
    plt.close()
    
    # 3. Gráfico de Calidad (Ratio de satisfacción respecto a la solución óptima - DP)
    # Para cada instancia, calculamos ratio = sat_algo / sat_DP.
    # Luego promediamos los ratios por (algo, N).
    quality_ratios = defaultdict(lambda: defaultdict(list))
    
    for (N_val, replica_val), algos_dict in instance_sat.items():
        opt_sat = algos_dict.get('DynamicProgramming')
        if not opt_sat or opt_sat <= 0:
            # Si no hay DP o es 0, buscar BruteForce (en casos pequeños)
            opt_sat = algos_dict.get('BruteForce')
            
        if not opt_sat or opt_sat <= 0:
            continue
            
        for algo, sat in algos_dict.items():
            ratio = sat / opt_sat
            quality_ratios[algo][N_val].append(ratio)
            
    plt.figure(figsize=(10, 6))
    for algo in algos:
        sorted_N = sorted(quality_ratios[algo].keys())
        avg_ratios = [sum(quality_ratios[algo][n]) / len(quality_ratios[algo][n]) for n in sorted_N]
        plt.plot(sorted_N, avg_ratios, marker='^', label=algo, linewidth=2)
        
    plt.title("Calidad de Solución Promedio respecto a la Óptima (Ratio)", fontsize=14, fontweight='bold')
    plt.xlabel("Cantidad de Animes (N)", fontsize=12)
    plt.ylabel("Ratio de Satisfacción (Algoritmo / Óptimo)", fontsize=12)
    plt.ylim(0, 1.1)
    plt.axhline(y=1.0, color='r', linestyle='--', alpha=0.7, label="Línea de Óptimo (1.0)")
    plt.grid(True, which="both", linestyle="--", alpha=0.6)
    plt.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig(os.path.join(PLOTS_DIR, "quality_vs_n.png"), dpi=150)
    plt.close()
    
    print(f"Gráficos generados correctamente en {PLOTS_DIR}")

if __name__ == '__main__':
    main()
