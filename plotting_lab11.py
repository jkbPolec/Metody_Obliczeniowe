# Przeklej w miejsce całej zawartości pliku rysuj_wykresy.py
import numpy as np
import matplotlib.pyplot as plt
import os

plt.style.use('seaborn-v0_8-whitegrid')
SCHEMES = ['Laasonen', 'CN']; SOLVERS = ['Thomas', 'LU']
LABEL_MAP = {'Laasonen': 'Laasonen', 'CN': 'Crank-Nicolson', 'Thomas': 'Thomas', 'LU': 'Dekompozycja LU'}

def generate_individual_error_plot(scheme, solver):
    """
    Generuje osobny wykres błędu w funkcji czasu w SKALI LINIOWEJ,
    dodając adnotację wyjaśniającą.
    """
    filename = f'error_vs_time_{scheme}_{solver}.dat'
    if not os.path.exists(filename): return

    data = np.loadtxt(filename, skiprows=1)
    t, error = data[:, 0], data[:, 1]

    fig, ax = plt.subplots(figsize=(10, 7))
    ax.plot(t, error, label='Max |U_num - U_an|')

    title = f"Zależność maksymalnego błędu od czasu ({LABEL_MAP[scheme]} + {LABEL_MAP[solver]})"
    ax.set_title(title, fontsize=16)
    ax.set_xlabel("Czas t"); ax.set_ylabel("Maksymalny błąd bezwzględny")
    ax.grid(True, which="both", ls="--")

    # Formatowanie osi Y, aby wyświetlała notację naukową, jeśli trzeba
    ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))

    # Dodawanie strzałki i adnotacji, tak jak w przykładzie
    max_idx = np.argmax(error)
    t_max, err_max = t[max_idx], error[max_idx]

    ax.legend(loc='lower right')

    output_filename = f'wykres_3a_blad_indywidualny_{scheme}_{solver}.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight'); plt.close(fig)
    print(f"Zapisano indywidualny wykres błędu (skala liniowa): {output_filename}")


def plot_error_vs_h():
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
    plt.savefig(output_filename, dpi=300, bbox_inches='tight'); plt.close(fig)
    print(f"Zapisano wykres zbieżności: {output_filename}")


def plot_evolution_comparison_all_methods():
    snapshots = {0.0: (1, 2), 0.1: (3, 4), 0.25: (5, 6), 0.5: (7, 8)}
    fig, axes = plt.subplots(2, 2, figsize=(18, 14), sharex=True, sharey=True)
    axes = axes.flatten()
    styles = {'Laasonen_Thomas': {'color': 'blue', 'marker': 'o'}, 'Laasonen_LU': {'color': 'cyan', 'marker': 'x'}, 'CN_Thomas': {'color': 'red', 'marker': 'o'}, 'CN_LU': {'color': 'magenta', 'marker': 'x'}}
    for i, (t, cols) in enumerate(snapshots.items()):
        ax = axes[i]
        any_file = f'solution_snapshots_{SCHEMES[0]}_{SOLVERS[0]}.dat'
        if not os.path.exists(any_file): print("Brak plików danych."); return
        data = np.loadtxt(any_file, skiprows=1)
        ax.plot(data[:, 0], data[:, cols[1]], 'k-', lw=3, label='Analityczne', zorder=10)
        for scheme in SCHEMES:
            for solver in SOLVERS:
                filename = f'solution_snapshots_{scheme}_{solver}.dat'
                if not os.path.exists(filename): continue
                data_num = np.loadtxt(filename, skiprows=1); style_key = f"{scheme}_{solver}"; label = f'{LABEL_MAP[scheme]} + {LABEL_MAP[solver]}'
                ax.plot(data_num[:, 0], data_num[:, cols[0]], 'None', marker=styles[style_key]['marker'], color=styles[style_key]['color'], markersize=5, label=label, alpha=0.8)
        ax.set_title(f'Czas t = {t}', fontsize=14); ax.set_xlabel('x'); ax.set_ylabel('U(x, t)'); ax.grid(True, which="both", ls="--")
    handles, labels = axes[0].get_legend_handles_labels(); fig.legend(handles, labels, loc='lower center', ncol=5, fontsize=12, bbox_to_anchor=(0.5, 0.01))
    fig.suptitle('Porównanie ewolucji rozwiązania dla wszystkich metod', fontsize=20); plt.tight_layout(rect=[0, 0.06, 1, 0.95])
    output_filename = 'wykres_2_porownanie_ewolucji.png'
    plt.savefig(output_filename, dpi=300); plt.close(fig)
    print(f"Zapisano zbiorczy wykres ewolucji: {output_filename}")


def plot_error_vs_time_comparison_all_methods():
    fig, ax = plt.subplots(figsize=(12, 8))
    styles = {'Thomas': '-', 'LU': '--'}; colors = {'Laasonen': 'blue', 'CN': 'red'}
    for scheme in SCHEMES:
        for solver in SOLVERS:
            filename = f'error_vs_time_{scheme}_{solver}.dat'
            if not os.path.exists(filename): continue
            data = np.loadtxt(filename, skiprows=1); t, error = data[:, 0], data[:, 1]
            label = f'{LABEL_MAP[scheme]} + {LABEL_MAP[solver]}'
            ax.plot(t, error, linestyle=styles[solver], color=colors[scheme], label=label)
    ax.set_title('Zbiorcze porównanie błędu w funkcji czasu', fontsize=16)
    ax.set_xlabel('Czas t'); ax.set_ylabel('Maksymalny błąd bezwzględny')
    ax.set_yscale('log'); ax.legend()
    output_filename = 'wykres_3b_porownanie_bledow.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight'); plt.close(fig)
    print(f"Zapisano zbiorczy wykres porównawczy błędów (skala log): {output_filename}")


if __name__ == '__main__':
    print("--- Rozpoczęto generowanie wykresów ---")
    plot_error_vs_h()
    plot_evolution_comparison_all_methods()
    print("\nGenerowanie indywidualnych wykresów błędu w czasie:")
    for scheme in SCHEMES:
        for solver in SOLVERS:
            generate_individual_error_plot(scheme, solver)
    print("\nGenerowanie zbiorczego wykresu błędu w czasie:")
    plot_error_vs_time_comparison_all_methods()
    print("\n--- Zakończono generowanie wykresów ---")