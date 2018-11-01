# This package is a extension of kohonen package 3.0.5.
# Copyright (C) 2018,  Ron Wehrens and Johannes Kruisselbrink
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


summary.kohonenDTW <- function(object, ...)
{
  cat("SOM of size ", object$grid$xdim, "x", object$grid$ydim,
      " with a ", object$grid$topo,
      if (object$grid$toroidal) "toroidal", " topology and a ",
      as.character(object$grid$neighbourhood.fct),
      " neighbourhood function.", sep="")

  if (!is.null(object$data)) {
    cat("\nTraining data included of",
        nrow(object$data[[1]]), "objects")
    cat("\nThe number of layers is", length(object$data))
    if (length(object$data) > length(object$whatmap))
      cat(", of which", length(object$whatmap),
          ifelse(length(object$whatmap) > 1, "have", "has"),
          "been used in training.")
    cat("\nMean distance to the closest unit in the map:",
        mean(object$distances, na.rm = TRUE))
  } else {
    cat("\nNo training data included in the object.")
  }
  
  cat("\n")
  
  invisible()
}

print.kohonenDTW <- function(x, ...)
{
  cat("SOM of size ", x$grid$xdim, "x", x$grid$ydim,
      " with a ", x$grid$topo, if (x$grid$toroidal) " toroidal",
      " topology.", sep="")
  if (!is.null(x$data))
    cat("\nTraining data included.")
  cat("\n")
  
  invisible()
}
