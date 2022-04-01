#include<fstream>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
using namespace std;

struct Alumno{
    Alumno(){}
    Alumno(string _codigo, string _nombre, string _apellidos , string _carrera){
        for(int i = 0; i<5; i++){
            if(i < _codigo.size()) codigo[i] = _codigo[i];
            else codigo[i] = ' ';
        }
        for(int i = 0; i<11; i++){
            if(i < _nombre.size()) nombre[i] = _nombre[i];
            else nombre[i] = ' ';
        }
        for(int i = 0; i<20; i++){
            if(i < _apellidos.size()) apellidos[i] = _apellidos[i];
            else apellidos[i] = ' ';
        }
        for(int i = 0; i<15; i++){
            if(i < _carrera.size()) carrera[i] = _carrera[i];
            else carrera[i] = ' ';
        }
    }
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];
};

ostream& operator<<(ostream& out, Alumno& a){
    out.write(a.codigo, sizeof(Alumno::codigo));
    out.write(a.nombre, sizeof(Alumno::nombre));
    out.write(a.apellidos, sizeof(Alumno::apellidos));
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
    is.ignore(1);
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
        Alumno temp;
        while(archivo>>temp){
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
        if(pos < 0) throw("No valores negativos");
        ifstream archivo;
        archivo.open(path, ios::in);

        archivo.seekg(0, archivo.end);
        if(archivo.tellg() <= pos*(sizeof(Alumno)+1)) throw("No existe ese registro");

        archivo.seekg(pos*(sizeof(Alumno)+1), archivo.beg);
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
        // Crear archivos
        auto f = FixedRecord("datos1.txt");
        
        // AGREGAR REGISTROS
        f.add(Alumno{"0001","Howard","Paredes Zegarra", "Computacion"});    
        f.add(Alumno{"0002","Penny","Vargas Cordero","Industrial"});
        f.add(Alumno{"0003","Sheldon","Cooper Vega","Mecatronica"});
        f.add(Alumno{"0004","Pamela","Vargas Cordero","Industrial"});
        f.add(Alumno{"0005","Vanesa","Cooper Quizpe","Mecatronica"});
        f.add(Alumno{"0006","Jesus","Vega Cordero","Industrial"});
        f.add(Alumno{"0007","Heider","Sanchez Quizpe","Mecatronica"});
        f.add(Alumno{"0008","Facundo","Gimenez Velasco","Informatica"});
        f.add(Alumno{"0009", "Facundo", "Gimenez Velasco", "Informatica"});
        
        // IMPRIMIR TODOS LOS REGISTRO
        auto av = f.load();
        for(auto i: av) cout<<i;
        cout<<endl;
        
        // LEER REGISTRO 5
        auto a2 = f.readRecord(5);
        cout<<a2;
    }
    catch(const char* a){
        cout<<a<<endl;
    }
    
    return 0;
}