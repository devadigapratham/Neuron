from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import SimpleRNN, Dense
import json

# Recurrent Neural Network
model = Sequential([
    SimpleRNN(50, input_shape=(10, 64)),
    Dense(30, activation='relu'),
    Dense(1, activation='sigmoid')
])

architecture = model.to_json()

with open('architecture.json', 'w') as f:
    f.write(architecture)
