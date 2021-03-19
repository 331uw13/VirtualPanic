import math

blob_max_count = 1000
max_count = 9251


blob_count = math.ceil(max_count/blob_max_count)
left = max_count-round(max_count/blob_max_count)*blob_max_count




print("Blobs: "+str(blob_count));
print("Left: "+str(left))




