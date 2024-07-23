# Neuron

**Neuron** is a visualization tool for neural network architectures utilizing SDL2 and Jansson libraries. This tool generates graphical representations of various neural network structures, including feedforward networks, convolutional networks, and more.

## Project Status

**Under Development**

The project is actively being developed, with ongoing enhancements and feature additions. Contributions and feedback are welcomed to help refine and improve the tool.

## Features

- **Graphical Visualization:** Visualize neural network architectures with detailed layer and connection diagrams.
- **Customizable Architectures:** Define and visualize different types of neural network structures.
- **Support for Various Models:** Includes support for basic feedforward networks, CNNs, RNNs, and LSTMs.

## Directory Structure

```
├── Makefile                   # Build instructions for the project
├── neuralnet.c                # Source code for visualizing neural networks with hardcoded values
├── neuralnet.h                # Header file for the visualization code
├── README.md                  # Project documentation
└── testing
    ├── basic_arch.py          # Python script for defining a basic feedforward network
    ├── cnn.py                 # Python script for defining a Convolutional Neural Network (CNN)
    ├── lstm.py                # Python script for defining an LSTM (Long Short-Term Memory) network
    ├── rnn.py                 # Python script for defining a Recurrent Neural Network (RNN)
    └── visualize_network.c    # Source code for network visualization (alternative to neuralnet.c)
```

## Getting Started

### Prerequisites

Ensure the following dependencies are installed:

- **GCC Compiler**
- **SDL2 Library**
- **Jansson Library**

On a Unix-based system, you can install these dependencies with:

```sh
sudo pacman -S sdl2 jansson
```

### Building the Project

1. **Clone the Repository:**

   ```sh
   git clone https://github.com/yourusername/neuron.git
   cd neuron
   ```

2. **Compile the Project:**

   Build the project using the provided `Makefile`:

   ```sh
   make
   ```

   This will create an executable named `visualize_network`.

3. **Run the Application:**

   Execute the compiled binary:

   ```sh
   ./visualize_network
   ```

   The application will launch, allowing you to view the neural network visualization.

### Python Scripts

The `testing` directory contains scripts for generating different neural network architectures:

- **`basic_arch.py`**: Defines a simple feedforward neural network.
- **`cnn.py`**: Defines a Convolutional Neural Network (CNN) architecture.
- **`lstm.py`**: Defines an LSTM (Long Short-Term Memory) network.
- **`rnn.py`**: Defines a Recurrent Neural Network (RNN).

These scripts can be used to generate network architecture JSON files for visualization with the `visualize_network` tool.

### Contributing

Contributions to the project are encouraged. If you have suggestions, improvements, or bug fixes, please submit a pull request or open an issue on the project's GitHub repository.

### License

This project is licensed under the [MIT License](LICENSE).
