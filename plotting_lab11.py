# Przeklej w miejsce całej zawartości pliku rysuj_wykresy.py
import numpy as np
import matplotlib.pyplot as plt
import os

# Ustawienie estetycznego stylu wykresów
plt.style.use('seaborn-v0_8-whitegrid')

# Definicje kombinacji do testowania i etykiet
SCHEMES = ['Laasonen', 'CN']
SOLVERS = ['Thomas', 'LU']
LABEL_MAP = {
    'Laasonen': 'Laasonen', 'CN': 'Crank-Nicolson',
    'Thomas': 'Thomas', 'LU': 'Dekompozycja LU'
}

def plot_evolution_comparison_all_methods():
    """
    Generuje JEDEN zbiorczy wykres 2x2, porównujący ewolucję rozwiązania
    dla wszystkich czterech metod numerycznych na tle rozwiązania analitycznego.
    """
    # Definicja "zdjęć" w czasie i odpowiadających im kolumn w plikach .dat
    snapshots = {
        0.0: (1, 2),   # (kolumna numeryczna, kolumna analityczna)
        0.1: (3, 4),
        0.25: (5, 6),
        0.5: (7, 8)
    }

    # Przygotowanie siatki wykresów 2x2
    fig, axes = plt.subplots(2, 2, figsize=(18, 14), sharex=True, sharey=True)
    axes = axes.flatten()

    # Definicja stylów, aby wykresy były czytelne
    # Kolor odróżnia schemat czasowy, a marker odróżnia solver liniowy
    styles = {
        'Laasonen_Thomas': {'color': 'blue', 'marker': 'o'},
        'Laasonen_LU':     {'color': 'cyan', 'marker': 'x'},
        'CN_Thomas':       {'color': 'red',  'marker': 'o'},
        'CN_LU':           {'color': 'magenta','marker': 'x'}
    }

    # Pętla po każdym sub-wykresie (każdym punkcie w czasie)
    for i, (t, cols) in enumerate(snapshots.items()):
        ax = axes[i]

        # --- Rysowanie rozwiązania analitycznego (tylko raz na subplot) ---
        # Wystarczy wczytać dowolny plik, bo dane analityczne są wszędzie te same
        any_file = f'solution_snapshots_{SCHEMES[0]}_{SOLVERS[0]}.dat'
        if not os.path.exists(any_file):
            print("Brak plików danych. Uruchom najpierw program C.")
            return

        data = np.loadtxt(any_file, skiprows=1)
        x_coords = data[:, 0]
        analytical_sol = data[:, cols[1]]
        ax.plot(x_coords, analytical_sol, 'k-', lw=3, label='Analityczne', zorder=10)

        # --- Pętla po wszystkich metodach numerycznych ---
        for scheme in SCHEMES:
            for solver in SOLVERS:
                filename = f'solution_snapshots_{scheme}_{solver}.dat'
                if not os.path.exists(filename):
                    continue

                data = np.loadtxt(filename, skiprows=1)
                numerical_sol = data[:, cols[0]]

                style_key = f"{scheme}_{solver}"
                label = f'{LABEL_MAP[scheme]} + {LABEL_MAP[solver]}'

                ax.plot(x_coords, numerical_sol,
                        linestyle='None',  # Nie łączymy punktów linią
                        marker=styles[style_key]['marker'],
                        color=styles[style_key]['color'],
                        markersize=5,
                        label=label,
                        alpha=0.8) # Przezroczystość na wypadek nakładania się

        ax.set_title(f'Czas t = {t}', fontsize=14)
        ax.set_xlabel('x')
        ax.set_ylabel('U(x, t)')
        ax.grid(True, which="both", ls="--")

    # Dodanie jednej, wspólnej legendy pod całym wykresem
    handles, labels = axes[0].get_legend_handles_labels()
    fig.legend(handles, labels, loc='lower center', ncol=5, fontsize=12, bbox_to_anchor=(0.5, 0.01))

    # Tytuł główny i dopasowanie układu
    fig.suptitle('Porównanie ewolucji rozwiązania dla wszystkich metod', fontsize=20)
    plt.tight_layout(rect=[0, 0.06, 1, 0.95]) # Zostaw miejsce na legendę i tytuł

    # Zapis do pliku
    output_filename = 'wykres_2c_porownanie_ewolucji_wszystkich_metod.png'
    plt.savefig(output_filename, dpi=300)
    plt.close(fig)
    print(f"Zapisano zbiorczy wykres porównawczy ewolucji: {output_filename}")


def plot_error_vs_h():
    # Ta funkcja pozostaje bez zmian
    filename = 'error_vs_h.dat'
    if not os.path.exists(filename): return
    data = np.loadtxt(filename, skiprows=1)
    h, err_laasonen, err_cn = data[:, 0], data[:, 1], data[:, 3]
    fig, ax = plt.subplots(figsize=(10, 7))
    ax.plot(h, err_laasonen, 'o-', label='Metoda Laasonen')
    ax.plot(h, err_cn, 's-', label='Metoda Cranka-Nicolson')
    C = err_cn[0] / (h[0]**2)
    h_theory = np.geomspace(h.min(), h.max(), 100)
    ax.plot(h_theory, C * h_theory**2, '--', color='gray', label='Teoretyczny rząd O($h^2$)')
    ax.set_xscale('log'); ax.set_yscale('log')
    ax.set_title('Zależność maksymalnego błędu od kroku h', fontsize=16)
    ax.set_xlabel('Krok h'); ax.set_ylabel('Maksymalny błąd')
    ax.legend(); ax.grid(True, which="both", ls="--")
    output_filename = 'wykres_1_blad_vs_h.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres zbieżności: {output_filename}")


def plot_error_vs_time_comparison_all_methods():
    # Ta funkcja pozostaje bez zmian
    fig, ax = plt.subplots(figsize=(12, 8))
    styles = {'Thomas': '-', 'LU': '--'}; colors = {'Laasonen': 'blue', 'CN': 'red'}
    for scheme in SCHEMES:
        for solver in SOLVERS:
            filename = f'error_vs_time_{scheme}_{solver}.dat'
            if not os.path.exists(filename): continue
            data = np.loadtxt(filename, skiprows=1)
            t, error = data[:, 0], data[:, 1]
            label = f'{LABEL_MAP[scheme]} + {LABEL_MAP[solver]}'
            ax.plot(t, error, linestyle=styles[solver], color=colors[scheme], label=label)
    ax.set_title('Porównanie błędu w funkcji czasu (dla dużej λ)', fontsize=16)
    ax.set_xlabel('Czas t'); ax.set_ylabel('Maksymalny błąd')
    ax.set_yscale('log'); ax.legend()
    output_filename = 'wykres_3b_porownanie_bledow.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres porównawczy błędów: {output_filename}")


if __name__ == '__main__':
    print("--- Rozpoczęto generowanie wykresów ---")
    plot_error_vs_h()
    plot_error_vs_time_comparison_all_methods()
    plot_evolution_comparison_all_methods()
    print("\n--- Zakończono generowanie wykresów ---")