# Przeklej w miejsce całej zawartości pliku rysuj_wykresy.py
import numpy as np
import matplotlib.pyplot as plt
import os

plt.style.use('seaborn-v0_8-whitegrid')

SCHEMES = ['Laasonen', 'CN']
SOLVERS = ['Thomas', 'LU']
LABEL_MAP = {
    'Laasonen': 'Laasonen', 'CN': 'Crank-Nicolson',
    'Thomas': 'Thomas', 'LU': 'Dekompozycja LU'
}

def generate_evolution_plot(scheme, solver):
    """
    Generuje wykres 2x2 pokazujący ewolucję rozwiązania w czasie
    dla podanej kombinacji metody i solvera.
    """
    filename = f'solution_snapshots_{scheme}_{solver}.dat'
    if not os.path.exists(filename):
        print(f"Brak pliku {filename}, pomijam generowanie wykresu ewolucji.")
        return

    data = np.loadtxt(filename, skiprows=1)
    x = data[:, 0]
    snapshots = {0.0: (1, 2), 0.1: (3, 4), 0.25: (5, 6), 0.5: (7, 8)}

    fig, axes = plt.subplots(2, 2, figsize=(14, 10), sharex=True)
    axes = axes.flatten()

    for i, (t, cols) in enumerate(snapshots.items()):
        ax = axes[i]
        col_num, col_an = cols

        ax.plot(x, data[:, col_an], '-', lw=2, label='Analityczne')
        ax.plot(x, data[:, col_num], 'o', markersize=3, label='Numeryczne', alpha=0.8)

        ax.set_title(f'Czas t = {t}')
        ax.set_ylim(0.8, 2.1) # Lekko poszerzony zakres
        ax.legend()
        ax.set_xlabel('x'); ax.set_ylabel('U(x, t)')

    title = f'Ewolucja rozwiązania (Metoda: {LABEL_MAP[scheme]} + {LABEL_MAP[solver]})'
    fig.suptitle(title, fontsize=18)
    plt.tight_layout(rect=[0, 0, 1, 0.96])

    output_filename = f'wykres_2a_ewolucja_{scheme}_{solver}.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres ewolucji: {output_filename}")


def plot_error_vs_h():
    """
    (WYKRES 1) Generuje wykres zależności błędu od kroku h.
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
    print(f"Zapisano wykres zbieżności: {output_filename}")


def plot_error_vs_time_comparison_all_methods():
    """
    (WYKRES PORÓWNAWCZY) Porównuje ewolucję błędu w czasie dla wszystkich metod.
    """
    fig, ax = plt.subplots(figsize=(12, 8))
    styles = {'Thomas': '-', 'LU': '--'}
    colors = {'Laasonen': 'blue', 'CN': 'red'}

    for scheme in SCHEMES:
        for solver in SOLVERS:
            filename = f'error_vs_time_{scheme}_{solver}.dat'
            if not os.path.exists(filename): continue
            data = np.loadtxt(filename, skiprows=1)
            t, error = data[:, 0], data[:, 1]
            label = f'{LABEL_MAP[scheme]} + {LABEL_MAP[solver]}'
            ax.plot(t, error, linestyle=styles[solver], color=colors[scheme], label=label)

    ax.set_title('Porównanie maksymalnego błędu w funkcji czasu (dla dużej siatki i λ)', fontsize=16)
    ax.set_xlabel('Czas t'); ax.set_ylabel('Maksymalny błąd bezwzględny'); ax.legend()
    # Użyjemy skali logarytmicznej na osi Y, aby lepiej widzieć różnice
    ax.set_yscale('log')

    output_filename = 'wykres_3b_porownanie_bledow.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres porównawczy błędów: {output_filename}")


if __name__ == '__main__':
    print("--- Rozpoczęto generowanie wykresów ---")

    # Wykres 1 - Zbieżność (niezmieniony)
    plot_error_vs_h()

    # Wykres porównawczy błędów
    plot_error_vs_time_comparison_all_methods()

    # Wykresy ewolucji - teraz generowane dla każdej kombinacji
    print("\nGenerowanie wykresów ewolucji dla każdej metody:")
    for scheme in SCHEMES:
        for solver in SOLVERS:
            generate_evolution_plot(scheme, solver)

    print("\n--- Zakończono generowanie wykresów ---")