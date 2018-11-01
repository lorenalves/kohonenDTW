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


tricolor <- function(grid,
                     phis = c(0, 2*pi/3, 4*pi/3), # default RGB
                     offset = 0) {
  if (offset < 0 | offset > .9)
    stop("Illegal offset value, should be in [0, .9]")
  
  rgbs <- matrix(0, nrow(grid$pts), 3)
  for (i in seq(along=phis)) {
    Rmat <- matrix(c(cos(phis[i]), sin(phis[i]), -sin(phis[i]), cos(phis[i])),
                   2, 2)
    X <- grid$pts %*% Rmat
    rgbs[,i] <- X[,2]
  }

  rgbs <- sweep(rgbs, 2, apply(rgbs, 2, min), FUN = "-")
  maxima <- apply(rgbs, 2, max) / (1 - offset)

  sweep(rgbs, 2, maxima, FUN = "/") + offset
}
