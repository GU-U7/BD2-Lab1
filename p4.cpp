#include<string>
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

struct Matricula
{
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;
};

ostream& operator<<(ostream& os, Matricula m){
    int len = m.codigo.size();
    os.write((char*) &len, sizeof(int));
    os.write(m.codigo.c_str(), len);
    len = sizeof(int);
    os.write((char*) &len, sizeof(int));
    os.write((char*) &m.ciclo, sizeof(Matricula::ciclo));
    len = sizeof(float);
    os.write((char*) &len, sizeof(int));
    os.write((char*) &m.mensualidad, sizeof(Matricula::mensualidad));
    len = m.observaciones.size();
    os.write((char*) &len, sizeof(int));
    os.write(m.observaciones.c_str(), len);
    return os;
}

istream& operator>>(istream& is, Matricula &m){
    int len;
    is.read((char*) &len, sizeof(int));
    char *miniBuffer = new char[len];
    is.read(miniBuffer, len);
    m.codigo = miniBuffer;
    delete []miniBuffer;
    is.read((char*) &len, sizeof(int));
    is.read((char*) &m.ciclo, len);
    is.read((char*) &len, sizeof(int));
    is.read((char*) &m.mensualidad, len);
    is.read((char*) &len, sizeof(int));
    miniBuffer = new char[len];
    is.read(miniBuffer, len);
    m.observaciones = miniBuffer;
    delete []miniBuffer;
    return is;
}

class VariableRecord{
    public:
    VariableRecord(string _path, string _pathCab)
    :path(_path), pathCab(_pathCab){
        ofstream f(path, ios::out|ios::binary|ios::app);
        f.close();
        f.open(pathCab, ios::out|ios::binary|ios::app);
        f.close();
    }
    void add(Matricula a){
        ofstream f(path, ios::out|ios::binary|ios::app);
        ofstream fCab(pathCab, ios::out|ios::binary|ios::app);
        int last_pos = f.tellp();
        fCab.write((char*) &last_pos, sizeof(last_pos));
        f<<a;
        f.close();
        fCab.close();
    }
    vector<Matricula> load(){
        vector<Matricula> result;
        ifstream f(path, ios::in|ios::binary);
        Matricula temp;
        while(f>>temp){
            result.push_back(temp);
        }
        return result;
    }
    Matricula readRecord(int pos){
        int fPos;
        ifstream f(pathCab, ios::in|ios::binary);

        f.seekg(0, f.end);
        if(f.tellg()<=pos*sizeof(int)) throw("No existe ese registro");

        f.seekg(pos*sizeof(int), f.beg);
        f.read((char*) &fPos, sizeof(int));
        f.close();
        Matricula result;
        f.open(path, ios::in|ios::binary);
        f.seekg(fPos, f.beg);
        f>>result;
        f.close();
        return result;
    }
    private:
    string path, pathCab;
};

int main(){
    try{
        // Crear documentos
        VariableRecord vr("datos4.dat", "posiciones.dat");

        // Agregar registros
        vr.add(Matricula{"A101", 3, 45.32f, "Duda mucho"});
        vr.add(Matricula{"A10112", 9, -45.32f, "No hace nada"});
        vr.add(Matricula{"A1", 2, 5.322f, "Llega tarde"});
        vr.add(Matricula{"A10042", 13, 0.9f, "Es lento"});

        // Seleccionar un registro
        Matricula mt = vr.readRecord(2);
        cout<<mt.codigo<<" "<<mt.ciclo<<" "<<mt.mensualidad<<" "<<mt.observaciones<<endl;
        cout<<endl;

        // Leer todos los registros
        auto vm = vr.load();
        for(auto i: vm)
            cout<<i.codigo<<" "<<i.ciclo<<" "<<i.mensualidad<<" "<<i.observaciones<<endl;
    }
    catch(char const* c){
        cout<<c;
    }
    return 0;
}