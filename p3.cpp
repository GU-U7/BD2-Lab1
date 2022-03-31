#include<string>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

struct Alumno{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;
};

struct Cabecera{
    int posicion;
    int tamanho;
};

ostream& operator<<(ostream& os, Cabecera c){
    os.write((char*) &c, sizeof(Cabecera));
    return os;
}

istream& operator>>(istream& is, Cabecera &c){
    is.read((char*) &c, sizeof(Cabecera));
    return is;
}

char* empaquetar(Alumno a){
    string mensualidad_str = to_string(a.mensualidad);
    int largo = (a.Nombre.size()+1) + (a.Apellidos.size()+1) + (a.Carrera.size()+1) + (mensualidad_str.size()+1);
    char *empaquetado = new char[largo+3+1];
    sprintf(empaquetado, "%s|%s|%s|%s", a.Nombre.c_str(), a.Apellidos.c_str(), a.Carrera.c_str(), mensualidad_str.c_str());
    return empaquetado;
}

Alumno desempaquetar(char*& str){
    Alumno result{};
    int posiciones[3]{};
    int len = strlen(str);
    for(int i = 0, j = 0; i<len; i++){
        if(str[i] == '|') posiciones[j++] = i;
    }
    result.Nombre = string(str, str+posiciones[0]);
    result.Apellidos = string(str+posiciones[0]+1, str+posiciones[1]);
    result.Carrera = string(str+posiciones[1]+1, str+posiciones[2]);
    result.mensualidad = stof(string(str+posiciones[2]+1, str+len));
    return result;
}

int operator<<(ostream& os, Alumno a){
    char *buf = empaquetar(a);
    int len = strlen(buf);
    os.write(buf, len);
    os.put('\n');
    delete buf;
    return len;
}

class VariableRecord{
    public:
    VariableRecord(string _path, string _pathCab)
    :path(_path), pathCab(_pathCab){
        ofstream f(path, ios::out|ios::app);
        f.close();
        f.open(pathCab, ios::out|ios::binary|ios::app);
        f.close();
    }
    vector<Alumno> load(){
        ifstream archivoCab(pathCab, ios::in | ios::binary);
        ifstream archivoAlu(path, ios::in);
        vector<Alumno> result;
        Cabecera ctemp;
        while(archivoCab>>ctemp){
            char *buffer = new char[ctemp.tamanho];
            archivoAlu.read(buffer, ctemp.tamanho);
            Alumno altemp = desempaquetar(buffer);
            archivoAlu.get();
            delete []buffer;
            result.push_back(altemp);
        }
        archivoAlu.close();
        archivoCab.close();
        return result;
    }
    Alumno readReacord(int pos){
        ifstream archivo(pathCab, ios::in | ios::binary);
        archivo.seekg(pos*(sizeof(Cabecera)), archivo.beg);
        Cabecera lectura;
        archivo>>lectura;
        archivo.close();

        archivo.open(path, ios::in);
        char *buffer = new char[lectura.tamanho];
        archivo.seekg(lectura.posicion, archivo.beg);
        archivo.read(buffer, lectura.tamanho);
        archivo.close();
        Alumno record = desempaquetar(buffer);
        delete []buffer;
        return record;
    }
    void add(Alumno a){
        ofstream f(path, ios::out|ios::app);
        Cabecera cc{};
        f.seekp(0, f.end);
        cc.posicion = f.tellp();
        cc.tamanho = (f<<a);
        f.close();
        f.open(pathCab, ios::out|ios::app|ios::binary);
        f<<cc;
        f.close();
    }
    private:
    string path, pathCab;
};

int main(){
    // creacion de archivos
    VariableRecord vr("datos3.txt", "cabecera.dat");
    // añadir registros
    vr.add(Alumno{"Howard", "Paredes Zegarra", "Computacion", 1500});
    vr.add(Alumno{"Sheldon", "Cooper Quizpe", "Mecatronica", 1850.00});
    vr.add(Alumno{"Lidya", "Cooper Quizpe", "Computacion", 1850.00});
    vr.add(Alumno{"Thony", "Cooper Quizpe", "Computacion", 1850.00});
    vr.add(Alumno{"Cristian", "Cueva", "Informatica", 764.22}); 
    // seleccionar registro n°3
    auto r2 = vr.readReacord(2);
    cout<<r2.Nombre<<"&&"<<r2.Apellidos<<"&&"<<r2.Carrera<<"&&"<<r2.mensualidad<<endl;
    cout<<endl;
    // seleccionar todos los registros
    auto rall = vr.load();
    for(auto i: rall)
        cout<<i.Nombre<<"&&"<<i.Apellidos<<"&&"<<i.Carrera<<"&&"<<i.mensualidad<<endl;

}