// v1.0

#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct SYSTEM_MATERIAL {
    string name;
    bool def_material = false;

    vector<float> Ka = {0.5f, 0.5f, 0.5f};
    vector<float> Kd = {0.5f, 0.5f, 0.5f};
    vector<float> Ks = {1.0f, 1.0f, 1.0f};
    vector<float> Ke = {0.0f, 0.0f, 0.0f};
    
    string map_Ka = "";
    string map_Kd = "";
    string map_Ks = "";

    float Ns = 32.0f;
    float d = 1.0f;
    vector<float> Tf = {1.0f, 1.0f, 1.0f};
    int illum = 2;
};

struct SYSTEM_OBJ_MODEL {
    vector<SYSTEM_MATERIAL> MATERIALS;
    vector<vector<float>> POLYGON_GROUPS;
    bool TEXTURES = false;
};

struct SYSTEM_OBJ_POLYGON_GROUP {
    vector<vector<int>> F_squares;
    vector<vector<int>> F_triangles;
    vector<float> V_FINAL;
};

vector<float> SYSTEM_VECTOR_OPERATION (vector<float> VECTOR_1, vector<float> VECTOR_2, char operation) {
    int size1 = VECTOR_1.size();
    int size2 = VECTOR_2.size();
    switch (operation) {
        case '+':
            for (int i = 0; i<=size1 && i<=size2; i+=1) {
                VECTOR_1[i] += VECTOR_2[i];
            }
            break;

        case '-':
            for (int i = 0; i<=size1 && i<=size2; i+=1) {
                VECTOR_1[i] -= VECTOR_2[i];
            }
            break;

        case '*':
            for (int i = 0; i<=size1 && i<=size2; i+=1) {
                VECTOR_1[i] *= VECTOR_2[i];
            }
            break;

        case '/':
            for (int i = 0; i<=size1 && i<=size2; i+=1) {
                VECTOR_1[i] /= VECTOR_2[i];
            }
            break;
    }

    return VECTOR_1;
}

vector<SYSTEM_MATERIAL> SYSTEM_MTL_LOAD(string file_dir, string folder_dir) {
    ifstream file_read(file_dir);
    vector<SYSTEM_MATERIAL> MATERIALS;

    if (!file_read.is_open()) {return MATERIALS;}

    string current_string;
    int current_index = 0;

    SYSTEM_MATERIAL default_material;
    default_material.def_material = true;
    MATERIALS.push_back(default_material);
    
    while (getline(file_read, current_string)) {
        int size = current_string.size();

        if (current_string.size() == 0) {continue;}
        string new_current_string;
        for (char i : current_string) {
            if (i != '#') {new_current_string.push_back(i);}
            else {break;}
        }

        size = new_current_string.size();

        if (size > 6) {

            int start_index = 0;
            if (new_current_string[0] == ' ' || new_current_string[0] == '	') {
                start_index += 1;
                for (int i = 1; i < size; i+=1) {
                    if (new_current_string[i] == ' ' || new_current_string[i] == '	') {start_index += 1;}
                    else {break;}
                }
            }
            
            string check;
            for (int i = start_index; i < start_index + 6; i+=1) {check.push_back(new_current_string[i]);}

            if (check[1] == ' ' || check[2] == ' ' || check[1] == '	' || check[2] == '	') {
                vector<float> Xx_vec;
                string current_float = "";
                
                for (int i = start_index + 2; i < new_current_string.size(); i+=1) {
                    if (new_current_string[i] == ' ' && current_float == "" || new_current_string[i] == '	' && current_float == "") {continue;}

                    else if (new_current_string[i] == ' ' && current_float != "" || new_current_string[i] == '	' && current_float != "") {
                        Xx_vec.push_back(stod(current_float));
                        current_float = "";
                    }

                    else {current_float += new_current_string[i];}
                }
                if (current_float != "") {Xx_vec.push_back(stod(current_float));}
                if (Xx_vec.size() == 1 || Xx_vec.size() == 2) {Xx_vec.push_back(Xx_vec[0]); Xx_vec.push_back(Xx_vec[0]);}
                
                if (check[0] == 'K') {
                    switch (check[1]) {
                        case 'a':
                            MATERIALS[current_index].Ka = Xx_vec;
                            break;

                        case 'd':
                            MATERIALS[current_index].Kd = Xx_vec;
                            break;

                        case 's':
                            MATERIALS[current_index].Ks = Xx_vec;
                            break;

                        case 'e':
                            MATERIALS[current_index].Ke = Xx_vec;
                            break;
                    }
                }

                else if (check[0] == 'T' && check[1] == 'f') {
                    MATERIALS[current_index].Tf = Xx_vec;
                }

                else if (check[0] == 'T' && check[1] == 'r') {
                    MATERIALS[current_index].d = 1 - Xx_vec[0];
                }

                else if (check[0] == 'N' && check[1] == 's') {
                    MATERIALS[current_index].Ns = Xx_vec[0];
                }

                else if (check[0] == 'd' && check[1] == ' ' || check[0] == 'd' && check[1] == '	') {
                    MATERIALS[current_index].d = Xx_vec[0];
                }
            }

            else if (check[0] == 'm' && check[1] == 'a' && check[2] == 'p') {
                vector<string> parameters;
                vector<int> parameters_start_index;
                string current_parameter = "";
                int skip_parameters = 0;
                
                for (int i = start_index + 7; i<new_current_string.size(); i+=1) {
                    if (new_current_string[i] == ' ' && current_parameter == "" || new_current_string[i] == '	' && current_parameter == "") {continue;}
                    else if (new_current_string[i] == ' ' && current_parameter != "" || new_current_string[i] == '	' && current_parameter != "") {
                        parameters.push_back(current_parameter);
                        current_parameter = "";
                    }
                    else if (new_current_string[i] != ' ' && current_parameter == "" || new_current_string[i] != '	' && current_parameter == "") {
                        parameters_start_index.push_back(i);
                        current_parameter += new_current_string[i];
                    }
                    else {current_parameter += new_current_string[i];}
                }
                if (current_parameter != "") {parameters.push_back(current_parameter);}

                for (string i : parameters) {
                    if (i[0] == '-') {
                        if (i == "-blendu") {skip_parameters += 2;}
                        else if (i == "-blendv") {skip_parameters += 2;}
                        else if (i == "-cc") {skip_parameters += 2;}
                        else if (i == "-clamp") {skip_parameters += 2;}
                        else if (i == "-mm") {skip_parameters += 3;}
                        else if (i == "-o") {skip_parameters += 4;}
                        else if (i == "-s") {skip_parameters += 4;}
                        else if (i == "-t") {skip_parameters += 4;}
                        else if (i == "-texres") {skip_parameters += 2;}
                    }
                }

                string texture_dir = folder_dir;
                for (int i = parameters_start_index[skip_parameters]; i<new_current_string.size(); i+=1) {
                    texture_dir += new_current_string[i];
                }

                if (check[4] == 'K') {
                    if (check[5] == 'a') {MATERIALS[current_index].map_Ka = texture_dir;}
                    else if (check[5] == 'd') {MATERIALS[current_index].map_Kd = texture_dir;}
                    else if (check[5] == 's') {MATERIALS[current_index].map_Ks = texture_dir;}
                }
            }

            else if (check == "illum ") {
                string illum_string;
                for (int i = start_index + 6; i<new_current_string.size(); i+=1) {
                    if (new_current_string[i] == ' ' || new_current_string[i] == '	') {continue;}
                    else {illum_string += new_current_string[i];}
                }
                
                MATERIALS[current_index].illum = stoi(illum_string);
            }

            else if (check == "newmtl") {
                SYSTEM_MATERIAL new_mtl;
                MATERIALS.push_back(new_mtl);
                current_index += 1;

                string mtl_name;
                for (int i = 7; i < size; i+=1) {mtl_name.push_back(new_current_string[i]);}
                MATERIALS[current_index].name = mtl_name;
            }
        }
    }
    
    return MATERIALS;
}

SYSTEM_OBJ_MODEL SYSTEM_OBJ_LOAD(string file_dir, bool *check) {
    ifstream file_read(file_dir);

    int file_dir_size = file_dir.size();
    string check_file_type = file_dir.substr(file_dir_size-4);
    if (!file_read.is_open() || check_file_type != ".obj") {
        *check = false;
        SYSTEM_OBJ_MODEL null_model;
        return null_model;
    }
    else {*check = true;}

    string current_string;
    string current_index;
    int spaces = 0;
    int type = 0;

    SYSTEM_MATERIAL default_material;
    default_material.def_material = true;

    SYSTEM_OBJ_MODEL model;
    model.MATERIALS.push_back(default_material);

    vector<SYSTEM_OBJ_POLYGON_GROUP> POLYGON_GROUPS;
    SYSTEM_OBJ_POLYGON_GROUP default_polygon_group;
    POLYGON_GROUPS.push_back(default_polygon_group);
    int current_polygon_group = 0;

    vector<vector<float>> V_VERTICES;
    vector<vector<float>> V_NORMALS;
    vector<vector<float>> V_TEXTURES;

    vector<SYSTEM_MATERIAL> MTL_MATERIALS;

    bool textures = false;
    bool normals = false;

    string current_dir = file_dir;
    if (current_dir.back() == '/' && current_dir.back() == '\\') {current_dir.pop_back();}
    for (int i = current_dir.size()-1; i>=0; i-=1) {
        if (current_dir[i] != '/' && current_dir[i] != '\\') {current_dir.pop_back();}
        else {break;}
    }

    while (getline(file_read, current_string)) {
        spaces = 0;
        
        if (current_string[0] == 'v' && current_string[1] == ' ') {
            vector<float> current_V;
            string current_float = "";
            for (char i : current_string) {
                if (i == ' ' && current_float == "" || i == 'v') {continue;}
                else if (i == ' ' && current_float != "") {current_V.push_back(stod(current_float)); current_float = "";}
                else {current_float += i;}
            }
            if (current_float != "") {current_V.push_back(stof(current_float));}
            V_VERTICES.push_back(current_V);
        }

        else if (current_string[0] == 'v' && current_string[1] == 't') {
            vector<float> current_V;
            string current_float = "";
            textures = true;
            model.TEXTURES = true;
            for (char i : current_string) {
                if (i == ' ' && current_float == "" || i == 'v' || i == 't') {continue;}
                else if (i == ' ' && current_float != "") {current_V.push_back(stod(current_float)); current_float = "";}
                else {current_float += i;}
            }
            if (current_float != "") {current_V.push_back(stof(current_float));}
            if (current_V.size() == 2) {current_V.push_back(0.0f);}
            V_TEXTURES.push_back(current_V);
        }

        else if (current_string[0] == 'v' && current_string[1] == 'n') {
            vector<float> current_V;
            string current_float = "";
            normals = true;
            for (char i : current_string) {
                if (i == ' ' && current_float == "" || i == 'v' || i == 'n') {continue;}
                else if (i == ' ' && current_float != "") {current_V.push_back(stod(current_float)); current_float = "";}
                else {current_float += i;}
            }
            if (current_float != "") {current_V.push_back(stof(current_float));}
            V_NORMALS.push_back(current_V);
        }

        else if (current_string[0] == 'f' && current_string[1] == ' ') {
            vector<int> current_I;
            string current_index;
            for (char i : current_string) {
                if (i == 'f') {continue;}
                if (i == ' ' && current_index == "" || i == 'f') {continue;}
                else if (i == ' ' && current_index != "" || i == '/' && current_index != "") {current_I.push_back(stoi(current_index)); current_index = "";}
                else if (i != '/' && i != ' ' && i != '\\'){current_index += i;}
            }
            if (current_index != "") {current_I.push_back(stoi(current_index));}
            
            int sizeI = current_I.size();
            
            if (textures && normals) {
                if (sizeI == 12) {POLYGON_GROUPS[current_polygon_group].F_squares.push_back(current_I);} 
                else if (sizeI == 9) {POLYGON_GROUPS[current_polygon_group].F_triangles.push_back(current_I);}
            }

            else if (textures || normals) {
                if (sizeI == 8) {POLYGON_GROUPS[current_polygon_group].F_squares.push_back(current_I);}
                else if (sizeI == 6) {POLYGON_GROUPS[current_polygon_group].F_triangles.push_back(current_I);}
            }

            else {
                if (sizeI == 4) {POLYGON_GROUPS[current_polygon_group].F_squares.push_back(current_I);}
                else if (sizeI == 3) {POLYGON_GROUPS[current_polygon_group].F_triangles.push_back(current_I);}
            }
        }

        else {
            if (current_string.size() < 6) {continue;}
            
            string check = "";
            for (int i = 0; i<6; i+=1) {check += current_string[i];}

            if (check == "mtllib") {
                string mtl_dir = current_dir;
                for (int i = 7; i < current_string.size(); i+=1) {mtl_dir.push_back(current_string[i]);}
                
                MTL_MATERIALS = SYSTEM_MTL_LOAD(mtl_dir, current_dir);
            }

            else if (check == "usemtl") {
                string material_name;
                bool check = false;

                for (int i = 7; i < current_string.size(); i+=1) {material_name.push_back(current_string[i]);}
                for (SYSTEM_MATERIAL i : MTL_MATERIALS) {if (i.name == material_name) {model.MATERIALS.push_back(i); check = true; break;}}

                if (check) {
                    current_polygon_group += 1;
                    POLYGON_GROUPS.push_back(default_polygon_group);
                }
            }
        }
    }

    // Textures and Normals ===========================================================================================================
    for (SYSTEM_OBJ_POLYGON_GROUP &polygon_group : POLYGON_GROUPS) {
        
        vector<vector<int>> &F_squares = polygon_group.F_squares;
        vector<vector<int>> &F_triangles = polygon_group.F_triangles;
        vector<float> &V_FINAL = polygon_group.V_FINAL;
        
        if (textures && normals) {
            for (vector<int> i : F_squares) {   

                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        V_TEXTURES[i[1]-1][0], V_TEXTURES[i[1]-1][1], V_TEXTURES[i[1]-1][2],
                                        V_NORMALS[i[2]-1][0], V_NORMALS[i[2]-1][1], V_NORMALS[i[2]-1][2]};
                
                vector<float> vertex2 = {V_VERTICES[i[3]-1][0], V_VERTICES[i[3]-1][1], V_VERTICES[i[3]-1][2],
                                        V_TEXTURES[i[4]-1][0], V_TEXTURES[i[4]-1][1], V_TEXTURES[i[4]-1][2],
                                        V_NORMALS[i[5]-1][0], V_NORMALS[i[5]-1][1], V_NORMALS[i[5]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[6]-1][0], V_VERTICES[i[6]-1][1], V_VERTICES[i[6]-1][2],
                                        V_TEXTURES[i[7]-1][0], V_TEXTURES[i[7]-1][1], V_TEXTURES[i[7]-1][2],
                                        V_NORMALS[i[8]-1][0], V_NORMALS[i[8]-1][1], V_NORMALS[i[8]-1][2]};

                vector<float> vertex4 = {V_VERTICES[i[9]-1][0], V_VERTICES[i[9]-1][1], V_VERTICES[i[9]-1][2],
                                        V_TEXTURES[i[10]-1][0], V_TEXTURES[i[10]-1][1], V_TEXTURES[i[10]-1][2],
                                        V_NORMALS[i[11]-1][0], V_NORMALS[i[11]-1][1], V_NORMALS[i[11]-1][2]};

                vector<float> polygon1;
                vector<float> polygon2;

                for (float j : vertex1) {polygon1.push_back(j);}
                for (float j : vertex2) {polygon1.push_back(j);}
                for (float j : vertex3) {polygon1.push_back(j);}

                for (float j : vertex1) {polygon2.push_back(j);}
                for (float j : vertex3) {polygon2.push_back(j);}
                for (float j : vertex4) {polygon2.push_back(j);}

                for (float j : polygon1) {V_FINAL.push_back(j);}
                for (float j : polygon2) {V_FINAL.push_back(j);}

            }

            for (vector<int> i : F_triangles) {
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        V_TEXTURES[i[1]-1][0], V_TEXTURES[i[1]-1][1], V_TEXTURES[i[1]-1][2],
                                        V_NORMALS[i[2]-1][0], V_NORMALS[i[2]-1][1], V_NORMALS[i[2]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[3]-1][0], V_VERTICES[i[3]-1][1], V_VERTICES[i[3]-1][2],
                                        V_TEXTURES[i[4]-1][0], V_TEXTURES[i[4]-1][1], V_TEXTURES[i[4]-1][2],
                                        V_NORMALS[i[5]-1][0], V_NORMALS[i[5]-1][1], V_NORMALS[i[5]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[6]-1][0], V_VERTICES[i[6]-1][1], V_VERTICES[i[6]-1][2],
                                        V_TEXTURES[i[7]-1][0], V_TEXTURES[i[7]-1][1], V_TEXTURES[i[7]-1][2],
                                        V_NORMALS[i[8]-1][0], V_NORMALS[i[8]-1][1], V_NORMALS[i[8]-1][2]};

                vector<float> polygon;

                for (float j : vertex1) {polygon.push_back(j);}
                for (float j : vertex2) {polygon.push_back(j);}
                for (float j : vertex3) {polygon.push_back(j);}

                for (float j : polygon) {V_FINAL.push_back(j);}
            }
        }

        // Only Normals ===========================================================================================================

        else if (normals) {
            for (vector<int> i : F_squares) {   

                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        V_NORMALS[i[1]-1][0], V_NORMALS[i[1]-1][1], V_NORMALS[i[1]-1][2]};
                
                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        V_NORMALS[i[3]-1][0], V_NORMALS[i[3]-1][1], V_NORMALS[i[3]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        V_NORMALS[i[5]-1][0], V_NORMALS[i[5]-1][1], V_NORMALS[i[5]-1][2]};

                vector<float> vertex4 = {V_VERTICES[i[6]-1][0], V_VERTICES[i[6]-1][1], V_VERTICES[i[6]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        V_NORMALS[i[7]-1][0], V_NORMALS[i[7]-1][1], V_NORMALS[i[7]-1][2]};

                vector<float> polygon1;
                vector<float> polygon2;

                for (float j : vertex1) {polygon1.push_back(j);}
                for (float j : vertex2) {polygon1.push_back(j);}
                for (float j : vertex3) {polygon1.push_back(j);}

                for (float j : vertex1) {polygon2.push_back(j);}
                for (float j : vertex3) {polygon2.push_back(j);}
                for (float j : vertex4) {polygon2.push_back(j);}

                for (float j : polygon1) {V_FINAL.push_back(j);}
                for (float j : polygon2) {V_FINAL.push_back(j);}

            }

            for (vector<int> i : F_triangles) {
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        V_NORMALS[i[1]-1][0], V_NORMALS[i[1]-1][1], V_NORMALS[i[1]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        V_NORMALS[i[3]-1][0], V_NORMALS[i[3]-1][1], V_NORMALS[i[3]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        V_NORMALS[i[5]-1][0], V_NORMALS[i[5]-1][1], V_NORMALS[i[5]-1][2]};

                vector<float> polygon;

                for (float j : vertex1) {polygon.push_back(j);}
                for (float j : vertex2) {polygon.push_back(j);}
                for (float j : vertex3) {polygon.push_back(j);}

                for (float j : polygon) {V_FINAL.push_back(j);}
            }
        }

        // Only Textures ===========================================================================================================

        else if (textures) {
            vector<vector<vector<float>>> polygons;
            vector<vector<float>> polygon_normals;
            vector<vector<float>> vertex_normals;

            for (vector<int> i : F_squares) {   
                
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2]};
                
                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2]};

                vector<float> vertex4 = {V_VERTICES[i[6]-1][0], V_VERTICES[i[6]-1][1], V_VERTICES[i[6]-1][2]};

                vector<vector<float>> final;
                final.push_back(vertex1);
                final.push_back(vertex2);
                final.push_back(vertex3);
                final.push_back(vertex4);

                polygons.push_back(final);
            }

            for (vector<int> i : F_triangles) {
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2]};

                vector<vector<float>> final;
                final.push_back(vertex1);
                final.push_back(vertex2);
                final.push_back(vertex3);

                polygons.push_back(final);
            }

            for (vector<vector<float>> polygon : polygons) {
                vector<float> normal {0.0f, 0.0f, 0.0f};

                if (polygon.size() == 3) {
                    vector<float> vec1 = SYSTEM_VECTOR_OPERATION(polygon[0], polygon[1], '-');
                    vector<float> vec2 = SYSTEM_VECTOR_OPERATION(polygon[0], polygon[2], '-');
                    normal[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
                    normal[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
                    normal[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
                }

                else if (polygon.size() == 4) {
                    vector<vector<float>> triangle;
                    triangle.push_back(polygon[0]);
                    triangle.push_back(polygon[1]);
                    triangle.push_back(polygon[2]);

                    vector<float> vec1 = SYSTEM_VECTOR_OPERATION(triangle[0], triangle[1], '-');
                    vector<float> vec2 = SYSTEM_VECTOR_OPERATION(triangle[0], triangle[2], '-');
                    normal[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
                    normal[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
                    normal[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
                }
                
                polygon_normals.push_back(normal);
            }
            
            for (vector<float> i : V_VERTICES) {
                vector<vector<float>> used_normals;

                for (int j = 0; j<polygons.size(); j+=1) {

                    if (polygons[j].size() == 3) {
                        if (polygons[j][0] == i || polygons[j][1] == i || polygons[j][2] == i) {used_normals.push_back(polygon_normals[j]);}
                    }
                    else if (polygons[j].size() == 4) {if (polygons[j][0] == i || polygons[j][1] == i || polygons[j][2] == i || polygons[j][3] == i) {used_normals.push_back(polygon_normals[j]);}}
                }

                vector<float> normal_count = {static_cast<float>(used_normals.size()), static_cast<float>(used_normals.size()), static_cast<float>(used_normals.size())};
                vector<float> normal_sum = {0.0f, 0.0f, 0.0f};

                for (vector<float> normal_current : used_normals) {normal_sum = SYSTEM_VECTOR_OPERATION(normal_sum, normal_current, '+');}

                vertex_normals.push_back(SYSTEM_VECTOR_OPERATION(normal_sum, normal_count, '/'));
            }

            for (vector<int> i : F_squares) {   

                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        V_TEXTURES[i[1]-1][0], V_TEXTURES[i[1]-1][1], V_TEXTURES[i[1]-1][2],
                                        vertex_normals[i[0]-1][0], vertex_normals[i[0]-1][1], vertex_normals[i[0]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2],
                                        V_TEXTURES[i[3]-1][0], V_TEXTURES[i[3]-1][1], V_TEXTURES[i[3]-1][2],
                                        vertex_normals[i[2]-1][0], vertex_normals[i[2]-1][1], vertex_normals[i[2]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2],
                                        V_TEXTURES[i[5]-1][0], V_TEXTURES[i[5]-1][1], V_TEXTURES[i[5]-1][2],
                                        vertex_normals[i[4]-1][0], vertex_normals[i[4]-1][1], vertex_normals[i[4]-1][2]};

                vector<float> vertex4 = {V_VERTICES[i[6]-1][0], V_VERTICES[i[6]-1][1], V_VERTICES[i[6]-1][2],
                                        V_TEXTURES[i[7]-1][0], V_TEXTURES[i[7]-1][1], V_TEXTURES[i[7]-1][2],
                                        vertex_normals[i[6]-1][0], vertex_normals[i[6]-1][1], vertex_normals[i[6]-1][2]};

                vector<float> polygon1;
                vector<float> polygon2;

                for (float j : vertex1) {polygon1.push_back(j);}
                for (float j : vertex2) {polygon1.push_back(j);}
                for (float j : vertex3) {polygon1.push_back(j);}

                for (float j : vertex1) {polygon2.push_back(j);}
                for (float j : vertex3) {polygon2.push_back(j);}
                for (float j : vertex4) {polygon2.push_back(j);}

                for (float j : polygon1) {V_FINAL.push_back(j);}
                for (float j : polygon2) {V_FINAL.push_back(j);}
            }

            for (vector<int> i : F_triangles) {
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        V_TEXTURES[i[1]-1][0], V_TEXTURES[i[1]-1][1], V_TEXTURES[i[1]-1][2],
                                        vertex_normals[i[0]-1][0], vertex_normals[i[0]-1][1], vertex_normals[i[0]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2],
                                        V_TEXTURES[i[3]-1][0], V_TEXTURES[i[3]-1][1], V_TEXTURES[i[3]-1][2],
                                        vertex_normals[i[2]-1][0], vertex_normals[i[2]-1][1], vertex_normals[i[2]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2],
                                        V_TEXTURES[i[5]-1][0], V_TEXTURES[i[5]-1][1], V_TEXTURES[i[5]-1][2],
                                        vertex_normals[i[4]-1][0], vertex_normals[i[4]-1][1], vertex_normals[i[4]-1][2]};

                vector<float> polygon;

                for (float j : vertex1) {polygon.push_back(j);}
                for (float j : vertex2) {polygon.push_back(j);}
                for (float j : vertex3) {polygon.push_back(j);}

                for (float j : polygon) {V_FINAL.push_back(j);}
            }
        }

        // Only Vertices ===========================================================================================================

        else {
            vector<vector<vector<float>>> polygons;
            vector<vector<float>> polygon_normals;
            vector<vector<float>> vertex_normals;

            for (vector<int> i : F_squares) {   
                
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2]};
                
                vector<float> vertex2 = {V_VERTICES[i[1]-1][0], V_VERTICES[i[1]-1][1], V_VERTICES[i[1]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2]};

                vector<float> vertex4 = {V_VERTICES[i[3]-1][0], V_VERTICES[i[3]-1][1], V_VERTICES[i[3]-1][2]};

                vector<vector<float>> final;
                final.push_back(vertex1);
                final.push_back(vertex2);
                final.push_back(vertex3);
                final.push_back(vertex4);

                polygons.push_back(final);
            }

            for (vector<int> i : F_triangles) {
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[1]-1][0], V_VERTICES[i[1]-1][1], V_VERTICES[i[1]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2]};

                vector<vector<float>> final;
                final.push_back(vertex1);
                final.push_back(vertex2);
                final.push_back(vertex3);

                polygons.push_back(final);
            }

            for (vector<vector<float>> polygon : polygons) {
                vector<float> normal {0.0f, 0.0f, 0.0f};

                if (polygon.size() == 3) {
                    vector<float> vec1 = SYSTEM_VECTOR_OPERATION(polygon[0], polygon[1], '-');
                    vector<float> vec2 = SYSTEM_VECTOR_OPERATION(polygon[0], polygon[2], '-');
                    normal[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
                    normal[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
                    normal[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
                }

                else if (polygon.size() == 4) {
                    vector<vector<float>> triangle;
                    triangle.push_back(polygon[0]);
                    triangle.push_back(polygon[1]);
                    triangle.push_back(polygon[2]);

                    vector<float> vec1 = SYSTEM_VECTOR_OPERATION(triangle[0], triangle[1], '-');
                    vector<float> vec2 = SYSTEM_VECTOR_OPERATION(triangle[0], triangle[2], '-');
                    normal[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
                    normal[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
                    normal[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
                }
                
                polygon_normals.push_back(normal);
            }
            
            for (vector<float> i : V_VERTICES) {
                vector<vector<float>> used_normals;

                for (int j = 0; j<polygons.size(); j+=1) {

                    if (polygons[j].size() == 3) {
                        if (polygons[j][0] == i || polygons[j][1] == i || polygons[j][2] == i) {used_normals.push_back(polygon_normals[j]);}
                    }
                    else if (polygons[j].size() == 4) {if (polygons[j][0] == i || polygons[j][1] == i || polygons[j][2] == i || polygons[j][3] == i) {used_normals.push_back(polygon_normals[j]);}}
                }

                vector<float> normal_count = {static_cast<float>(used_normals.size()), static_cast<float>(used_normals.size()), static_cast<float>(used_normals.size())};
                vector<float> normal_sum = {0.0f, 0.0f, 0.0f};

                for (vector<float> normal_current : used_normals) {normal_sum = SYSTEM_VECTOR_OPERATION(normal_sum, normal_current, '+');}

                vertex_normals.push_back(SYSTEM_VECTOR_OPERATION(normal_sum, normal_count, '/'));
            }

            for (vector<int> i : F_squares) {   

                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        vertex_normals[i[0]-1][0], vertex_normals[i[0]-1][1], vertex_normals[i[0]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        vertex_normals[i[2]-1][0], vertex_normals[i[2]-1][1], vertex_normals[i[2]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        vertex_normals[i[4]-1][0], vertex_normals[i[4]-1][1], vertex_normals[i[4]-1][2]};

                vector<float> vertex4 = {V_VERTICES[i[6]-1][0], V_VERTICES[i[6]-1][1], V_VERTICES[i[6]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        vertex_normals[i[6]-1][0], vertex_normals[i[6]-1][1], vertex_normals[i[6]-1][2]};

                vector<float> polygon1;
                vector<float> polygon2;

                for (float j : vertex1) {polygon1.push_back(j);}
                for (float j : vertex2) {polygon1.push_back(j);}
                for (float j : vertex3) {polygon1.push_back(j);}

                for (float j : vertex1) {polygon2.push_back(j);}
                for (float j : vertex3) {polygon2.push_back(j);}
                for (float j : vertex4) {polygon2.push_back(j);}

                for (float j : polygon1) {V_FINAL.push_back(j);}
                for (float j : polygon2) {V_FINAL.push_back(j);}
            }

            for (vector<int> i : F_triangles) {
                vector<float> vertex1 = {V_VERTICES[i[0]-1][0], V_VERTICES[i[0]-1][1], V_VERTICES[i[0]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        vertex_normals[i[0]-1][0], vertex_normals[i[0]-1][1], vertex_normals[i[0]-1][2]};

                vector<float> vertex2 = {V_VERTICES[i[2]-1][0], V_VERTICES[i[2]-1][1], V_VERTICES[i[2]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        vertex_normals[i[2]-1][0], vertex_normals[i[2]-1][1], vertex_normals[i[2]-1][2]};

                vector<float> vertex3 = {V_VERTICES[i[4]-1][0], V_VERTICES[i[4]-1][1], V_VERTICES[i[4]-1][2],
                                        0.0f, 0.0f, 0.0f,
                                        vertex_normals[i[4]-1][0], vertex_normals[i[4]-1][1], vertex_normals[i[4]-1][2]};

                vector<float> polygon;

                for (float j : vertex1) {polygon.push_back(j);}
                for (float j : vertex2) {polygon.push_back(j);}
                for (float j : vertex3) {polygon.push_back(j);}

                for (float j : polygon) {V_FINAL.push_back(j);}
            }
        }

        model.POLYGON_GROUPS.push_back(V_FINAL);
    }
    
    return model;
}