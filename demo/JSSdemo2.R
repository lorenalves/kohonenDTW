# This package is a extension of kohonen packageversion 3.0.5 
# Copyright (C) 2017,  Ron Wehrens and Johannes Kruisselbrink
#

### Demo presenting the code to replicate the second example from the
### 2017 JSS publication on the kohonen package v. 3.0, the ecological
### case linking species diversity at different sites to soil
### characteristics.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.





require("kohonenDTW")
require("Rcpp")

### Ecology example using Bray-Curtis distance
## definition of the distance measure in C++
BCcode <- '
  #include <Rcpp.h>
  typedef double (*DistanceFunctionPtr)(double *, double *, int, int);
  
  double brayCurtisDissim(double *data, double *codes, int n, int nNA) {
    if (nNA > 0) return NA_REAL;

    double num = 0.0, denom = 0.0;
    for (int i = 0; i < n; i++) {
        num += std::abs(data[i] - codes[i]);
        denom += data[i] + codes[i];
    }
 
    return num/denom;
  }

  // [[Rcpp::export]]
  Rcpp::XPtr<DistanceFunctionPtr> BrayCurtis() {
    return Rcpp::XPtr<DistanceFunctionPtr>(new DistanceFunctionPtr(&brayCurtisDissim));
  } '
sourceCpp(code = BCcode)

## alternatively, use the BC.cpp file in the inst/Distances
## subdirectory of the package:
## sourceCpp(file = paste(path.package("kohonen"),
##                        "Distances/BC.cpp", sep = "/"))

## the example from the paper - load the data, and split in training
## and test sets. For the soil data standardization is performed based
## on the means and standard deviations from the training set.
data("varespec", package = "vegan")
data("varechem", package = "vegan")
varechem <- as.matrix(varechem)
varespec <- as.matrix(varespec)
n <- nrow(varechem)
tr.idx <- 1:(n-5)
tst.idx <- (n-4):n
chemmat.tr <- scale(varechem[tr.idx,])
chemmat.tst <- scale(varechem[tst.idx,], 
                     center = colMeans(varechem[tr.idx,]),
                     scale = apply(varechem[tr.idx,], 2, sd))
specmat.tr <- varespec[tr.idx,]
specmat.tst <- varespec[tst.idx,]

## train the map using the training data
set.seed(101)
varesom <- supersom(list(species = specmat.tr, soil = chemmat.tr), 
                    somgrid(4, 3, "hexagonal"),
                    dist.fcts = c("BrayCurtis", "sumofsquares"))

trainingPred <- predict(varesom) ## these are standardized units
plot(varesom, type = "property",
     main = "Expected nitrogen content (standardized)",
     property = trainingPred$unit.predictions$soil[,"N"])

plot(varesom, type = "property",
     main = "Expected phosphorus content (standardized)",
     property = trainingPred$unit.predictions$soil[,"P"])

plot(varesom, type = "property",
     main = "Expected potassium content (standardized)",
     property = trainingPred$unit.predictions$soil[,"K"])

plot(varesom, type = "property",
     main = "Expected Calcium content (standardized)",
     property = trainingPred$unit.predictions$soil[,"Ca"])

## now get predictions for the test data. Predictions may be based on
## either the soil data, the species data, or both. Here we show the
## test set predictions, based on the information in the soil data
## layer. 
soilBasedPred <- predict(varesom, 
                         newdata = list(soil = chemmat.tst))
lapply(soilBasedPred$predictions, function(x) head(x[,1:4]))

