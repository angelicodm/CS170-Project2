# CS170-Project2
Feature Selection with Nearest Neighbor
Given the fact that all the data sets have a best feature subset of 3, I limited the search space to 3 features. Once the algorithm finds the best 3 features, it stops the search as it is unlikely it'll find a better subset. This is only meant to speed up testing and the if statement can be commented out. This modification really only affects forwardSelection.
