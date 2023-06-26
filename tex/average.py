import matplotlib.pyplot as plt
x = [10,20,30,40,50]

y1 = [26466.6 , 106451.7 , 214708.8 , 307152 , 492387.8]
y2 = [20042.42 , 105376.3, 198435.2 , 234363.4 , 421743.4]
y3 = [16253.36 , 269393.2 , 667841.2, 1119062 , 1971684]

plt.plot(x,y1,label="TAS")
plt.plot(x,y2,label="CAS")
plt.plot(x,y3,label="CAS_BOUND")

plt.scatter(x,y1)
plt.scatter(x,y2)
plt.scatter(x,y3)

plt.grid()
plt.xlabel("N")
plt.ylabel("$T_{avg}$ in $\mu$ sec")
plt.legend()
plt.show()
