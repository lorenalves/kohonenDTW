/*This package is a extension of kohonen package 3.0.5.
  The distance DTW was added by author Lorena Santos in this file to attending 
  the kohonenDTW package.
  
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
 
#include "distance-functions.h"

#include <math.h>
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>
#include <Rcpp.h>

#define UNIF unif_rand()

/*
 * Creates an array of n distance function pointers according to the specified
 * types.
 */
// [[Rcpp::export]]
Rcpp::ExpressionVector CreateStdDistancePointers(const Rcpp::IntegerVector &types, bool considerNaNs) {
  Rcpp::ExpressionVector distanceFunctions(types.size());
  for (int l = 0; l < types.size(); l++) {
    distanceFunctions[l] = CreateStdDistancePointer(types[l], considerNaNs);
  }
  return distanceFunctions;
}

/*
 * Returns a distance function XPtr pointer of the specified type.
 */
// [[Rcpp::export]]
Rcpp::XPtr<DistanceFunctionPtr> CreateStdDistancePointer(int type, bool considerNaNs) {
  if (considerNaNs) {
    return CreateNaNDistanceFunctionXPtr(type);
  } else {
    return CreateNonNaNDistanceFunctionXPtr(type);
  }
}

/*
 * Returns an XPtr pointer to a distance function of the specified type that
 * does not account for NaNs.
 */
Rcpp::XPtr<DistanceFunctionPtr> CreateNonNaNDistanceFunctionXPtr(int type) {
  switch ((DistanceType)type) {
    case EUCLIDEAN:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&EuclideanDistance)));
    case SUMOFSQUARES:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&SumOfSquaresDistance)));
    case MANHATTAN:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&ManhattanDistance)));
    case TANIMOTO:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&TanimotoDistance)));
    default:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&EuclideanDistance)));
  }
  return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&EuclideanDistance)));
}

/*
 * Returns an XPtr pointer to a distance function of the specified type that
 * accounts for NaNs.
 */
Rcpp::XPtr<DistanceFunctionPtr> CreateNaNDistanceFunctionXPtr(int type) {
  switch ((DistanceType)type) {
    case EUCLIDEAN:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&EuclideanDistanceNaN)));
    case SUMOFSQUARES:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&SumOfSquaresDistanceNaN)));
    case MANHATTAN:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&ManhattanDistanceNaN)));
    case TANIMOTO:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&TanimotoDistanceNaN)));
    default:
      return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&EuclideanDistanceNaN)));
  }
  return (Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&EuclideanDistanceNaN)));
}

/*
 * Creates an array of n distance function pointers according to the specified
 * types.
 */
std::vector<DistanceFunctionPtr> GetDistanceFunctions(const Rcpp::ExpressionVector &distanceFunctionXPtrs) {
  std::vector<DistanceFunctionPtr> distanceFunctions;
  for (int l = 0; l < distanceFunctionXPtrs.size(); l++) {
    distanceFunctions.push_back(AsDistanceFunctionPtr(distanceFunctionXPtrs[l]));
  }
  return distanceFunctions;
}

/*
 * Computes the distances between all objects of the data matrix. Returns the
 * lower triangle of the distance matrix as a vector.
 */
Rcpp::NumericVector ObjectDistances(Rcpp::NumericMatrix data,
            Rcpp::IntegerVector numVars,
            Rcpp::IntegerMatrix numNAs,
            Rcpp::ExpressionVector distanceFunctions,
            Rcpp::NumericVector weights) {
  
  int numObjects = data.ncol();
  int totalVars = data.nrow();
  int numLayers = numVars.size();

  Rcpp::NumericVector offsets(numLayers);
  Rcpp::NumericVector distances((numObjects * (numObjects - 1)) / 2);
  
  totalVars = 0;
  for (int l = 0; l < numLayers; l++) {
    offsets[l] = totalVars;
    totalVars += numVars[l];
  }

  double *pWeights = REAL(weights);
  double *pDistances = REAL(distances);
  int *pNumVars = INTEGER(numVars);
  int *pNumNAs = INTEGER(numNAs);

  /* Get the distance function pointers. */
  std::vector<DistanceFunctionPtr> distanceFunctionPtrs =
    GetDistanceFunctions(distanceFunctions);

  int ix = 0;
  for (int i = 0; i < numObjects - 1; ++i) {
    for (int j = i + 1; j < numObjects; ++j) {
      pDistances[ix] = 0.0;
      for (int l = 0; l < numLayers; ++l) {
        pDistances[ix] += pWeights[l] * (*distanceFunctionPtrs[l])(
          &data[i * totalVars + offsets[l]],
          &data[j * totalVars + offsets[l]],
          pNumVars[l],
          pNumNAs[i * numLayers + l]);
      }
      ix++;
    }
  }
  
  return distances;
}

/*
 * Finds the best matching codebook unit for the given data object and stores
 * its index and distance in the specified nearest unit index and nearest unit
 * distance references.
 */
void FindBestMatchingUnit(
  double *object,
  double *codes,
  int *offsets,
  int *numNAs,
  int numCodes,
  int numLayers,
  int *numVars,
  int totalVars,
  const std::vector<DistanceFunctionPtr> &distanceFunctions,
  double *weights,
  int &index,
  double &distance) {
  int nind = 0;
  double dist;

  index = NA_INTEGER;
  distance = DOUBLE_XMAX;
  for (int cd = 0; cd < numCodes; ++cd) {

    /* Calculate current unit distance */
    dist = 0.0;
    for (int l = 0; l < numLayers; ++l) {
      dist += weights[l] * (*distanceFunctions[l])(
        &object[offsets[l]],
        &codes[cd * totalVars + offsets[l]],
        numVars[l],
        numNAs[l]);
    }

    /* Update best matching unit */
    if (dist <= distance * (1 + EPS)) {
      if (dist < distance * (1 - EPS)) {
        nind = 0;
        index = cd;
      } else {
        if (++nind * UNIF < 1.0) {
          index = cd;
        }
      }
      distance = dist;
    }
  }
  
  if (distance == DOUBLE_XMAX) {
    distance = NA_REAL;
    index = NA_INTEGER;
  }
}

/*
 * Returns a function pointer to compute the euclidean distance between a data
 * vector and a codebook vector.
 */
double EuclideanDistanceNaN(double *data, double *codes, int n, int nNA) {
  if (nNA == 0) {
    return EuclideanDistance(data, codes, n, nNA);
  } else if (nNA == n) {
    return NA_REAL;
  }
  double tmp, d = 0.0;
  for (int i = 0; i < n; ++i) {
    if (!std::isnan(data[i])) {
      tmp = data[i] - codes[i];
      d += tmp * tmp;
    }
  }
  d *= n / (n - nNA);
  d = sqrt(d);
  return d;
}

/*
 * Returns a function pointer to compute the euclidean distance between a data
 * vector and a codebook vector.
 */
double EuclideanDistance(double *data, double *codes, int n, int nNA) {
  double tmp, d = 0.0;
  for (int i = 0; i < n; ++i) {
    tmp = data[i] - codes[i];
    d += tmp * tmp;
  }
  d = sqrt(d);
  return d;
}

/*
 * Returns a function pointer to compute the distance as the the sum of squared
 * differences between a data vector and a codebook vector.
 */
double SumOfSquaresDistanceNaN(double *data, double *codes, int n, int nNA) {
  if (nNA == 0) {
    return SumOfSquaresDistance(data, codes, n, nNA);
  } else if (nNA == n) {
    return NA_REAL;
  }
  double tmp, d = 0.0;
  for (int i = 0; i < n; ++i) {
    if (!std::isnan(data[i])) {
      tmp = data[i] - codes[i];
      d += tmp * tmp;
    }
  }
  d *= n / (n - nNA);
  return d;
}

/*
 * Returns a function pointer to compute the distance as the the sum of squared
 * differences between a data vector and a codebook vector.
 */
double SumOfSquaresDistance(double *data, double *codes, int n, int nNA) {
  double tmp, d = 0.0;
  for (int i = 0; i < n; ++i) {
    tmp = data[i] - codes[i];
    d += tmp * tmp;
  }
  return d;
}

/*
 * Returns a function pointer to compute the tanimoto distance between a data
 * vector and a codebook vector.
 */
double TanimotoDistanceNaN(double *data, double *codes, int n, int nNA) {
  if (nNA == 0) {
    return TanimotoDistance(data, codes, n, nNA);
  } else if (nNA == n) {
    return NA_REAL;
  }
  double d = 0.0;
  for (int i = 0; i < n; ++i) {
    if (!std::isnan(data[i])) {
      if ((data[i] > .5 && codes[i] < .5)
        || (data[i] <= .5 && codes[i] >= .5)) {
        d += 1.0;
      }
    }
  }
  d *= n / (n - nNA);
  return d / n;
}

/*
 * Returns a function pointer to compute the tanimoto distance between a data
 * vector and a codebook vector.
 */
double TanimotoDistance(double *data, double *codes, int n, int nNA) {
  double d = 0.0;
  for (int i = 0; i < n; ++i) {
    if ((data[i] > .5 && codes[i] < .5)
      || (data[i] <= .5 && codes[i] >= .5)) {
      d += 1.0;
    }
  }
  return d / n;
}

/*
 * Returns a function pointer to compute the Manhattan or taxicab distance
 * between a data vector and a codebook vector.
 */
double ManhattanDistanceNaN(double *data, double *codes, int n, int nNA) {
  if (nNA == 0) {
    return ManhattanDistance(data, codes, n, nNA);
  } else if (nNA == n) {
    return NA_REAL;
  }
  double d = 0.0;
  for (int i = 0; i < n; ++i) {
    if (!std::isnan(data[i])) {
      d += fabs(data[i] - codes[i]);
    }
  }
  d *= n / (n - nNA);
  return d;
}

/*
 * Returns a function pointer to compute the Manhattan or taxicab distance
 * between a data vector and a codebook vector.
 */
double ManhattanDistance(double *data, double *codes, int n, int nNA) {
  double d = 0.0;
  for (int i = 0; i < n; ++i) {
    d += fabs(data[i] - codes[i]);
  }
  return d;
}
