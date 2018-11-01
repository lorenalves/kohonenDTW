/* map.c: calculate distances of objects to units, now using the
 appropriate distance functions. We also need to take into account
 weights. We do not return all distances but only the winners and
 the associated overall distances.
 
 This package is a extension of kohonen package 3.0.5.
 Copyright (C) July 2016,  Ron Wehrens and Johannes Kruisselbrink
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 */


#include <Rcpp.h>

#include "kohonenDTW.h"
#include "distance-functions.h"

Rcpp::List RcppMap(
    Rcpp::NumericMatrix data,   /* objects to be mapped */
    Rcpp::IntegerVector numVars,
    Rcpp::IntegerMatrix numNAs,
    Rcpp::NumericMatrix codes,
    Rcpp::NumericVector weights,
    Rcpp::ExpressionVector distanceFunctions)
{
  int
    numObjects = data.ncol(),     /* number of objects */
    numLayers = numVars.size(),   /* number of layers */
    numCodes = codes.ncol(),      /* number of units in the map */
    totalVars = data.nrow(),      /* total number of variables sum(numVars) */
    i, l, nearest;

  double distance;

  Rcpp::IntegerVector offsets(numLayers);
  Rcpp::IntegerVector winners(numObjects);
  Rcpp::NumericVector unitDistances(numObjects);

  double
    *pCodes = REAL(codes),
    *pWeights = REAL(weights);

  int
    *pNumVars = INTEGER(numVars),
    *pOffsets = INTEGER(offsets);

  /* Get the distance function pointers. */
  std::vector<DistanceFunctionPtr> distanceFunctionPtrs =
    GetDistanceFunctions(distanceFunctions);

  /* Compute the layer data offsets and the total object length. */
  totalVars = 0;
  for (l = 0; l < numLayers; l++) {
    offsets[l] = totalVars;
    totalVars += numVars[l];
  }

  /* Loop over all data objects */
  for (i = 0; i < numObjects; i++) {
    /* Find best matching unit index and distance */
    FindBestMatchingUnit(
      &data[i * totalVars],
      pCodes,
      pOffsets,
      &numNAs[i * numLayers],
      numCodes,
      numLayers,
      pNumVars,
      totalVars,
      distanceFunctionPtrs,
      pWeights,
      nearest,
      distance);

    winners[i] = nearest;
    unitDistances[i] = distance;
  }

  return Rcpp::List::create(
    Rcpp::Named("winners") = winners,
    Rcpp::Named("unitdistances") = unitDistances);
}
