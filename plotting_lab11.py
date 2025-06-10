import numpy as np
import matplotlib.pyplot as plt
import os

# Ustawienie estetycznego stylu wykresów
plt.style.use('seaborn-v0_8-whitegrid')

def plot_error_vs_h():
    """
    Generuje wykres zależności błędu od kroku h (Część 1 zadania).
    Zapisuje go do pliku 'wykres1_blad_vs_h.png'.
    """
    filename = 'error_vs_h.dat'
    if not os.path.exists(filename):
        print(f"Plik {filename} nie został znaleziony! Uruchom najpierw program C.")
        return

    # Wczytanie danych, pomijając linię nagłówka
    data = np.loadtxt(filename, skiprows=1)

    # Kolumny: 0-h, 1-err_laasonen_thomas, 3-err_cn_thomas
    h = data[:, 0]
    err_laasonen = data[:, 1]
    err_cn = data[:, 3]

    # Stworzenie wykresu
    fig, ax = plt.subplots(figsize=(10, 7))

    ax.plot(h, err_laasonen, 'o-', label='Metoda Laasonen')
    ax.plot(h, err_cn, 's-', label='Metoda Cranka-Nicolson')

    # Dodanie linii teoretycznego rzędu O(h^2) dla porównania
    # Dopasowujemy stałą C tak, aby linia przechodziła przez pierwszy punkt C-N
    C = err_cn[0] / (h[0]**2)
    h_theory = np.geomspace(h.min(), h.max(), 100)
    ax.plot(h_theory, C * h_theory**2, '--', color='gray', label='Teoretyczny rząd O($h^2$)')

    # Ustawienia wykresu
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_title('Zależność maksymalnego błędu od kroku przestrzennego h', fontsize=16)
    ax.set_xlabel('Krok przestrzenny h')
    ax.set_ylabel('Maksymalny błąd bezwzględny (t=t_max)')
    ax.legend()
    ax.grid(True, which="both", ls="--")

    # Zapis do pliku
    output_filename = 'wykres1_blad_vs_h.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres: {output_filename}")


def plot_solution_snapshots():
    """
    Generuje wykresy porównujące rozwiązanie numeryczne i analityczne
    w kilku chwilach czasu (Część 2 zadania).
    Zapisuje go do pliku 'wykres2_rozwiazania.png'.
    """
    filename = 'solution_snapshots.dat'
    if not os.path.exists(filename):
        print(f"Plik {filename} nie został znaleziony! Uruchom najpierw program C.")
        return

    data = np.loadtxt(filename, skiprows=1)
    x = data[:, 0]

    # Definicja czasów i odpowiadających im kolumn
    # (kol_num, kol_analityczne)
    snapshots = {
        0.0: (1, 2),
        0.1: (3, 4),
        0.25: (5, 6),
        0.5: (7, 8)
    }

    fig, axes = plt.subplots(2, 2, figsize=(14, 10), sharex=True)
    axes = axes.flatten() # Ułatwia iterację

    for i, (t, cols) in enumerate(snapshots.items()):
        ax = axes[i]
        col_num, col_an = cols

        # Wykres analityczny jako linia ciągła
        ax.plot(x, data[:, col_an], '-', lw=2, label='Analityczne')
        # Wykres numeryczny jako punkty
        ax.plot(x, data[:, col_num], 'o', markersize=4, label='Numeryczne')

        ax.set_title(f'Czas t = {t}')
        ax.set_ylim(0.9, 2.1) # Ustawienie stałego zakresu osi Y dla lepszego porównania
        ax.legend()
        ax.set_xlabel('x')
        ax.set_ylabel('U(x, t)')

    fig.suptitle('Porównanie rozwiązania numerycznego z analitycznym', fontsize=18)
    plt.tight_layout(rect=[0, 0, 1, 0.96]) # Dopasowanie układu, by tytuł nie nachodził

    output_filename = 'wykres2_rozwiazania.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres: {output_filename}")


def plot_error_vs_time():
    """
    Generuje wykres zależności błędu od czasu (Część 3 zadania).
    Zapisuje go do pliku 'wykres3_blad_vs_czas.png'.
    """
    filename = 'error_vs_time.dat'
    if not os.path.exists(filename):
        print(f"Plik {filename} nie został znaleziony! Uruchom najpierw program C.")
        return

    data = np.loadtxt(filename, skiprows=1)
    t = data[:, 0]
    error = data[:, 1]

    fig, ax = plt.subplots(figsize=(10, 7))

    ax.plot(t, error, '-', label='Max |U_num - U_an|')

    ax.set_title('Zależność maksymalnego błędu od czasu', fontsize=16)
    ax.set_xlabel('Czas t')
    ax.set_ylabel('Maksymalny błąd bezwzględny')
    ax.legend()

    # Dodanie adnotacji wyjaśniającej kształt wykresu
    max_idx = np.argmax(error)
    t_max, err_max = t[max_idx], error[max_idx]
    ax.annotate(
        'Błąd rośnie przez kumulację błędów lokalnych,\n'
        'a następnie maleje, gdy rozwiązanie\n'
        'dąży do prostego stanu stacjonarnego.',
        xy=(t_max, err_max),
        xytext=(t_max + 0.1, err_max * 0.8),
        arrowprops=dict(facecolor='black', shrink=0.05, width=1, headwidth=8),
        fontsize=10,
        bbox=dict(boxstyle="round,pad=0.3", fc="wheat", ec="black", lw=1, alpha=0.8)
    )

    output_filename = 'wykres3_blad_vs_czas.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Zapisano wykres: {output_filename}")


if __name__ == '__main__':
    print("--- Rozpoczęto generowanie wykresów ---")
    plot_error_vs_h()
    plot_solution_snapshots()
    plot_error_vs_time()
    print("--- Zakończono generowanie wykresów ---")