# **Gambit**  
This is a chess engine developed as part of a final year project with Royal Holloway, University of London.

Gambit is a UCI compliant chess engine which capitalises on human fallibility through speculative play and opponent modelling.

Awarded the "Computer Science Prize 2024-25 For Best Final Year Project" at Royal Holloway, University of London.

## **Usage**  

### **Terminal Usage**
Open the "executables" directory in a terminal and run the following command:
```bash
./Gambit.exe
```
This will run Gambit and allow interaction through the [UCI Protocol](https://backscattering.de/chess/uci/) within the terminal.

### **BanksiaGUI Usage**
For a more conveniant method of interaction, you can run Gambit through a GUI.

### 1. **Download BanksiaGUI**
Download [BanksiaGUI](https://banksiagui.com/download/) and go through their download instructions.

### 2. **Adding Gambit to BanksiaGUI**
Open BanksiaGUI and head over to the top left: File -> Settings -> Engines
Click the + icon to add an engine.
Under "Engine File" paste the location of Gambit.exe.

### 3. **Play Versus Gambit**
Navigate back to the home page of BanksiaGUI.
Select "New Tournament" at the top of the screen.
On the right hand side, you should see sections "Participants" and "Available".
Ensure that within participants is 1. hm::Human (you) and 2. Gambit.
If this is not the case, simply double click the missing player from the available section and this should move them into the participants section.
Next click start.
Depending on your configuration you may find BanksiaGUI has automatically played an opening for you and Gambit.
In either case, you can now play versus Gambit.
