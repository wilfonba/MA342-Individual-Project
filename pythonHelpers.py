import matplotlib.pyplot as plt
import numpy as np

def plotBoundary(alpha,beta,h,r):
    fig,ax = plt.subplots()
    beta = np.pi/180*beta
    alpha = np.pi/180*alpha
    ax.plot([-r/2,-r/2-np.cos(alpha)],[0,h],'-',color='blue')
    ax.plot([r/2,r/2+np.cos(beta)],[0,h],'-',color='blue')
    ax.plot([-r/2,r/2],[0,0],'--',color='black')
    ax.plot([-r/2-np.cos(alpha),r/2+np.cos(beta)],[h,h],'--',color='black')
    ax.set(xlabel='x (m)', ylabel='y (m)',
    title='Boundary')
    ax.set_aspect('equal','box')

    plt.show()