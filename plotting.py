import numpy as np
import matplotlib.pyplot as plt

# Wczytaj dane z plików
original = np.loadtxt("original.txt")
equi = np.loadtxt("interpol_equi.txt")
cheb = np.loadtxt("interpol_cheb.txt")

# Rysuj
plt.figure(figsize=(10, 6))
plt.plot(original[:, 0], original[:, 1], label="f(x)", linewidth=2)
plt.plot(equi[:, 0], equi[:, 1], label="Lagrange - równoodległe", linestyle='--')
plt.plot(cheb[:, 0], cheb[:, 1], label="Lagrange - Czebyszewa", linestyle=':')

plt.title("Interpolacja Lagrange’a – zjawisko Rungego")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
