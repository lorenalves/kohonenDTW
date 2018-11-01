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


classvec2classmat <- function(yvec)
{
  if (!is.factor(yvec)) yvec <- factor(yvec)
  nclasses <- nlevels(yvec)

  outmat <- matrix(0, length(yvec), nclasses)
  dimnames(outmat) <- list(NULL, levels(yvec))
  
  for (i in 1:nclasses)
    outmat[which(as.integer(yvec) == i),i] <- 1

  outmat
}

classmat2classvec <- function(ymat, threshold=0)
{
  class.names <- dimnames(ymat)[[2]]
  if (is.null(class.names)) class.names <- 1:ncol(ymat)

  classes <- apply(ymat, 1, function(x) which(x == max(x))[1])
  classes[apply(ymat, 1, max) < threshold] <- NA
  
  factor(class.names[classes], levels = class.names)
}
