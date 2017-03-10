from pylab import *

data = np.load("solution.npz")

figure()
subplot(211)
title("Host")
c = contourf(data["x"], data["y"], data["u"])
subplot(212)
title("Device")
contourf(data["x"], data["y"], data["u_cl"], levels=c.levels)
tight_layout()

diff = np.sum((data["u"] - data["u_cl"])**2)
print "Difference between host and device solution: ", diff
print "Difference between host and device solution: ", hex(diff)

show()
