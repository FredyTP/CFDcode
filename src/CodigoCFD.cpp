// CodigoCFD.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//


#include <Eigen/Eigen>

#include <mesh/mesh.h>
#include <mesh/reader/luismi_format_mesh_reader.h>

#include <math/interpolation.h>

int main()
{
    
    using namespace std;

    mesh::LuisMiformatMeshReader meshReader("D://Alfredo//Academic//MASTER AERO//B//CFD//ProyectoCFD//CodigoCFD//mesh//nodes_4096.dat",
        "D://Alfredo//Academic//MASTER AERO//B//CFD//ProyectoCFD//CodigoCFD//mesh//cells_4096.dat");;

    mesh::Mesh mesh;
    mesh.loadMesh(&meshReader);

    std::cout <<"Mesh size:(MB): " <<mesh.getMemorySize() /1024.0f/1024.0f << std::endl;
   
    std::cout << "Interpolation: " << math::FaceInterpolation::uds<double>(vector2d(1,1),vector2d(-1,-1),10,-10) << std::endl;
    /*Node node1(1, {0,0});
    Node node2(2, { 1,0 });
    Node node3(3, { 0,1 });
    Node node4(4, { 1,1 });

    Cell cell1;
    Cell cell2;

    Face face1(&node1, &node2);
    Face face2(&node1, &node3);
    Face face3(&node2, &node3);
    Face face4(&node4, &node2);
    Face face5(&node3, &node4);


    cell1.setNodes({ &node1,&node2,&node3 });
    cell1.setFaces({ &face1,&face2,&face3 });

    cell2.setNodes({ &node2,&node3,&node4 });
    cell2.setFaces({ &face3,&face4,&face5 });

    cell1.build();
    cell2.build();

    face1.setCells(&cell1, nullptr);
    face2.setCells(&cell1, nullptr);
    face3.setCells(&cell1, &cell2);
    face4.setCells(&cell2, nullptr);
    face5.setCells(&cell2, nullptr);
  

    face1.build();
    face2.build();
    face3.build();
    face4.build();
    face5.build();

    auto normal11 = face1.getNormal(&cell1);
    auto normal21 = face2.getNormal(&cell1);
    auto normal31 = face3.getNormal(&cell1);
    auto normal32 = face3.getNormal(&cell2);
    auto normal42 = face4.getNormal(&cell2);
    auto normal52 = face5.getNormal(&cell2);


    std::cout << "n11: \n" << normal11 << std::endl;
    std::cout << "n21: \n" << normal21 << std::endl;
    std::cout << "n31: \n" << normal31 << std::endl;
    std::cout << "n32: \n" << normal32 << std::endl;
    std::cout << "n42: \n" << normal42 << std::endl;
    std::cout << "n52: \n" << normal52 << std::endl;*/





    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
