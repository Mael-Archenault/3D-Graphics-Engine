#include "src/include/SDL2/SDL.h"
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "drawing_functions.h"



#undef main

const int WIDTH = 640;
const int HEIGHT = 480;
const int TARGET_FRAMERATE = 60;

const int tritowatch = 12;
float objectDistance = 30.0f;
const int modelColor[4] = {255, 0, 0, 255};

typedef struct vec3D {
    float x, y, z;
}Vec3D;

typedef struct triangle {
    Vec3D p[3];
    float color[4];
}Triangle;

void displayTriangle(Triangle *triangle){
    printf(" %.5f %.5f %.5f   %.5f %.5f %.5f   %.5f %.5f %.5f", triangle->p[0].x, triangle->p[0].y, triangle->p[0].z, triangle->p[1].x, triangle->p[1].y, triangle->p[1].z, triangle->p[2].x, triangle->p[2].y, triangle->p[2].z);
    printf("\n");
}

typedef struct mesh {
    int triangles_number;
    int capacity;
    Triangle tris[10000];
}Mesh;

typedef struct mat4x4{
    float m[4][4];
}Mat4x4;

void displayMatrix(Mat4x4* m){
    for (int i = 0; i <4; i++){
        for (int j = 0; j < 4; j++){
            printf("%.5f ", m->m[i][j]);
        }
        printf("\n");
    }
}

int compare_distance(const void* a, const void* b) {
    Triangle t1 = *(Triangle*)a;
    Triangle t2 = *(Triangle*)b;
    float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z)/3.0f;
    float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z)/3.0f;


    if (z1 > z2) {
        return -1;
    }
    else if (z1 < z2) {
        return 1;
    }
    else {
        return 0;
    }
}
Mesh openOBJ(){
    FILE *file = fopen("bear.obj", "r");

    Vec3D vertices[10000];
    Triangle faces[10000];
    int vertex_count = 0;
    int face_count = 0;
    char line[128];
    while (fgets(line, sizeof(line), file)){


        if (line[0] == 'v' && line[1] == ' '){
            Vec3D v;
            sscanf(line, "v %f %f %f",&v.x, &v.y, &v.z);
            vertices[vertex_count++] = v;

           
           
        }
        if (line[0] == 'f' && line[1] == ' '){
            Triangle tri;
            int indexes[3];
            sscanf(line, "f %d %d %d",&indexes[0], &indexes[1], &indexes[2]);
            tri.p[0] = vertices[indexes[0] - 1];
            tri.p[1] = vertices[indexes[1] - 1];
            tri.p[2] = vertices[indexes[2] - 1];
            
            faces[face_count++] = tri;
            
           
    }
    }

    //printf("%d %d \n", vertex_count, face_count);

    Mesh mesh;
    mesh.triangles_number = face_count;
    mesh.capacity = face_count;
    for (int i = 0; i < face_count; i++){
        if (i==tritowatch){
            displayTriangle(&faces[i]);
        }
        mesh.tris[i] = faces[i];
        
    }

    return mesh;



}
Mesh createMeshCube(){

     //South face
    Triangle t1 = { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f };
    Triangle t2 = { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f };

    //North face
    Triangle t3 = { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f };
    Triangle t4 = { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f };

    //Up face
    Triangle t5 = { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f };
    Triangle t6 = { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f };

    //Down face
    Triangle t7 = { 0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f };
    Triangle t8 = { 0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f };

    //Left face
    Triangle t9 = { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f };
    Triangle t10 = { 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f };

    //Right face
    Triangle t11 = { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f };
    Triangle t12 = { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f };


    Mesh cubeMesh = {12, 12, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
    return cubeMesh;
    
}

void multiplyMatrixVect(Vec3D * vin, Vec3D * vout, Mat4x4 m){
        vout->x = m.m[0][0]*vin->x + m.m[1][0]*vin->y + m.m[2][0]*vin->z + m.m[3][0];
        vout->y = m.m[0][1]*vin->x + m.m[1][1]*vin->y + m.m[2][1]*vin->z + m.m[3][1];
        vout->z = m.m[0][2]*vin->x + m.m[1][2]*vin->y + m.m[2][2]*vin->z + m.m[3][2];

        float w = m.m[0][3]*vin->x + m.m[1][3]*vin->y + m.m[2][3]*vin->z + m.m[3][3];
        
        if (w != 0.0f){
            vout->x /= w;
            vout->y /= w;
            vout->z /= w;
        }

    };

void setupRotationMatrix(Mat4x4 *m, float itheta, int iaxis){
    if (iaxis == 0){
        m->m[0][0] = 1;
        m->m[1][1] = cosf(itheta);
        m->m[2][2] = cosf(itheta);
        m->m[1][2] = -sinf(itheta);
        m->m[2][1] = sinf(itheta);
        m->m[3][3] = 1;
    }
    if (iaxis == 1){
        
        m->m[0][0] = cosf(itheta);
        m->m[1][1] = cosf(itheta);
        m->m[0][1] = -sinf(itheta);
        m->m[1][0] = sinf(itheta);
        m->m[2][2] = 1;
        m->m[3][3] = 1;
    }
}
        
int main(int argc, char *argv[]){
    
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)HEIGHT / (float)WIDTH ;
    float fFovRad = 1.0f/ tanf(fFov * 0.5f / 180.0f * 3.14159f);
    float fQ = fFar / (fFar - fNear);


    Mat4x4 projectionMatrix = { 0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0};
    Mat4x4 rotationMatrixX = { 0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0};
    Mat4x4 rotationMatrixZ = { 0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0};

    projectionMatrix.m[0][0] = fAspectRatio * fFovRad;
    projectionMatrix.m[1][1] = fFovRad;
    projectionMatrix.m[2][2] = fQ;
    projectionMatrix.m[3][2] = -fNear * fQ;
    projectionMatrix.m[2][3] = 1.0f;

    Vec3D vcamera = {0.0f,0.0f,0.0f};

    Vec3D light_direction = {0.0f,0.0f,-1.0f};
    float l = sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
    light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

    Mesh cubeMesh = openOBJ();

    displayTriangle(&cubeMesh.tris[tritowatch]);

    float thetaX = 17*3.14/180;
    float thetaZ = 85*3.14/180;

    setupRotationMatrix(&rotationMatrixX, thetaX, 0);
    setupRotationMatrix(&rotationMatrixZ, thetaZ, 1);

    Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
    
    
    

    

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    

    int frameStart, frameTime;
    
    
    // cubeMesh->triangles_number = 4;
    int running = 1;
    
    while (running==1)
    {
        frameStart = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
        SDL_RenderClear(renderer);
        
        Triangle triangleToRaster[4000];
        int facingTriangles = 0;
        float luminance;

        //printf("il y a %d triangles \n", cubeMesh.triangles_number);

        for(int i = 0; i <cubeMesh.triangles_number; i++){

            
            multiplyMatrixVect(&cubeMesh.tris[i].p[0], &triRotatedZ.p[0], rotationMatrixZ);
            multiplyMatrixVect(&cubeMesh.tris[i].p[1], &triRotatedZ.p[1], rotationMatrixZ);
            multiplyMatrixVect(&cubeMesh.tris[i].p[2], &triRotatedZ.p[2], rotationMatrixZ);

            multiplyMatrixVect(&triRotatedZ.p[0], &triRotatedZX.p[0], rotationMatrixX);
            multiplyMatrixVect(&triRotatedZ.p[1], &triRotatedZX.p[1], rotationMatrixX);
            multiplyMatrixVect(&triRotatedZ.p[2], &triRotatedZX.p[2], rotationMatrixX);



            
            

            triTranslated = triRotatedZX;
            triTranslated.p[0].z = triRotatedZX.p[0].z + objectDistance;
            triTranslated.p[1].z = triRotatedZX.p[1].z + objectDistance;
            triTranslated.p[2].z = triRotatedZX.p[2].z + objectDistance;


            Vec3D normal, line1, line2;
            line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
            line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
            line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

            line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
            line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
            line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

            normal.x = line1.y*line2.z - line1.z*line2.y;
            normal.y = line1.z*line2.x - line1.x*line2.z;
            normal.z = line1.x*line2.y - line1.y*line2.x;

            float l = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
            normal.x /= l;
            normal.y /= l;
            normal.z /= l;

            luminance = light_direction.x*normal.x + light_direction.y*normal.y + light_direction.z*normal.z;
            luminance = (luminance < 0.0f)?0.0f : luminance;
    
            if (normal.x*(triTranslated.p[0].x - vcamera.x) +
                normal.y*(triTranslated.p[0].y - vcamera.y) +
                normal.z*(triTranslated.p[0].z - vcamera.z) < 0.0f)
            {


                
            
                multiplyMatrixVect(&triTranslated.p[0], &triProjected.p[0], projectionMatrix);
                multiplyMatrixVect(&triTranslated.p[1], &triProjected.p[1], projectionMatrix);
                multiplyMatrixVect(&triTranslated.p[2], &triProjected.p[2], projectionMatrix);

                triProjected.p[0].x += 1.0f;triProjected.p[0].y += 1.0f;
                triProjected.p[1].x += 1.0f;triProjected.p[1].y += 1.0f;
                triProjected.p[2].x += 1.0f;triProjected.p[2].y += 1.0f;
                
        

                triProjected.p[0].x *= 0.5f * (float)WIDTH;
                triProjected.p[0].y *= 0.5f * (float)HEIGHT;
                triProjected.p[1].x *= 0.5f * (float)WIDTH;
                triProjected.p[1].y *= 0.5f * (float)HEIGHT;
                triProjected.p[2].x *= 0.5f * (float)WIDTH;
                triProjected.p[2].y *= 0.5f * (float)HEIGHT;

                triProjected.color[0] = modelColor[0]*luminance;
                triProjected.color[1] = modelColor[1]*luminance;
                triProjected.color[2] = modelColor[2]*luminance;
                triProjected.color[3] = 255;


                triangleToRaster[facingTriangles] = triProjected;
                facingTriangles++;

                //printf("%d, %.5f \n",i,triProjected.p[1].x);

                //printf("%f %f %f %f %f %f %f \n", triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y, triProjected.p[2].x, triProjected.p[2].y);
                
                
                //drawTriangle(renderer, (int)triProjected.p[0].x, (int)triProjected.p[0].y, (int)triProjected.p[1].x, (int)triProjected.p[1].y, (int)triProjected.p[2].x, (int)triProjected.p[2].y);
                //printf("%f \n" , luminance);

                //printf("%d   %d %d    %d %d    %d %d     \n", i, (int)triProjected.p[0].x,(int)triProjected.p[0].y,(int)triProjected.p[1].x,(int)triProjected.p[1].y,(int)triProjected.p[2].x,(int)triProjected.p[2].y);
            
            
            }

        }

        // sorting triangles to display
        // printf("\n %d\n\n", facingTriangles);  
        qsort(triangleToRaster, facingTriangles, sizeof(Triangle),compare_distance);
        // for (int i = 0; i <facingTriangles; i++) {
        //     float zi = (triangleToRaster[i].p[0].z + triangleToRaster[i].p[1].z + triangleToRaster[i].p[2].z)/3.0f;
        //     printf(" %f ", zi);
        // }
        // printf("\n\n\n");


        for (int i = 0; i < facingTriangles; i++){
            Triangle tri = triangleToRaster[i];
            fillTriangle(renderer, (int)tri.p[0].x,(int)tri.p[0].y, (int)tri.p[1].x,(int)tri.p[1].y, (int)tri.p[2].x,(int)tri.p[2].y, tri.color);
            drawTriangle(renderer, (int)tri.p[0].x,(int)tri.p[0].y, (int)tri.p[1].x,(int)tri.p[1].y, (int)tri.p[2].x,(int)tri.p[2].y);
        }

        
        
        // drawTriangle(renderer, 0,0,100,10,10,100);
        SDL_RenderPresent(renderer);

        
        thetaZ += 0.01f;
        thetaX = thetaX + 0.01f;
        setupRotationMatrix(&rotationMatrixX, thetaX, 0);
        setupRotationMatrix(&rotationMatrixZ, thetaZ, 1);

        // objectDistance += 0.1f;
        // printf("%f \n", objectDistance);
        
        
        
        frameTime = SDL_GetTicks() - frameStart;

        // displayTriangle(&cubeMesh->tris[0]);
        if (1000 / TARGET_FRAMERATE > frameTime)
        {
            SDL_Delay(1000 / TARGET_FRAMERATE - frameTime);
        }

        
    }

    return 0;
}


