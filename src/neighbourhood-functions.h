/* This package is a extension of kohonen package 3.0.5.
 Copyright (C) 2018,  Ron Wehrens and Johannes Kruisselbrink
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 */

#ifndef NEIGHBOURHOOD_H
#define NEIGHBOURHOOD_H

typedef enum {
  BUBBLE = 1,
  GAUSSIAN = 2
} NeighbourhoodFunctionType;

typedef double (*NeighbourhoodFunctionPtr)(double, double);

/*
 * Returns a neighbourhood function pointer of the specified type.
 */
NeighbourhoodFunctionPtr CreateNeighbourhoodFunction(NeighbourhoodFunctionType type);

/*
 * Gaussian neighbourhood function.
 */
double Gaussian(double distance, double radius);

/*
 * Bubble neighbourhood function.
 */
double Bubble(double distance, double radius);

#endif
