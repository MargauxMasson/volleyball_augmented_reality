#include <QApplication>

#include "../interface/myWidgetGL.hpp"
#include "../interface/myWindow.hpp"
#include "../../lib/interface/application_qt.hpp"

#include <QCoreApplication>
#include <GL/glew.h>
#include <QMouseEvent>

#include "scene.hpp"

#include "../../lib/opengl/glutils.hpp"
#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"
#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include "opencv2/opencv.hpp"

#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>





using namespace cpe;
CvPoint min_loc, max_loc;
CvPoint min_loc2, max_loc_2;

int etat=0;


////////////////////////////////////DETECTION MAIN////////////////////////////////////////////////

int scene::detection(int argc, CvCapture* capture, int key){
    /////////////////////////////INITIALISATION DES VALEURS ET DONNES //////////////////////////////////
    //définition des images
    IplImage *src=cvCreateImage(cvSize(640,480), 8, 3); //960//720
    IplImage *templ = cvCreateImage(cvSize(150,200), 8, 3);
    IplImage *templ2 = cvCreateImage(cvSize(150,200), 8, 3);

    //définition de la taililetle(largeur, hauteur) de l'image ftmp
    int iwidth = abs(src->width - templ->width )+ 1;
    int iheight = abs(src->height - templ->height) + 1 ;
    IplImage *ftmp = cvCreateImage(cvSize(iwidth,iheight),IPL_DEPTH_32F,1);

    //définition de la taille(largeur, hauteur) de l'image ftmp2
    int iwidth2 = abs(src->width - templ2->width )+ 1;
    int iheight2 = abs(src->height - templ2->height) + 1 ;
    IplImage *ftmp2 = cvCreateImage(cvSize(iwidth2,iheight2),IPL_DEPTH_32F,1);

    //définitions de points
    cv::Point cadre_pt1 = cvPoint((src->width - templ->width ),(src->height - templ->height )/16);
    cv::Point cadre_pt2 = cvPoint(cadre_pt1.x + templ->width , cadre_pt1.y + templ->height );
    cv::Point cadre_pt1_2=cvPoint( ( src->width - templ2->width) /4 , (src->height - templ2->height)/4);
    cv::Point cadre_pt2_2 = cvPoint(cadre_pt1_2.x + templ2->width , cadre_pt1_2.y + templ2->height);
    double seuil = 20.0;

    //Qt main application (handles exceptions)
    application_qt app(argc, NULL);

    //The main window
    myWindow win;
    win.show();


    //////////////////////////////////GESTION CLAVIER//////////////////////////////////

    while (1)
    {
        //capture ecran
        src = cvRetrieveFrame(capture);

        //applique le filtre médian pour réduire le bruit
        cvSmooth(src,src,CV_MEDIAN,3);

        switch (key)
        {

        case 'q':           //quitter jeu
        case 'Q':
        case 1048689:
            exit(0);
            break;


        //////////////////////////////////1ERE MAIN : capture //////////////////////////////////

        case 1048679://touche g => main gauche
        {

            //dessine un rectangle noir autour de la zone qui est "photographiée"
            cvRectangle(src, cadre_pt1,cadre_pt2, cvScalar(0,0,0));

            //définition une région d'intérêt ROI
            CvRect roi = cvRect (cadre_pt1.x,cadre_pt1.y,templ->width,templ->height);

            //fixe la ROI à l'image
            cvSetImageROI(src,roi);

            //copie le rectangle sélectionné de 'src' à 'templ'
            cvCopy(src,templ);

            // libérer la Region Of Interest de 'src'
            cvResetImageROI(src);

            etat+=1;


        }break;

        //////////////////////////////////2EME MAIN : capture //////////////////////////////////

        case 1048676://touche d => main droite
        {
            //dessine un rectangle noir autour de la zone qui est "photographiée"
            cvRectangle(src, cadre_pt1_2,cadre_pt2_2, cvScalar(0,0,0));

            //définition une région d'intérêt ROI2
            CvRect roi2 = cvRect (cadre_pt1_2.x,cadre_pt1_2.y,templ2->width,templ2->height);

            //fixe la ROI2 à l'image
            cvSetImageROI(src,roi2);

            //copie le rectangle sélectionné de 'src' à 'templ2'
            cvCopy(src,templ2);

            // libérer la Region Of Interest de 'src'
            cvResetImageROI(src);

            etat+=1;


        }break;
        }
        //////////////////////////////////1ERE MAIN : CORRELATION//////////////////////////////////
        cvMatchTemplate( src, templ, ftmp, CV_TM_CCOEFF_NORMED );

        //retrouver dans 'ftmp' les coordonnées du point ayant une valeur maximale
        double min_val, max_val;
        //CvPoint min_loc, max_loc;
        cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc);

        CvPoint max_loc2 = cvPoint(max_loc.x + templ->width, max_loc.y + templ->height);//définir le deuxième point en fonction de la taille du template

        // 'seuil' appartient à [0 1]
        seuil = (double) seuil / 100.0;

        //si la valeur maximale de 'ftmp' est supérieure au 'seuil'
        //dessiner un rectangle rouge utilisant les coordonnées des deux points 'max_loc' et 'max_loc2'
        if( max_val > seuil && max_val!=1 ) {cvRectangle(src, max_loc,max_loc2, cvScalar(0,0,255));}

        //////////////////////////////////2EME MAIN : CORRELATION//////////////////////////////////

        cvMatchTemplate( src, templ2, ftmp2, CV_TM_CCOEFF_NORMED );

        //retrouver dans 'ftmp' les coordonnées du point ayant une valeur maximale
        double min_val2, max_val2;
        cvMinMaxLoc(ftmp2, &min_val2, &max_val2, &min_loc2, &max_loc_2);

        CvPoint max_loc22 = cvPoint(max_loc_2.x + templ->width, max_loc_2.y + templ2->height);
        if(max_val2 > seuil && max_val2!=1 ) {cvRectangle(src, max_loc_2,max_loc22, cvScalar(0,255,0));}


        //////////////////////////////////AFFICHAGE//////////////////////////////

        cvNamedWindow( "out", CV_WINDOW_AUTOSIZE );
        cvShowImage( "out", src );

        // On attend 5 ms
        key = cvWaitKey(50);

        // On essaye de capturer les frames suivantes
        if(!cvGrabFrame( capture )) key = 27;

    }

    // On détruit les fenêtres créées
    cvDestroyAllWindows();

    // On détruit les captures
    cvReleaseCapture( &capture );

    return app.exec();
}



////////////////////////////////////////SCENE//////////////////////////////////////////////

void scene::load_scene()
{

    //*****************************************//
    // Preload default structure               //
    //*****************************************//
    texture_default = load_texture_file("data/white.jpg");
    shader_program_id = read_shader("shaders/shader_mesh.vert",
                                    "shaders/shader_mesh.frag");


    //*****************************************//
    // OBJ Mesh                                //
    //*****************************************//
    texture_ball=load_texture_file("data/Ball1.png");
    mesh_ball=load_mesh_file("data/Ball.obj");
    mesh_ball.transform_apply_translation({0.8f,0.4f,4.4f});            //ballon
    mesh_ball.transform_apply_scale(0.2f);
    mesh_ball_opengl.fill_vbo(mesh_ball);

    texture_kirby=load_texture_file("data/kirby.png");
    mesh_kirby=load_mesh_file("data/kirby.obj");
    mesh_kirby.transform_apply_scale(0.02f);
    mesh_kirby.transform_apply_translation({0.2f,-0.25f,-1.0f});        // joueur adverse
    mesh_kirby_opengl.fill_vbo(mesh_kirby);

    texture_beach=load_texture_file("data/BeachMap/Material6.png");
    mesh_beach=load_mesh_file("data/BeachMap/Beach.obj");               //terrain extérieur
    mesh_beach.transform_apply_scale(1.0f);
    mesh_beach.transform_apply_translation({3.7f,-0.3f,-0.5f});
    mesh_beach.transform_apply_rotation({0.0f,1.0f,0.0f},M_PI/2);
    mesh_beach_opengl.fill_vbo(mesh_beach);


    texture_joueur_hand1=load_texture_file("data/white.jpg");       // main gauche
    mesh_joueur_hand1=load_mesh_file("data/hand.obj");
    mesh_joueur_hand1.transform_apply_scale(0.2f);
    mesh_joueur_hand1.transform_apply_translation({0.8f,-0.25f,1.0f});
    mesh_joueur_hand1.transform_apply_rotation({0.0f,0.0f,1.0f},-M_PI);
    mesh_joueur_hand1_opengl.fill_vbo(mesh_joueur_hand1);


    texture_joueur_hand2=load_texture_file("data/white.jpg");
    mesh_joueur_hand2=load_mesh_file("data/hand.obj");
    mesh_joueur_hand2.transform_apply_scale(0.2f);
    mesh_joueur_hand2.transform_apply_translation({-0.1f,-0.25f,1.0f});     //main_droite
    mesh_joueur_hand2.transform_apply_rotation({0.0f,0.0f,1.0f},-M_PI);
    mesh_joueur_hand2_opengl.fill_vbo(mesh_joueur_hand2);


    //*****************************************//
    // Generate user defined mesh              //
    //*****************************************//

    int Nu=20;
    int Nv=20;


    mesh_cylindre.cylindre(18,0.02,0.5,1.3,0.0);
    mesh_cylindre.fill_empty_field_by_default();    //poteau 1
    mesh_cylindre_opengl.fill_vbo(mesh_cylindre);

    mesh_cylindre2.cylindre(18,0.02,0.5,-1.3,0.0);
    mesh_cylindre2.fill_empty_field_by_default();       //poteau 2
    mesh_cylindre2_opengl.fill_vbo(mesh_cylindre2);


    texture_piste=load_texture_file("data/sand.jpg");
    mesh_piste.maillage2(-1.5,1.5,-1.2,1.2,Nu,Nv);          //terrain de sable
    mesh_piste.fill_empty_field_by_default();
    mesh_piste_opengl.fill_vbo(mesh_piste);

    texture_filet=load_texture_file("data/white.jpg");
    for (int i=1;i<4;i++)       //barre horizontale
    {
        mesh_filet[i].filet(-1.1,1.1,0.05+0.06*i,0.06+0.06*i);      //filet
        mesh_filet[i].fill_empty_field_by_default();
        mesh_filet_opengl[i].fill_vbo(mesh_filet[i]);
    }

    mesh_filet[0].filet(-1.3,1.3,0.05,0.08);
    mesh_filet[0].fill_empty_field_by_default();
    mesh_filet_opengl[0].fill_vbo(mesh_filet[0]);                 //filet

    mesh_filet[4].filet(-1.3,1.3,0.27,0.3);
    mesh_filet[4].fill_empty_field_by_default();                  //filet
    mesh_filet_opengl[4].fill_vbo(mesh_filet[4]);


    for (int j=0;j<23;j++)      //barre verticale
    {
        mesh_filet[j+5].filet(-1.1+0.1*j,-1.1+0.01+0.1*j,0.08,0.27);    //filet
        mesh_filet[j+5].fill_empty_field_by_default();
        mesh_filet_opengl[j+5].fill_vbo(mesh_filet[j+5]);
    }




}

 void scene::draw_scene()
{
    //Setup uniform parameters
    glUseProgram(shader_program_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"normal_matrix"),1,false,cam.normal.pointer());           PRINT_OPENGL_ERROR();


    //Draw the meshes

    glBindTexture(GL_TEXTURE_2D,texture_default);  PRINT_OPENGL_ERROR();
    mesh_cylindre_opengl.draw();
    mesh_cylindre2_opengl.draw();


    glBindTexture(GL_TEXTURE_2D,texture_piste); PRINT_OPENGL_ERROR();
    mesh_piste_opengl.draw();

    for (int i=0;i<28;i++)
    {
        glBindTexture(GL_TEXTURE_2D,texture_filet); PRINT_OPENGL_ERROR();
        mesh_filet_opengl[i].draw();
    }

    glBindTexture(GL_TEXTURE_2D,texture_ground); PRINT_OPENGL_ERROR();

    glBindTexture(GL_TEXTURE_2D,texture_ball); PRINT_OPENGL_ERROR();
    mesh_ball_opengl.draw();

    glBindTexture(GL_TEXTURE_2D,texture_kirby); PRINT_OPENGL_ERROR();
    mesh_kirby_opengl.draw();

    glBindTexture(GL_TEXTURE_2D,texture_joueur_hand1); PRINT_OPENGL_ERROR();
    mesh_joueur_hand1_opengl.draw();

    glBindTexture(GL_TEXTURE_2D,texture_joueur_hand2); PRINT_OPENGL_ERROR();
    mesh_joueur_hand2_opengl.draw();

    glBindTexture(GL_TEXTURE_2D,texture_beach); PRINT_OPENGL_ERROR();
    mesh_beach_opengl.draw();




    if (etat==2)
    {
         mouvement_ball();

    }

    mouvement_hand1();
    mouvement_hand2();
    mouvement_kirby();

}


scene::scene()
    :shader_program_id(0),
      speed(0.0f,-0.0345f,-0.02f),        //0.1m par seconde et toute les 0.03s   x=-0.3*0.03f
      dt(1/10000.0f)     //vitesse du jeu

{


}


GLuint scene::load_texture_file(std::string const& filename)
{
    return pwidget->load_texture_file(filename);
}

void scene::set_widget(myWidgetGL* widget_param)
{
    pwidget=widget_param;
}





void scene::mouvement_ball()
{
    //numerical integration using Forward Euler method
    //*************************************************//

    float const damping = 0.0f;        //deceleration force
    vec3  const gravity = {0.0f,-9.81f,0.0f}; //gravity force


    //speed.x=20.0f;
    //speed.y=25.0f;

    //Solve Sum(forces)=mass*acceleration (with mass=1)
    speed = speed + dt*gravity;       //integrate speed
    speed = speed - dt*damping*speed; //apply damping (~friction force)
    //p = p+dt*speed;               //integrate position

    mesh_ball.transform_apply_translation({speed.x(),speed.y(),speed.z()});

    mesh_ball_opengl.update_vbo_vertex(mesh_ball);
    speed=collision_sand();
    speed=collision_kirby();
    speed= collision_hands();

    //mesh_joueur_hand2.transform_apply_translation({speed.x(),0.0f,0.0f}); //a bouger
    //mesh_joueur_hand2_opengl.update_vbo_vertex(mesh_joueur_hand2);

}

vec3 scene::collision_sand()
{

    vec3 new_speed=speed;
   int nombre=mesh_ball.size_vertex();
     for(int i=1;i<nombre;i++)
        {
            vec3 &v1=mesh_ball.vertex(i);
            if(v1.y()<-0.2f)
            {
                //std::cout<<"SPEED  "<<speed.y()<<std::endl;
                if(speed.y()<-0.08){
                    new_speed.y()=-1*new_speed.y()-0.02;
                }
                else {new_speed.y()*=-1;}

                return new_speed;


            }
        }

    return new_speed;

}


vec3 scene::collision_kirby()
{

    vec3 new_speed=speed;
   int nombre=mesh_ball.size_vertex();
     for(int i=1;i<nombre;i++)
        {
            vec3 &v1=mesh_ball.vertex(i);
            //std::cout<<"pos ball x  "<<v1.x()<<std::endl;
            if(v1.z()<-0.9f)
            {

                new_speed=vec3(0.0f,0.0355f,0.024f);
                if (v1.x()>0.8)
                {
                    new_speed.x()=-0.01;
                    //std::cout<<"top droite  "<<std::endl;
                    return new_speed;
                }
                else if (v1.x()<-0.8)
                {
                    new_speed.x()=0.01;
                    //std::cout<<"top gauche  "<<std::endl;
                    return new_speed;
                }
                else
                {
                    int hazard=rand();
                    //std::cout<<"hazard  "<<hazard%4<<std::endl;
                    if (hazard%4==0)
                    {
                       new_speed.x()=0.004;
                    }
                    else if (hazard%4==1)
                    {
                       new_speed.x()=-0.004;
                    }
                    else if (hazard%4==2)
                    {
                       new_speed.x()=0.002;
                    }
                    else
                    {
                       new_speed.x()=-0.002;

                    }
                    return new_speed;

                }
             }

        }

    return new_speed;

}



void scene::mouvement_hand1()
{
    vec3 &v1=mesh_joueur_hand1.vertex(1);
    float dplct=0.0f;
    if (v1.x()>((max_loc.x*1.0f-320+100)/320))
    {
        dplct=fabs((max_loc.x*1.0f-320+100)/320)-fabs(v1.x()*1.0f);
    }
    else
    {
        dplct=fabs(v1.x()*1.0f)-fabs((max_loc.x*1.0f-320+100)/320);
    }
    mesh_joueur_hand1.transform_apply_translation({dplct,0.0f,0.0f});
    mesh_joueur_hand1_opengl.update_vbo_vertex(mesh_joueur_hand1);

}


void scene::mouvement_hand2()
{
   vec3 &v2=mesh_joueur_hand2.vertex(1);
   float dplct=0.0f;
   if (v2.x()>((max_loc_2.x*1.0f-320+100)/320))
   {
       dplct=fabs((max_loc_2.x*1.0f-320+100)/320)-fabs(v2.x()*1.0f);
   }
   else
   {
       dplct=fabs(v2.x()*1.0f)-fabs((max_loc_2.x*1.0f-320+100)/320);
   }
   mesh_joueur_hand2.transform_apply_translation({dplct,0.0f,0.0f});
   mesh_joueur_hand2_opengl.update_vbo_vertex(mesh_joueur_hand2);

}

void scene::mouvement_kirby()
{
    mesh_kirby.transform_apply_translation({speed.x(),0.0f,0.0f}); //kirby suit la balle
    mesh_kirby_opengl.update_vbo_vertex(mesh_kirby);

}




vec3 scene::collision_hands()
{

    vec3 new_speed=speed;
    int nombre=mesh_joueur_hand1.size_vertex();
    for(int i=1;i<nombre;i++)
        {
            vec3 &vecteur1=mesh_joueur_hand1.vertex(i);
            //std::cout<<"i"<<i<<"  et x=  "<<vecteur1.x()<<std::endl;
            vec3 &vecteur2=mesh_joueur_hand2.vertex(i);
            vec3 &v1=mesh_ball.vertex(i);

                if((v1.z()>0.9f && (fabs(vecteur1.x()-v1.x())<0.001) || (fabs(vecteur2.x()-v1.x())<0.001) && v1.z()>0.9f))
                {

                    new_speed=vec3(0.0f,0.035f,-0.024f);
                    val_compteur+=1;
                    std::cout<<val_compteur<<std::endl;

                    return new_speed;

                }

                //délimitations côtés terrain
                if(v1.x()<-1.5f || v1.x()>1.5f )
                {
                    //new_speed.x()=-1*new_speed.x();
                    if(v1.y()>0.8)
                    {
                        new_speed.y()=-new_speed.y();
                    }

                }

            }
    return new_speed;
}



void scene::bouger_hands(int key)     // permet la phase de test sans detection des mains par webcam
                                      // onbouge les mains à l'aide des touches du clavier
{
    float dL=0.3f;
    //std::cout<<key<<std::endl;
    switch (key)
    {
    case  16777235:      //KEY_UP:
        mesh_joueur_hand1.transform_apply_translation({0.0f,dL,0.0f});
        mesh_joueur_hand2.transform_apply_translation({0.0f,dL,0.0f});
        break;
    case  16777237:     //KEY_DOWN:
        mesh_joueur_hand1.transform_apply_translation({0.0f,-dL,0.0f});
        mesh_joueur_hand2.transform_apply_translation({0.0f,-dL,0.0f});
        break;
    case  16777234:    //KEY_LEFT:
        mesh_joueur_hand1.transform_apply_translation({-dL,0.0f,0.0f});
        mesh_joueur_hand2.transform_apply_translation({-dL,0.0f,0.0f});
        break;
    case  16777236:   //KEY_RIGHT:
        mesh_joueur_hand1.transform_apply_translation({dL,0.0f,0.0f});
        mesh_joueur_hand2.transform_apply_translation({dL,0.0f,0.0f});
        break;
    case  32:   //SPACE:

        if(etat==0)
        {
            etat=2;     //etat permettant de jouer
        }
        else
        {
            etat=0;         // etat de pause
        }


        break;
    }


    mesh_joueur_hand1_opengl.update_vbo_vertex(mesh_joueur_hand1);
    mesh_joueur_hand2_opengl.update_vbo_vertex(mesh_joueur_hand2);

}



