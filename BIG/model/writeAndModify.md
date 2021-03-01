# Write and Modify Capabilities of the Library

This document specifies all capabilities of the library to create, modify, verify and write into data files.

The library is able to continue writing into file when writing was interrupted, i.e., the library can recover from the most i/o errors.
 
## Creating a new file

At the beginning, user has to define
* name of a file where to write the data,
* a permutation of dimensions,
* size of each dimension except the first one, i.e., size of each dimension of each entity.

At the beginning, user might define
* size of the first dimension
* data-types of all entities
* maximal size of a cache

## Modifying a file

At the beginning, user has to define
* name of a file

At the beginning, user might define
* maximal size of a cache

The library scans the file to enable consecutive veryfication or modification of the file. 

## Verifying a file

Any time, user can ask for
* a permutation of dimensions
* size of each dimension (including the first one)
* presence of any entity (the provided index must be less than size of the first dimension)
* data-type of any entity (the provided index must be less than size of the first dimension and the entity must be present)
* xml data

## Writing into a file

Any time, user can

* provide data of an entity (pushEntity)
  * dimensions of the data must be equal to dimensions of the entity
  * optionally, user can provide data type to which the data will be converted
* provide data of an entity and its index (addEntity)
  * dimensions of the data must be equal to dimensions of the entity
  * index must be less than size of the first dimension
  * (cannot be used if size of the first dimension is not provided and no entity has been provided yet, i.e., cannot be used if size of the first dimension equals zero)
  * optionally, user can provide data type to which the data will be converted
* provide data of an entity and index (insertEntity)
  * dimensions of the data must be equal to dimensions of the entity
  * index must be less than or equal to size of the first dimension
  * the new entity is inserted before an entity given by the index or at the end
  * optionally, user can provide data type to which the data will be converted
* remove last entity (popEntity)
* remove an entity with a given index (removeEntity)
  * the index must be less than size of the first dimension

* provide data of an image and its index (addImage)
  * dimensions of the data must equal to dimensions of the image, i.e., (image height) x (image width) x (number of (color) planes)
  * the index must be less than number of images
  * data are converted to data types of respective entities
  * cannot be used if size of the first dimension equals zero
  * cannot be used if data-types of all entities has not been provided 

* provide data of a pixel and its coordinates (addPixel)
  * the coordinates (number of image, row and column) must be less than size of the corresponding dimensions
  * data are converted to data types of respective entities
  * cannot be used if size of the first dimension equals zero
  * cannot be used if data-types of all entities has not been provided 

* provide data of an *element* and its coordinates (addElement)
  * the coordinates must be less than size of the corresponding dimensions
  * data are converted to data types of respective entities
  * cannot be used if size of the first dimension equals zero
  * cannot be used if data-types of all entities has not been provided 
