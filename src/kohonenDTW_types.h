/* This package is a extension of kohonen package 3.0.5.
   The DTW distance was added by Lorena Santos.
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

typedef enum {
    SUMOFSQUARES = 1,
    EUCLIDEAN = 2,
    MANHATTAN = 3,
    TANIMOTO = 4
} DistanceType;

typedef double (*DistanceFunctionPtr)(double *, double *, int, int);
typedef DistanceFunctionPtr* DistanceFunctionPtrs;
