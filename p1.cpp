#include<fstream>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
using namespace std;

struct Alumno{
    /* Alumno(){}
    Alumno(string _codigo, string _nombre, string _apellidos , string _carrera){
        for(int i = 0; i<sizeof(codigo); i++){
            if(i < _codigo.size()) codigo[i] = _codigo[i];
            else codigo[i] = ' ';
        }
        for(int i = 0; i<sizeof(nombre); i++){
            if(i < _nombre.size()) nombre[i] = _nombre[i];
            else nombre[i] = ' ';
        }
        for(int i = 0; i<sizeof(apellidos); i++){
            if(i < _apellidos.size()) apellidos[i] = _apellidos[i];
            else apellidos[i] = ' ';
        }
        for(int i = 0; i<sizeof(carrera); i++){
            if(i < _carrera.size()) carrera[i] = _carrera[i];
            else carrera[i] = ' ';
        }
    } */
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];
};

ostream& operator<<(ostream& out, Alumno a){
    int lCod = strlen(a.codigo), lNom = strlen(a.nombre), lApe = strlen(a.apellidos), lCar = strlen(a.carrera);
    for(int i = 0; i<sizeof(Alumno::codigo)-lCod; i++) a.codigo[lCod+i] = ' ';
    // a.codigo[sizeof(Alumno::codigo)-1] = '\0';
    out.write(a.codigo, sizeof(Alumno::codigo));
    for(int i = 0; i<sizeof(Alumno::nombre)-lNom; i++) a.nombre[lNom+i] = ' ';
    // a.nombre[sizeof(Alumno::nombre)-1] = '\0';
    out.write(a.nombre, sizeof(Alumno::nombre));
    for(int i = 0; i<sizeof(Alumno::apellidos)-lApe; i++) a.apellidos[lApe+i] = ' ';
    // a.apellidos[sizeof(Alumno::apellidos)-1] = '\0';
    out.write(a.apellidos, sizeof(Alumno::apellidos));
    for(int i = 0; i<sizeof(Alumno::carrera)-lCar; i++) a.carrera[lCar+i] = ' ';
    // a.carrera[sizeof(Alumno::carrera)-1] = '\0';
    out.write(a.carrera, sizeof(Alumno::carrera));
    out.put('\n');
    out.flush();
    return out;
}

istream& operator>>(istream& is, Alumno& a){
    is.read(a.codigo, sizeof(Alumno::codigo));
    is.read(a.nombre, sizeof(Alumno::nombre));
    is.read(a.apellidos, sizeof(Alumno::apellidos));
    is.read(a.carrera, sizeof(Alumno::carrera));
    is.get();
    return is;
}

class FixedRecord{
public:
    FixedRecord(string _path): path(_path){
        ofstream archivo;
        archivo.open(path, ios::out|ios::app);
        archivo.close();
    }
    vector<Alumno> load(){
        ifstream archivo;
        archivo.open(path, ios::in);
        vector<Alumno> result;
        while(archivo.peek() != EOF){
            Alumno temp;
            archivo>>temp;
            result.push_back(temp);
        }
        archivo.close();
        return result;
    }
    void add(Alumno a){
        ofstream archivo;
        archivo.open(path, ios::app);
        archivo<<a;
        archivo.close();
    }
    Alumno readRecord(int pos){
        ifstream archivo;
        archivo.open(path, ios::in);
        for (int i = 0; i < pos; i++){
            archivo.ignore(sizeof(Alumno)+1);
        }
        if(archivo.peek() == EOF) throw("No existe el registro");
        Alumno result;
        archivo>>result;
        archivo.close();

        return result;
    }
private:
    string path;
};

int main(){

    try{
        auto f = FixedRecord("datos1.txt");
        // IMPRIMIR TODOS LOS REGISTRO
        for(auto i: f.load()) cout<<i;
        // AGREGAR REGISTRO
        Alumno a1{"0009", "Facundo", "Gimenez Velasco", "Informatica"};
        f.add(a1);
        // LEER REGISTRO 5
        auto a2 = f.readRecord(4);
        cout<<a2;
    }
    catch(const char* a){
        cout<<a<<endl;
    }
    
    return 0;
}