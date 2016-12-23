/*
Copyright (C) 2003, 2010 - Wolfire Games
Copyright (C) 2010-2016 - Lugaru contributors (see AUTHORS file)

This file is part of Lugaru.

Lugaru is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Lugaru is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Lugaru.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Graphic/Decal.hpp"

#include "Environment/Terrain.hpp"

Decal::Decal() :
    position(),
    type(shadowdecal),
    opacity(0),
    rotation(0),
    alivetime(0),
    brightness(0)
{
}

Decal::Decal(XYZ _position, decal_type _type, float _opacity, float _rotation, float _brightness, int whichx, int whichy, float size, const Terrain& terrain, bool first) :
    position(_position),
    type(_type),
    opacity(_opacity),
    rotation(_rotation),
    alivetime(0),
    brightness(_brightness)
{
    float placex, placez;
    placex = (float)whichx * terrain.scale + terrain.scale;
    placez = (float)whichy * terrain.scale;

    texcoords[0][0] = (placex - position.x) / size / 2 + .5;
    texcoords[0][1] = (placez - position.z) / size / 2 + .5;

    vertex[0].x = placex;
    vertex[0].z = placez;
    vertex[0].y = terrain.heightmap[whichx + 1][whichy] * terrain.scale + .01;

    if (first) {
        placex = (float)whichx * terrain.scale + terrain.scale;
        placez = (float)whichy * terrain.scale + terrain.scale;
    } else {
        placex = (float)whichx * terrain.scale;
        placez = (float)whichy * terrain.scale;
    }

    texcoords[1][0] = (placex - position.x) / size / 2 + .5;
    texcoords[1][1] = (placez - position.z) / size / 2 + .5;

    vertex[1].x = placex;
    vertex[1].z = placez;
    if (first) {
        vertex[1].y = terrain.heightmap[whichx + 1][whichy + 1] * terrain.scale + .01;
    } else {
        vertex[1].y = terrain.heightmap[whichx][whichy] * terrain.scale + .01;
    }


    placex = (float)whichx * terrain.scale;
    placez = (float)whichy * terrain.scale + terrain.scale;

    texcoords[2][0] = (placex - position.x) / size / 2 + .5;
    texcoords[2][1] = (placez - position.z) / size / 2 + .5;

    vertex[2].x = placex;
    vertex[2].z = placez;
    vertex[2].y = terrain.heightmap[whichx][whichy + 1] * terrain.scale + .01;

    XYZ rot;
    if (rotation) {
        for (int i = 0; i < 3; i++) {
            rot.y = 0;
            rot.x = texcoords[i][0] - .5;
            rot.z = texcoords[i][1] - .5;
            rot = DoRotation(rot, 0, -rotation, 0);
            texcoords[i][0] = rot.x + .5;
            texcoords[i][1] = rot.z + .5;
        }
    }
}
