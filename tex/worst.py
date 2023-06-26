import matplotlib.pyplot as plt
x = [10,20,30,40,50]


x1 = [140559.2 , 858354.6 , 1871478 , 2179560 , 4558210]
x2 = [147070 , 926673 , 1618836 , 2192402 , 2778104]
x3 = [47361.4 , 411287.8 , 909224.8 , 1503492 , 2502718]

plt.plot(x,x1,label="TAS")
plt.plot(x,x2,label="CAS")
plt.plot(x,x3,label="CAS_BOUND")

plt.scatter(x,x1)
plt.scatter(x,x2)
plt.scatter(x,x3)

plt.grid()
plt.xlabel("N")
plt.ylabel("$T_{worst}$ in $\mu$ sec")
plt.legend()
plt.show()
