#include "engine/model.hpp"
#include "engine/shader.hpp"

using namespace std;
using namespace glm;

/*  Functions   */
// Constructor, expects filepath and floats of matrices translate and scale
Model::Model(std::string const& path, std::vector<float>& xyz){
    GLuint i;
    std::cout << "size de xyz : " << xyz.size() << std::endl;
    for(i = 0; i < xyz.size(); i+=9){
        for(int j = 0; j < 9; j++){
        std::cout << "xyz[" << j << "] = " << xyz[j] << "  ";
        }
        std::cout << std::endl;
        glm::mat4 ViewTranslate = translate(glm::mat4(1.0f), vec3(xyz[i], xyz[i+1], xyz[i+2]));
      //  modelmat = rotate(modelmat, xyz[i+9], vec3((int)xyz[i+6], (int)xyz[i+7], (int)xyz[i+8]));

        glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, xyz[i+6], glm::vec3(1.f, 0.0f, 0.0f));
        glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, xyz[i+7], glm::vec3(0.0f, 1.f, 0.0f));
        glm::mat4 View = glm::rotate(ViewRotateY, xyz[i+8], glm::vec3(0.0f, 0.0f, 1.f));
        std::cout << ViewTranslate << std::endl;
        glm::mat4 scaled = scale(glm::mat4(1.0f), vec3(xyz[i+3], xyz[i+4], xyz[i+5]));
        std::cout << ViewRotateX << std::endl;
        std::cout << ViewRotateY << std::endl;
        std::cout << View << std::endl;
        std::cout << scaled << std::endl;
        glm::mat4 modelmat = View * scaled;

        std::cout << modelmat << std::endl;
        m_modelmatVector.push_back(modelmat);
    }
    this->loadModel(path);
}


// Draws the model, and thus all its meshes
void Model::Draw(Shader const& shader){
    for(unsigned int j = 0; j < m_modelmatVector.size() ; ++j) {
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m_modelmatVector[j]));
        for(GLuint i = 0; i < this->meshes.size(); i++){
            this->meshes[i].Draw(shader);
        }
    }
}

//Create the models with the path, translate and scale matrix
void drawModels(map<int, unique_ptr<Model> > const& models, Shader const& shader){
        //Load a list of ModelMatrix from a text file
        GLuint i;
        for(i = 0; i < models.size(); i++){
            models.at(i)->Draw(shader);
        }
}

glm::mat4 Model::getModelMatrix(int numModelMat){
    return m_modelmatVector[numModelMat];
}

//Creates a map of models pointers from file
map<int, unique_ptr<Model> > modelsFromFile(string const& filepath){
    ifstream myFile; //creation du ifstream qui contiendra les données du fichier
    myFile.open(filepath); //ouverture du fichier où sont contenus toutes les infos des modeles (une ligne, un model)
    std::map<int, std::unique_ptr<Model> > models;
        string path, line; //path est une variable temporaire
        string stx, sty, stz, ssx, ssy, ssz, srx, sry, srz;

         if (!myFile.is_open()){
             std::cerr << "Erreur lors de l'ouverture du fichier: " << strerror(errno) << std::endl;
         }
         int i = 0;
         bool first_model_already_exists = false; 
         std::vector<float> xyz;
        
        while(getline(myFile, line)){ //tant qu'il existe une ligne après celle-ci{
            istringstream lineStream(line); //on prend les données de la ligne suivante

            lineStream >> stx; //The first "word" of the line goes into stx
            if(stx != "#"){
                std::cout << "PAS UN COMMENTAIRE : STX = " << stx << " et ";
                if(stx == "Model" || stx == "Fin"){
                    if(first_model_already_exists == true){
                        std::cout << "MODEL DEJA : Creation de model" << std::endl;

                        //std::cout << "i = " << i << std::endl;
                        models[i].reset(new Model(path, xyz));
                        xyz.clear(); //xyz has to be clean again for the new model
                        ++i;
                    }  
                    if(stx == "Model"){ 
                     std::cout << "MODEL" << std::endl;
                     lineStream >> path; } //Beginning of a new model
                }
                else{
                    std::cout << "MATRICE" << std::endl;

                    lineStream >> sty >> stz >> ssx >> ssy >> ssz >> srx >> sry >> srz;
                    //on rentre les données de la ligne dans les différentes variables temporaires
                    //std::cout << "i : " << path << stx << sty << stz << ssx << ssy << ssz << std::endl;
                    std::vector<float> abc;
                    std::cout << "abc : " << stx << sty << stz << ssx << ssy << ssz << srx << sry << srz << std::endl;
                    abc.push_back(stof(stx));
                    abc.push_back(stof(sty));
                    abc.push_back(stof(stz));
                    abc.push_back(stof(ssx));
                    abc.push_back(stof(ssy));
                    abc.push_back(stof(ssz));
                    abc.push_back(stof(srx));
                    abc.push_back(stof(sry));
                    abc.push_back(stof(srz));

                    xyz.insert(xyz.end(), abc.begin(), abc.end() );
                }
                first_model_already_exists = true; //permits to create a model with the xyz created when meeting the next path
            }
            else{ std::cout << "UN COMMENTAIRE" << std::endl;}
        }
    myFile.close();
    //std::cout << "file closed" << std::endl;
    return models;
}

/*  Functions   */

// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(string path)
{
    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs);
      // Check for errors
    ifstream myFile; //creation du ifstream qui contiendra les données du fichier
    myFile.open(path);
    if(!myFile.is_open()){
        std::cerr << "Erreur: Fichier modèle non trouvé" << std::endl;
    }
    std::cout << "Ouverture réussie pour " << path << std::endl;

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // Retrieve the directory path of the filepath
    this->directory = path.substr(0, path.find_last_of('/'));

    // Process ASSIMP's root node recursively
    this->processNode(scene->mRootNode, scene);
}

// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process each mesh located at the current node
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        // The node object only contains indices to index the actual objects in the scene.
        // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }
    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // Data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // Texture Coordinates
        if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // Process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // Diffuse: texture_diffuseN
    // Specular: texture_specularN
    // Normal: texture_normalN

    // 1. Diffuse maps
    vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. Specular maps
    vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // Return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        GLboolean skip = false;
        for(GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if(textures_loaded[j].path == str)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // If texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

GLint Model::TextureFromFile(const string path, string directory)
{
    //Generate texture ID and load texture data
    // TODO use filepath class
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;

    glGenTextures(1, &textureID);
    unique_ptr<glimac::Image> image = glimac::loadImage(filename.c_str());
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
