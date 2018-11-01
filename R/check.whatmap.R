# This package is a extension of kohonen package  version 3.0.5. 
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

check.whatmap <- function(x, whatmap)
{
  whatmap <- unique(whatmap)

  checkpar <- NULL
  if (class(x) == "kohonenDTW") {
    checkpar <- getCodes(x)
  } else {
    if (is.list(x)) # not foolproof!
      checkpar <- x
  }
  if (is.null(checkpar))
    stop("no possibility to check argument 'whatmap'!")
  
  if (is.null(whatmap)) {
    if (is.null(x$whatmap)) {
      return(1:length(checkpar))  # no selection, return all layers
    } else {
      return(x$whatmap)
    }
  }

  if (is.numeric(whatmap) && all(whatmap %in% 1:length(checkpar)))
    return(sort(whatmap))

  if (is.character(whatmap)) {
    idx <- match(whatmap, names(checkpar)) ## works also when comparing to NULL
    if (any(!is.na(idx))) 
      return(sort(idx))
  }

  stop("incorrect whatmap argument") # invalid selection
}
