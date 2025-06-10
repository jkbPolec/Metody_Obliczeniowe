# Przeklej w miejsce całej zawartości pliku rysuj_wykresy.py
import numpy as np
import matplotlib.pyplot as plt
import os

# Ustawienie estetycznego stylu wykresów
plt.style.use('seaborn-v0_8-whitegrid')

# Definicje kombinacji do testowania
SCHEMES = ['Laasonen', 'CN']
SOLVERS = ['Thomas', 'LU']

# Słownik dla ładniejszych etykiet na wykresach
LABEL_MAP = {
    'Laasonen': 'Laasonen',
    'CN': 'Crank-Nicolson',
    'Thomas': 'Thomas',
    'LU': 'Dekompozycja LU'
}

# --- WYKRESY Z PIERWOTNEJ WERSJI (DLA NAJLEPSZEJ METODY) ---

def plot_solution_snapshots_single_method():
    """
    (PIERWOTNY WYKRES 2)
    Generuje wykres 2x2 pokazujący ewolucję rozwiązania w czasie
    dla jednej, wybranej metody (Crank-Nicolson + Thomas).
    """
    # Wybieramy najlepszą metodę (dokładna i szybka)
    scheme, solver = 'CN', 'Thomas'
    filename = f'solution_snapshots_{scheme}_{solver}.dat'

    if not os.path.exists(filename):
        print(f"Plik {filename} nie został znaleziony! Uruchom najpierw program C.")
        return

    data = np.loadtxt(filename, skiprows=1)
    x = data[:, 0]

    snapshots = { 0.0: (1, 2), 0.1: (3, 4), 0.25: (5, 6), 0.5: (7, 8) }

    fig, axes = plt.subplots(2, 2, figsize=(14, 10), sharex=True)
    axes = axes.flatten()

    for i, (t, cols) in enumerate(snapshots.items()):
        ax = axes[i]
        col_num, col_an = cols

        ax.plot(x, data[:, col_an], '-', lw=2, label='Analityczne')
        ax.plot(x, data[:, col_num], 'o', markersize=4, label='Numeryczne')

        ax.set_title(f'Czas t = {t}')
        ax.set_ylim(0.9, 2.1)
        ax.legend()
        ax.set_xlabel('x')
        ax.set_ylabel('U(x, t)')

    fig.suptitle(f'Ewolucja rozwiązania (Metoda: {LABEL_MAP[scheme]} + {LABEL_MAP[solver]})', fontsize=18)
    plt.tight_layout(rect=[0, 0, 1, 0.96])

    output_filename = 'wykres_2a_rozwiazanie_w_czasie.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres (stary typ): {output_filename}")

def plot_error_vs_time_single_method():
    """
    (PIERWOTNY WYKRES 3)
    Generuje wykres błędu w czasie dla jednej, wybranej metody.
    """
    scheme, solver = 'CN', 'Thomas'
    filename = f'error_vs_time_{scheme}_{solver}.dat'
    if not os.path.exists(filename):
        print(f"Plik {filename} nie został znaleziony!")
        return

    data = np.loadtxt(filename, skiprows=1)
    t, error = data[:, 0], data[:, 1]

    fig, ax = plt.subplots(figsize=(10, 7))
    ax.plot(t, error, '-')

    ax.set_title(f'Zależność maksymalnego błędu od czasu (Metoda: {LABEL_MAP[scheme]} + {LABEL_MAP[solver]})', fontsize=16)
    ax.set_xlabel('Czas t')
    ax.set_ylabel('Maksymalny błąd bezwzględny')

    output_filename = 'wykres_3a_blad_w_czasie.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres (stary typ): {output_filename}")


# --- NOWE WYKRESY PORÓWNAWCZE ---

def plot_error_vs_h():
    """
    (WYKRES 1)
    Generuje wykres zależności błędu od kroku h (bez zmian).
    """
    filename = 'error_vs_h.dat'
    if not os.path.exists(filename):
        print(f"Plik {filename} nie został znaleziony!")
        return

    data = np.loadtxt(filename, skiprows=1)
    h, err_laasonen, err_cn = data[:, 0], data[:, 1], data[:, 3]

    fig, ax = plt.subplots(figsize=(10, 7))
    ax.plot(h, err_laasonen, 'o-', label='Metoda Laasonen')
    ax.plot(h, err_cn, 's-', label='Metoda Cranka-Nicolson')

    C = err_cn[0] / (h[0]**2)
    h_theory = np.geomspace(h.min(), h.max(), 100)
    ax.plot(h_theory, C * h_theory**2, '--', color='gray', label='Teoretyczny rząd O($h^2$)')

    ax.set_xscale('log'); ax.set_yscale('log')
    ax.set_title('Zależność maksymalnego błędu od kroku przestrzennego h', fontsize=16)
    ax.set_xlabel('Krok przestrzenny h'); ax.set_ylabel('Maksymalny błąd bezwzględny (t=t_max)')
    ax.legend(); ax.grid(True, which="both", ls="--")

    output_filename = 'wykres_1_blad_vs_h.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres: {output_filename}")

def plot_solution_comparison_all_methods():
    """
    (NOWY WYKRES 2)
    Porównuje rozwiązania numeryczne wszystkich metod z analitycznym
    w wybranej chwili czasu.
    """
    fig, ax = plt.subplots(figsize=(12, 8))

    t_snapshot = 0.25; col_num, col_an = 5, 6

    first_file = True
    for scheme in SCHEMES:
        for solver in SOLVERS:
            filename = f'solution_snapshots_{scheme}_{solver}.dat'
            if not os.path.exists(filename): continue

            data = np.loadtxt(filename, skiprows=1)
            if first_file:
                ax.plot(data[:, 0], data[:, col_an], 'k-', lw=3, label='Analityczne', zorder=10)
                first_file = False

            label = f'{LABEL_MAP[scheme]} + {LABEL_MAP[solver]}'
            ax.plot(data[:, 0], data[:, col_num], 'o', markersize=4, label=label, alpha=0.8)

    ax.set_title(f'Porównanie rozwiązań wszystkich metod w chwili t = {t_snapshot}', fontsize=16)
    ax.set_xlabel('x'); ax.set_ylabel('U(x, t)'); ax.legend()

    output_filename = 'wykres_2b_porownanie_rozwiazan.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres (porównawczy): {output_filename}")

def plot_error_vs_time_comparison_all_methods():
    """
    (NOWY WYKRES 3)
    Porównuje ewolucję błędu w czasie dla wszystkich metod.
    """
    fig, ax = plt.subplots(figsize=(12, 8))

    styles = {'Laasonen_Thomas': '-', 'Laasonen_LU': '--', 'CN_Thomas': '-', 'CN_LU': '--'}
    colors = {'Laasonen': 'blue', 'CN': 'red'}

    for scheme in SCHEMES:
        for solver in SOLVERS:
            filename = f'error_vs_time_{scheme}_{solver}.dat'
            if not os.path.exists(filename): continue

            data = np.loadtxt(filename, skiprows=1)
            t, error = data[:, 0], data[:, 1]

            label = f'{LABEL_MAP[scheme]} + {LABEL_MAP[solver]}'
            style_key = f"{scheme}_{solver}"
            ax.plot(t, error, linestyle=styles[style_key], color=colors[scheme], label=label)

    ax.set_title('Porównanie maksymalnego błędu w funkcji czasu', fontsize=16)
    ax.set_xlabel('Czas t'); ax.set_ylabel('Maksymalny błąd bezwzględny'); ax.legend()

    output_filename = 'wykres_3b_porownanie_bledow.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres (porównawczy): {output_filename}")

if __name__ == '__main__':
    print("--- Rozpoczęto generowanie wykresów ---")

    # Wykres 1 - Zbieżność
    plot_error_vs_h()

    # "Stare" wykresy dla najlepszej metody
    plot_solution_snapshots_single_method()
    plot_error_vs_time_single_method()

    # "Nowe" wykresy porównawcze
    plot_solution_comparison_all_methods()
    plot_error_vs_time_comparison_all_methods()

    print("--- Zakończono generowanie wykresów ---")