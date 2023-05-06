import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec

pitch_prog = np.loadtxt('pitch_programa.f0')
pitch_wave = np.loadtxt('pitch_wave.f0')

# Create 2x2 sub plots
gs = gridspec.GridSpec(2, 2)

plt.figure()
ax = plt.subplot(gs[0, 0]) # row 0, col 0
plt.plot(pitch_prog, '8', c='blue', markersize = 1)
plt.title('Pitch sistema própio', fontweight = 'bold')
plt.xlabel('s', fontsize = 10)
plt.ylabel('Hz', fontsize = 10)

ax = plt.subplot(gs[0, 1]) # row 0, col 1
plt.plot(pitch_wave, 'D', c='red', markersize = 1)
plt.title('Pitch WaveSurfer', fontweight = 'bold')
plt.xlabel('s', fontsize = 10)
plt.ylabel('Hz', fontsize = 10)

ax = plt.subplot(gs[1, :]) # row 1, span all columns
plt.plot(pitch_prog, '8', c='blue', markersize = 1)
plt.plot(pitch_wave, 'D', c='red', markersize = 1)
plt.title('Comparación del pitch', fontweight = 'bold')
plt.xlabel('s', fontsize = 10)
plt.ylabel('Hz', fontsize = 10)

plt.tight_layout()
plt.show()