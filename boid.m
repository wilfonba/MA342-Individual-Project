classdef boid
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here

    properties
        pX;
        pY;
        vX;
        vY;
        size;
    end

    methods
        function obj = boid(vX,vY,pX,pY,size)
            %UNTITLED Construct an instance of this class
            %   Detailed explanation goes here
            if nargin > 0
                obj.pX = pX;
                obj.pY = pY;
                obj.vX = vX;
                obj.vY = vY;
                obj.size = size;
            end
        end

        function plot(obj)
            %METHOD1 Summary of this method goes here
            %   Detailed explanation goes here
            theta = atan2(obj.vY,obj.vX) - pi/2;
            R = [cos(theta) -sin(theta);
                 sin(theta)  cos(theta)];
            coordinates = [-1 0 1 0 -1;
                           -1 -0.5 -1 2 -1].*obj.size;
            coordinates = R*coordinates + [obj.pX;obj.pY];
            fill(coordinates(1,:),coordinates(2,:),'k');
        end
    end
end