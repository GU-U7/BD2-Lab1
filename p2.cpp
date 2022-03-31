#include<fstream>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
using namespace std;

struct Alumno{
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];
    int ciclo;
    float mensualidad;
};

ostream& operator<<(ostream& out, Alumno& a){
    out.write((char*) &a, sizeof(Alumno));
    return out;
}

istream& operator>>(istream& is, Alumno& a){
    is.read((char*) &a, sizeof(Alumno));
    return is;
}

class FixedRecord{
    public:
    FixedRecord(string _path): path(_path){
        ofstream archivo;
        archivo.open(path, ios::out|ios::app |ios::binary);
        archivo.close();
    }
    void add(Alumno a){
        ofstream archivo;
        archivo.open(path, ios::out|ios::app | ios::binary);
        archivo<<a;
        archivo.close();
    }
    vector<Alumno> load(){
        ifstream archivo;
        archivo.open(path, ios::in | ios::binary);

        vector<Alumno> result;
        Alumno atemp;
        while(archivo>>atemp){
            result.push_back(atemp);
        }
        archivo.close();
        return result;
    }
    Alumno readRecord(int pos){
        Alumno result;
        ifstream archivo;
        archivo.open(path, ios::in | ios::binary);
        archivo.seekg(pos*sizeof(Alumno), ios::beg);
        if(!(archivo>>result)){ 
            archivo.close();
            throw("Registro no hallado");
        }
        archivo.close();
        return result;
    }
    bool delete_reg(int pos){
        return bool();
    }
    private:
    string path;
};

int main(){
    FixedRecord fr("datos2.dat");
    fr.add(Alumno{"a101", "grover", "nolberto solano", "ciencia", 5, 9832.12});
}