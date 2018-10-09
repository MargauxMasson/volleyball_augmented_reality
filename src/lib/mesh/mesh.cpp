/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mesh.hpp"
#include "../perlin/perlin.hpp"

#include "../common/error_handling.hpp"
#include "../3d/mat3.hpp"
#include "../3d/mat4.hpp"
#include <cmath>
#include <string>
#include <sstream>
namespace cpe
{


vec3 const& mesh::vertex(int const index) const          {return mesh_basic::vertex(index);}
vec3& mesh::vertex(int const index)                      {return mesh_basic::vertex(index);}
vec3 const& mesh::normal(int const index) const          {return mesh_basic::normal(index);}
vec3& mesh::normal(int const index)                      {return mesh_basic::normal(index);}
vec3 const& mesh::color(int const index) const           {return mesh_basic::color(index);}
vec3& mesh::color(int const index)                       {return mesh_basic::color(index);}
vec2 const& mesh::texture_coord(int const index) const   {return mesh_basic::texture_coord(index);}
vec2& mesh::texture_coord(int const index)               {return mesh_basic::texture_coord(index);}
triangle_index mesh::connectivity(int const index) const {return mesh_basic::connectivity(index);}
triangle_index& mesh::connectivity(int const index)      {return mesh_basic::connectivity(index);}

void mesh::add_vertex(vec3 const& v)                     {mesh_basic::add_vertex(v);}
void mesh::add_normal(vec3 const& n)                     {mesh_basic::add_normal(n);}
void mesh::add_color(vec3 const& c)                      {mesh_basic::add_color(c);}
void mesh::add_texture_coord(vec2 const& t)              {mesh_basic::add_texture_coord(t);}
void mesh::add_triangle_index(triangle_index const& idx) {mesh_basic::add_triangle_index(idx);}

void mesh::maillage(float xmin, float xmax, float zmin, float zmax, int Nu, int Nv)
    {

    float len=xmax-xmin;
    float len2=zmax-zmin;
    perlin y2;
    for(int kv=0;kv<Nv;kv++)
    {
        for(int ku=0;ku<Nu;ku++)
        {
            float x=xmin + ku*len/(Nu-1);
            float z=zmin+kv*len2/(Nv-1);
            if(((x>=-1) && (x<=1)) && ((z<=2) && (z>=-2)))
            {
            mesh_basic::add_vertex( {x,-0.25f,z} );
            }
            else
            {
            mesh_basic::add_vertex( {x,y2({float(ku*0.1),float(kv*0.1)}),z} );
            add_texture_coord({ku/static_cast<float>(Nu-1),kv/static_cast<float>(Nv-1)});
            }
        }

    }



        for(int i=0;i<Nv-1;i++)
    {
        for(int j=0;j<Nu-1;j++)
        {

            mesh_basic::add_triangle_index({i*Nu+j,
                                            i*Nu+j+1,
                                            Nu*(i+1)+j+1});
            mesh_basic::add_triangle_index({i*Nu+j,
                                            Nu*(i+1)+j+1,
                                            Nu*(i+1)+j});

        }

    }


}

void mesh::cylindre (int N, float r, float h, float xcentre, float ycentre)
{
 float  teta=2*M_PI/N;

   for (int i=0; i<=N; i++)
   {
       float t=teta*i;
       mesh_basic::add_vertex({static_cast <float>(r*cos(t)+xcentre),-0.2+h,static_cast <float>(r*sin(t)+ycentre)});
       mesh_basic::add_vertex({static_cast <float>(r*cos(t)+xcentre),-0.2,static_cast <float>(r*sin(t)+ycentre)});
   }

   for (int j=0; j<N-1;j++)
   {
       mesh_basic::add_triangle_index({0+2*j,1+2*j,2+2*j});
        mesh_basic::add_triangle_index({1+2*j,2+2*j,3+2*j});
   }
    mesh_basic::add_triangle_index({0,1,2*N-1});
    mesh_basic::add_triangle_index({0,2*N-2,2*N-1});
}





void mesh::maillage2(float xmin,float xmax,float zmin,float zmax,int Nu,int Nv)
{
    float len=xmax-xmin;
    float len2=zmax-zmin;

    for(int kv=0;kv<Nv;kv++)
    {
        for(int ku=0;ku<Nu;ku++)
        {
            float x=xmin + ku*len/(Nu-1);
            float z=zmin+kv*len2/(Nv-1);




            mesh_basic::add_vertex( {x,-0.20,z} );
            add_texture_coord({ku/static_cast<float>(Nu-1),kv/static_cast<float>(Nv-1)});

        }

    }



        for(int i=0;i<Nv-1;i++)
    {
        for(int j=0;j<Nu-1;j++)
        {

            mesh_basic::add_triangle_index({i*Nu+j,
                                            i*Nu+j+1,
                                            Nu*(i+1)+j+1});
            mesh_basic::add_triangle_index({i*Nu+j,
                                            Nu*(i+1)+j+1,
                                            Nu*(i+1)+j});
        }
    }
}


void mesh::filet(float xmin,float xmax,float ymin,float ymax)
{






        mesh_basic::add_vertex( {xmin,ymin,0.0f} );
        //add_texture_coord({1.0f,1.0f});
        mesh_basic::add_vertex( {xmin,ymax,0.0f} );
        //add_texture_coord({1.0f,1.0f});


        mesh_basic::add_vertex( {xmax,ymin,0.0f} );
        //add_texture_coord({1.0f,1.0f});
        mesh_basic::add_vertex( {xmax,ymax,0.0f} );
        //add_texture_coord({1.0f,1.0f});



        mesh_basic::add_triangle_index({0,
                                            1,
                                            2});
        mesh_basic::add_triangle_index({1,
                                            2,
                                            3});

}







}
