function [face_name] = getFaceInterpolationName(face_interpolation)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here
switch(face_interpolation)
    case 0
        face_name = "Upwind Diffencing Scheme";
    case 1
        face_name = "Central Diffencing Scheme";
    case 2
        face_name = "Powerlaw Diffencing Scheme";
    case 3
        face_name = "Second Order Upwind";
end
end

