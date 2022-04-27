import matplotlib.pyplot as plt
import numpy as np

def plotBoundary(alpha,beta,h,r):
    fig,ax = plt.subplots()
    beta = np.pi/180*beta
    alpha = np.pi/180*alpha
    ax.plot([0,-np.cos(alpha)],[0,h],'-',color='blue')
    ax.plot([r,r+np.cos(beta)],[0,h],'-',color='blue')
    ax.plot([0,r],[0,0],'--',color='black')
    ax.plot([-np.cos(alpha),r+np.cos(beta)],[h,h],'--',color='black')
    ax.set(xlabel='x (m)', ylabel='y (m)',
    title='Boundary')
    ax.set_aspect('equal','box')
    
    plt.show()
