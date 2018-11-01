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

#include "neighbourhood-functions.h"

#include <math.h>

/*
 * Returns a neighbourhood function pointer of the specified type.
 */
NeighbourhoodFunctionPtr CreateNeighbourhoodFunction(NeighbourhoodFunctionType type) {
  NeighbourhoodFunctionPtr neighbourhoodFunction;
  switch (type) {
    case BUBBLE:
      neighbourhoodFunction = &Bubble;
      break;
    case GAUSSIAN:
      neighbourhoodFunction = &Gaussian;
      break;
    default:
      neighbourhoodFunction = &Bubble;
      break;
  }
  return neighbourhoodFunction;
}

/*
 * Gaussian neighbourhood function.
 */
double Gaussian(double distance, double radius) {
  return exp(-(distance * distance) / (2 * radius * radius));
}

/*
 * Bubble neighbourhood function.
 */
double Bubble(double distance, double radius) {
  if (distance <= radius) {
    return 1.0;
  }
  return 0.0;
}
