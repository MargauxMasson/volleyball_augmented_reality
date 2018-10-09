#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <GL/gl.h>
#include <GL/glew.h>

#include <QWidget>

#include "../../lib/3d/mat3.hpp"
#include "../../lib/3d/vec3.hpp"
#include "../../lib/3d/vec2.hpp"

#include "../../lib/mesh/mesh.hpp"
#include "../../lib/opengl/mesh_opengl.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include <vector>

#include <QtOpenGL/QGLWidget>
#include <QMouseEvent>
#include "opencv2/opencv.hpp"



using namespace cpe;

class myWidgetGL;





class scene
{



public:

    scene();



    /** \brief Method called only once at the beginning (load off files ...) */
    void load_scene();

    /** \brief Method called at every frame */
     void draw_scene();


    /** Set the pointer to the parent Widget */
    void set_widget(myWidgetGL* widget_param);

        int detection(int argc,    CvCapture* capture,int key);

    void mouvement_ball();
    void mouvement_hand1();
    void mouvement_hand2();
    void mouvement_kirby();
    void bouger_hands(int key);


    int val_compteur=0;




private:

    /** Load a texture from a given file and returns its id */
    GLuint load_texture_file(std::string const& filename);

    /** Access to the parent object */
    myWidgetGL* pwidget;

    /** Default id for the texture (white texture) */
    GLuint texture_default;

    /** The id of the shader do draw meshes */
    GLuint shader_program_id;


    // Data of the scene
    cpe::mesh mesh_dinosaur;
    cpe::mesh_opengl mesh_dinosaur_opengl;
    GLuint texture_dinosaur;

    cpe::mesh mesh_camel;
    cpe::mesh_opengl mesh_camel_opengl;

    cpe::mesh mesh_ground;
    cpe::mesh_opengl mesh_ground_opengl;
    GLuint texture_ground;

    cpe::mesh mesh_piste;
    cpe::mesh_opengl mesh_piste_opengl;
    GLuint texture_piste;


    cpe::mesh mesh_cylindre;
    cpe::mesh_opengl mesh_cylindre_opengl;
    GLuint texture_cylindre;

    cpe::mesh mesh_cylindre2;
    cpe::mesh_opengl mesh_cylindre2_opengl;
    GLuint texture_cylindre2;

    cpe::mesh mesh_ball;
    cpe::mesh_opengl mesh_ball_opengl;
    GLuint texture_ball;

    cpe::mesh mesh_kirby;
    cpe::mesh_opengl mesh_kirby_opengl;
    GLuint texture_kirby;



    cpe::mesh mesh_filet[28];            //5+23
    cpe::mesh_opengl mesh_filet_opengl[28];
    GLuint texture_filet;

    cpe::mesh mesh_joueur_hand1;
    cpe::mesh_opengl mesh_joueur_hand1_opengl;
    GLuint texture_joueur_hand1;

    cpe::mesh mesh_joueur_hand2;
    cpe::mesh_opengl mesh_joueur_hand2_opengl;
    GLuint texture_joueur_hand2;

    cpe::mesh mesh_beach;
    cpe::mesh_opengl mesh_beach_opengl;
    GLuint texture_beach;



    /** The time integration step */
    float dt;

    /** The speed of the ball */
    vec3 speed;

    vec3 collision_sand();
    cpe::vec3 collision_kirby();
    vec3 collision_hands();




};



#endif
