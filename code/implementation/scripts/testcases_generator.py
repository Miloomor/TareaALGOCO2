import os
import random

# Definir directorios
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
INPUTS_DIR = os.path.join(SCRIPT_DIR, '..', 'data', 'inputs')
os.makedirs(INPUTS_DIR, exist_ok=True)

# Listas de nombres de animes para variedad en los casos de prueba
ANIME_NAMES = [
    "shonen_quest", "romcom_days", "mecha_nova", "slice_cafe", "cyber_clash",
    "fantasy_tail", "isekai_reborn", "detective_x", "sports_spirit", "magic_academy",
    "demon_slayer_v2", "titan_fall", "ninja_path", "death_game", "neon_drive",
    "steampunk_sky", "chrono_trigger", "beast_stars", "music_resonance", "hero_academia",
    "space_odyssey", "abyss_depth", "pixel_world", "ghost_hunt", "vampire_lord",
    "ancient_lore", "high_school_star", "cooking_warrior", "idol_dream", "samurai_edge"
]

def generate_case(n, i):
    animes_data = []
    total_t = 0
    total_c = 0
    
    for idx in range(n):
        name_base = ANIME_NAMES[idx % len(ANIME_NAMES)]
        if idx >= len(ANIME_NAMES):
            name = f"{name_base}_{idx}"
        else:
            name = name_base
            
        q = random.randint(2, 10)  # Entre 2 y 10 episodios por anime
        bono = random.randint(15, 80) # Bono de completación
        
        episodes = []
        for _ in range(q):
            t = random.randint(20, 50)  # Duración
            c = random.randint(5, 25)   # Energía
            v = random.randint(15, 100) # Satisfacción
            episodes.append((t, c, v))
            total_t += t
            total_c += c
            
        animes_data.append((name, q, bono, episodes))
    
    # Restricciones M y E apretadas (entre el 35% y el 65% del total necesario)
    M = int(total_t * random.uniform(0.35, 0.65))
    E = int(total_c * random.uniform(0.35, 0.65))
    
    # Escribir el caso de prueba
    filepath = os.path.join(INPUTS_DIR, f"testcases_{n}_{i}.txt")
    with open(filepath, 'w') as f:
        f.write(f"{n} {M} {E}\n")
        for name, q, bono, eps in animes_data:
            f.write(f"{name} {q} {bono}\n")
            for t, c, v in eps:
                f.write(f"{t} {c} {v}\n")
                
    print(f"Generado: {filepath} (n={n}, M={M}, E={E})")

def main():
    random.seed(42)  # Para repetibilidad de los experimentos
    
    # Generar casos pequeños (n = 3, 5, 8)
    for n in [3, 5, 8]:
        for i in range(1, 4):
            generate_case(n, i)
            
    # Generar casos medianos (n = 20, 40, 80)
    for n in [20, 40, 80]:
        for i in range(1, 4):
            generate_case(n, i)
            
    # Generar casos grandes (n = 100, 150, 200)
    for n in [100, 150, 200]:
        for i in range(1, 4):
            generate_case(n, i)

if __name__ == '__main__':
    main()