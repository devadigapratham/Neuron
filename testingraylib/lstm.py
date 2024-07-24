from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense
import json

# LSTM Network
model = Sequential([
    LSTM(100, input_shape=(10, 64)),
    Dense(50, activation='relu'),
    Dense(1, activation='sigmoid')
])

architecture = model.to_json()

with open('architecture.json', 'w') as f:
    f.write(architecture)
