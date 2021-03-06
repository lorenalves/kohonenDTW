# This package is a extension of kohonen package version 3.0.5
# The distance DTW was added in this function.
# Copyright (C) 2018,  Ron Wehrens and Johannes Kruisselbrink
# Copyright (C) 2018,  Lorena Santos
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.



## function to convert a vector of strings (either prefab distances,
## C++ code definitions or file names containing code definitions)
## to a vector of function pointers. 

## first version (NOT WORKING) is simply to cut the appropriate code
## from the supersom function

getDistancePointers <- function(dist.fcts,
                                prefabDists = c("sumofsquares", "euclidean",
                                                "manhattan", "tanimoto"),
                                maxNA.fraction) {  
  ## Text distance functions can be predefined names, C++ function
  ## definitions, or file names containing these definitions.
  
  dist.ptrs <- vector(length(dist.fcts), mode = "list")
  
  ## prefab dists first
  if (any(prefab.idx <- dist.fcts %in% prefabDists)) {
    ## first convert to a factor, and then to a pointer...
    dist.ptrs[prefab.idx] <-
      CreateStdDistancePointers(factor(dist.fcts[prefab.idx],
                                       levels = prefabDists),
                                maxNA.fraction > 0L)
  }
  
  ## for the rest:
  if (any(rest.idx <- !prefab.idx)) {
    rest.idx <- which(rest.idx)
    for (rdx in rest.idx) {
      if (!exists(dist.fcts[rdx])) {
        stop(paste("Cannot find (custom) distance function: ", dist.fcts[rdx], sep=""))
      }
      dist.ptrs[[rdx]] <- eval(call(dist.fcts[rdx]))
    }
  }
  
  dist.ptrs
}
