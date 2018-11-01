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

#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::List RcppSupersom(
  Rcpp::NumericMatrix data,
  Rcpp::NumericMatrix codes,
  Rcpp::IntegerVector numVars,
  Rcpp::NumericVector weights,
  Rcpp::ExpressionVector distanceFunctions,
  Rcpp::IntegerMatrix numNAs,
  Rcpp::NumericMatrix neighbourhoodDistances,
  int neighbourhoodFct,
  Rcpp::NumericVector alphas,
  Rcpp::NumericVector radii,
  int numEpochs,
  int decay
  );

// [[Rcpp::export]]
Rcpp::List RcppBatchSupersom(
  Rcpp::NumericMatrix data,
  Rcpp::NumericMatrix codes,
  Rcpp::IntegerVector numVars,
  Rcpp::NumericVector weights,
  Rcpp::ExpressionVector distanceFunctions,
  Rcpp::IntegerMatrix numNAs,
  Rcpp::NumericMatrix neighbourhoodDistances,
  int neighbourhoodFct,
  Rcpp::NumericVector radii,
  int numEpochs
  );
  
// [[Rcpp::export]]
Rcpp::List RcppParallelBatchSupersom(
  Rcpp::NumericMatrix data,
  Rcpp::NumericMatrix codes,
  Rcpp::IntegerVector numVars,
  Rcpp::NumericVector weights,
  Rcpp::ExpressionVector distanceFunctions,
  Rcpp::IntegerMatrix numNAs,
  Rcpp::NumericMatrix neighbourhoodDistances,
  int neighbourhoodFct,
  Rcpp::NumericVector radii,
  int numEpochs,
  int numCores
  );  

// [[Rcpp::export]]
Rcpp::List RcppMap(
    Rcpp::NumericMatrix data,   /* objects to be mapped */
    Rcpp::IntegerVector numVars,
    Rcpp::IntegerMatrix numNAs,
    Rcpp::NumericMatrix codes,
    Rcpp::NumericVector weights,
    Rcpp::ExpressionVector distanceFunctions);
    
