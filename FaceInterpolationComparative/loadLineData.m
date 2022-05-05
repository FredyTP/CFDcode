function [data] = loadLineData(mesh_type,face_interpolation,n_cell)
if(mesh_type==0)
    mesh_type_str = "TRI";
else
    mesh_type_str = "QUAD";
end

switch(face_interpolation)
    case 0
        face_interp_name = "Upwind_Diffencing_Scheme";
    case 1
        face_interp_name = "Central_Diffencing_Scheme";
    case 2
        face_interp_name = "Powerlaw_Diffencing_Scheme";
    case 3
        face_interp_name = "Second_Order_Upwind";
end

name = strcat("Center_Line_Temperature_Stationary_",face_interp_name,"_",mesh_type_str,"_",num2str(n_cell),".csv");
data=csvread(name,1);
end

