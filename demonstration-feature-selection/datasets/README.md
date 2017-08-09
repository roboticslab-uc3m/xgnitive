# Experiment 1

### raw folder
The .csv files of this directory contain logs of the movements of the centroids of two objects: red, green. Recorded using https://github.com/roboticslab-uc3m/teo-head/blob/095d7e3497d2a0c1253082c53aeff66e989acbd9/share/cv1/scripts/cv1.xml

The data delimiter used is the space character ('\ '), and decimals are separated by the point character ('.'). 

#### Codebook
The data is set in the following order, in plain text and no specific header:

- Timestamp [s]
- x1 (X coordinate of the position of the centroid of red object [mm])
- y1 (Y coordinate of the position of the centroid of red object [mm])
- z1 (Z coordinate of the position of the centroid of red object [mm])
- x2 (X coordinate of the position of the centroid of green object [mm])
- y2 (Y coordinate of the position of the centroid of green object [mm])
- z2 (Z coordinate of the position of the centroid of green object [mm])
- x1-x2
- y1-y2
- z1-z2
- fabs(x1-x2)
- fabs(y1-y2)
- fabs(z1-z2)
- sqrt( pow((x1-x2),2) + pow((y1-y2),2) + pow((z1-z2),2) )

### selected-demonstrations folder
The .csv files of this directory are a selection of those in the raw folder, using different algorithms (the file implementing the demonstration selector is in https://github.com/smorante/continuous-goal-directed-actions/blob/master/demonstration-feature-selection/src/main_demons_selector.py). The .csv files are unmodified version of those in raw folder. Each algorithm just selects those it believes relevant, and place a copy in the corresponcet folder.
