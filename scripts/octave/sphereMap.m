function [M]=sphereMap(numPixels)
% function [M]=sphereMap(numPixels)
%
% it generates an image of size (numPixels x numPixels)
% with the encoded (x,y,z) coordinates of the sphere

% init the image with the coordinates
i = 0:1:(numPixels-1);
% convert to -1..1 range
u = 2 * (i+0.5) / numPixels - 1;
% create matrix with coordinates
X = ones(numPixels,1) * u;
Z = u' * ones(1, numPixels);
% radius
R = sqrt(X .* X + Z .* Z);
% encode Y
Y = max(-ones(numPixels,numPixels), 1 - 2 * R);
% normalize X,Z (no div by zero, since never in the center ;)
N = 1./R;
R = sqrt(1-Y.*Y);
X = R .* X .* N;
Z = R .* Z .* N;


M(:,:,1) = X;
M(:,:,2) = Y;
M(:,:,3) = Z;



