clc;clear;close all;

filename = "04-27-2022_23-09-34";
bSize = 0.125;
axisLimits = [-5 55 -5 55];

data = csvread(strcat(filename,".csv"));

N = size(find(data(:,1) == 0),1);
nSteps = size(data,1)/N;

filename = strcat(filename,".avi");
writerObj = VideoWriter(filename,'Motion JPEG AVI');
open(writerObj);

fig = figure("position",[50 50 800 800]);
hold on;axis equal;axis(axisLimits);
fig.Visible = "off";

k = 1;
boids(N) = boid();
f = waitbar(0,"progress");
for i = 1:nSteps
    cla;
    for j = 1:N
        boids(j) = boid(data(k,3),data(k,4),data(k,5),data(k,6),bSize);
        k = k + 1;
    end
    FLOCK = flock(boids,axisLimits,N);
    FLOCK.plotFlock();
    writeVideo(writerObj,getframe(gcf));
    waitbar(i/nSteps,f,"progress");
end

close(writerObj);
close(f);