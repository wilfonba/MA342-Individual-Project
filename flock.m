classdef flock
    %UNTITLED3 Summary of this class goes here
    %   Detailed explanation goes here

    properties
        boids;
        axisLims;
        N;
    end

    methods
        function obj = flock(boids,axisLims,N)
            %UNTITLED3 Construct an instance of this class
            %   Detailed explanation goes here
            obj.boids = boids;
            obj.axisLims = axisLims;
            obj.N = N;
        end

        function plotFlock(obj)
            %METHOD1 Summary of this method goes here
            %   Detailed explanation goes here
            for i = 1:obj.N
                obj.boids(i).plot;
            end
            drawnow;
        end
    end
end