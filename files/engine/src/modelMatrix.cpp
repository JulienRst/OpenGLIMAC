#include "engine/modelMatrix.hpp"

using namespace std;


ModelMatrix::ModelMatrix(){
    m_path = "";
    m_translate = vec3(0., 0., 0.);
    m_scale = vec3(0., 0., 0.);
}

ModelMatrix::ModelMatrix(string path, float tx, float ty, float tz, float sx, float sy, float sz){
    m_path = path;
    m_translate = vec3(tx, ty, tz);
    m_scale = vec3(sx, sy, sz);

}
//creation d'un vector de pointeurs vers des modelmatrix. ça evite de devoir faire des free.
vector<unique_ptr<ModelMatrix>> ModelMatrix::loadModelsFromFile(string filepath){
    ifstream myFile; //creation du ifstream qui contiendra les données du fichier
    myFile.open(filepath); //ouverture du fichier où sont contenus toutes les infos des modeles (une ligne, un model)
    vector<unique_ptr<ModelMatrix>> modelmatVector;
    if (myFile.is_open()) {
        string path, line;  //path est une variable temporaire
        float tx, ty, tz, sx, sy, sz; //variables temporaires

        while(getline(myFile, line)){ //tant qu'il existe une ligne après celle-ci{
            istringstream lineStream(line); //on prend les données de la ligne suivante
            lineStream >> path >> tx >> ty >> tz >> sx >> sy >> sz; //on rentre les données de la ligne dans les différentes variables temporaires

            modelmatVector.emplace_back(new ModelMatrix(path, tx, ty, tz, sx, sy, sz)); //on crée la modelmatrix avec les variables temporaires et on place la modelmatrix à la suite des autres dans le vector de modelmatrix

        }
    }
    myFile.close();

    return modelmatVector;
}

string ModelMatrix::getPath(){
    return m_path;
}
