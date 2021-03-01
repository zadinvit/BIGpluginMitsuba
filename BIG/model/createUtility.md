# Utility for Creation of the Data File

The utility accepts:
* name of the created data file
* list of images data files in OpenEXR or PNG format
* optionally, XML data (possibly created by the XML utility)
* optionally, *permutation* of the *dimensions* (otherwise, default *permutation* 1 2 3 4 is used)
* optionally, *data-types* of *entities* (otherwise, data-type is inferred from the image data files - half or float for the OpenEXR format and unsigned 8-bit integer for PNG format)
 
Issues:
* what data-types use, if entity is not an image (*permutation* starts with 2, 3, or 4) and data-types are not provided?
