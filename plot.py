import pandas as pd
import matplotlib.pyplot as plt

# Load the data
df = pd.read_csv('build/sim_data.csv')

# RECONSTRUCT TIME: Since 'time' is all 0s in the CSV, 
# we create it manually using the simulation dt (0.01)
dt = 0.01
df['time'] = df.index * dt

fig, axs = plt.subplots(3, 1, figsize=(10, 12), sharex=True)

# Plot X position
axs[0].plot(df['time'].values, df['x'].values, label='Actual X', color='blue')
axs[0].plot(df['time'].values, df['ref_x'].values, 'r--', label='Ref X')
axs[0].set_ylabel('X Position (m)')
axs[0].legend()
axs[0].grid(True)

# Plot Y position
axs[1].plot(df['time'].values, df['y'].values, label='Actual Y', color='green')
axs[1].plot(df['time'].values, df['ref_y'].values, 'r--', label='Ref Y')
axs[1].set_ylabel('Y Position (m)')
axs[1].legend()
axs[1].grid(True)

# Plot Z position
axs[2].plot(df['time'].values, df['z'].values, label='Actual Z', color='purple')
axs[2].plot(df['time'].values, df['ref_z'].values, 'r--', label='Ref Z')
axs[2].set_ylabel('Z Position (m)')
axs[2].set_xlabel('Time (s)')
axs[2].legend()
axs[2].grid(True)

plt.suptitle('Quadcopter NMPC Tracking Performance')
plt.tight_layout()
plt.show()