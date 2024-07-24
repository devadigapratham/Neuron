from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
import json

# Define neural network architecture
model = Sequential([
    Dense(64, activation='relu', input_shape=(32,), name='Input Layer'),
    Dense(128, activation='relu', name='Hidden Layer 1'),
    Dense(10, activation='softmax', name='Output Layer')
])

architecture = []
for layer in model.layers:
    layer_info = {
        'units': layer.units,
        'activation': layer.activation.__name__,
        'name': layer.name
    }
    architecture.append(layer_info)

with open('architecture.json', 'w') as f:
    json.dump(architecture, f, indent=4)
