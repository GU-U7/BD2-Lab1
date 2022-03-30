#include<string>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<fstream>

using namespace std;

struct Alumno{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;
};

char* empaquetar(Alumno a){
    string mensualidad_str = to_string(a.mensualidad);
    int largo = (a.Nombre.size()+1) + (a.Apellidos.size()+1) + (a.Carrera.size()+1) + (mensualidad_str.size()+1);
    char *empaquetado = new char[largo+3+1];
    sprintf(empaquetado, "%s|%s|%s|%s", a.Nombre.c_str(), a.Apellidos.c_str(), a.Carrera.c_str(), mensualidad_str.c_str());
    return empaquetado;
}

ostream& operator<<(ostream& os, Alumno a){
    char *buf = empaquetar(a);
    os.write(buf, strlen(buf));
    os.put('\n');
    delete buf;
    return os;
}

class VariableRecord{
    public:
    VariableRecord(string _path, string _pathCab)
    :path(_path), pathCab(_pathCab){
        ofstream f(path, ios::out|ios::app);
        f.close();
        f.open(pathCab);
        f.close();
    }
    void add(Alumno a){
        ofstream f(path, ios::out|ios::app);
        f<<a;
        f.close();
    }
    private:
    string path, pathCab;
};

int main(){
    Alumno a{"Grovergsd", "Ugdfsfa", "CSienci", 34.4f};
    auto vr = VariableRecord("datos3.txt", "cabecera.dat");
    vr.add(a);
}