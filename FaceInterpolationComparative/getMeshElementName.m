function [mesh_name] = getMeshElementName(mesh_type)
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
if(mesh_type==0)   
    mesh_name="TRI CELL";
else
    mesh_name="QUAD CELL";
end
end

