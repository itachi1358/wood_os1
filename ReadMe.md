# woodOS - A Mini Operating System Shell

woodOS is a lightweight, low-level shell interface simulating the behavior of an operating system. Built entirely from scratch using C/C++ and Assembly (for low-level interfacing), this project runs directly on a virtual machine (like QEMU), bypassing modern operating system abstractions.

Here's the Video Demo: https://youtu.be/NBYLa1Dkw4k

> 🔧 Built with: Assembly, C/C++ | 💻 Platform: QEMU (or bare metal) |  No external libraries used

---

Features

Core Shell Functionalities
- **Command-line interface**: Raw character-by-character input handling
- **Built-in commands**:
  - `hello` – Prints a greeting message from woodOS
  - `clear` – Clears the display
  - `ltf` – Lists all virtual text files
  - `touch <id>` – Creates a new virtual file with index `id`
  - `write <id>` – Opens a notepad-like editor for the specified file
  - `show <id>` – Displays contents of a virtual file
  - `calc <mathematical-expression>` - Calculates the given Mathematical expression
  - `del <id>` – Deletes a file by index
  - `uchange` – Changes the username
  - `shutdown` – Halts the virtual machine
---

## 📝 Notepad System

- Multi-line text input
- Backspace and character rendering handled manually
- Save or discard changes
- Persistent across function calls using `char notepad_files[6][180][180];`

---

## 🔐 User Authentication

- Prompts for **username** and **password**
- Masked input with backspace support
- Supports dynamic username change via `uchange` command

---

## 🏗️ Architecture Overview

- **Screen Rendering**: Managed via direct memory writes to video buffer
- **Input Handling**: Real-time key capture using `keyboard.get_key()`
- **File Storage**: Simulated using a 3D character array
- **No external libraries**: All logic, including string comparison (`strcmp`), rendering, and memory management is written manually

---

##  How to use it Online

- The Build Files is too Large to be uploaded on Github
-  So  here's the link: https://drive.google.com/drive/folders/1WT1JohNk0i3bVwRvYRJz9_-Uq-n1Iol2?usp=sharing
-  Download "myos.iso" in which the operating system is present
-  Then open up https://copy.sh/v86/ , and scroll down to find "CD Image"
-  Right beside CD Image , There's a button that says choose file
-  choose the iso file that you just installed
-  now click on Start Emulating
 
## 🧪 Sample Commands

```bash
hello           # Prints welcome message
touch 1        # Creates file at index 1
write 1         # Write content to file 1
show 1          # View contents of file 1
del 1           # Deletes file 1
ltf             # List available text files
uchange         # Change the login username
calc  15+76-25^3*(18+45-64/8 +1)
#calculates the output of any mathematical expression
shutdown        # Power off



