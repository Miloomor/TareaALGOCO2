#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h>

using namespace std;

// Estructura para almacenar los resultados de un algoritmo
struct AlgResult {
    string name;
    long long satisfaction;
    double time_ms;
    long memory_kb;
    bool skipped;
};

// Función para ejecutar un algoritmo binario en un proceso hijo
AlgResult run_algorithm(const string& algo_name, const string& binary_path, const string& input_file_path) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        cerr << "Error creando pipe para " << algo_name << endl;
        return {algo_name, -1, -1.0, -1, false};
    }

    auto start_time = chrono::high_resolution_clock::now();
    pid_t pid = fork();

    if (pid == -1) {
        cerr << "Error en fork para " << algo_name << endl;
        close(pipefd[0]);
        close(pipefd[1]);
        return {algo_name, -1, -1.0, -1, false};
    }

    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]); // Cerrar lectura del pipe
        
        // Redirigir stdout al pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            exit(1);
        }
        close(pipefd[1]);

        // Redirigir stdin desde el archivo de entrada
        int infd = open(input_file_path.c_str(), O_RDONLY);
        if (infd == -1) {
            perror("open input_file");
            exit(1);
        }
        if (dup2(infd, STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            exit(1);
        }
        close(infd);

        // Ejecutar el binario
        execl(binary_path.c_str(), binary_path.c_str(), nullptr);
        // Si execl retorna, ocurrió un error
        perror("execl");
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]); // Cerrar escritura del pipe

        // Leer la salida del hijo (satisfacción)
        string output;
        char buf[256];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buf, sizeof(buf) - 1)) > 0) {
            buf[bytes_read] = '\0';
            output += buf;
        }
        close(pipefd[0]);

        int status;
        struct rusage usage;
        // wait4 obtiene el uso de recursos del proceso hijo específico
        if (wait4(pid, &status, 0, &usage) == -1) {
            cerr << "Error en wait4 para " << algo_name << endl;
        }
        auto end_time = chrono::high_resolution_clock::now();
        double elapsed_ms = chrono::duration<double, milli>(end_time - start_time).count();

        long long satisfaction = -1;
        try {
            if (!output.empty()) {
                satisfaction = stoll(output);
            }
        } catch (...) {
            satisfaction = -1;
        }

        long memory_kb = usage.ru_maxrss; // En Linux, ru_maxrss está en KB

        return {algo_name, satisfaction, elapsed_ms, memory_kb, false};
    }
}

int main(int argc, char* argv[]) {
    // Asegurar que existan los directorios
    int dir_status = system("mkdir -p data/inputs data/outputs data/measurements data/plots");
    (void)dir_status;

    string input_file_path;
    bool is_temp = false;

    if (argc > 1) {
        input_file_path = argv[1];
    } else {
        // Si no hay argumento, leer de stdin y guardar en un archivo temporal
        input_file_path = "data/inputs/temp_input.txt";
        ofstream temp_out(input_file_path);
        if (!temp_out.is_open()) {
            cerr << "Error al crear archivo temporal para stdin" << endl;
            return 1;
        }
        string line;
        while (getline(cin, line)) {
            temp_out << line << "\n";
        }
        temp_out.close();
        is_temp = true;
    }

    // Intentar abrir el archivo de entrada para leer los parámetros básicos (N, M, E)
    ifstream infile(input_file_path);
    if (!infile.is_open()) {
        cerr << "Error al abrir el archivo de entrada: " << input_file_path << endl;
        return 1;
    }

    int n = 0, M = 0, E = 0;
    if (!(infile >> n >> M >> E)) {
        cerr << "Error al leer N, M, E del archivo de entrada" << endl;
        return 1;
    }
    infile.close();

    // Extraer el nombre base del archivo de entrada
    string base_name = "stdin";
    if (!is_temp) {
        size_t last_slash = input_file_path.find_last_of("/\\");
        string filename = (last_slash == string::npos) ? input_file_path : input_file_path.substr(last_slash + 1);
        size_t last_dot = filename.find_last_of('.');
        base_name = (last_dot == string::npos) ? filename : filename.substr(0, last_dot);
    }

    // Ejecutar algoritmos secuencialmente
    vector<AlgResult> results;

    // 1. Fuerza Bruta (sólo si n <= 10)
    if (n <= 10) {
        cerr << "Ejecutando Fuerza Bruta para " << base_name << "..." << endl;
        results.push_back(run_algorithm("BruteForce", "./brute-force", input_file_path));
    } else {
        cerr << "Saltando Fuerza Bruta para " << base_name << " (n = " << n << " > 10)" << endl;
        results.push_back({"BruteForce", -1, -1.0, -1, true});
    }

    // 2. Greedy 1
    cerr << "Ejecutando Greedy 1 para " << base_name << "..." << endl;
    results.push_back(run_algorithm("Greedy1", "./greedy1", input_file_path));

    // 3. Greedy 2
    cerr << "Ejecutando Greedy 2 para " << base_name << "..." << endl;
    results.push_back(run_algorithm("Greedy2", "./greedy2", input_file_path));

    // 4. Programación Dinámica
    cerr << "Ejecutando Programación Dinámica para " << base_name << "..." << endl;
    results.push_back(run_algorithm("DynamicProgramming", "./dynamic-programming", input_file_path));

    // Escribir archivo de salida (resultados de satisfacción)
    string output_file_path = "data/outputs/output_" + base_name + ".txt";
    ofstream outfile_results(output_file_path);
    if (outfile_results.is_open()) {
        outfile_results << "Resultados de Satisfaccion para " << base_name << " (n=" << n << ", M=" << M << ", E=" << E << "):\n";
        for (const auto& r : results) {
            outfile_results << r.name << ": ";
            if (r.skipped) {
                outfile_results << "N/A\n";
            } else if (r.satisfaction == -1) {
                outfile_results << "Error/Fail\n";
            } else {
                outfile_results << r.satisfaction << "\n";
            }
        }
        outfile_results.close();
    } else {
        cerr << "Error al abrir archivo de salida para guardar resultados." << endl;
    }

    // Escribir archivo de mediciones (CSV-like)
    string measure_file_path = "data/measurements/measurements_" + base_name + ".txt";
    ofstream outfile_measure(measure_file_path);
    if (outfile_measure.is_open()) {
        outfile_measure << "Algorithm,N,M,E,Satisfaction,Time_ms,Memory_KB,Skipped\n";
        for (const auto& r : results) {
            outfile_measure << r.name << ","
                            << n << ","
                            << M << ","
                            << E << ",";
            if (r.skipped) {
                outfile_measure << "N/A,N/A,N/A,1\n";
            } else {
                outfile_measure << r.satisfaction << ","
                                << fixed << setprecision(4) << r.time_ms << ","
                                << r.memory_kb << ",0\n";
            }
        }
        outfile_measure.close();
    } else {
        cerr << "Error al abrir archivo de mediciones." << endl;
    }

    // Si usamos un archivo temporal para stdin, lo removemos
    if (is_temp) {
        remove(input_file_path.c_str());
    }

    cerr << "Procesamiento completado para " << base_name << endl;
    return 0;
}
