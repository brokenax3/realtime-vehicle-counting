```
January 20, 2022
Prepared by: Mark Cai Yee Lee
```

# Report

## Summary of Work Completed

1. Research on Object Tracking
2. Testing multiple Deep Learning Object Detection Algorithms
3. Writing a simple object tracking algorithm


## Writing a Simple Object Tracking Algorithm

The premise behind object tracking is that the centroid of an object can be remembered at first and compared to the next available centroids in the next frames. When tracking multiple objects, the centroid with the smallest distance between the previous centroid is taken. Next, any additional centroids are treated as new objects. When an object has not been detected for a set amount of frames, it is removed from the list of objects.

