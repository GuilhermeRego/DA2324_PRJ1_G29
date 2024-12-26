# Water Supply Management

This project was developed for the **Desenho de Algoritmos** (Algorithm Design) course (2023-2024) at **FEUP**. It aims to manage and optimize water supply systems efficiently. The system includes various components, such as data management, graph-based algorithms, and user interfaces, to ensure a seamless and effective water distribution system.

## Overview

This project provides a tool for the management team of a water supply network to make informed decisions on how to best allocate resources (financial and physical), identify sensitive sections of the network, and optimize water distribution to minimize disruptions.
The goal is to maximize the flow of water through the network using the **Edmonds-Karp** algorithm for maximum flow.

## Features

- **Statistics**: Numbers about the database that was loaded into the program. Includes informations about the number of cities, reservoirs and stations, total population, total demand, connections, etc.
- **Basic Service Metrics**: Determine maximum water reach for each city and know if the current network configuration meets the water needs of its costumers.
- **Reliability and Sensitivity to Failures**: Evaluate what happens in terms of delivery capacity of the network if one specific water reservoir is out of comission, the effect of removing a single pumping station from the network, and check the effect of pipeline failures.
- **Print and Clear Informations**: The information about the max flow from a specific reservoir to a specific city is written to a file that can be printed out, to check the history of the user, and cleared.
- **Graph Algorithms**: Implementation of graph-based algorithms to make the features stated before.
- **User Interface**: An intuitive and user-friendly interface that allows management to interact with the water supply network, configure network elements, and view results.
- **Scalability**: The system is designed to handle large-scale water supply networks, making it suitable for real-world applications.
- **Documentation**: Comprehensive documentation generated using Doxygen to provide clear explanations of the system's design and functionality.

## Architecture

The system architecture is based on a graph structure, where each node represents a key component of the water supply network (e.g., a reservoir, pumping station, or delivery site), and the edges represent the pipelines connecting these components.

- **City**: Represents a city or consumer site within the network, with specific water demand requirements.
- **DataManager**: Responsible for managing and storing the data related to the water supply network, such as the capacities of reservoirs, pumping stations, and the demands of delivery sites.
- **Graph**: Implements the directed graph structure, including the nodes and edges, and contains the logic for running maximum-flow algorithms to optimize water distribution.
- **Menu**: Provides an interactive command-line interface for users to interact with the system and configure the network.
- **Reservoir**: Represents a water reservoir in the network, providing water to the system with a maximum delivery capacity.
- **Station**: Represents a pumping station that directs water flow from reservoirs to delivery sites or other stations.

### Graph Structure

The graph consists of:

- **Vertexes**: Represent different types of elements in the network, such as reservoirs, pumping stations, and delivery sites.
- **Edges**: Represent the pipelines connecting these elements, each with a maximum flow capacity.

### Authors
- Diogo Ramos (up202207954)
- Gabriel Braga (up202207784)
- Guilherme Rego (up202207041)
