from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
import json

# simple neural network architecture (Simple FeedForward Neural Network)
model = Sequential([
    Dense(64, activation='relu', input_shape=(32,)),
    Dense(128, activation='relu'),
    Dense(10, activation='softmax')
])

architecture = model.to_json()

with open('architecture.json', 'w') as f:
    f.write(architecture)
