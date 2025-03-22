#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"
#include <cmath>


Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Texture* Scene::normal = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

vector<vec3>* playertangents = nullptr;

// custom variables
float cur_time = 0.0f;  // custom time variable
//mat4 matAnimationHead[3];
float cur_time_head = 0.0f;

bool Scene::buttonFlag = true;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "colorMap", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);

    playertangents = new vector<vec3>(playerVertices.size(), vec3(0.0f));
    Scene::player = new Object(program, material, playerVertices, playerIndices, *playertangents);

    player->calculateTangents(*playertangents);
    player->load(playerVertices, playerIndices, *playertangents);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::normal = new Texture(Scene::program, 1, "normalMap", playerNormal, playerSize);
    normal->update();


//    Scene::lineColor = new Texture(Scene::program, 0, "ColorMap", {{0xFF, 0x00, 0x00}}, 1);
//    Scene::lineMaterial = new Material(Scene::program, lineColor);
//    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);

    // provide lightDir variable
    GLint lightDirtLoc = glGetUniformLocation(program->get(), "lightDir");
    if (lightDirtLoc >= 0) glUniform3f(lightDirtLoc, camera->eye.x,camera->eye.y, camera->eye.z);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

void Scene::update(float deltaTime) {
    Scene::program->use();
    Scene::camera->update();

    /*
     *
     * Write your code. (final ver.)
     *
     */
    // animation matrices
    mat4 matLocalTransform[28]; // Ml
    mat4 matAnimation[28];      // Ma
    mat4 matToParent[28];      // Mp
    mat4 matToDresspose[28];    // Md
    mat4 matToDresspose_inverse[28];

    // time variables
    float w = 0.0f;

    float cur_time_ceiling = 0.0f;
    float cur_time_floor = 0.0f;
    float interpolation_w = 0.0f;

    float cur_time_ceiling_head = 0.0f;  // head
    float cur_time_floor_head = 0.0f;
    float interpolation_w_head = 0.0f;

    // matrices & vectors for interpolation
    mat4 matLocalTransform_ceiling[28];
    mat4 matLocalTransform_floor[28];

    quat matLocalTransform_ceiling_q[28];
    quat matLocalTransform_floor_q[28];
    quat matLocalTransform_q[28];

    vec3 matLocalTransform_ceiling_t[28];
    vec3 matLocalTransform_floor_t[28];
    vec3 matLocalTransform_t[28];

    // update time
    cur_time += deltaTime;
    if (cur_time >= 4.0f)
        cur_time = fmod(cur_time, 4.0f);
    cur_time_ceiling = (int)ceil(cur_time);
    if (cur_time_ceiling >= 4.0f)
        cur_time_ceiling = fmod(cur_time_ceiling, 4.0f);
    cur_time_floor = (int)floor(cur_time);
    interpolation_w = cur_time - cur_time_floor;

    if (buttonFlag)
        cur_time_head += deltaTime;
    else
        cur_time_head = cur_time_head;

    if (cur_time_head >= 4.0f)
        cur_time_head = fmod(cur_time_head, 4.0f);
    cur_time_ceiling_head = (int)ceil(cur_time_head);
    if (cur_time_ceiling_head >= 4.0f)
        cur_time_ceiling_head = fmod(cur_time_ceiling_head, 4.0f);
    cur_time_floor_head = (int)floor(cur_time_head);
    interpolation_w_head = cur_time_head - cur_time_floor_head;


//    LOG_PRINT_DEBUG("cur_time : %f", cur_time);
//    LOG_PRINT_DEBUG("interpolation_w : %f", interpolation_w);
//    LOG_PRINT_DEBUG("cur_time_ceiling : %f", cur_time_ceiling);
//    LOG_PRINT_DEBUG("cur_time_floor : %f", cur_time_floor);
    LOG_PRINT_DEBUG("motion %d", buttonFlag);
    LOG_PRINT_DEBUG("cur_time : %f", cur_time_head);
    LOG_PRINT_DEBUG("interpolation_w : %f", interpolation_w_head);
    LOG_PRINT_DEBUG("cur_time_ceiling : %f", cur_time_ceiling_head);
    LOG_PRINT_DEBUG("cur_time_floor : %f", cur_time_floor_head);




    // Initialize Mp, Md, Md-1
    for (int i = 0; i < jOffsets.size(); i++){
        matToParent[i] = (translate(mat4(1.0f), jOffsets[i]));
    }

    matToDresspose[0] = mat4(1.0f); // root
    matToDresspose_inverse[0] = inverse(matToDresspose[0]);
    for (int i = 1; i < 28; i++){
        matToDresspose[i] = matToDresspose[jParents[i]] * matToParent[i];
        matToDresspose_inverse[i] = inverse(matToDresspose[i]);
    }






    // run 4 keyframe (w/o interpolation for debugging)
//    vector<float> keyframe = motions[cur_time];
//
//    for (int i = 0; i < 28; i++) {
//        //int i = 0;                      // for debugging
//        mat4 rotate_y, rotate_x, rotate_z;
//
//        rotate_x = rotate(radians(keyframe[(int) (3 + 3 * i)]), vec3(1.0f, 0.0f, 0.0f));
//        rotate_y = rotate(radians(keyframe[(int) (4 + 3 * i)]), vec3(0.0f, 1.0f, 0.0f));
//        rotate_z = rotate(radians(keyframe[(int) (5 + 3 * i)]), vec3(0.0f, 0.0f, 1.0f));
//
//        if (i == 0) {
//            mat4 trans;
//            trans = translate(mat4(1.0f), vec3(keyframe[0], keyframe[1], keyframe[2]));
//            matLocalTransform[i] = trans * rotate_z * rotate_x * rotate_y;
//        } else
//            matLocalTransform[i] = rotate_z * rotate_x * rotate_y;
//    }
//
//    // Ma
//    matAnimation[0] = mat4(1.0f);
//    for (int i = 1; i < 28; i++){
//        matAnimation[i] = matAnimation[jParents[i]] * matToParent[i] * matLocalTransform[i];
//    }

    vector<float> keyframe_ceiling = motions[cur_time_ceiling];
    vector<float> keyframe_floor = motions[cur_time_floor];


    for (int i = 0; i < 28; i++) {
        mat4 rotate_y, rotate_x, rotate_z;

        rotate_x = rotate(radians(keyframe_ceiling[(int) (3 + 3 * i)]), vec3(1.0f, 0.0f, 0.0f));
        rotate_y = rotate(radians(keyframe_ceiling[(int) (4 + 3 * i)]), vec3(0.0f, 1.0f, 0.0f));
        rotate_z = rotate(radians(keyframe_ceiling[(int) (5 + 3 * i)]), vec3(0.0f, 0.0f, 1.0f));

        if (i == 0) {
            mat4 trans;
            trans = translate(mat4(1.0f), vec3(keyframe_ceiling[0], keyframe_ceiling[1], keyframe_ceiling[2]));
            matLocalTransform_ceiling[i] = trans * rotate_z * rotate_x * rotate_y;
        } else {
            if (i >= 13 && i < 16)
                continue;
            matLocalTransform_ceiling[i] = rotate_z * rotate_x * rotate_y;
        }
    }

    for (int i = 0; i < 28; i++) {
        mat4 rotate_y, rotate_x, rotate_z;

        rotate_x = rotate(radians(keyframe_floor[(int) (3 + 3 * i)]), vec3(1.0f, 0.0f, 0.0f));
        rotate_y = rotate(radians(keyframe_floor[(int) (4 + 3 * i)]), vec3(0.0f, 1.0f, 0.0f));
        rotate_z = rotate(radians(keyframe_floor[(int) (5 + 3 * i)]), vec3(0.0f, 0.0f, 1.0f));

        if (i == 0) {
            mat4 trans;
            trans = translate(mat4(1.0f), vec3(keyframe_floor[0], keyframe_floor[1], keyframe_floor[2]));
            matLocalTransform_floor[i] = trans * rotate_z * rotate_x * rotate_y;
        } else{
            if (i >= 13 && i < 16)
                continue;
            matLocalTransform_floor[i] = rotate_z * rotate_x * rotate_y;
        }
    }



    // head
    vector<float> keyframe_ceiling_head = motions[cur_time_ceiling_head];
    vector<float> keyframe_floor_head = motions[cur_time_floor_head];


    for (int i = 13; i < 16; i++) {
        mat4 rotate_y, rotate_x, rotate_z;

        rotate_x = rotate(radians(keyframe_ceiling_head[(int) (3 + 3 * i)]), vec3(1.0f, 0.0f, 0.0f));
        rotate_y = rotate(radians(keyframe_ceiling_head[(int) (4 + 3 * i)]), vec3(0.0f, 1.0f, 0.0f));
        rotate_z = rotate(radians(keyframe_ceiling_head[(int) (5 + 3 * i)]), vec3(0.0f, 0.0f, 1.0f));

        matLocalTransform_ceiling[i] = rotate_z * rotate_x * rotate_y;
    }

    for (int i = 13; i < 16; i++) {
        mat4 rotate_y, rotate_x, rotate_z;

        rotate_x = rotate(radians(keyframe_floor_head[(int) (3 + 3 * i)]), vec3(1.0f, 0.0f, 0.0f));
        rotate_y = rotate(radians(keyframe_floor_head[(int) (4 + 3 * i)]), vec3(0.0f, 1.0f, 0.0f));
        rotate_z = rotate(radians(keyframe_floor_head[(int) (5 + 3 * i)]), vec3(0.0f, 0.0f, 1.0f));

        matLocalTransform_floor[i] = rotate_z * rotate_x * rotate_y;
    }





    // Calculating quaternion q & t
    for (int i = 0; i < 28; i++){
        if (i >= 13 && i < 16)
            w = interpolation_w_head;
        else
            w = interpolation_w;

        // q
        matLocalTransform_ceiling_q[i] = quat_cast(mat3(matToParent[i] * matLocalTransform_ceiling[i]));
        matLocalTransform_floor_q[i] = quat_cast(mat3(matToParent[i] * matLocalTransform_floor[i]));

        matLocalTransform_q[i] = slerp(matLocalTransform_floor_q[i], matLocalTransform_ceiling_q[i], w);

        //t
        for (int j = 0; j < 3; j++)
        {
            matLocalTransform_ceiling_t[i][j] = ((matToParent[i] * matLocalTransform_ceiling[i])[3][j]);
            matLocalTransform_floor_t[i][j] = ((matToParent[i] * matLocalTransform_floor[i])[3][j]);
            matLocalTransform_t[i][j] = mix(matLocalTransform_floor_t[i][j], matLocalTransform_ceiling_t[i][j], w);

        }
    }




    // Calculating interpolated Ma
    for (int i = 0; i < 28; i++){
        mat4 matLocalTransform_interpolated = mat4_cast(matLocalTransform_q[i]);
        for (int j = 0; j < 3; j++)
            matLocalTransform_interpolated[3][j] = matLocalTransform_t[i][j];

        if (i == 0) // root
            matAnimation[i] = mat4(1.0f);
        else
            matAnimation[i] = matAnimation[jParents[i]] * matLocalTransform_interpolated;
    }


    // Line Drawer
//    glLineWidth(20);
//    Scene::lineDraw->load({{vec3(-20.0f, 0.0f, 0.0f)}, {vec3(20.0f, 0.0f, 0.0f)}}, {0, 1});
//    Scene::lineDraw->draw();






    // Skinning & Calculating the location in the character space

    vector<Vertex> playerVertices_new;
    for (int i = 0; i < playerVertices.size(); i++)
    {
        playerVertices_new.push_back(playerVertices[i]);    // original info.

        ivec4 bone = playerVertices[i].bone;
        vec4 weight = playerVertices[i].weight;
        vec4 pos_weighted = {0.0f, 0.0f, 0.0f, 0.0f};

        vec4 pos[4];
        for (int j = 0; j < 4; j++)
            if (bone[j] == -1)
                pos[j] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
            else
                pos[j] = matAnimation[bone[j]] * matToDresspose_inverse[bone[j]] * vec4(playerVertices[i].pos, 1.0f);

        for (int j = 0; j < 4; j++)
            pos_weighted += pos[j] * weight[j];

        playerVertices_new[i].pos = vec3(pos_weighted[0], pos_weighted[1], pos_weighted[2]);
    }
    // ======================================================

    player->calculateTangents(*playertangents);
    Scene::player->load(playerVertices_new, playerIndices, *playertangents);
    Scene::player->draw();

}

void Scene::setButtonFlag(bool flag)
{
    Scene::buttonFlag = flag;
}