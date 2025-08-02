# 🏋️‍♂️ Sports Event Management System – Final Project in C

This project is a console-based sports event management system developed in the C programming language. It was submitted as a final assignment for the "Programming Principles and the C Language" course.

## 👥 Contributors
- **Niv Alex** – nivalex.01@gmail.com  
- **Dan Khilkevich** – danchuk213@gmail.com

## 📄 Project Description

This system allows users to manage athletes and sports events dynamically. It provides a user-friendly interface via a command-line menu with 12 interactive options (0–11) for managing data such as athletes, events, and clubs.

Key functionalities include:
- Adding athletes and events
- Displaying and querying athlete-event relations
- Analyzing participation per event or club
- Sorting and deleting events
- Generating a combined club events report

## 📘 Topics Covered

This project demonstrates knowledge and practical application of the following C programming topics:

- 📁 **File Handling**
  - Reading from and writing to `.txt` files
  - Ensuring persistent storage of user data
- 📦 **Structs**
  - Defining and using structured data types for athletes and events
- 📍 **Pointers**
  - Managing dynamic memory and passing complex data to functions
- 🧠 **Dynamic Memory Allocation**
  - Using `malloc`, `realloc`, and `free` for flexible data management
- 🧮 **Strings and Arrays**
  - String manipulation with `strcpy`, `strcmp`, `strlen`, etc.
  - Managing arrays of structs dynamically
- 🔁 **Control Structures**
  - Mastery of `if`, `switch`, `for`, `while`, and `do-while`
- 🔎 **Input Validation**
  - Preventing invalid data entry through checks and filters
- 🧪 **Functions**
  - Decomposing logic into reusable helper functions
- 🔀 **Sorting Algorithms**
  - Sorting event data by year for clean output
- 📊 **User Interaction**
  - CLI-based UI with a menu-driven system and real-time feedback

## 📋 Menu Options

| Option | Description |
|--------|-------------|
| 0 | Exit the system |
| 1 | Add a new athlete |
| 2 | Add a new event |
| 3 | Display all athletes |
| 4 | Display all athletes with their events |
| 5 | Display events for a specific athlete |
| 6 | Count participants in a specific event |
| 7 | Show the best club (most event participation) |
| 8 | Show all co-participants with a specific athlete |
| 9 | Show sorted events by club |
| 10 | Delete a specific event |
| 11 | Create a file with all unique events of two clubs |

## 🧪 Input Validation & Consistency

- IDs must be numeric  
- Names must be strings  
- Gender must be `0` (female) or `1` (male)  
- Club names may contain numbers (e.g., Schalke 04)  
- Input is standardized to ensure all saved data follows a uniform format

---

## 🧠 Implementation Highlights

- Extensive use of helper functions and modular design
- Use of constants for easy adjustments:
  - `MAX_LINE_LENGTH = 100`
  - `MAX_NAME_LENGTH = 50`
  - `MAX_CLUB_NUMBER = 200`
  - `MAX_EVENT_NUMBER = 200`
- Dynamic memory management for scalability
- Comments and documentation throughout the code for clarity

---

## 📁 File Structure

- `SportsmanData.txt`: Contains all registered athletes
- `EventData.txt`: Contains all recorded events
- `Club.txt`: Generated file for merged club events

---

## 🖼️ Screenshots

Sample outputs and screenshots of functionality are included in the project documentation to demonstrate:

- Adding and displaying athletes/events
  
  <img width="1383" height="607" alt="image" src="https://github.com/user-attachments/assets/db86eb8e-fba4-4223-acb3-eb67789004c3" />

- Adding new sport event

  <img width="1386" height="572" alt="image" src="https://github.com/user-attachments/assets/18520fd6-5e50-4b1d-8939-b452e9e259d4" />

- Display sports man array

  <img width="1371" height="680" alt="image" src="https://github.com/user-attachments/assets/7ae93612-b338-4e1e-b999-3314f7a7900b" />
  <img width="1381" height="522" alt="image" src="https://github.com/user-attachments/assets/422d4bd9-6cb2-4160-87e6-3594fed76b6d" />


---

## 📚 Learnings & Takeaways

This project taught us how to:

- Design a user-friendly CLI-based application
- Work with dynamic memory in C
- Parse and format text input/output reliably
- Validate user input rigorously
- Apply structured programming principles


## 🚀 How to Run

### 🖥️ Option 1: Using Visual Studio (Recommended)

1. **Clone the repository:**

   ```bash
   git clone https://github.com/nivalex01/Introduction-to-computer-science-in-C-final-project.git

2. **Open the solution file:**
   
   Navigate to the `Prog_NivAlex_DanKhilkevich` folder and open the `Prog_NivAlex_DanKhilkevich.sln` file in Visual Studio.
   
3. **Build the project:**

   From the top menu, click **Build > Build Solution** (`Ctrl+Shift+B`).

4. **Run the program:**

   Press `Ctrl + F5` or click **Debug > Start Without Debugging**.

   
### 🧰 Option 2: Using Command Line (GCC / Clang)

If you're not using Visual Studio, you can compile the program using GCC (on Linux/macOS/WSL):

```bash
gcc -o sportman_app program.c
./sportman_app
