import os
import math
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import mpl_toolkits.axisartist.floating_axes as floating_axes
from matplotlib.projections import PolarAxes
from mpl_toolkits.axisartist.grid_finder import FixedLocator, MaxNLocator, DictFormatter
import random

matplotlib.rc('font', family='sans-serif') 
matplotlib.rc('font', serif='Helvetica') 

# ------------------------------------ #

def setup_arc_radial_axes(fig, rect, angle_ticks, radius_ticks, min_rad, max_rad):

    tr = PolarAxes.PolarTransform()

    pi = np.pi

    grid_locator1 = FixedLocator([v for v, s in angle_ticks])
    tick_formatter1 = DictFormatter(dict(angle_ticks))

    grid_locator2 = FixedLocator([a for a, b in radius_ticks])
    tick_formatter2 = DictFormatter(dict(radius_ticks))

    grid_helper = floating_axes.GridHelperCurveLinear(tr,
                                extremes=((390.0*(pi/180.0)), (60.0*(pi/180.0)), max_rad, min_rad),
                                grid_locator1=grid_locator1,
                                grid_locator2=grid_locator2,
                                tick_formatter1=tick_formatter1,
                                tick_formatter2=tick_formatter2,
                                )

    ax1 = floating_axes.FloatingSubplot(fig, rect, grid_helper=grid_helper)
    fig.add_subplot(ax1)

    ax1.grid(True)

    # create a parasite axes whose transData in RA, cz
    aux_ax = ax1.get_aux_axes(tr)

    aux_ax.patch = ax1.patch
    ax1.patch.zorder=0.9

    #ax1.axis["left"].set_ticklabel_direction("+")

    return ax1, aux_ax

# ------------------------------------ #
# write angle values to the plotting array
angles = []
for mic_num in range(38):
    angle = float(mic_num)*(180.0/36.0)*(math.pi/180.0)
    angles.append(angle)

# ------------------------------------ #
### these are merely the ticks that appear on the plot axis
### these don't actually get plotted

angle_ticks = range(0,390,10)
angle_ticks_rads = [(a%360)*math.pi/180.0 for a in angle_ticks]
angle_ticks_for_plot = []
for i in range(len(angle_ticks)):
    angle_ticks_for_plot.append((angle_ticks_rads[i],r"$"+str(angle_ticks[i])+"$"))

# ------------------------------------ #

scale = 1.
aspect = 1.50
height = 8.0
fig = plt.figure(1, figsize=(height*aspect*scale, height*scale))
fig.subplots_adjust(wspace=0.3, left=0.05, right=0.95, top=0.84)
fig.subplots_adjust()

plot_real_min = 80.0
plot_real_max = 180.0

plot_fake_min = 0.0
plot_fake_max = 1000.0

rad_tick_increment = 500.0

radius_ticks = []
for i in range(int(plot_fake_min),int(plot_fake_max)+int(rad_tick_increment),int(rad_tick_increment)):
    plot_fake_val = ((i-plot_fake_min)/(plot_fake_max-plot_fake_min))*(plot_real_max-plot_real_min)+plot_real_min
    radius_ticks.append((plot_fake_val, r"$"+str(i)+"$"))

ax2, aux_ax2 = setup_arc_radial_axes(fig, 111, angle_ticks_for_plot, radius_ticks, plot_real_min, plot_real_max)

values = np.loadtxt("../mat/pi_activity.mat", usecols=xrange(1, 1000, 10)) 
azimuths = np.radians(np.linspace(0, 360, 18))
#azimuths_adjusted = [ (x + math.pi) for x in azimuths ]
zeniths = np.arange(0, 1000, 10)
zeniths_adjusted = [((x-plot_fake_min)/(plot_fake_max-plot_fake_min))*(plot_real_max-plot_real_min)+plot_real_min for x in zeniths]
#values = 90.0+5.0*np.random.random((len(azimuths), len(zeniths))) #
print values.shape
r, theta = np.meshgrid(zeniths_adjusted, azimuths)


ctf = aux_ax2.contourf(theta, r, values, cmap='hot', levels=np.linspace(values.min(),values.max(),50))

cbar = plt.colorbar(ctf, orientation='vertical')
#cbar = plt.colorbar(aux_ax2.contourf(theta, r, values), orientation='vertical')
#cbar.ax.set_ylabel('Contour Value [Unit]', fontsize = 16)

plt.legend(loc=3,prop={'size':20})
plt.xlabel('Angle [$^\circ$]', fontsize=20, labelpad=2000)
plt.ylabel('Time [s]', fontsize=20)

# plt.show()
plt.savefig('../../figs/circ.eps', dpi=90)
plt.close()
