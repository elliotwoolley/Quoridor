# Quoridor

A web-based implementation of the board game [Quoridor](https://en.wikipedia.org/wiki/Quoridor), featuring an AI powered by a C engine compiled to WebAssembly.

Play now: [https://quoridor.dev](https://quoridor.dev).

---

## Features

* **Robust game logic** written in C with efficient bitboard representations
* **Iterative deepening** search for strong AI play
* **WebAssembly** build via Emscripten for near-native performance in the browser
* **React + TypeScript** frontend built with Vite for fast iteration
* **Web Worker** to offload AI computations without blocking the UI
* **CI/CD** pipeline using GitHub Actions and deployment to GitHub Pages

---

## Prerequisites

* Node.js
* npm
* CMake
* Git

---

## Running Locally

1. **Clone the repository**

   ```bash
   git clone https://github.com/elliotwoolley/Quoridor.git
   cd Quoridor
   ```

2. **Setup Emscripten**

    MacOS and Linux:
    ```bash
    chmod +x emscripten_setup.sh
    ./emscripten_setup.sh
    ```

    Windows:
    ```bash
    emscripten_setup.bat
    ```

3. **Install JavaScript Dependencies**

   ```bash
   npm install
   ```

4. **Run in Development Mode**

   ```bash
   npm run dev
   ```

---
