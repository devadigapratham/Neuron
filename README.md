# Neuron

**Neuron** is a tool for visualizing neural network architectures using SDL2 and Jansson libraries. It provides graphical representations of neural networks, including layers and connections, based on a defined architecture.

## Project Status

**Under Development**

This project is currently under development. New features are being added, and improvements are underway. Contributions and feedback are welcome!

## Features

- **Graphical Visualization:** Render neural network architectures with layers and connections.
- **Customizable Architecture:** Define and visualize various neural network architectures.
- **Support for Multiple Architectures:** Includes simple feedforward networks, convolutional networks, and more.

## Getting Started

### Prerequisites

Ensure you have the following installed:

- **GCC Compiler**
- **SDL2 Library**
- **Jansson Library**

On a Unix-based system (e.g., Linux), you can install these dependencies using your package manager. For example:

```sh
sudo pacman -S sdl2 jansson
```

### Building the Project

1. **Clone the Repository:**

   ```sh
   git clone https://github.com/yourusername/neuron.git
   cd neuron
   ```

2. **Build the Project:**

   Use the provided `Makefile` to compile the project:

   ```sh
   make
   ```

   This will generate an executable named `visualize_network`.

3. **Run the Executable:**

   ```sh
   ./visualize_network
   ```

   The application will start, and you can view the neural network visualization.

### Directory Structure

- `visualize_network.c` - Source code for the visualization tool.
- `Makefile` - Build instructions.
- `README.md` - Project documentation.

### Contributing

Contributions are welcome! If you have suggestions, improvements, or bug fixes, please submit a pull request or open an issue.

### License

This project is licensed under the [MIT License](LICENSE).
