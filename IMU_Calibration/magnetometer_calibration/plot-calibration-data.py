"""

PLOT UNCALIBRTED AND CALIBRATED MAGNETOMETER MEASUREMENTS

Read data file with raw measurements and compare calibrated vs. uncalibrated
measurements.

Code By: Michael Wrona
Created: 14 Jan 2021

The calibration equation is:
    h_calib = A * (h_meas - b)

h_calib: Calibrated measurements (3x1)
A: Soft iron, scale factor, and misalignment correction matrix (3x3, symmetric)
h_meas: Raw measurements (3x1)
b: Hard-iron offset correction vector (3x1)

Calibration parametrers were determined by the Magneto calibration software (see below).

Resources
---------

Magnetometer/IMU I used:
    https://www.adafruit.com/product/3463

Magneto magnetometer calibration software download:
    https://sites.google.com/site/sailboatinstruments1/home

"""


import numpy as np
import matplotlib.pyplot as plt


# Define calibration parameters
b = np.array([18.714979, -3.108033, -29.281154])
A = np.array([[1.934501, 0.165994, -0.058816],  # 'A^-1' matrix from Magneto
              [0.165994, 1.717464, 0.148240],
              [-0.058816, 0.148240, 1.921080]])


# Read raw data and apply calibration
rawData = np.genfromtxt('magnetometer_calibration/mag-readings.txt', delimiter='\t')  # Read raw measurements

# Choose the number of points to skip (e.g., 9 out of 10 points)
skip_points = 10

N = len(rawData)
calibData = np.zeros((N // skip_points, 3), dtype='float')
for i in range(0, N, skip_points):
    currMeas = np.array([rawData[i, 0], rawData[i, 1], rawData[i, 2]])
    calibData[i // skip_points-1, :] = A @ (currMeas - b)

# Plot XY data
plt.figure()
plt.plot(rawData[::skip_points, 0], rawData[::skip_points, 1], 'b*', label='Raw Meas.')
plt.plot(calibData[:, 0], calibData[:, 1], 'r*', label='Calibrated Meas.')
plt.title('XY Magnetometer Data')
plt.xlabel('X [uT]')
plt.ylabel('Y [uT]')
plt.legend()
plt.grid()
plt.axis('equal')

# Plot YZ data
plt.figure()
plt.plot(rawData[::skip_points, 1], rawData[::skip_points, 2], 'b*', label='Raw Meas.')
plt.plot(calibData[:, 1], calibData[:, 2], 'r*', label='Calibrated Meas.')
plt.title('YZ Magnetometer Data')
plt.xlabel('Y [uT]')
plt.ylabel('Z [uT]')
plt.legend()
plt.grid()
plt.axis('equal')

# Plot XZ data
plt.figure()
plt.plot(rawData[::skip_points, 0], rawData[::skip_points, 2], 'b*', label='Raw Meas.')
plt.plot(calibData[:, 0], calibData[:, 2], 'r*', label='Calibrated Meas.')
plt.title('XZ Magnetometer Data')
plt.xlabel('X [uT]')
plt.ylabel('Z [uT]')
plt.legend()
plt.grid()
plt.axis('equal')

# Plot 3D scatter
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i in range(0, N, skip_points):
    xraw = rawData[i, 0]
    yraw = rawData[i, 1]
    zraw = rawData[i, 2]

    xcalib = calibData[i // skip_points-1, 0]
    ycalib = calibData[i // skip_points-1, 1]
    zcalib = calibData[i // skip_points-1, 2]
    ax.scatter(xraw, yraw, zraw, color='r')
    ax.scatter(xcalib, ycalib, zcalib, color='b')

ax.set_title('3D Scatter Plot of Magnetometer Data')
ax.set_xlabel('X [uT]')
ax.set_ylabel('Y [uT]')
ax.set_zlabel('Z [uT]')

plt.show()
