Definitions of the Terms Used in Other Documents
================================================

* dimension
  * in default, the library works with four-dimensional data
  * the four types of dimensions are: (1) number of images, (2) image height, (3) image width, (4) number of (color) planes of an image
  * the size of a dimension can be arbitrary

* entity
  * a three-dimensional subset of the data
  * the three *dimensions* are chosen by their *permutation*
  * in the default permutation (1 2 3 4), entity is each image and its types of dimensions are: (2) image height, (3) image width, (4) number of (color) planes of an image
  * in permutation 4 x y z, entity is each (color) plane across all images
  * size of entity is given by sizes of its dimensions, e.g., (image height) x (image width) x (number of (color) planes)

* permutation (of dimensions)
  * defines ordering of data in a memory
  * defines which subset of the data is called the *entity*
  * is expressed by a sequence of numbers (that stand for the types of *dimensions*) in order of importance
  * the most important dimension is changing the least often when sequentially going through data
  * the least important dimension is changing in each step of sequential walk-through
  * the default permutation is 1 2 3 4 (number of images, image height, image width, number of (color) planes of an image)
 
* data-type
  * defines type of data of each entity
  * each entity has its own data-type
  * allowed data types are:
    1 unsigned 8-bit integer,
    2 unsigned 16-bit integer,
    3 unsigned 32-bit integer,
    4 unsigned 64-bit integer,
    5 signed 8-bit integer,
    6 signed 16-bit integer,
    7 signed 32-bit integer,
    8 signed 64-bit integer,
    9 16-bit floating point number according to IEEE 754,
    10 32-bit floating point number according to IEEE 754,
    11 64-bit floating point number according to IEEE 754,

* image
  * a three-dimensional object
  * its size is given by sizes of its dimensions, i.e., (image height) x (image width) x (number of (color) planes)
  * its data are ordered into one-dimensional memory as in *permutation* 2 3 4, i.e., the row is changed the least often and the (color) plane is changed in each step

* element
  * a one piece of data
  * has a specific data-type
  * has four coordinates (image index, row, column, (color) plane index) specifiying its position in the data