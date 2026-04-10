import sys
import os

build_path = os.path.join(os.getcwd(), 'build')
sys.path.append(build_path)

import cpp_data_engine as engine

pipe = engine.load_data("test2.csv")

pipe.normalize_column(1)  
pipe.normalize_column(2)  

X = pipe.feature_to_double(1)
y = pipe.feature_to_double(2)

print(f"Data Sample (X): {X[:5]}")
print(f"Data Sample (y): {y[:5]}")

assert all(0 <= val <= 1 for val in X), "Normalization failed!"